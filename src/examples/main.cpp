// main.cpp : Defines the entry point for the console application.
#ifdef _WIN64
   //define something for Windows (64-bit)
	#include "Poco/WindowsConsoleChannel.h"
#elif _WIN32
   //define something for Windows (32-bit)
	#include "Poco/WindowsConsoleChannel.h"
#elif __APPLE__
    #if TARGET_IPHONE_SIMULATOR
         // iOS Simulator
    #elif TARGET_OS_IPHONE
        // iOS device
    #elif TARGET_OS_MAC
        // Other kinds of Mac OS
    #else
        // Unsupported platform
    #endif
#elif __linux
    // linux
	#include "Poco/ConsoleChannel.h"
#elif __unix // all unices not caught above
    // Unix
	#include "Poco/ConsoleChannel.h"
#elif __posix
    // POSIX
	#include "Poco/ConsoleChannel.h"
#endif


#include "Poco/Thread.h"

#include "SIOClient.h"
#include "TestTarget.h"
#include "TestEndpointTarget.h"

#include <iostream>

using Poco::Thread;

int main(int argc, char* argv[])
{
	//create a c++ Poco logger to use and set its channel to the windows console
	//this is the same logger instance that the library will hook into
	Logger *logger = &(Logger::get("example"));

#ifdef _WIN64
   //define something for Windows (64-bit)
	logger->setChannel(new Poco::WindowsConsoleChannel());
#elif _WIN32
   //define something for Windows (32-bit)
	logger->setChannel(new Poco::WindowsConsoleChannel());
#elif __APPLE__
    #if TARGET_IPHONE_SIMULATOR
         // iOS Simulator
    #elif TARGET_OS_IPHONE
        // iOS device
    #elif TARGET_OS_MAC
        // Other kinds of Mac OS
    #else
        // Unsupported platform
    #endif
#elif __linux
    // linux
	logger->setChannel(new Poco::ConsoleChannel());
#elif __unix // all unices not caught above
    // Unix
	logger->setChannel(new Poco::ConsoleChannel());
#elif __posix
    // POSIX
	logger->setChannel(new Poco::ConsoleChannel());
#endif

	
	//Establish the socket.io connection
	//JS: var socket = io.connect("localhost:3000")
	SIOClient *sio = SIOClient::connect("http://localhost:3000");

	//Create a target and register object its method onUpdate for the Update event
	//JS: socket.on("Update", function(data) {...});
	TestTarget *target = new TestTarget();
	sio->on("message", target, callback(&TestTarget::onMessage));
	
	//setup is now complete, messages and events can be send and received
	logger->information("Socket.io client setup complete\n");

	//test the message sending
	logger->information("Sending message string");
	sio->send("Message - String");
  logger->information("Sending message json\n");
	sio->send("{\"name\":\"myname\",\"type\":\"mytype\"}");

	//test the event emitter, this will return the same event so let's register a callback too
	sio->on("chat", target, callback(&TestTarget::onEvent));
  logger->information("Emit \"chat\" event with string");
	sio->emit("chat","Event - String");
  logger->information("Emit \"chat\" event with json");
	sio->emit("chat", "[{\"name\":\"myname\",\"type\":\"mytype\"}]");
	
	//test connecting to an endpoint 'testpoint'
	//~ TestEndpointTarget *target2 = new TestEndpointTarget();
	//~ SIOClient *testpoint = SIOClient::connect("http://localhost:3000/testpoint");
	//~ testpoint->send("Hello Socket.IO Testpoint");
	//~ testpoint->on("Update", target2, callback(&TestEndpointTarget::onUpdate));
	//~ testpoint->on("testevent", target2, callback(&TestEndpointTarget::ontestevent));
	//~ testpoint->emit("testevent", "[{\"name\":\"myname\",\"type\":\"mytype\"}]");

	//wait for user input to move to next section of code
	//socket receiving occurs in another thread and will not be halted
	logger->information("Press ENTER to continue...");
	std::cin.get();

	//test disconnecting a single endpoint, other endpoints stay connected
	//~ testpoint->disconnect();

	//disconnecting the default socket with no endpoint will also disconnect all endpoints
	sio->disconnect();

	logger->information("Press ENTER to quit...");
	std::cin.get();

	return 0;
}

