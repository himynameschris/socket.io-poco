#include "TestTarget.h"
//#include "SIOEventRegistry.h"
//#include "SIOEventTarget.h"
//#include "Poco/Delegate.h"
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