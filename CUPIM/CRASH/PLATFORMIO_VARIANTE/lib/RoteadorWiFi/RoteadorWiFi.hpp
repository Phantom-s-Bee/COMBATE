#ifndef _ROTEADOR_WIFI_HPP_
#define _ROTEADOR_WIFI_HPP_

#include <ESP8266WiFi.h>
#include <NetworkConfig.hpp>


class RoteadorWiFi 
{
private:
    NetworkConfig _networkConfig;

    String _ssid{"CRASH: "};
    String _psk{emptyString};
    int _channel{1};
    int _ssid_hidden{0};
    int _max_connection{4};
    

public:
    RoteadorWiFi(const NetworkConfig&);

    void begin(void);
};


#endif