#pragma once

#include "SIOEventTarget.h"

class TestTarget : public SIOEventTarget
{
public:
	TestTarget();
	~TestTarget() {};

	void onUpdate(const void* pSender, Array::Ptr& arg);
	void ontestevent(const void* pSender, Array::Ptr& arg);

};
