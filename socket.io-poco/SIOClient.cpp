#include "StdAfx.h"
#include "SIOClient.h"
#include <iostream>

#include "Poco/Net/WebSocket.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPMessage.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/StreamCopier.h"
#include "Poco/Format.h"
#include <iostream>
#include <sstream>
#include <limits>

using Poco::UInt16;
using Poco::Net::WebSocket;
using Poco::Net::HTTPClientSession;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPMessage;
using Poco::Net::NetException;
using Poco::Net::SocketAddress;
using Poco::StreamCopier;

void pause()
{
    std::cout << "\nPress <ENTER> to continue . . .\n";
	std::cin.get();
    
}

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
		StreamCopier::copyStream(*rs, std::cout);
		return true;
	}

	return false;
}


bool SIOClient::init()
{

	handshake();
	

	/*

	

	WebSocket *ws;

	try {
		ws = new WebSocket(*session, *req, *res);
	}
	catch(NetException ne) {
		std::cout << ne.displayText() << " : " << ne.code() << " - " << ne.what() << "\n";
		pause();
		return 0;
	}

	std::cout << "WebSocket Created\n";
	*/

	

	return true;

}
