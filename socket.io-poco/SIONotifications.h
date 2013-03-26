#pragma once
#include "Poco/Notification.h"

using Poco::Notification;

class SIOMessage: public Notification
{

};

class SIOJSONMessage: public Notification
{

};

class SIOEvent: public Notification
{

};