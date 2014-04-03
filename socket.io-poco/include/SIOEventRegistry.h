#ifndef SIOEVENTREGISTRY_HPP_
#define SIOEVENTREGISTRY_HPP_

#include "SIOEventTarget.h"
#include <map>
#include <string>

#include "Poco/JSON/Parser.h"
#include "Poco/BasicEvent.h"
#include "Poco/Delegate.h"

namespace sio_poco
{

typedef void (SIOEventTarget::*callback)(const void*, Poco::JSON::Object::Ptr&);

class SIOClient;

class SIOEventRegistry
{
public:
	SIOEventRegistry(void);
	~SIOEventRegistry(void);

	//static SIOEventRegistry *sharedInstance();
	bool registerEvent(const char *name, SIOEventTarget *target, callback c);
	void fireEvent(SIOClient *client, const char *name, Poco::JSON::Object::Ptr data);

private:

	std::map<std::string, Poco::BasicEvent< Poco::JSON::Object::Ptr > *> mEventMap; //!< the map containing event names and handlers
};
}
#endif
