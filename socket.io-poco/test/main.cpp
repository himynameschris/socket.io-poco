#include "Poco/Logger.h"
#include "Poco/ConsoleChannel.h"
#include "Poco/Thread.h"

#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>

#include "talk/base/json.h"

#include "SIOClient.h"
#include "TestTarget.h"
#include "TestEndpointTarget.h"

#include <iostream>

using Poco::Thread;

int main(int argc, char* argv[])
{
	//create a c++ Poco logger to use and set its channel to the windows console
	//this is the same logger instance that the library will hook into
	Poco::Logger *logger = &(Poco::Logger::get("SIOClientLog"));
	if(argc > 1)
		logger->setChannel(new Poco::ConsoleChannel());
	
	TestTarget *target = new TestTarget();
	std::string server = "http://192.168.1.142:6001";
	
	//Establish the socket.io connection
	SIOClient *sio = SIOClient::connect(server);
	if(sio == NULL)
	{
		std::cout << "Wrong server" << std::endl;
		return EXIT_FAILURE;
	}
	sio->on("event", target, callback(&TestTarget::ontestevent));

	//Create a target and register object its method onUpdate for the Update event
	sio->on("message", target, callback(&TestTarget::onUpdate));

	//setup is now complete, messages and events can be send and received
	std::cout << "Socket.io setup complete" <<std::endl;

	sio->send("Hello world");
	sio->emit("event","Hello World");// pass V09x pass v10x
	sio->send("{\"type\" : \"message sendWithout backslash\"}"); //pass V09x pass V10x
	sio->send("{\\\"type\\\" : \\\"message sendWith backslash\\\"}"); //pass V10x pass V09x
	sio->emit("event","{\"type\" : \"emit Without backslash\"}");// pass V09x pass v10x
	sio->emit("event","{\\\"type\\\" : \\\"emit With backslash\\\"}");// pass V09x pass v10x

	Json::Value message;
	message["type"] = "json talk/base";
	Json::StyledWriter writer;
	std::string sJson = writer.write(message);
	sio->send(sJson);//pass v09x
	sio->emit("event",sJson);//pass v09x

	sleep(3);
	sio->disconnect();
	return EXIT_SUCCESS;
}

