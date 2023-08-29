#include <Arduino.h>
#include <PubSubClient.h>

#include "sensors/bme280.h"
#include "wifi/wifiManager.h"

/**
 * On D1 mini : D1 -> SDL / D2 -> SDA
 * TODO : Customise PIN to use
 */
Bme280 bme;
WifiManager wifi;

PubSubClient mqttClient(wifi.getWifiClient());

void mqttCallback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Recv MQTT [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
    }
    Serial.println();
}

void setup()
{
    Serial.begin(9600);

    if (!bme.setup())
    {
        Serial.println("Could not find a valid BME280 sensor, check wiring or try a different address!");
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

    //Setup MQTT client
    mqttClient.setServer(MQTT_SERVER_IP, MQTT_SERVER_PORT);
    mqttClient.setCallback(mqttCallback);
}

void publishMqttDatas(float temperature, float pressure, float humidity)
{
    //Connect to MQTT server
    while(!mqttClient.connected())
    {
        Serial.print("Attente MQTT connection...");
 
        // test connexion
        if (mqttClient.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PWD))
        {
            Serial.println("connected");
        }
        else
        {
            Serial.print("ECHEC, rc=");
            Serial.print(mqttClient.state());
            Serial.println(" nouvelle tentative dans 5 secondes");
            delay(5000);
        }
    }

    //Send datas
    mqttClient.publish("weatherstation/temperature", String(temperature).c_str());
    mqttClient.publish("weatherstation/pressure", String(pressure).c_str());
    mqttClient.publish("weatherstation/humidity", String(humidity).c_str());

    mqttClient.disconnect();
}

void loop()
{
    if (bme.measure())
    {
        Serial.print(F("Temperature = "));
        Serial.print(bme.readTemperature());
        Serial.println(" *C");

        Serial.print(F("Pressure = "));
        Serial.print(bme.readPressure());
        Serial.println(" Pa");

        Serial.print(F("Humidity = "));
        Serial.print(bme.readHumidity());
        Serial.println(" %");

        Serial.println();

        publishMqttDatas(bme.readTemperature(), bme.readPressure(), bme.readHumidity());

        delay(2000);
    }
    else
    {
        Serial.println("Forced measurement failed!");
    }
}