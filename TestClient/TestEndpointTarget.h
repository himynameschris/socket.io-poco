#pragma once

#include "SIOEventTarget.h"

class TestEndpointTarget : public SIOEventTarget
{
public:
	TestEndpointTarget();
	~TestEndpointTarget() {};

	void onUpdate(const void* pSender, Object::Ptr& arg);
	void ontestevent(const void* pSender, Object::Ptr& arg);

};
