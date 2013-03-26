// TestClient.cpp : Defines the entry point for the console application.
//
#include "StdAfx.h"

#include "SIOClient.h"
#include "Poco/RunnableAdapter.h"
#include "Poco/Thread.h"
#include "SIONotificationHandler.h"

#include <iostream>

using Poco::Thread;

Poco::Thread;

int _tmain(int argc, char* argv[])
{
	NotificationCenter *nc = new NotificationCenter;
	SIONotificationHandler *sioHandler = new SIONotificationHandler(nc);
	SIOClient *sio = new SIOClient(3000, "localhost", nc);
	

	sio->connect();

	Thread thread;

	Poco::RunnableAdapter<SIOClient> runnable(*sio, &SIOClient::monitor);

	thread.start(runnable);

	do{

	}while(true);

	return 0;
}

