#include "wifiManager.h"

#define CONNECTION_TRIES 20
#define DELAY_BETWEEN_TRIES 500

bool WifiManager::setup(const char* ssid, const char* pwd)
{
    WiFi.begin(ssid, pwd);
    int tryNumber = 1;

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(DELAY_BETWEEN_TRIES);
        if (++tryNumber > CONNECTION_TRIES) {
            return false;
        }
    }

    return true; 
}

