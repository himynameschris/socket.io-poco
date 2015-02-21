#include "TestEndpointTarget.h"
#include "Poco/Logger.h"

using Poco::Logger;


TestEndpointTarget::TestEndpointTarget()
{
	
}

void TestEndpointTarget::onUpdate(const void* pSender, Array::Ptr& arg)
{
	Logger::get("example").information("TestEndpointTarget onUpdate!");
}

void TestEndpointTarget::ontestevent(const void* pSender, Array::Ptr& arg)
{
	Logger::get("example").information("TestEndpointTarget ontestevent!");
}
