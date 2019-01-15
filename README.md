# Homie OTA

Small helpers for OTA (over the air update) and status display.

[![Travis](https://img.shields.io/travis/luebbe/homie-ota.svg?branch=master&style=flat)](https://travis-ci.org/luebbe/homie-ota)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/016749a7abc84198bc8ec26a6ab9806a)](https://www.codacy.com/app/luebbe/homie-ota/dashboard)
[![License](https://img.shields.io/github/license/mashape/apistatus.svg?style=flat)](https://opensource.org/licenses/MIT)
[![GitHub release](https://img.shields.io/github/release/luebbe/homie-ota.svg?style=flat)](https://github.com/luebbe/homie-ota/releases)

## main.cpp

Basic OTA example using Homie and ArduinoOTA using one of the three classes from ota.cpp

These are additions for software Projects using the [Homie (v2.0)](https://github.com/marvinroger/homie-esp8266) framework.
Developed using [PlatformIO](https://github.com/platformio)

## ota.cpp

Helper classes that display the over the air update status on an OLED display using different drivers. Uncomment the desired version in platformio.ini

* plain serial using Homie.getlogger()
* [ssd1306 library](https://github.com/squix78/esp8266-oled-ssd1306.git)
* [u8g2 library](https://github.com/olikraus/u8g2.git)

## welcome.cpp

Helper that displays info about the esp, reset reason and more on the serial console upon boot. If an OLED display is attached, it displays the firmware name and version on the OLED display. It uses the same defines as ```ota.cpp```
