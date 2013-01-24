// TestClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SIOClient.h"



int _tmain(int argc, _TCHAR* argv[])
{
	SIOClient *sio = new SIOClient(3000, "localhost");



	return 0;
}

