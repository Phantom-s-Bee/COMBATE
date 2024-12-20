#include <Servidor.hpp>

Servidor::Servidor(const NetworkConfig& networkConfig)
:   _networkConfig(networkConfig)
{}

void Servidor::begin(void) 
{
    _dnsServer.start(_DNS_PORT, "*", _networkConfig.local_IP);
}

void Servidor::update(void) 
{
    _dnsServer.processNextRequest();
}

// String responseHTML = ""
                    //   "<!DOCTYPE html><html lang='pt-br'><head>"
                    //   "<meta name='viewport' content='width=device-width'>"
                    //   "<title>Hello World</title></head><body>"
                    //   "<h1>Hello World</h1>"
                    //   "</body></html>";