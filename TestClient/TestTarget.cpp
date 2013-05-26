#include "TestTarget.h"
#include "SIOEventRegistry.h"
#include "SIOEventTarget.h"
#include "Poco/Delegate.h"

TestTarget::TestTarget()
{
	this->on("Update", callback(&TestTarget::onUpdate));
}

void TestTarget::onUpdate(const void* pSender, Object::Ptr& arg)
{
	Logger *logger = &(Logger::get("SIOClientLog"));

	logger->information("onUpdate!");

}