#pragma once

#include <string>

class SIOClient
{
private:

public:
	SIOClient(void);
	SIOClient(int port, std::string host);
	~SIOClient(void);

	SIOClient* init(int port, std::string host);
};

