// TestClient.cpp : Defines the entry point for the console application.
//
#include "StdAfx.h"

#include "SIOClient.h"

#include <iostream>

int _tmain(int argc, char* argv[])
{
	SIOClient *sio = new SIOClient(3000, "localhost");

	sio->pauser();

	return 0;
}

