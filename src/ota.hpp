/*
 * OtaLogger.hpp
 * Wraps ArduinoOTA into a class and shows the OTA
 * status on serial or an OLED display.
 *
 * Version: 1.1
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#pragma once

#include <Arduino.h>
#include "ArduinoOTA.h"

typedef std::function<void(void)> TOtaCallback;

// -----------------------------------------------------------------------------
// OTA info via logger
// -----------------------------------------------------------------------------

class OtaLogger
{
private:
  TOtaCallback _otaCallback;

protected:
  Print *_logger;
  String getErrorMessage(ota_error_t error);

  virtual void onStart();
  virtual void onEnd();
  virtual void onError(ota_error_t error);
  virtual void onProgress(unsigned int progress, unsigned int total);

public:
  explicit OtaLogger(TOtaCallback otaCallback = NULL);

  virtual void setup(uint16_t port = 8266, const char *password = "");
  virtual void loop();
};

// -----------------------------------------------------------------------------
// Base class for the two loggers using an OLED display
// -----------------------------------------------------------------------------

class OtaDisplay : public OtaLogger
{
protected:
  virtual void drawMessage(const char *message);
  void onStart() override;
  void onEnd() override;
  void onError(ota_error_t error) override;

public:
  explicit OtaDisplay(TOtaCallback otaCallback = NULL);
};

// -----------------------------------------------------------------------------
// OTA info via 128x64 OLED Display using the esp8266-oled-ssd1306 library
// -----------------------------------------------------------------------------
#if defined(DISPLAY_SSD1306)

#include <SSD1306Wire.h> 
#define cMediumFont ArialMT_Plain_16

class OtaDisplaySSD1306 : public OtaDisplay
{
private:
  OLEDDisplay &_display;

protected:
  void drawMessage(const char *message) override;
  void onProgress(unsigned int progress, unsigned int total) override;

public:
  explicit OtaDisplaySSD1306(OLEDDisplay &display, TOtaCallback otaCallback = NULL);

  void setup(uint16_t port = 8266, const char *password = "") override;
};

// -----------------------------------------------------------------------------
// OTA info via 128x32 OLED Display using the u8g2 library
// -----------------------------------------------------------------------------
#elif defined(DISPLAY_U8G2)

#include <u8g2lib.h> 
#define cMediumFont u8g2_font_helvR14_tf

class OtaDisplayU8G2 : public OtaDisplay
{
private:
  U8G2 &_display;
  uint8_t _height;
  uint8_t _width;
  uint8_t _baseLine;
  uint8_t _progress;

protected:
  void drawMessage(const char *message) override;
  void onProgress(unsigned int progress, unsigned int total) override;

public:
  explicit OtaDisplayU8G2(U8G2 &display, TOtaCallback otaCallback = NULL);

  void setup(uint16_t port = 8266, const char *password = "") override;
};
#endif
