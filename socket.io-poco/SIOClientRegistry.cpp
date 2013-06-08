#include "SIOClientRegistry.h"
#include "SIOClient.h"

SIOClientRegistry *SIOClientRegistry::_inst = NULL;

SIOClientRegistry *SIOClientRegistry::instance() {
	
	if(!_inst)
		_inst = new SIOClientRegistry();
	
	return _inst;

}

SIOClient *SIOClientRegistry::getClient(std::string uri) {

	SIOClient *c = NULL;

	std::map<std::string,SIOClient *>::iterator it = _clientMap.find(uri);
	if(it != _clientMap.end())
    {
		
		 c = it->second;

    }

	return c;

}

void SIOClientRegistry::addClient(SIOClient *client) {

	_clientMap[client->getUri()] = client;

}

void SIOClientRegistry::removeClient(std::string uri) {
	_clientMap.erase(uri);
}

SIOClientImpl *SIOClientRegistry::getSocket(std::string uri) {

	SIOClientImpl *c = NULL;

	std::map<std::string,SIOClientImpl *>::iterator it = _socketMap.find(uri);
	if(it != _socketMap.end())
    {
		
		 c = it->second;

    }

	return c;

}

void SIOClientRegistry::addSocket(SIOClientImpl *socket, std::string uri) {

	_socketMap[uri] = socket;

}

void SIOClientRegistry::removeSocket(std::string uri) {
	_socketMap.erase(uri);
}