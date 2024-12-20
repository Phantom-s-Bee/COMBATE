#ifndef _SERVIDOR_HPP_
#define _SERVIDOR_HPP_

#include <NetworkConfig.hpp>

#include <DNSServer.h>
#include <ESP8266WebServer.h>


class Servidor 
{
private:
    NetworkConfig _networkConfig;

    const byte _DNS_PORT{53};
    DNSServer _dnsServer;

    ESP8266WebServer _server{80};

public:
    
    Servidor(const NetworkConfig&);

    void begin(void);

    void update(void);
};


#endif