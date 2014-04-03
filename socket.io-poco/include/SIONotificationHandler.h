#ifndef SIONOTIFICATIONHANDLER_HPP_
#define SIONOTIFICATIONHANDLER_HPP_

#include "Poco/NotificationCenter.h"
#include "SIONotifications.h"
#include "Poco/Logger.h"

namespace sio_poco
{

class SIONotificationHandler
{
public:
	SIONotificationHandler(void);
	SIONotificationHandler(Poco::NotificationCenter* nc);
	~SIONotificationHandler(void);

	void handleMessage(SIOMessage* pNf);
	void handleJSONMessage(SIOJSONMessage* pNf);
	void handleEvent(SIOEvent* pNf);

	void registerCallbacks(Poco::NotificationCenter* nc);

	Poco::NotificationCenter* _nCenter;

	Poco::Logger *_logger;

	void setNCenter(Poco::NotificationCenter* nc);
};
}

#endif
