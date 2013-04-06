// main.cpp : Defines the entry point for the console application.
#include "TestClient.h"

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

	TestClient *sio = new TestClient(3000, "localhost");
	
	sio->connect();
		
	logger->information("sio setup complete, sending message");

	sio->send("Hello Socket.IO");

	logger->information("emitting test event");

	sio->emit("testevent", "[{\"name\":\"myname\",\"type\":\"mytype\"}]");

	logger->information("entering loop");
	
	do{

	}while(true);

	return 0;
}

