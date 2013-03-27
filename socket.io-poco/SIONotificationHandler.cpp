#include "SIONotificationHandler.h"
#include "Poco/Observer.h"
#include "SIONotifications.h"

using Poco::Observer;

SIONotificationHandler::SIONotificationHandler(void)
{
}

SIONotificationHandler::SIONotificationHandler(NotificationCenter* nc)
{
	_nCenter = nc;
	registerCallbacks(_nCenter);
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
	std::cout << "handling message, message received: " << pNf->getMsg() << "\n";
	pNf->release();
}

void SIONotificationHandler::handleJSONMessage(SIOJSONMessage* pNf)
{
	std::cout << "handling JSON message";
	pNf->release();
}

void SIONotificationHandler::handleEvent(SIOEvent* pNf)
{
	std::cout << "handling Event";
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