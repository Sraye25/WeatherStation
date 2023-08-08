#include "bmp280.h"

#define BMP280_ADDR 0x76

bool Bmp280::setup()
{
    unsigned status = m_bmp.begin(BMP280_ADDR);
    if (!status) return false;

    /* Default settings from datasheet. */
    m_bmp.setSampling(Adafruit_BMP280::MODE_FORCED,     /* Operating Mode. */
                    Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                    Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                    Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                    Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

    return true;
}
