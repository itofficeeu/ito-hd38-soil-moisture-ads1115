/*
 * This program reads continous the moisture in percent determined by a HD-38 soil
 * moisture sensor via a ADS1115 and outputs it in the Arduino monitor, alias terminal.
 * 
 * Measuring the moisture in soil is a more complex task. The sensor HD-38 measures 
 * the resistance and outputs a corresponding voltage. This is converted to a
 * simple linear percent scale. It is only a naive and fast approximation
 * for measuring the moisture in soil. Other methods exist that might be more correct.
 * 
 * The software is written with adaptability in mind, and shows the calculations 
 * behind the output in a way where it is possible to understand and modify these.
 * 
 * The precision is dependent on your own calibration.
 * 
 * 
 * SOFTWARE
 * 
 * C/C++ in the Arduino IDE
 * 
 * Author: Andreas Chr. Dyhrberg @itoffice.eu
 * 
 * Licens: This software may be distributed and modified under the terms of the 
 * GNU General Public License version 2 (GPL2) as published by the Free Software 
 * Foundation and appearing in the file GPL2.TXT included in the packaging of 
 * this file. Please note that GPL2 Section 2[b] requires that all works based 
 * on this software must also be made publicly available under the terms of the 
 * GPL2 ("Copyleft").
 * 
 * If you have the Heltec WiFi Lora 32 and use pin 4,15 for I2C, then 
 * go into the file "Arduino/libraries/Adafruit_ADS1X15/Adafruit_ADS1015.cpp"
 * and change "Wire.begin()" to this:
 * 
 * void Adafruit_ADS1015::begin() {
 *   Wire.begin(4,15);
 * }
 * 
 * Not best practise, but works as a fast fix just to get it running for now.
 * 
 * 
 * HARDWARE
 * 
 * A HD-38 soil moisture sensor mounted on the Minimal-X Extension Board from itoffice.eu
 * 
 * ESP32 in a Heltec WiFi Lora 32 that runs on top of the Minimal-X Extension Board.
 */

#include <Wire.h>
#include <Adafruit_ADS1015.h>
Adafruit_ADS1115 ads(0x48);

/* Limitations of the ADS1115 */
float ads1115_analog_volt_limit  = 6.144; /* Volt - Maximal voltage rated for the pin */
float ads1115_analog_read_limit  = 32767; /* Esp32: 4095, Arduino: 1023, ADS1115: 32767 (positive part of the 16 bit signed) */

/* The circuit on the Minimal-X Extension Board where the sensor is inserted. */
float voltage_divider_r1 = 26850; /* Ohm */
float voltage_divider_r2 = 26850; /* Ohm */
float voltage_divider_ratio = voltage_divider_r2 / (voltage_divider_r1+voltage_divider_r2); /* In this case is the result 0.5 */

/* Implications of the ADS1115 and the extension board combined. */
float voltage_divider_volt_limit = ads1115_analog_volt_limit / voltage_divider_ratio;

/* Moisture scale */
float moisture_scale_voltage_maximum = 0.1700;
float moisture_scale_voltage_minimum = 0.7859;
float moisture_scale_range = moisture_scale_voltage_maximum - moisture_scale_voltage_minimum;

void setup()
{
  Serial.begin(115200);
  ads.begin();
  /* Print a header */
  /* - Using columns make it easy to copy-paste into a spreadsheet. */
  Serial.print("     Volt ;");
  Serial.print(" Humidity/Percent ;");
  Serial.println("");
}

void loop()
{
  /* Read the raw integer value from the ADS1115. */
  int16_t analog_read_value = ads.readADC_SingleEnded(0);

  /* Convert the analog_read_value to a voltage. - ADS1115 + Minimal-X Extension Board */
  float analog_read_volt = (analog_read_value / ads1115_analog_read_limit ) * voltage_divider_volt_limit;

  /* Calculate the moisture in percent based on the voltage output of the sensor. */
  float moisture_fraction = analog_read_volt -moisture_scale_voltage_minimum;
  float hd38_moisture_percent = (moisture_fraction *100) / moisture_scale_range;

  if (hd38_moisture_percent<0) {
    hd38_moisture_percent = 0;
  }

  if (hd38_moisture_percent>100) {
    hd38_moisture_percent = 100;
  }

  /* The value hd38_moisture_percent will still fluctuate 
   * if you haven't attached a capacitor to avoid noise.
   * A running mean, or a mean of 10 samples could maybe also help. */

  Serial.print("   ");
  Serial.print(analog_read_volt,4);

  Serial.print(" ;          ");
  Serial.print(hd38_moisture_percent,1);

  Serial.println("");
  delay (1000);
}
