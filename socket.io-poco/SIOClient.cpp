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

using Poco::UInt16;

using Poco::Net::HTTPClientSession;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPMessage;
using Poco::Net::NetException;
using Poco::Net::SocketAddress;
using Poco::StreamCopier;
using Poco::WindowsConsoleChannel;

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
		std::stringstream session;

		StreamCopier::copyToString(*rs, temp);

		_logger->information("response: %s\n",temp);
		
		for(std::string::size_type i = 0; i < temp.size(); ++i) {
			
			if(temp[i] == ':') break;
			
			session << temp[i];

		}

		_logger->information("session: %s",session.str());

		_sid = session.str();

		return true;
	}

	return false;
}


bool SIOClient::init()
{
	_logger = &(Logger::get("TestLogger"));
	_logger->setChannel(new WindowsConsoleChannel());

	handshake();
	
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

	this->pauser();

	return true;

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