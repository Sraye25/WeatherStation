#include <Arduino.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>

#include "sensors/bme280.h"

#define WIFI_DELAY_BETWEEN_TRIES_MS 500
#define DELAY_BTW_BME_MEASURE_IN_US 20e6

/**
 * On D1 mini : D1 -> SDL / D2 -> SDA
 * TODO : Customise PIN to use
 */
Bme280 _bme;

WiFiClient _wifiClient;
PubSubClient _mqttClient(_wifiClient);

void publishMqttDatas(float temperature, float pressure, float humidity);
void mqttCallback(char* topic, byte* payload, unsigned int length);

void wifiConnect();

//====================================================================================================================
void setup()
{
    Serial.begin(9600);

    if (!_bme.setup())
    {
        Serial.println("Could not find a valid BME280 sensor, check wiring or try a different address!");
        while (1) delay(10);
    }

    //Setup MQTT client
    _mqttClient.setServer(MQTT_SERVER_IP, MQTT_SERVER_PORT);
    _mqttClient.setCallback(mqttCallback);
}

//====================================================================================================================
void loop()
{
    //Connect to Wifi after sleep
    wifiConnect();

    if (_bme.measure())
    {
        Serial.print(F("Temperature = "));
        Serial.print(_bme.readTemperature());
        Serial.println(" *C");

        Serial.print(F("Pressure = "));
        Serial.print(_bme.readPressure());
        Serial.println(" Pa");

        Serial.print(F("Humidity = "));
        Serial.print(_bme.readHumidity());
        Serial.println(" %");

        Serial.println();

        publishMqttDatas(_bme.readTemperature(), _bme.readPressure(), _bme.readHumidity());
    }
    else
    {
        Serial.println("Forced measurement failed!");
    }

    Serial.print("Preparing to sleep ...");
    ESP.deepSleep(DELAY_BTW_BME_MEASURE_IN_US);
}

//====================================================================================================================
void wifiConnect()
{
    Serial.print("Connecting to Wifi ...");
    WiFi.begin(SSID_NAME,SSID_PASSWORD);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(WIFI_DELAY_BETWEEN_TRIES_MS);
    }

    Serial.println(); Serial.print("Connected with local ip ");
    Serial.println(WiFi.localIP());
}

//====================================================================================================================
void publishMqttDatas(float temperature, float pressure, float humidity)
{
    //Connect to MQTT server
    while(!_mqttClient.connected())
    {
        Serial.print("Waiting MQTT connection...");
 
        // test connexion
        if (_mqttClient.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PWD))
        {
            Serial.println("connected");
        }
        else
        {
            Serial.print("KO, rc=");
            Serial.print(_mqttClient.state());
            Serial.println("Retry in 5 seconds ...");
            delay(5000);
        }
    }

    //Send datas
    _mqttClient.publish("weatherstation/temperature", String(temperature).c_str());
    _mqttClient.publish("weatherstation/pressure", String(pressure).c_str());
    _mqttClient.publish("weatherstation/humidity", String(humidity).c_str());

    _mqttClient.disconnect();
}

//====================================================================================================================
void mqttCallback(char* topic, byte* payload, unsigned int length)
{
    Serial.print("Recv MQTT [");
    Serial.print(topic);
    Serial.print("] ");
    for (unsigned int i = 0; i < length; i++) Serial.print((char)payload[i]);
    Serial.println();
}