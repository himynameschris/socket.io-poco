#include "TestEndpointTarget.h"
#include "Poco/Logger.h"

using Poco::Logger;


TestEndpointTarget::TestEndpointTarget()
{
	
}

void TestEndpointTarget::onUpdate(const void* pSender, Array::Ptr& arg)
{
	Logger *logger = &(Logger::get("SIOClientLog"));

	logger->information("TestEndpointTarget onUpdate!");

}

void TestEndpointTarget::ontestevent(const void* pSender, Array::Ptr& arg)
{
	Logger *logger = &(Logger::get("SIOClientLog"));

	logger->information("TestEndpointTarget ontestevent!");

}
