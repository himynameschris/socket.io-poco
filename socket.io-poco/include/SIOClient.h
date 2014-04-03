#ifndef SIOCLIENT_HPP_
#define SIOCLIENT_HPP_

#include "SIOClientImpl.h"
#include "Poco/JSON/Object.h"

namespace sio_poco
{
	
class SIOClient
{
private:
	~SIOClient();

	SIOClientImpl *_socket;
	
	std::string _uri;
	std::string _endpoint;

	Poco::NotificationCenter* _nCenter;

	SIOEventRegistry *_registry;
	SIONotificationHandler *_sioHandler; 

public:
	SIOClient(std::string uri, std::string endpoint, SIOClientImpl *impl);

	bool init();

	static SIOClient* connect(std::string uri);
	void disconnect();
	void send(std::string s);
	void emit(std::string eventname, std::string args);
	std::string getUri();
	Poco::NotificationCenter* getNCenter();

	typedef void (SIOEventTarget::*callback)(const void*, Poco::JSON::Object::Ptr&);

	void on(const char *name, SIOEventTarget *target, callback c);

	void fireEvent(const char * name, Poco::JSON::Object::Ptr args);
};
}

#endif
