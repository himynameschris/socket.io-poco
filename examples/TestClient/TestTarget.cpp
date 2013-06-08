#include "TestTarget.h"
#include "Poco/Logger.h"

using Poco::Logger;


TestTarget::TestTarget()
{
	
}

void TestTarget::onUpdate(const void* pSender, Object::Ptr& arg)
{
	Logger *logger = &(Logger::get("SIOClientLog"));

	logger->information("onUpdate!");

}

void TestTarget::ontestevent(const void* pSender, Object::Ptr& arg)
{
	Logger *logger = &(Logger::get("SIOClientLog"));

	logger->information("ontestevent!");

}