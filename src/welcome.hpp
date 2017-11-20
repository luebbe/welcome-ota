#pragma once

#include <Arduino.h>
#include <Homie.h>

// -----------------------------------------------------------------------------
// Base class using just serial logging
// -----------------------------------------------------------------------------

class Welcome
{
protected:
  const char *_fw_name;
  const char *_fw_version;

public:
  Welcome(const char *fw_name, const char *fw_version);
  virtual void show();
};

// -----------------------------------------------------------------------------
// Welcome message via 128x64 OLED Display using the esp8266-oled-ssd1306 library
// -----------------------------------------------------------------------------
#if defined(DISPLAY_SSD1306)

#include <SSD1306Wire.h>
#define cMediumFont ArialMT_Plain_10

class WelcomeSSD1306 : public Welcome
{
private:
  OLEDDisplay &_display;

public:
  WelcomeSSD1306(OLEDDisplay &display, const char *fw_name, const char *fw_version);
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
