#include "SIONotificationHandler.h"
#include "Poco/Observer.h"
#include "SIONotifications.h"
#include "Poco/NotificationCenter.h"
#include "Poco/JSON/Parser.h"
#include "Poco/JSON/ParseHandler.h"

#include "SIOEventRegistry.h"
#include "SIOClient.h"

using Poco::Observer;
using Poco::JSON::Parser;
using Poco::JSON::ParseHandler;
using Poco::Dynamic::Var;
using Poco::JSON::Array;
using Poco::JSON::Object;

SIONotificationHandler::SIONotificationHandler(void)
{
}

SIONotificationHandler::SIONotificationHandler(NotificationCenter* nc)
{
	_nCenter = nc;
	registerCallbacks(_nCenter);

	_logger = &(Logger::get("SIOClientLog"));
}

SIONotificationHandler::~SIONotificationHandler(void)
{
	_nCenter->removeObserver(
		Observer<SIONotificationHandler, SIOMessage>(*this, &SIONotificationHandler::handleMessage)
		);
	_nCenter->removeObserver(
		Observer<SIONotificationHandler, SIOJSONMessage>(*this, &SIONotificationHandler::handleJSONMessage)
		);
	_nCenter->removeObserver(
		Observer<SIONotificationHandler, SIOEvent>(*this, &SIONotificationHandler::handleEvent)
		);
}

void SIONotificationHandler::handleMessage(SIOMessage* pNf)
{
	_logger->information("handling message, message received: %s",pNf->getMsg());
	pNf->release();
}

void SIONotificationHandler::handleJSONMessage(SIOJSONMessage* pNf)
{
	_logger->information("handling JSON message");
	pNf->release();
}

void SIONotificationHandler::handleEvent(SIOEvent* pNf)
{
	_logger->information("handling Event");
	_logger->information("data: %s", pNf->_data);

	ParseHandler::Ptr pHandler = new ParseHandler(true);
	Parser parser(pHandler);
	
	Var result = parser.parse(pNf->_data);
	Object::Ptr object = result.extract<Object::Ptr>();
	Var temp = object->get("name");

	std::string eventName = temp.convert<std::string>();
	
	Array::Ptr arr = object->getArray("args");
	Object::Ptr args = arr->getObject(0);

	pNf->_client->fireEvent(eventName.c_str(), args);

	pNf->release();
}

void SIONotificationHandler::registerCallbacks(NotificationCenter* nc)
{
	_nCenter = nc;

	_nCenter->addObserver(
		Observer<SIONotificationHandler, SIOMessage>(*this, &SIONotificationHandler::handleMessage)
		);
	_nCenter->addObserver(
		Observer<SIONotificationHandler, SIOJSONMessage>(*this, &SIONotificationHandler::handleJSONMessage)
		);
	_nCenter->addObserver(
		Observer<SIONotificationHandler, SIOEvent>(*this, &SIONotificationHandler::handleEvent)
		);
}

void SIONotificationHandler::setNCenter(NotificationCenter* nc)
{
	_nCenter = nc;
	registerCallbacks(_nCenter);
}