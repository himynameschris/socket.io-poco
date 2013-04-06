#pragma once
#include "SIOClient.h"
#include "SIOEventTarget.h"
#include <map>

#include "Poco/JSON/Parser.h"
#include "Poco/BasicEvent.h"
#include "Poco/Delegate.h"

using Poco::JSON::Object;
using Poco::BasicEvent;
using Poco::Delegate;

class SIOEventRegistry
{
public:
	SIOEventRegistry(void);
	~SIOEventRegistry(void);

	__declspec(dllexport) static SIOEventRegistry *sharedInstance();
	__declspec(dllexport) bool registerEvent(const char *name, SIOEventTarget *target, callback c);
	void fireEvent(SIOClient *client, const char *name, Object::Ptr data);

private:

	std::map<std::string, BasicEvent< Object::Ptr > *> mEventMap; //!< the map containing event names and handlers
};

static SIOEventRegistry *sioEventRegistryInstance = NULL;