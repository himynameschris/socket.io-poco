#pragma once
#include "Poco/NotificationCenter.h"
#include "SIONotifications.h"
#include "Poco/Logger.h"

using Poco::NotificationCenter;
using Poco::Logger;

class SIONotificationHandler
{
public:
	SIONotificationHandler(void);
	SIONotificationHandler(NotificationCenter* nc);
	~SIONotificationHandler(void);

	void handleMessage(SIOMessage* pNf);
	void handleJSONMessage(SIOJSONMessage* pNf);
	void handleEvent(SIOEvent* pNf);

	void registerCallbacks(NotificationCenter* nc);

	NotificationCenter* _nCenter;

	Logger *_logger;

	void setNCenter(NotificationCenter* nc);
};

