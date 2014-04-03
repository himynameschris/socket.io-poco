
#ifndef SIONOTIFICATIONS_HPP_
#define SIONOTIFICATIONS_HPP_

#include "Poco/Notification.h"

namespace sio_poco
{

class SIOClient;

class SIOMessage: public Poco::Notification
{
public:
	SIOMessage(std::string msg) : _msg(msg) {}
	std::string getMsg() const {return _msg;}
private:
	std::string _msg;
};

class SIOJSONMessage: public Poco::Notification
{
public:
	SIOJSONMessage(std::string msg) : _msg(msg) {}
	std::string getMsg() const {return _msg;}
private:
	std::string _msg;
};

class SIOEvent: public Poco::Notification
{
public:
	SIOEvent(SIOClient *client, std::string data) : _client(client), _data(data) {}

	SIOClient *_client;
	std::string _name, _data;
};
}
#endif
