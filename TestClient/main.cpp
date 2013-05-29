// main.cpp : Defines the entry point for the console application.
#include "TestTarget.h"

#include "Poco/WindowsConsoleChannel.h"
#include "Poco/Thread.h"
#include "SIONotificationHandler.h"

#include <iostream>

using Poco::WindowsConsoleChannel;
using Poco::Thread;

int main(int argc, char* argv[])
{
	Logger *logger = &(Logger::get("SIOClientLog"));
	logger->setChannel(new WindowsConsoleChannel());

	SIOClient *sio = SIOClient::connect("http://localhost:3000");

	TestTarget *t = new TestTarget();

	sio->on(t, "Update", callback(&TestTarget::onUpdate));
		
	logger->information("sio setup complete, sending message");

	sio->send("Hello Socket.IO");

	logger->information("emitting test event");

	sio->emit("testevent", "[{\"name\":\"myname\",\"type\":\"mytype\"}]");

	logger->information("entering loop");
	
	do{

	}while(true);

	return 0;
}

