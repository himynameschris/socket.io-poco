#include "TestClient.h"
#include "SIOEventRegistry.h"
#include "SIOEventTarget.h"
#include "Poco/Delegate.h"

using Poco::Delegate;

TestClient::TestClient(int port, std::string host)
	: SIOClient(port, host)
{
	//ON_EVENT(SIOTestClient, Update)

	//event_callback callback = (event_callback)&SIOTestClient::onUpdate;

	SIOEventRegistry::sharedInstance()->registerEvent("onUpdate", this, callback(&TestClient::onUpdate));
}


void TestClient::onUpdate(const void* pSender, Object::Ptr& arg)
{
	Logger *logger = &(Logger::get("SIOClientLog"));

	logger->information("onUpdate!");

}