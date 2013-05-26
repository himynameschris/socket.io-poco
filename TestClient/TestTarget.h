#pragma once

#include "SIOEventTarget.h"

class TestTarget : SIOEventTarget
{
public:
	TestTarget();
	~TestTarget() {};

	void onUpdate(const void* pSender, Object::Ptr& arg);

};
