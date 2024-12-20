#include <NetworkConfig.hpp>

NetworkConfig::NetworkConfig(const IPAddress& local_IP, const IPAddress& gateway, const IPAddress& subnet)
:   local_IP(local_IP),
    gateway(gateway),
    subnet(subnet)
{}