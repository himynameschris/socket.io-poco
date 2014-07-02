#include "SIOClientImpl.h"

#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPMessage.h"
#include "Poco/Net/Context.h"
#include "Poco/Net/WebSocket.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/StreamCopier.h"
#include "Poco/Format.h"
#include <iostream>
#include <sstream>
#include <limits>
#include "Poco/StringTokenizer.h"
#include "Poco/String.h"
#include "Poco/Timer.h"
#include "Poco/RunnableAdapter.h"
#include "Poco/URI.h"

#include "SIONotifications.h"
#include "SIOClientRegistry.h"
#include "SIOClient.h"

using Poco::JSON::Parser;
using Poco::JSON::ParseHandler;
using Poco::Dynamic::Var;
using Poco::JSON::Array;
using Poco::JSON::Object;

using Poco::Net::HTTPClientSession;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPMessage;
using Poco::Net::NetException;
using Poco::Net::SocketAddress;
using Poco::Net::Context;
using Poco::Net::HTTPSClientSession;
using Poco::StreamCopier;
using Poco::StringTokenizer;
using Poco::cat;
using Poco::UInt16;
using Poco::Timer;
using Poco::TimerCallback;
using Poco::Dynamic::Var;
using Poco::Net::WebSocket;
using Poco::URI;

SIOClientImpl::SIOClientImpl()
{
	SIOClientImpl(URI("http://localhost:8080"));
}

SIOClientImpl::SIOClientImpl(URI uri) :
	_buffer(NULL),
	_buffer_size(0),
	_port(uri.getPort()),
	_host(uri.getHost()),
	_refCount(0)
{
	_uri = uri;
	_ws = NULL;	

}

SIOClientImpl::~SIOClientImpl(void)
{
	
	_thread.join();
	disconnect("");

	_ws->shutdown();
	delete(_ws);

	delete(_heartbeatTimer);
	delete(_session);
	if(_buffer)
	{
		delete[] _buffer;
		_buffer = NULL;
		_buffer_size = 0;
	}

	std::stringstream ss;
	ss << _uri.getHost() << ":" << _uri.getPort() << _uri.getPath();
	std::string uri = ss.str();
	SIOClientRegistry::instance()->removeSocket(uri);
}

bool SIOClientImpl::init() {
	_logger = &(Logger::get("SIOClientLog"));

	if(handshake()) 
	{
		if(openSocket()) return true;
	}

	return false;

}

bool SIOClientImpl::handshake()
{
	UInt16 aport = _port;
	if(_uri.getScheme() == "https")
	{
		const Context::Ptr context(new Context(Context::CLIENT_USE,"","","",Context::VERIFY_NONE));
		_session = new HTTPSClientSession(_host, aport, context);
	} else {
		_session = new HTTPClientSession(_host, aport);
	}
	_session->setKeepAlive(false);
	HTTPRequest req(HTTPRequest::HTTP_GET,"/socket.io/1/?EIO=2&transport=polling",HTTPMessage::HTTP_1_1);
	req.set("Accept","*/*");
	req.setContentType("text/plain");
	req.setHost(_host);

	_logger->information("Send Handshake Post request...:");
	HTTPResponse res;
	std::string temp;

	try {
		_session->sendRequest(req);
		std::istream& rs = _session->receiveResponse(res);
		_logger->information("Receive Handshake Post request...");
		StreamCopier::copyToString(rs, temp);
		if (res.getStatus() != Poco::Net::HTTPResponse::HTTP_OK)
		{
			_logger->error("%s %s",res.getStatus(),res.getReason());
			_logger->error("response: %s\n",temp);
			return false;
		}

	} catch (std::exception &e) {
		return false;
	}

	_logger->information("%s %s",res.getStatus(),res.getReason());
	_logger->information("response: %s\n",temp);

	if(temp.at(temp.size()-1) == '}')
	{
		_version = SocketIOPacket::V10x;
		//�0{"sid":"HBlgZ7rOi8Y3QrUaAAAB","upgrades":["websocket"],"pingInterval":25000,"pingTimeout":60000}
		int a = temp.find('{');
		temp = temp.substr(a,temp.size()-a);
		temp = temp.substr(0,temp.find('}',temp.size()-5)+1);
		ParseHandler::Ptr pHandler = new ParseHandler(false);
		Parser parser(pHandler);
		Var result = parser.parse(temp);
		Object::Ptr msg = result.extract<Object::Ptr>();

		_logger->information("session: %s",msg->get("sid").toString());
		_logger->information("heartbeat: %s",msg->get("pingInterval").toString());
		_logger->information("timeout: %s",msg->get("pingTimeout").toString());

		_sid = msg->get("sid").toString();
		_heartbeat_timeout = atoi(msg->get("pingInterval").toString().c_str())/1000;
		_timeout = atoi(msg->get("pingTimeout").toString().c_str())/1000;
	}
	else
	{
		_version = SocketIOPacket::V09x;
		StringTokenizer msg(temp, ":");
		//3GYzE9md2Ig-lm3cf8Rv:60:60:websocket,htmlfile,xhr-polling,jsonp-polling
		_logger->information("session: %s",msg[0]);
		_logger->information("heartbeat: %s",msg[1]);
		_logger->information("timeout: %s",msg[2]);
		_logger->information("transports: %s",msg[3]);
		_sid = msg[0];
		_heartbeat_timeout = atoi(msg[1].c_str());
		_timeout = atoi(msg[2].c_str());
	}


	return true;
}

bool SIOClientImpl::openSocket()
{
	UInt16 aport = _port;
	HTTPResponse res;
	HTTPRequest req;
	req.setMethod(HTTPRequest::HTTP_GET);
	req.setVersion(HTTPMessage::HTTP_1_1);
	switch(_version)
	{
	case SocketIOPacket::V09x:
		{
			req.setURI("/socket.io/1/websocket/"+_sid);
		}	break;
	case SocketIOPacket::V10x:
		{
			req.setURI("/socket.io/1/websocket/?EIO=2&transport=websocket&sid="+_sid);
		}	break;
	}

	_logger->information("WebSocket To Create for %s",_sid);
	Poco::Timestamp now;
	now.update();
	do
	{
		try
		{
			_ws = new WebSocket(*_session, req, res);
		}
		catch(NetException& ne)
		{

			_logger->warning("Exception when creating websocket %s : %s - %s",ne.displayText(),ne.code(),ne.what());
			if(_ws)
			{
				delete _ws;
				_ws = NULL;
			}
			Poco::Thread::sleep(100);
		}
	}while(_ws == NULL && now.elapsed() < 1000000);
	if(_ws == NULL)
	{
		_logger->error("Impossible to create websocket");
		return _connected;
	}

	if(_version == SocketIOPacket::V10x)
	{
		std::string s = "5";//That's a ping https://github.com/Automattic/engine.io-parser/blob/1b8e077b2218f4947a69f5ad18be2a512ed54e93/lib/index.js#L21
		_ws->sendFrame(s.data(), s.size());
	}

	_logger->information("WebSocket Created and initialised");

	_connected = true;//FIXME on 1.0.x the server acknowledge the connection

	int hbInterval = this->_heartbeat_timeout*.75*1000;
	_heartbeatTimer = new Timer(hbInterval, hbInterval);
	TimerCallback<SIOClientImpl> heartbeat(*this, &SIOClientImpl::heartbeat);
	_heartbeatTimer->start(heartbeat);

	_thread.start(*this);

	return _connected;

}


SIOClientImpl* SIOClientImpl::connect(URI uri)
{
	SIOClientImpl *s = new SIOClientImpl(uri);

	if(s && s->init()) {
		return s;
	}

	return NULL;
}

void SIOClientImpl::disconnect(std::string endpoint)
{
	std::string s;
	if(_version == SocketIOPacket::V09x)
		s = "0::" + endpoint;
	else
		s = "41" + endpoint;
	_ws->sendFrame(s.data(), s.size());
	if(endpoint == "")
	{
		_logger->information("Disconnect");
		_heartbeatTimer->stop();
		_connected = false;
	}

	if(_version == SocketIOPacket::V10x)
		_ws->shutdown();
}

void SIOClientImpl::connectToEndpoint(std::string endpoint)
{
//	std::string s;
//	switch(_version)
//		{
//		case SocketIOPacket::V09x:
//			s = "1::" + endpoint;
//			break;
//		case SocketIOPacket::V10x:
//			s = "41" + endpoint;
//			break;
//		}
//	_ws->sendFrame(s.data(), s.size());
	_logger->information("heartbeat called");
	SocketIOPacket *packet = SocketIOPacket::createPacketWithType("connect",_version);
	packet->setEndpoint(endpoint);
	this->send(packet);

}

void SIOClientImpl::heartbeat(Poco::Timer& timer)
{
	_logger->information("heartbeat called");
	SocketIOPacket *packet = SocketIOPacket::createPacketWithType("heartbeat",_version);
	this->send(packet);
//	std::string s;
//	switch(_version)
//	{
//	case SocketIOPacket::V09x:
//		s = "2::";
//		break;
//	case SocketIOPacket::V10x:
//		s = "2probe";
//		break;
//	}
//	_ws->sendFrame(s.data(), s.size());
}

void SIOClientImpl::run() {

	monitor();

}
 
void SIOClientImpl::monitor() {
	do 
	{
		receive();
	} while (_connected);
}

void SIOClientImpl::send(std::string endpoint, std::string s)
{
	switch (_version) {
		case SocketIOPacket::V09x:
		{
			_logger->information("Sending Message");
			SocketIOPacket *packet = SocketIOPacket::createPacketWithType("message",_version);
			packet->setEndpoint(endpoint);
			packet->addData(s);
			this->send(packet);
		}	break;
		case SocketIOPacket::V10x:
			this->emit(endpoint,"message",s);
			break;
	}
}

void SIOClientImpl::emit(std::string endpoint, std::string eventname, std::string args)
{
	_logger->information("Emitting event \"%s\"",eventname);
	SocketIOPacket *packet = SocketIOPacket::createPacketWithType("event",_version);
	packet->setEndpoint(endpoint);
	packet->setEvent(eventname);
	packet->addData(args);
	this->send(packet);
}

void SIOClientImpl::send(SocketIOPacket *packet)
{
	std::string req = packet->toString();
	if(_connected)
	{
		_logger->information("-->SEND:%s",req);
		_ws->sendFrame(req.data(),req.size());
	}
	else
		_logger->warning("Cant send the message (%s) because disconnected",req);
}

bool SIOClientImpl::receive()
{
	if(!_buffer)
	 {
	 	int rcv_size = _ws->getReceiveBufferSize();
	 	_buffer = new char[rcv_size];
	 	_buffer_size = rcv_size;
	 }
	int flags;
	int n;

	n = _ws->receiveFrame(_buffer, _buffer_size, flags);
	_logger->information("I received something...bytes received: %d ",n);

	SocketIOPacket *packetOut;

	std::stringstream s;
	for(int i = 0; i < n; i++) {
		s << _buffer[i];
	}
	SIOClient *c;
	std::stringstream suri;
	suri << _uri.getHost() << ":" << _uri.getPort() << _uri.getPath();
	std::string uri = suri.str();

	switch(_version)
	{
		case SocketIOPacket::V09x:
		{
			const char first = s.str().at(0);
			int control = atoi(&first);
			_logger->information("buffer received: [%s]\tControl code: [%i]",s.str(),control);
			StringTokenizer st(s.str(), ":");
			std::string endpoint = st[2];

			std::stringstream ss;
			uri += endpoint;
			_logger->information("URI:%s",uri);

			c = SIOClientRegistry::instance()->getClient(uri);

			std::string payload = "";
			packetOut = SocketIOPacket::createPacketWithTypeIndex(control,_version);
			packetOut->setEndpoint(endpoint);

			switch(control)
			{
				case 0:
					_logger->information("Socket Disconnected");
					break;
				case 1:
					_logger->information("Connected to endpoint: %s", st[2]);
					break;
				case 2:
					_logger->information("Heartbeat received");
					break;
				case 3:
					if(st.count() >= 3)
					{
						for(int i = 3; i < st.count(); i++)
						{//merge piece that have been separated because they contain ':'
							if(i != 3) payload += ":";
								payload += st[i];
						}
						_logger->information("Message received(%s)",payload);
					}
					packetOut->setEvent("message");
					packetOut->addData(payload);
					c->getNCenter()->postNotification(new SIOEvent(c,packetOut));
					break;
				case 4:
					if(st.count() >= 3)
					{
						for(int i = 3; i < st.count(); i++)
						{//merge piece that have been separated because they contain ':'
							if(i != 3) payload += ":";
								payload += st[i];
						}
						_logger->information("JSON Message Received(%s)",payload);
					}
					packetOut->setEvent("message");
					packetOut->addData(payload);
					c->getNCenter()->postNotification(new SIOEvent(c,packetOut));
					break;
				case 5:
				{
					if(st.count() >= 3)
					{
						for(int i = 3; i < st.count(); i++)
						{//merge piece that have been separated because they contain ':'
							if(i != 3) payload += ":";
							payload += st[i];
						}
						_logger->information("Event Dispatched (%s)",payload);
						ParseHandler::Ptr pHandler = new ParseHandler(false);
						Parser parser(pHandler);
						Var result = parser.parse(payload);
						Object::Ptr msg = result.extract<Object::Ptr>();
						packetOut->setEvent(msg->get("name"));
						packetOut->addData(msg->getArray("args"));
						c->getNCenter()->postNotification(new SIOEvent(c,packetOut));
					}
				}break;
				case 6:
					_logger->information("Message Ack");
					break;
				case 7:
					_logger->information("Error");
					break;
				case 8:
					_logger->information("Noop");
					break;
			}
		}break;

		case SocketIOPacket::V10x:
		{
			const char first = s.str().at(0);
			std::string data = s.str().substr(1);
			int control = atoi(&first);
			_logger->information("Buffer received: [%s]\tControl code: [%i]",s.str(),control);
			switch(control)
			{
				case 0:
					_logger->information("Not supposed to receive control 0 for websocket");
					_logger->warning("That's not good");
					break;
				case 1:
					_logger->information("Not supposed to receive control 1 for websocket");
					break;
				case 2:
					_logger->information("Ping received, send pong");
					data = "3"+data;
					_ws->sendFrame(data.c_str(),data.size());
					break;
				case 3:
					_logger->information("Pong received");
					if(data == "probe")
					{
						_logger->information("Request Update");
						_ws->sendFrame("5",1);
					}
					break;
				case 4:
				{
					packetOut = SocketIOPacket::createPacketWithType("event",_version);
					const char second = data.at(0);
					data = data.substr(1);
					int nendpoint = data.find("[");
					std::string endpoint = "";
					if(nendpoint != std::string::npos)
					{
						endpoint += data.substr(0,nendpoint);
						data = data.substr(nendpoint);
						uri+=endpoint;
					}
					packetOut->setEndpoint(endpoint);
					c = SIOClientRegistry::instance()->getClient(uri);

					control = atoi(&second);
					_logger->information("Message code: [%i]",control);
					switch(control)
					{
						case 0:
							_logger->information("Socket Connected");
							_connected = true;
							break;
						case 1:
							_logger->information("Socket Disconnected");
							//this->disconnect("/");//FIXME the server is telling us it is disconnecting
							break;
						case 2:
						{
							_logger->information("Event Dispatched (%s)",data);
							ParseHandler::Ptr pHandler = new ParseHandler(false);
							Parser parser(pHandler);
							Var result = parser.parse(data);
							Array::Ptr msg = result.extract<Array::Ptr>();
							packetOut->setEvent(msg->get(0));
							for(int i = 1; i < msg->size() ; ++i)
								packetOut->addData(msg->get(i).toString());
							c->getNCenter()->postNotification(new SIOEvent(c,packetOut));


//							//42["message","{\"type\":\"redirect\",\"url\":\"/logout\",\"rid\":\"test\",\"info\":\"Internal error: could not get csInfo.\",\"action\":\"reject\"}"]
//							size_t start_pos = 0;
//							std::string from = "\\\"", to = "\"";
//							while((start_pos = data.find(from, start_pos)) != std::string::npos)
//							{
//								data.replace(start_pos, from.length(), to);
//								start_pos += to.length();
//							}
//							std::string eventtype;
//							std::string databody;
//							std::string endpoint;
//							_logger->information("Parse this: %s",data);
//							int one,two;
//							one = data.find("\"")+1;
//							two = data.find("\"",one);
//							eventtype = data.substr(one,two-one);
//							one = data.find("{");
//							two = data.find_last_of("}")+1;
//							databody = data.substr(one,two-one);
//
//
//							uri += endpoint;
//							c = SIOClientRegistry::instance()->getClient(uri);
//
//							_logger->information("Type event is: %s\nData:%s",eventtype,databody);
//							//if message
//							if(eventtype == "message")
//								c->getNCenter()->postNotification(new SIOMessage(c,databody));
//							else// need to find event
//							{
//								std::string eventPayload = "{\"name\":\""+eventtype+"\",\"args\":["+databody+"]}";
//								//{"name":"event","args":[{"type":"emit Without backslash"}]}
//								c->getNCenter()->postNotification(new SIOEvent(c, eventPayload));
//							}

						}	break;
						case 3:
							_logger->information("Message Ack");
							break;
						case 4:
							_logger->information("Error");
							break;
						case 5:
							_logger->information("Binary Event");
							break;
						case 6:
							_logger->information("Binary Ack");
							break;
					}
				}break;
				case 5:
					_logger->information("Upgrade required");
					break;
				case 6:
					_logger->information("Noop");
					break;
			}
		}break;
	}

	return true;

}

void SIOClientImpl::addref() {
	_refCount++;
}

void SIOClientImpl::release() {
	if(--_refCount == 0) delete this;
}
