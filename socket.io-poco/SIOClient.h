#pragma once

#include <string>

#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/WebSocket.h"
#include "Poco/Logger.h"
#include "Poco/Timer.h"
#include "Poco/NotificationCenter.h"
#include "Poco/Thread.h"
#include "Poco/ThreadTarget.h"
#include "Poco/RunnableAdapter.h"

#include "Poco/JSON/Parser.h"

using Poco::JSON::Object;

using Poco::Net::HTTPClientSession;
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

	HTTPClientSession *_session;
	WebSocket *_ws;
	Timer *_heartbeatTimer;
	Logger *_logger;
	Thread _thread;
	NotificationCenter* _nCenter;	

public:
	SIOClient();
	SIOClient(int port, std::string host);
	~SIOClient(void);

	bool handshake();
	bool init();
	
	
	bool connect();
	void monitor();
	virtual void run();
	void heartbeat(Poco::Timer& timer);
	bool receive();
	void send(std::string s);
	void emit(std::string eventname, std::string args);
	NotificationCenter* getNCenter();
};

