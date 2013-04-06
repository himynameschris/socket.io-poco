#pragma once

#include "SIOEventRegistry.h"

SIOEventRegistry::SIOEventRegistry(void)
{
}


SIOEventRegistry::~SIOEventRegistry(void)
{
}

SIOEventRegistry* SIOEventRegistry::sharedInstance()
{
	if(!sioEventRegistryInstance)
	{
		sioEventRegistryInstance = new SIOEventRegistry();
	}
    return sioEventRegistryInstance;
}

bool SIOEventRegistry::registerEvent(const char *name, SIOEventTarget *target, callback c)
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

void SIOEventRegistry::fireEvent(SIOClient *client, const char *name, Object::Ptr data)
{

	char eventName[255];
	sprintf(eventName, "on%s", name);

	std::map<std::string,BasicEvent<Object::Ptr> *>::iterator it= mEventMap.find(std::string(eventName));
	if(it != mEventMap.end())
    {
		
		BasicEvent<Object::Ptr> *e = it->second;

		e->notify(client, data);

    }else
	{

		//no event handler found

	}

}