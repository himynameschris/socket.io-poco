#include "SIOClient.h"

#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPMessage.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/StreamCopier.h"
#include "Poco/Format.h"
#include "Poco/WindowsConsoleChannel.h"
#include <iostream>
#include <sstream>
#include <limits>
#include "Poco/StringTokenizer.h"
#include "Poco/String.h"
#include "Poco/Timer.h"
#include "Poco/RunnableAdapter.h"

#include "SIONotifications.h"
#include "SIOEventCallback.h"

using Poco::Net::HTTPClientSession;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPMessage;
using Poco::Net::NetException;
using Poco::Net::SocketAddress;
using Poco::StreamCopier;
using Poco::WindowsConsoleChannel;
using Poco::StringTokenizer;
using Poco::cat;
using Poco::UInt16;
using Poco::Timer;
using Poco::TimerCallback;

ON_EVENT(SIOClient, Update)

SIOClient::SIOClient()
{
	_port = 0;
	_host = "";
	_nCenter = NULL;
}

SIOClient::SIOClient(int port, std::string host, NotificationCenter* nc) :
	_port(port),
	_host(host),
	_nCenter(nc)
{
	init();

	

}


SIOClient::~SIOClient(void)
{
}

bool SIOClient::init()
{
	_logger = &(Logger::get("SIOClientLog"));
	_logger->setChannel(new WindowsConsoleChannel());

	return true;

}

bool SIOClient::handshake()
{
	UInt16 aport = _port;
	HTTPClientSession *session = new HTTPClientSession(_host, aport);

	HTTPRequest *req = new HTTPRequest(HTTPRequest::HTTP_POST,"/socket.io/1",HTTPMessage::HTTP_1_1);

	HTTPResponse *res = new HTTPResponse();

	session->sendRequest(*req);
	std::istream *rs = &session->receiveResponse(*res);

	std::cout << res->getStatus() << " " << res->getReason() << std::endl;

	if (res->getStatus() != Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED)
	{
		std::string temp;

		StreamCopier::copyToString(*rs, temp);

		_logger->information("response: %s\n",temp);

		StringTokenizer msg(temp, ":");

		_logger->information("session: %s",msg[0]);
		_logger->information("heartbeat: %s",msg[1]);
		_logger->information("timeout: %s",msg[2]);
		_logger->information("transports: %s",msg[3]);

		_sid = msg[0];
		_heartbeat_timeout = atoi(msg[1].c_str());
		_timeout = atoi(msg[2].c_str());

		return true;
	}

	return false;
}


bool SIOClient::connect() {

	if(handshake()) {
	
		UInt16 aport = _port;
		HTTPClientSession *session = new HTTPClientSession(_host, aport);

		HTTPRequest *req = new HTTPRequest(HTTPRequest::HTTP_GET,"/socket.io/1/websocket/"+_sid,HTTPMessage::HTTP_1_1);

		HTTPResponse *res = new HTTPResponse();

		pauser();

		try {
			_ws = new WebSocket(*session, *req, *res);
		}
		catch(NetException ne) {
			std::cout << ne.displayText() << " : " << ne.code() << " - " << ne.what() << "\n";
			this->pauser();
			return 0;
		}

		_logger->information("WebSocket Created\n");

		_connected = true;

		int hbInterval = this->_heartbeat_timeout*.75*1000;

		_heartbeatTimer = new Timer(hbInterval, hbInterval);

		TimerCallback<SIOClient> heartbeat(*this, &SIOClient::heartbeat);

		_heartbeatTimer->start(heartbeat);

		_thread.start(*this);

		return _connected;

	}

	return false;
}

void SIOClient::heartbeat(Poco::Timer& timer) {
	_logger->information("heartbeat called\n");

	std::string s = "2::";

	const void *buffer = s.c_str();

	_ws->sendFrame(buffer, s.length());

}

void SIOClient::run()
{

	monitor();

}
 
void SIOClient::monitor()
{
	do 
	{
		receive();

	} while (_connected);
}

bool SIOClient::receive() {

	char buffer[1024];
	int flags;
	int n;

	n = _ws->receiveFrame(buffer, sizeof(buffer), flags);
	_logger->information("bytes received: %d ",n);

	std::stringstream s;
	for(int i = 0; i < n; i++) {
		s << buffer[i];
	}

	_logger->information("buffer received: \"%s\"\n",s.str());

	int control = atoi(&buffer[0]);
	StringTokenizer st(s.str(), ":");

	switch(control) {
		case 0: 
			_logger->information("Socket Disconnected\n");
			break;
		case 1: 
			_logger->information("Connected to endpoint\n");
			break;
		case 2: 
			_logger->information("Heartbeat received\n");
			break;
		case 3:
			_logger->information("Message received\n");
			_nCenter->postNotification(new SIOMessage(st[3]));
			break;
		case 4:
			_logger->information("JSON Message Received\n");
			_nCenter->postNotification(new SIOJSONMessage(st[3]));
			break;
		case 5:
			_logger->information("Event Dispatched\n");
			_nCenter->postNotification(new SIOEvent);
			break;
		case 6:
			_logger->information("Message Ack\n");
			break;
		case 7:
			_logger->information("Error\n");
			break;
		case 8:
			_logger->information("Noop\n");
			break;
	}
	
	return true;

}

void SIOClient::pauser()
{
    _logger->information("\nPress <ENTER> to continue . . .\n");
	std::cin.get();
    
	return;
}

NotificationCenter* SIOClient::getNCenter()
{
	return _nCenter;
}

void SIOClient::onUpdate(std::string *data)
{

}

