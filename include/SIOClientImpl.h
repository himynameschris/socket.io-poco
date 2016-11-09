#ifndef SIO_ClientImpl_DEFINED
#define SIO_ClientImpl_DEFINED

#include <string>

#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/WebSocket.h"
#include "Poco/Logger.h"
#include "Poco/Timer.h"
#include "Poco/NotificationCenter.h"
#include "Poco/Thread.h"
#include "Poco/ThreadTarget.h"
#include "Poco/RunnableAdapter.h"
#include "Poco/URI.h"

#include "Poco/JSON/Parser.h"

#include "SIONotificationHandler.h"
#include "SIOEventRegistry.h"
#include "SIOEventTarget.h"
#include "SIOPacket.h"

using Poco::Net::HTTPClientSession;
using Poco::Net::WebSocket;
using Poco::Logger;
using Poco::Timer;
using Poco::TimerCallback;
using Poco::NotificationCenter;
using Poco::Thread;
using Poco::ThreadTarget;


class SIOClientImpl: public Poco::Runnable
{
public:
	bool handshake();
	bool openSocket();
	bool init();

	void release();
	void addref();

	static SIOClientImpl* connect(Poco::URI uri);
	void disconnect(std::string endpoint);
	void connectToEndpoint(std::string endpoint);
	void monitor();
	virtual void run();
	void heartbeat(Poco::Timer& timer);
	bool receive();
	void send(std::string endpoint, std::string s);
	void send(SocketIOPacket *packet);
	void emit(std::string endpoint, std::string eventname, std::string args);
  void emit(std::string endpoint, std::string eventname, Poco::JSON::Object::Ptr args);

	std::string getUri();

private:


	SIOClientImpl();
	SIOClientImpl(Poco::URI uri);
	~SIOClientImpl(void);
	
	std::string _sid;
	int _heartbeat_timeout;
	int _timeout;
	std::string _host;
	int _port;
	Poco::URI _uri;
	bool _connected;
	SocketIOPacket::SocketIOVersion _version;

	HTTPClientSession *_session;
	WebSocket *_ws;
	Timer *_heartbeatTimer;
	Logger *_logger;
	Thread _thread;

	int _refCount;
	char *_buffer;
	std::size_t _buffer_size;
	
	//SIOEventRegistry* _registry;
	//SIONotificationHandler *_sioHandler;
};

#endif
