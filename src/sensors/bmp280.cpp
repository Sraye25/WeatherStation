#include "bme280.h"

#define BME280_ADDR 0x76

bool Bme280::setup()
{
    unsigned status = m_bme.begin(BME280_ADDR);
    if (!status) return false;

    /* Default settings from datasheet. */
    m_bme.setSampling(Adafruit_BME280::MODE_FORCED,
                      Adafruit_BME280::SAMPLING_X1, // temperature
                      Adafruit_BME280::SAMPLING_X1, // pressure
                      Adafruit_BME280::SAMPLING_X1, // humidity
                      Adafruit_BME280::FILTER_OFF); /* Standby time. */

    return true;
}
