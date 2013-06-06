#pragma once

#include <map>

class SIOClient;
class SIOClientImpl;

class SIOClientRegistry
{
private:
	SIOClientRegistry() {};
	~SIOClientRegistry() {delete _inst;};

	static SIOClientRegistry *_inst;

	std::map<std::string, SIOClient *> _clientMap;
	std::map<std::string, SIOClientImpl *> _socketMap;

public:
	static SIOClientRegistry *instance();

	SIOClient *getClient(std::string uri);
	void addClient(SIOClient *client);

	SIOClientImpl *getSocket(std::string uri);
	void addSocket(SIOClientImpl *socket, std::string uri);
	
};

