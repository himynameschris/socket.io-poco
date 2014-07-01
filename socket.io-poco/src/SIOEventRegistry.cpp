#include "SIOEventRegistry.h"

SIOEventRegistry::SIOEventRegistry(void)
{
}

SIOEventRegistry::~SIOEventRegistry(void)
{
}

bool SIOEventRegistry::registerEvent(const char *name, SIOEventTarget *target, callback c)
{
	std::map<std::string,BasicEvent<Array::Ptr> *>::iterator it= mEventMap.find(std::string(name));
	if(it != mEventMap.end())
    {
		BasicEvent<Array::Ptr> *e = it->second;
		*e += Poco::delegate(target, c);
    }else
	{
		BasicEvent<Array::Ptr> *e = new BasicEvent<Array::Ptr>();
		*e += Poco::delegate(target, c);
		mEventMap[std::string(name)] = e;
	}
	
	return true;
}

void SIOEventRegistry::fireEvent(SIOClient *client, const char *name, Array::Ptr data)
{
	std::map<std::string,BasicEvent<Array::Ptr> *>::iterator it= mEventMap.find(std::string(name));
	if(it != mEventMap.end())
    {
		BasicEvent<Array::Ptr> *e = it->second;
		e->notify(client, data);
    }else
	{
		//no event handler found
	}

}
