#ifndef __BMP_280_H__
#define __BMP_280_H__

#include <Adafruit_BMP280.h>

class Bmp280 {

    public:

    Bmp280() {}

    bool setup();
    bool measure() { return m_bmp.takeForcedMeasurement(); }
    float readTemperature() { return m_bmp.readTemperature(); }
    float readPressure() { return m_bmp.readPressure(); }
    float readAltitude(float pressureSeaLevel) { return m_bmp.readAltitude(pressureSeaLevel); }

    private:

    Adafruit_BMP280 m_bmp;
};

#endif
