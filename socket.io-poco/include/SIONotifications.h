#pragma once
#include "Poco/Notification.h"
#include "SIOPacket.h"

using Poco::Notification;

class SIOClient;

//class SIOMessage: public Notification
//{
//public:
//	SIOMessage(SIOClient *client, SocketIOPacket msg) : _client(client), _msg(msg) {}
//	SocketIOPacket getMsg() const {return _msg;}
//	SIOClient *_client;
//private:
//	SocketIOPacket _msg;
//};

//class SIOJSONMessage: public Notification
//{
//public:
//	SIOJSONMessage(std::string msg) : _msg(msg) {}
//	std::string getMsg() const {return _msg;}
//private:
//	std::string _msg;
//};

class SIOEvent: public Notification
{
public:
	SIOEvent(SIOClient *client, SocketIOPacket *data) : client(client), data(data) {}

	SIOClient *client;
	SocketIOPacket * data;

protected:
	~SIOEvent(){delete data;};
};
