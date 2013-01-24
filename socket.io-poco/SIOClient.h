#pragma once

#include <string>

class SIOClient
{
private:
	std::string _sid;
	std::string _host;
	int _port;

public:
	SIOClient(void);
	SIOClient(int port, std::string host);
	~SIOClient(void);

	bool handshake();
	bool init();
};

