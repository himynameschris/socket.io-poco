// main.cpp : Defines the entry point for the console application.
#include "SIOClient.h"

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

	NotificationCenter *nc = new NotificationCenter;
	SIONotificationHandler *sioHandler = new SIONotificationHandler(nc);
	SIOClient *sio = new SIOClient(3000, "localhost", nc);
	
	sio->connect();
		
	logger->information("sio setup complete, entering loop");
	
	do{

	}while(true);

	return 0;
}

