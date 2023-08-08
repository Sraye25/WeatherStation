#include <Arduino.h>

#include "sensors/bmp280.h"
#include "wifi/wifiManager.h"

/**
 * On D1 mini : D1 -> SDL / D2 -> SDA
 * TODO : Customise PIN to use
 */
Bmp280 bmp;
WifiManager wifi;

void setup()
{
    Serial.begin(9600);

    if (!bmp.setup())
    {
        Serial.println("Could not find a valid BMP280 sensor, check wiring or try a different address!");
        while (1) delay(10);
    }

    if(!wifi.setup(SSID_NAME,SSID_PASSWORD))
    {
        Serial.print("No connection to ssid = ");
        Serial.println(SSID_NAME);
        while (1) delay(10); //TODO : retry
    }

    Serial.println(); Serial.print("Connected with local ip ");
    Serial.println(wifi.getLocalIp());
}

void loop()
{
    if (bmp.measure())
    {
        Serial.print(F("Temperature = "));
        Serial.print(bmp.readTemperature());
        Serial.println(" *C");

        Serial.print(F("Pressure = "));
        Serial.print(bmp.readPressure());
        Serial.println(" Pa");

        Serial.print(F("Approx altitude = "));
        Serial.print(bmp.readAltitude(1013.25));
        Serial.println(" m");

        Serial.println();
        delay(2000);
    }
    else
    {
        Serial.println("Forced measurement failed!");
    }

}