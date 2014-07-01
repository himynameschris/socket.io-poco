#pragma once

#include "SIOEventTarget.h"
#include "Poco/Exception.h"

class TestTarget : public SIOEventTarget
{
public:
	TestTarget();
	~TestTarget() {};

	void onUpdate(const void* pSender, Array::Ptr& arg);
	void ontestevent(const void* pSender, Array::Ptr& arg);

};
