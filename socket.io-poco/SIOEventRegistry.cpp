#pragma once

#include "SIOEventRegistry.h"
#include "SIOEventCallback.h"

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

bool SIOEventRegistry::registerEvent(const char *name, SIOEventCallbackBase *callback)
{
	mEventMap[std::string(name)] = callback;
	return true;
}

void SIOEventRegistry::fireEvent(SIOClient *client, const char *name, std::string *data)
{
    const char *clientName = client->nameForObject();
    char functionName[255];
	sprintf(functionName, "%s::on%s", clientName, name);

    std::map<std::string,SIOEventCallbackBase*>::const_iterator it = mEventMap.find(functionName);
    if(it != mEventMap.end())
    {
        (*it).second->fire(client, data);
    }
}