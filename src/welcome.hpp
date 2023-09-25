#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>

// #define SHOW_SPIFFS_INFO
// #define SHOW_LITTLEFS_INFO

#if defined(SHOW_SPIFFS_INFO) || defined(SHOW_LITTLEFS_INFO)
#include "FS.h"
#if defined(SHOW_LITTLEFS_INFO)
#include <LittleFS.h>
#endif
#endif

// -----------------------------------------------------------------------------
// Base class using just serial logging
// -----------------------------------------------------------------------------

class Welcome
{
protected:
  const char *_fw_name;
  const char *_fw_version;
  Print *_logger;

public:
  Welcome(const char *fw_name, const char *fw_version);
  virtual void show();
};

// -----------------------------------------------------------------------------
// Welcome message via 128x64 OLED Display using the esp8266-oled-ssd1306 library
// -----------------------------------------------------------------------------
#if defined(DISPLAY_SSD1306)

#include <SSD1306Wire.h>
#define cMediumFont ArialMT_Plain_16
#define DISPLAY_HEIGHT 64
#define DISPLAY_WIDTH 128

class WelcomeSSD1306 : public Welcome
{
private:
  OLEDDisplay &_display;
  bool _flipScreen;

public:
  WelcomeSSD1306(OLEDDisplay &display, const char *fw_name, const char *fw_version, bool flipScreen = false);
  void show() override;
};

// -----------------------------------------------------------------------------
// Welcome message via OLED Display using the u8g2 library
// -----------------------------------------------------------------------------
#elif defined(DISPLAY_U8G2)

#include <u8g2lib.h>
#define cMediumFont u8g2_font_helvR14_tf

class WelcomeU8G2 : public Welcome
{
private:
  U8G2 &_display;

public:
  WelcomeU8G2(U8G2 &display, const char *fw_name, const char *fw_version);
  void show() override;
};
#endif
