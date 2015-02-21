#pragma once

#include "SIOEventTarget.h"

class TestTarget : public SIOEventTarget
{
public:
	TestTarget();
	~TestTarget() {};

	void onMessage(const void* pSender, Array::Ptr& arg);
	void onEvent(const void* pSender, Array::Ptr& arg);

};
