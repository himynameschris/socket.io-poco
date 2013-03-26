#pragma once
#include "Poco/NotificationCenter.h"
#include "SIONotifications.h"

using Poco::NotificationCenter;

class SIONotificationHandler
{
public:
	SIONotificationHandler(void);
	__declspec(dllexport) SIONotificationHandler(NotificationCenter* nc);
	~SIONotificationHandler(void);

	void handleMessage(SIOMessage* pNf);
	void handleJSONMessage(SIOJSONMessage* pNf);
	void handleEvent(SIOEvent* pNf);

	void registerCallbacks(NotificationCenter* nc);

	NotificationCenter* _nCenter;

	void setNCenter(NotificationCenter* nc);
};

