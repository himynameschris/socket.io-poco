#include "SIONotificationHandler.h"
#include "Poco/Observer.h"
#include "SIONotifications.h"
#include "Poco/NotificationCenter.h"
#include "Poco/WindowsConsoleChannel.h"

using Poco::WindowsConsoleChannel;
using Poco::Observer;

SIONotificationHandler::SIONotificationHandler(void)
{
}

SIONotificationHandler::SIONotificationHandler(NotificationCenter* nc)
{
	_nCenter = nc;
	registerCallbacks(_nCenter);

	_logger = &(Logger::get("SIOClientLog"));
	_logger->setChannel(new WindowsConsoleChannel());
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