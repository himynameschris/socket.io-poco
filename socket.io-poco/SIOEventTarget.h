#pragma once

//#include "SIOEventCallback.h"

class SIOEventTarget
{
private:
	
public:
	virtual ~SIOEventTarget() {};

	//virtual const char *nameForObject() const = 0;
	
};

typedef void (SIOEventTarget::*callback)(const void*, Object::Ptr&);

#define EVENT_TARGET(A) const char *nameForObject() const {return #A;}