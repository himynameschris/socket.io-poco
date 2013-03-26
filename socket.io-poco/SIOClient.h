#pragma once

#include <string>

#include "Poco/Net/WebSocket.h"
#include "Poco/Logger.h"
#include "Poco/Timer.h"
#include "Poco/NotificationCenter.h"

using Poco::Net::WebSocket;
using Poco::Logger;
using Poco::Timer;
using Poco::TimerCallback;
using Poco::NotificationCenter;

class SIOClient 
{
private:
	std::string _sid;
	int _heartbeat_timeout;
	int _timeout;
	std::string _host;
	int _port;
	bool _connected;

	WebSocket *_ws;

	Timer *_heartbeatTimer;

	Logger *_logger;

	NotificationCenter* _nCenter;

public:
	__declspec(dllexport) SIOClient(void);
	__declspec(dllexport) SIOClient(int port, std::string host, NotificationCenter* nc);
	~SIOClient(void);

	bool handshake();
	bool init();
	__declspec(dllexport) bool connect();
	__declspec(dllexport) void monitor();
	void heartbeat(Poco::Timer& timer);
	__declspec(dllexport) bool receive();
	__declspec(dllexport) void pauser();
	__declspec(dllexport) NotificationCenter* getNCenter();
};

