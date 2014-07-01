#pragma once

#include "SIOEventTarget.h"
#include <map>
#include <string>

#include "Poco/JSON/Parser.h"
#include "Poco/BasicEvent.h"
#include "Poco/Delegate.h"

using Poco::JSON::Array;
using Poco::BasicEvent;
using Poco::Delegate;

typedef void (SIOEventTarget::*callback)(const void*, Array::Ptr&);

class SIOClient;

class SIOEventRegistry
{
public:
	SIOEventRegistry(void);
	~SIOEventRegistry(void);

	//static SIOEventRegistry *sharedInstance();
	bool registerEvent(const char *name, SIOEventTarget *target, callback c);
	void fireEvent(SIOClient *client, const char *name, Array::Ptr data);

private:

	std::map<std::string, BasicEvent< Array::Ptr > *> mEventMap; //!< the map containing event names and handlers
};
