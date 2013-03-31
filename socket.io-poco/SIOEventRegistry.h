#pragma once
//#include "SIOEventCallback.h"
#include "SIOClient.h"
#include <map>

class SIOEventCallbackBase;

class SIOEventRegistry
{
public:
	SIOEventRegistry(void);
	~SIOEventRegistry(void);

	static SIOEventRegistry *sharedInstance();
	bool registerEvent(const char *name, SIOEventCallbackBase *callback);
	void fireEvent(SIOClient *client, const char *name, std::string *data);

private:

	std::map<std::string, SIOEventCallbackBase *> mEventMap; //!< the map containing event names and handlers
};

static SIOEventRegistry *sioEventRegistryInstance = NULL;