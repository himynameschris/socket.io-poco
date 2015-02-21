#include "SIOPacket.h"

#include <sstream>
#include <algorithm>

SocketIOPacket::SocketIOPacket()
{
	_separator = ":";
	_type = "";//message type
	_separator = ":";//for stringify the object
	_pId = "";//id message
	_ack = "";//
	_name = "";//event name
	_endpoint = "";//
	_types.push_back("disconnect");
	_types.push_back("connect");
	_types.push_back("heartbeat");
	_types.push_back("message");
	_types.push_back("json");
	_types.push_back("event");
	_types.push_back("ack");
	_types.push_back("error");
	_types.push_back("noop");
}

SocketIOPacket::~SocketIOPacket()
{
	_types.clear();
	_args.clear();
	_type = "";
	_pId = "";
	_name = "";
	_ack = "";
	_endpoint = "";
}

void SocketIOPacket::initWithType(std::string packetType)
{
	_type = packetType;
}
void SocketIOPacket::initWithTypeIndex(int index)
{
	_type = _types.at(index);
}

std::string SocketIOPacket::toString()
{
	std::stringstream encoded;
	encoded << this->typeAsNumber();
	encoded << this->_separator;

	std::string pIdL = _pId;
	if (_ack == "data")
	{
		pIdL += "+";
	}

	// Do not write pid for acknowledgements
	if (_type != "ack")
	{
		encoded << pIdL;
	}
	encoded << this->_separator;

	// Add the end point for the namespace to be used, as long as it is not
	// an ACK, heartbeat, or disconnect packet
	if (_type != "ack" && _type != "heartbeat" && _type != "disconnect")
		encoded << _endpoint;
	encoded << this->_separator;

	if (_args.size() != 0)
	{
		std::string ackpId = "";
		// This is an acknowledgement packet, so, prepend the ack pid to the data
		if (_type == "ack")
		{
			ackpId += pIdL+"+";
		}
		encoded << ackpId << this->stringify();
	}
	return encoded.str();
}
int SocketIOPacket::typeAsNumber()
{
	int num = 0;
	std::vector<std::string>::iterator item = std::find(_types.begin(), _types.end(), _type);
	if(item != _types.end())
	{
		num = item - _types.begin();
	}
	return num;
}
std::string SocketIOPacket::typeForIndex(int index)
{
	return _types.at(index);
}

void SocketIOPacket::addData(std::string data)
{
	this->_args.add(data);
}

void SocketIOPacket::addData(Poco::JSON::Array::Ptr data)
{
	for(int i = 0 ; i<data->size();++i)
		this->_args.add(data->get(i));
}

std::string SocketIOPacket::stringify()
{
	std::string outS;
	if(_type == "message")
	{
		outS = _args.get(0).toString();
	}
	else
	{
		Poco::JSON::Object obj;
		obj.set("name",_name);
		// do not require arguments
		if (_args.size() != 0)
		{
			obj.set("args",_args);
		}
		std::stringstream ss;
		obj.stringify(ss);
		outS = ss.str();
	}
	return outS;
}

SocketIOPacketV10x::SocketIOPacketV10x()
{
	_separator = ":";
	_type = "";//message type
	_separator = "";//for stringify the object
	_pId = "";//id message
	_ack = "";//
	_name = "";//event name
	_endpoint = "";//
	_types.push_back("disconnected");
	_types.push_back("connected");
	_types.push_back("heartbeat");
	_types.push_back("pong");
	_types.push_back("message");
	_types.push_back("upgrade");
	_types.push_back("noop");
	_typesMessage.push_back("connect");
	_typesMessage.push_back("disconnect");
	_typesMessage.push_back("event");
	_typesMessage.push_back("ack");
	_typesMessage.push_back("error");
	_typesMessage.push_back("binarevent");
	_typesMessage.push_back("binaryack");
	_typesMessage.push_back("connect");
}

int SocketIOPacketV10x::typeAsNumber()
{
	int num = 0;
	std::vector<std::string>::iterator item = std::find(_typesMessage.begin(), _typesMessage.end(), _type);
	if(item != _typesMessage.end())
	{//it's a message
		num = item - _typesMessage.begin();
		num += 40;
	}
	else
	{
		item = std::find(_types.begin(), _types.end(), _type);
		num += item - _types.begin();
	}
    return num;
}

std::string SocketIOPacketV10x::stringify()
{
	std::stringstream ss;
	Poco::JSON::Array data;
	data.add(_name);
	for(int i = 0 ; i<_args.size();++i)
		data.add(_args.get(i));
	data.stringify(ss);
	return ss.str();
}

SocketIOPacketV10x::~SocketIOPacketV10x()
{
	_types.clear();
	_typesMessage.clear();
	_args.clear();
	_type = "";
	_pId = "";
	_name = "";
	_ack = "";
	_endpoint = "";
}

SocketIOPacket * SocketIOPacket::createPacketWithType(std::string type, SocketIOPacket::SocketIOVersion version)
{
	SocketIOPacket *ret;
	switch (version)
	{
		case SocketIOPacket::V09x:
			ret = new SocketIOPacket;
			break;
		case SocketIOPacket::V10x:
			ret = new SocketIOPacketV10x;
			break;
	}
	ret->initWithType(type);
	return ret;
}


SocketIOPacket * SocketIOPacket::createPacketWithTypeIndex(int type, SocketIOPacket::SocketIOVersion version)
{
	SocketIOPacket *ret;
	switch (version)
	{
		case SocketIOPacket::V09x:
			ret = new SocketIOPacket;
			break;
		case SocketIOPacket::V10x:
			return new SocketIOPacketV10x;
			break;
	}
	ret->initWithTypeIndex(type);
	return ret;
}
