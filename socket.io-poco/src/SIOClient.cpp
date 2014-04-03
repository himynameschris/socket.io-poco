#include "SIOClient.h"
#include "SIOClientRegistry.h"

#include "Poco/URI.h"

using Poco::NotificationCenter;
using Poco::URI;
using Poco::JSON::Object;

sio_poco::SIOClient::SIOClient(std::string uri, std::string endpoint, SIOClientImpl *impl)
	: _uri(uri), _endpoint(endpoint), _socket(impl)
{
	_socket->addref();

	_nCenter = new NotificationCenter;
	_sioHandler = new SIONotificationHandler(_nCenter);

	_registry = new SIOEventRegistry();
}

sio_poco::SIOClient::~SIOClient() {
	_socket->release();
	delete(_sioHandler);
	delete(_nCenter);
	delete(_registry);

	SIOClientRegistry::instance()->removeClient(_uri);
}

sio_poco::SIOClient* 
sio_poco::SIOClient::connect(std::string uri) {

	//check if connection to endpoint exists 
	URI tmp_uri(uri);
	std::stringstream ss;
	ss << tmp_uri.getHost() << ":" << tmp_uri.getPort() << tmp_uri.getPath();
	std::string fullpath = ss.str();
	SIOClient *c = SIOClientRegistry::instance()->getClient(fullpath);

	if(!c) {

		//check if connection to socket already exists
		ss.str("");
		ss.clear();
		ss << tmp_uri.getHost() << ":" << tmp_uri.getPort();
		std::string spath = ss.str();
		SIOClientImpl *impl = SIOClientRegistry::instance()->getSocket(spath);

		if(!impl) {

			impl = SIOClientImpl::connect(tmp_uri.getHost(), tmp_uri.getPort());

			if (!impl) return NULL; //connect failed

			SIOClientRegistry::instance()->addSocket(impl, spath);
			
		} 
		
		if(tmp_uri.getPath() != "") {
			impl->connectToEndpoint(tmp_uri.getPath());
		}

		c = new SIOClient(fullpath, tmp_uri.getPath(), impl);
		SIOClientRegistry::instance()->addClient(c);
		
	}

	//TODO: add method to handle force new connection
	return c;

}

void 
sio_poco::SIOClient::disconnect() {
	_socket->disconnect(_endpoint);
	delete this;
}

std::string 
sio_poco::SIOClient::getUri() {

	return _uri;

}

NotificationCenter* 
sio_poco::SIOClient::getNCenter() {
	return _nCenter;
}

void 
sio_poco::SIOClient::on(const char *name, SIOEventTarget *target, callback c) {
	_registry->registerEvent(name, target, c);
}

void 
sio_poco::SIOClient::fireEvent(const char * name, Object::Ptr args) {

	_registry->fireEvent(this, name, args);

}

void 
sio_poco::SIOClient::send(std::string s) {

	_socket->send(_endpoint, s);

}

void 
sio_poco::SIOClient::emit(std::string eventname, std::string args) {

	_socket->emit(_endpoint, eventname, args);

}
