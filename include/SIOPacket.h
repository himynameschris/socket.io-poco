#ifndef SIO_Packet_INCLUDED
#define SIO_Packet_INCLUDED

#include <string>
#include <vector>
#include "Poco/JSON/Array.h"
#include "Poco/JSON/Object.h"

using Poco::JSON::Array;

class SocketIOPacketV10x;

class SocketIOPacket
{
public:
	typedef enum
	{
		V09x,
		V10x
	}SocketIOVersion;

	SocketIOPacket();
	virtual ~SocketIOPacket();
	void initWithType(std::string packetType);
	void initWithTypeIndex(int index);

	std::string toString();
	virtual int typeAsNumber();
	std::string typeForIndex(int index);

	void setEndpoint(std::string endpoint){_endpoint = endpoint;};
	std::string getEndpoint(){return _endpoint;};
	void setEvent(std::string event){_name = event;};
	std::string getEvent(){return _name;};

	void addData(std::string data);
	void addData(Poco::JSON::Array::Ptr data);
	Poco::JSON::Array getDatas(){return _args;};
	virtual std::string stringify();

	static SocketIOPacket * createPacketWithType(std::string type, SocketIOPacket::SocketIOVersion version);
	static SocketIOPacket * createPacketWithTypeIndex(int type, SocketIOPacket::SocketIOVersion version);
protected:
	std::string _pId;//id message
	std::string _ack;//
	std::string _name;//event name
	Poco::JSON::Array _args;//array of objects
	std::string _endpoint;//
	std::string _type;//message type
	std::string _separator;//for stringify the object
	std::vector<std::string> _types;//types of messages
};

class SocketIOPacketV10x : public SocketIOPacket
{
public:
	SocketIOPacketV10x();
	virtual ~SocketIOPacketV10x();
	int typeAsNumber();
	std::string stringify();
private:
    std::vector<std::string> _typesMessage;
};



#endif
