# Welcome - OTA

Small helpers for OTA (over the air update) and status display. Displays status information via serial upon starting. Displays OTA progres.

[![License](https://img.shields.io/github/license/mashape/apistatus.svg?style=flat)](https://opensource.org/licenses/MIT)
[![GitHub release](https://img.shields.io/github/release/luebbe/welcome-ota.svg?style=flat)](https://github.com/luebbe/welcome-ota/releases)

## main.cpp

Basic OTA example using and ArduinoOTA using one of the three classes from ota.cpp

Developed using [PlatformIO](https://github.com/platformio)

## ota.cpp

Helper classes that display the over the air update status on an OLED display using different drivers. Uncomment the desired version in platformio.ini

- plain serial 
- [ssd1306 library](https://github.com/ThingPulse/esp8266-oled-ssd1306)
- [u8g2 library](https://github.com/olikraus/u8g2.git)

## welcome.cpp

Helper that displays info about the esp, reset reason and more on the serial console upon boot. If an OLED display is attached, it displays the firmware name and version on the OLED display. It uses the same defines as `ota.cpp`
