#include "SIOEventTarget.h"
#include "SIOEventRegistry.h"

void SIOEventTarget::on(const char *name, callback c)
{
	SIOEventRegistry::sharedInstance()->registerEvent(name, this, c);
}