#pragma once

#include "SIOClient.h"
#include "SIOEventTarget.h"

class TestClient :
	public SIOClient, SIOEventTarget
{
public:
	TestClient(int port, std::string host);
	~TestClient() {};

	void onUpdate(const void* pSender, Object::Ptr& arg);

};
