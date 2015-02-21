#include "TestTarget.h"
#include "Poco/Logger.h"

using Poco::Logger;


TestTarget::TestTarget()
{
	
}

void TestTarget::onMessage(const void* pSender, Array::Ptr& arg)
{
  std::string msg = "";
  for(int i=0;i<arg->size();++i)
    msg += arg->get(i).toString() + "";
	Logger::get("example").information("onMessage: %s",msg);
  
  
}

void TestTarget::onEvent(const void* pSender, Array::Ptr& arg)
{
  std::string msg = "";
  for(int i=0;i<arg->size();++i)
    msg += arg->get(i).toString() + " ";
	Logger::get("example").information("onEvent!");
}
