#ifndef __WIFI_MANAGER_H__
#define __WIFI_MANAGER_H__

#include <ESP8266WiFi.h>

class WifiManager
{
    public:

    bool setup(const char* ssid, const char* pwd);
    inline IPAddress getLocalIp() { return WiFi.localIP(); }

    WiFiClient& getWifiClient() { return m_wifiClient; }

    private:

    WiFiClient m_wifiClient;
};

#endif
