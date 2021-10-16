#include "Battery.h"
#include <math.h>

Battery::Battery(adc1_channel_t adc_channel) : m_adc_channel(adc_channel)
{
  // see https://github.com/espressif/esp-idf/commit/d890a516a1097f0a07788e203fdb1a82bb83520e
  // though this doesn't seem to be required with the current
  // button handling code which is pretty robust
  // adc_power_acquire();
  adc1_config_width(ADC_WIDTH_12Bit);
  adc1_config_channel_atten(m_adc_channel, ADC_ATTEN_DB_11);
  esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &m_adc_chars);
}

float Battery::get_voltage()
{
  // get the adc calibration
  auto adc_value = adc1_get_raw(m_adc_channel);
  // get the actual calibrated voltage
  auto voltage = 2 * esp_adc_cal_raw_to_voltage(adc_value, &m_adc_chars);
  return voltage;
}

float Battery::get_percentage()
{
  auto voltage = get_voltage() / 1000.0f;
  if (voltage >= 4.20)
  {
    return 100;
  }
  if (voltage <= 3.50)
  {
    return 0;
  }
  // see here for inspiration: https://github.com/G6EJD/ESP32-e-Paper-Weather-Display/issues/146
  return 2836.9625 * pow(voltage, 4) - 43987.4889 * pow(voltage, 3) + 255233.8134 * pow(voltage, 2) - 656689.7123 * voltage + 632041.7303;
}