#include "SIOClientRegistry.h"
#include "SIOClient.h"

sio_poco::SIOClientRegistry *
sio_poco::SIOClientRegistry::_inst = NULL;

sio_poco::SIOClientRegistry *
sio_poco::SIOClientRegistry::instance() {
	
	if(!_inst)
		_inst = new SIOClientRegistry();
	
	return _inst;

}

sio_poco::SIOClient *
sio_poco::SIOClientRegistry::getClient(std::string uri) {

	SIOClient *c = NULL;

	std::map<std::string,SIOClient *>::iterator it = _clientMap.find(uri);
	if(it != _clientMap.end())
    {
		
		 c = it->second;

    }

	return c;

}

void 
sio_poco::SIOClientRegistry::addClient(SIOClient *client) {

	_clientMap[client->getUri()] = client;

}

void 
sio_poco::SIOClientRegistry::removeClient(std::string uri) {
	_clientMap.erase(uri);
}

sio_poco::SIOClientImpl *
sio_poco::SIOClientRegistry::getSocket(std::string uri) {

	SIOClientImpl *c = NULL;

	std::map<std::string,SIOClientImpl *>::iterator it = _socketMap.find(uri);
	if(it != _socketMap.end())
    {
		
		 c = it->second;

    }

	return c;

}

void 
sio_poco::SIOClientRegistry::addSocket(SIOClientImpl *socket, std::string uri) {

	_socketMap[uri] = socket;

}

void 
sio_poco::SIOClientRegistry::removeSocket(std::string uri) {
	_socketMap.erase(uri);
}
