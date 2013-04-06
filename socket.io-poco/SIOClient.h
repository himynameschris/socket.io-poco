#pragma once

#include <string>

#include "Poco/Net/WebSocket.h"
#include "Poco/Logger.h"
#include "Poco/Timer.h"
#include "Poco/NotificationCenter.h"
#include "Poco/Thread.h"
#include "Poco/ThreadTarget.h"
#include "Poco/RunnableAdapter.h"

#include "Poco/JSON/Parser.h"

using Poco::JSON::Object;

using Poco::Net::WebSocket;
using Poco::Logger;
using Poco::Timer;
using Poco::TimerCallback;
using Poco::NotificationCenter;
using Poco::Thread;
using Poco::ThreadTarget;


class SIOClient: public Poco::Runnable
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
	Thread _thread;
	NotificationCenter* _nCenter;	

public:
	__declspec(dllexport) SIOClient();
	__declspec(dllexport) SIOClient(int port, std::string host);
	__declspec(dllexport) ~SIOClient(void);

	bool handshake();
	bool init();
	
	
	__declspec(dllexport) bool connect();
	__declspec(dllexport) void monitor();
	__declspec(dllexport) virtual void run();
	void heartbeat(Poco::Timer& timer);
	__declspec(dllexport) bool receive();
	__declspec(dllexport) void send(std::string s);
	__declspec(dllexport) void pauser();
	__declspec(dllexport) NotificationCenter* getNCenter();
};

