#ifndef _NETWORK_CONFIG_HPP_
#define _NETWORK_CONFIG_HPP_

#include <IPAddress.h>

class NetworkConfig 
{
public:
    IPAddress local_IP;
    IPAddress gateway;
    IPAddress subnet;

    NetworkConfig();
    NetworkConfig(const IPAddress&, const IPAddress&, const IPAddress&);
};


#endif