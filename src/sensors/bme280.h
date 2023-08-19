#ifndef __BME_280_H__
#define __BME_280_H__

#include <Adafruit_BME280.h>

class Bme280 {

    public:

    Bme280() {}

    bool setup();
    bool measure() { return m_bme.takeForcedMeasurement(); }
    float readTemperature() { return m_bme.readTemperature(); }
    float readPressure() { return m_bme.readPressure(); }
    float readHumidity() { return m_bme.readHumidity(); }

    private:

    Adafruit_BME280 m_bme;
};

#endif
