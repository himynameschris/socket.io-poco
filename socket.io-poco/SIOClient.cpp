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
#include "Poco/String.h" // for cat
#include "Poco/Timer.h"

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

SIOClient::SIOClient(void)
{
	
}

SIOClient::SIOClient(int port, std::string host) :
	_port(port),
	_host(host)
{
	init();

}


SIOClient::~SIOClient(void)
{
}

bool SIOClient::init()
{
	_logger = &(Logger::get("TestLogger"));
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
	_logger->information("bytes received: %d\n",n);


	int control = buffer[0];

	switch(control) {
		case 0: 
			std::cout << "0";
				
			break;
		case 1: 
			break;
		case 2: 

			std::cout << "Heartbeat received";

			break;
	}
	
	std::stringstream s;
	for(int i = 0; i < n; i++) {
		s << buffer[i];
	}

	_logger->information("Message received: \"%s\"\n",s.str());

	return true;

}

void SIOClient::pauser()
{
    _logger->information("\nPress <ENTER> to continue . . .\n");
	std::cin.get();
    
	return;
}