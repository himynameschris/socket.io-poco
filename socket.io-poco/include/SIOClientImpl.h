#ifndef SIOCLIENTIMPL_HPP_
#define SIOCLIENTIMPL_HPP_

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

#include "SIONotificationHandler.h"
#include "SIOEventRegistry.h"
#include "SIOEventTarget.h"


namespace sio_poco
{
class SIOClientImpl: public Poco::Runnable
{
private:
	SIOClientImpl();
	SIOClientImpl(std::string host, int port);
	~SIOClientImpl(void);
	
	std::string _sid;
	int _heartbeat_timeout;
	int _timeout;
	std::string _host;
	int _port;
	std::string _uri;
	bool _connected;
	

	Poco::Net::HTTPClientSession *_session;
	Poco::Net::WebSocket *_ws;
	Poco::Timer *_heartbeatTimer;
	Poco::Logger *_logger;
	Poco::Thread _thread;

	int _refCount;
	
	char *_buffer;
	std::size_t _buffer_size;
	
	//SIOEventRegistry* _registry;
	//SIONotificationHandler *_sioHandler;

public:

	bool handshake();
	bool openSocket();
	bool init();

	void release();
	void addref();	
	
	static SIOClientImpl* connect(std::string host, int port);
	void disconnect(std::string endpoint);
	void connectToEndpoint(std::string endpoint);
	void monitor();
	virtual void run();
	void heartbeat(Poco::Timer& timer);
	bool receive();
	void send(std::string endpoint, std::string s);
	void emit(std::string endpoint, std::string eventname, std::string args);

	std::string getUri();
};
}

#endif
