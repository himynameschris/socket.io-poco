#include "SIOEventRegistry.h"

using Poco::BasicEvent;
using Poco::JSON::Object;

sio_poco::SIOEventRegistry::SIOEventRegistry(void)
{
}

sio_poco::SIOEventRegistry::~SIOEventRegistry(void)
{
}

bool 
sio_poco::SIOEventRegistry::registerEvent(const char *name, SIOEventTarget *target, callback c)
{
	std::map<std::string,BasicEvent<Object::Ptr> *>::iterator it= mEventMap.find(std::string(name));
	if(it != mEventMap.end())
    {
		
		BasicEvent<Object::Ptr> *e = it->second;

		*e += Poco::delegate(target, c);

    }else
	{

		BasicEvent<Object::Ptr> *e = new BasicEvent<Object::Ptr>();

		*e += Poco::delegate(target, c);

		mEventMap[std::string(name)] = e;

	}
	
	return true;
}

void 
sio_poco::SIOEventRegistry::fireEvent(SIOClient *client, const char *name, Object::Ptr data)
{

	std::map<std::string,BasicEvent<Object::Ptr> *>::iterator it= mEventMap.find(std::string(name));
	if(it != mEventMap.end())
    {
		
		BasicEvent<Object::Ptr> *e = it->second;

		e->notify(client, data);

    }else
	{

		//no event handler found

	}

}
