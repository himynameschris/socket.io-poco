#pragma once

#include <string>


#include "Poco/Net/WebSocket.h"
#include "Poco/Logger.h"

using Poco::Net::WebSocket;
using Poco::Logger;

class SIOClient
{
private:
	std::string _sid;
	std::string _host;
	int _port;

	WebSocket *_ws;

	Logger *_logger;


public:
	__declspec(dllexport) SIOClient(void);
	__declspec(dllexport) SIOClient(int port, std::string host);
	~SIOClient(void);

	bool handshake();
	bool init();
	__declspec(dllexport) bool receive();
	__declspec(dllexport) void pauser();
};

