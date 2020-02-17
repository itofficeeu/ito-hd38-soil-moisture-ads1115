# ito-hd38-soil-moisture-ads1115

This program reads continous the moisture in percent determined by a HD-38 soil moisture sensor via a ADS1115 and outputs it in the Arduino monitor, alias terminal.

Measuring the moisture in soil is a more complex task. The sensor HD-38 measures the resistance and outputs a corresponding voltage. This is converted to a simple linear percent scale. It is only a naive and fast approximation for measuring the moisture in soil. Other methods exist that might be more correct.

The software is written with adaptability in mind, and shows the calculations behind the output in a way where it is possible to understand and modify these.

The precision is dependent on your own calibration.


# Software

C/C++ in the Arduino IDE

Author: Andreas Chr. Dyhrberg @itoffice.eu

Licens: This software may be distributed and modified under the terms of the GNU General Public License version 2 (GPL2) as published by the Free Software Foundation and appearing in the file GPL2.TXT included in the packaging of this file. Please note that GPL2 Section 2[b] requires that all works based on this software must also be made publicly available under the terms of the GPL2 ("Copyleft").

If you have the Heltec WiFi Lora 32 and use pin 4,15 for I2C, then go into the file "Arduino/libraries/Adafruit_ADS1X15/Adafruit_ADS1015.cpp" and change "Wire.begin()" to this:

void Adafruit_ADS1015::begin() {
  Wire.begin(4,15);
}

Not best practise, but works as a fast fix just to get it running for now.


# Hardware

A HD-38 soil moisture sensor mounted on the Minimal-X Extension Board from https://itoffice.eu/.

ESP32 in a Heltec WiFi Lora 32 that runs on top of the Minimal-X Extension Board from https://itoffice.eu/.
