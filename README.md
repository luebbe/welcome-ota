# Homie OTA
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

Basic OTA example using Homie and ArduinoOTA

OTA progress is shown on the serial console and, when an OLED display is connected and configured properly, also on the display

The software is based on [Homie (v2.0)](https://github.com/marvinroger/homie-esp8266) and is developed using [PlatformIO](https://github.com/platformio)

The Homie 2.0 framework https://github.com/marvinroger/homie-esp8266.git, is *not* included as lib_dep in platformio.ini, because I installed it into the global PlatformIO library storage.

