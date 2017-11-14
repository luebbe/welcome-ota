# Homie OTA
Small helpers for OTA (over the air update) and status display.

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

## main.cpp
Basic OTA example using Homie and ArduinoOTA using one of the three classes from ota.cpp

These are additions for software Projects using the [Homie (v2.0)](https://github.com/marvinroger/homie-esp8266) framework. 
Developed using [PlatformIO](https://github.com/platformio)

## ota.cpp
Helper classes for that display the over the air update status using different display drivers. Uncomment the desired version in platformio.ini
* plain serial using Homie.getlogger()
* [ssd1306 library](https://github.com/squix78/esp8266-oled-ssd1306.git)
* [u8g2 library](https://github.com/olikraus/u8g2.git)

## welcome.cpp
Helper that displays info about the esp, reset reason and more on the serial console upon boot.
