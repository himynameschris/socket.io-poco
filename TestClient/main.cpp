// main.cpp : Defines the entry point for the console application.
#include "Poco/WindowsConsoleChannel.h"
#include "Poco/Thread.h"

#include "SIOClient.h"
#include "TestTarget.h"
#include "TestEndpointTarget.h"

#include <iostream>

using Poco::WindowsConsoleChannel;
using Poco::Thread;

int main(int argc, char* argv[])
{
	//create a c++ Poco logger to use and set its channel to the windows console
	//this is the same logger instance that the library will hook into
	Logger *logger = &(Logger::get("SIOClientLog"));
	logger->setChannel(new WindowsConsoleChannel());
	
	//Establish the socket.io connection
	//JS: var socket = io.connect("localhost:3000")
	SIOClient *sio = SIOClient::connect("http://localhost:3000");

	//Create a target and register object its method onUpdate for the Update event
	//JS: socket.on("Update", function(data) {...});
	TestTarget *target = new TestTarget();
	sio->on("Update", target, callback(&TestTarget::onUpdate));
	
	//setup is now complete, messages and events can be send and received
	logger->information("sio setup complete\n");

	//test the message sending
	logger->information("sending message\n");
	sio->send("Hello Socket.IO");

	//test the event emitter, this will return the same event so let's register a callback too
	sio->on("testevent", target, callback(&TestTarget::ontestevent));
	logger->information("emitting test event\n");
	sio->emit("testevent", "[{\"name\":\"myname\",\"type\":\"mytype\"}]");
	
	//test connecting to an endpoint 'testpoint'
	TestEndpointTarget *target2 = new TestEndpointTarget();
	SIOClient *testpoint = SIOClient::connect("http://localhost:3000/testpoint");
	testpoint->send("Hello Socket.IO Testpoint");
	testpoint->on("Update", target2, callback(&TestEndpointTarget::onUpdate));
	testpoint->on("testevent", target2, callback(&TestEndpointTarget::ontestevent));
	testpoint->emit("testevent", "[{\"name\":\"myname\",\"type\":\"mytype\"}]");

	//wait for user input to move to next section of code
	//socket receiving occurs in another thread and will not be halted
	logger->information("Press any key to continue...");
	std::cin.get();

	//test disconnecting a single endpoint, other endpoints stay connected
	testpoint->disconnect();

	//disconnecting the default socket with no endpoint will also disconnect all endpoints
	sio->disconnect();

	logger->information("Press any key to quit...");
	std::cin.get();

	return 0;
}

