// main.cpp : Defines the entry point for the console application.
#include "Poco/WindowsConsoleChannel.h"
#include "Poco/Thread.h"

#include "SIOClient.h"
#include "TestTarget.h"

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

	//test the event emitter
	logger->information("emitting test event\n");
	sio->emit("testevent", "[{\"name\":\"myname\",\"type\":\"mytype\"}]");

	//enter a do loop to keep the program and connection alive
	//ctrl+c or close window to end the program and close the connection
	logger->information("entering loop");
	
	do{

	}while(true);

	return 0;
}

