#include <RoteadorWifi.hpp>


RoteadorWiFi::RoteadorWiFi(const NetworkConfig& networkConfig)
:   _networkConfig(networkConfig)
{}

void RoteadorWiFi::begin(void) 
{
    _ssid += rand();

    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(
        _networkConfig.local_IP, _networkConfig.gateway, _networkConfig.subnet);
    WiFi.softAP(_ssid, _psk, _channel, _ssid_hidden, _max_connection);
}

