/*
 * OtaLogger.cpp
 * Wraps ArduinoOTA into a class and shows the OTA
 * status on serial or an OLED display.
 *
 * Version: 1.2
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#include "ota.hpp"

// -----------------------------------------------------------------------------
// OTA info via logger
// -----------------------------------------------------------------------------

OtaLogger::OtaLogger(TOtaCallback otaCallback)
    : _otaCallback(otaCallback), _logger(&Serial)
{
}

String OtaLogger::getErrorMessage(ota_error_t error)
{
  if (error == OTA_AUTH_ERROR)
    return "Auth Failed";
  else if (error == OTA_BEGIN_ERROR)
    return "Begin Failed";
  else if (error == OTA_CONNECT_ERROR)
    return "Connect Failed";
  else if (error == OTA_RECEIVE_ERROR)
    return "Receive Failed";
  else if (error == OTA_END_ERROR)
    return "End Failed";
  else
    return "Unknown Error";
};

void OtaLogger::setup(uint16_t port, const char *password)
{
  _logger->print("• OTA - Setup");
  ArduinoOTA.setPort(port);
  ArduinoOTA.setHostname(WiFi.hostname().c_str()); // Hostname defaults to esp8266-[ChipID]

  if (not strcmp(password, ""))
  {
    ArduinoOTA.setPassword(password);
  }

  ArduinoOTA.onStart([this]()
                     { onStart(); });

  ArduinoOTA.onEnd([this]()
                   { onEnd(); });

  ArduinoOTA.onProgress([this](unsigned int progress, unsigned int total)
                        { onProgress(progress, total); });

  ArduinoOTA.onError([this](ota_error_t error)
                     { onError(error); });

  ArduinoOTA.begin();
  _logger->println(" done");
};

void OtaLogger::loop()
{
  ArduinoOTA.handle();
};

void OtaLogger::onStart()
{
  _logger->println("• OTA - Start");
  if (_otaCallback)
  {
    _otaCallback();
  }
};

void OtaLogger::onEnd()
{
  _logger->println("• OTA - End");
};

void OtaLogger::onError(ota_error_t error)
{
  _logger->printf("• OTA - Error %s", getErrorMessage(error).c_str());
};

void OtaLogger::onProgress(unsigned int progress, unsigned int total)
{
  static unsigned int lastprogress = 0;
  unsigned int curprogress = (progress / (total / 100));
  if (lastprogress != curprogress)
  {
    lastprogress = curprogress;
    if (curprogress > 0)
    {
      _logger->print(".");
      if (curprogress % 25 == 0)
      {
        const char *sp;
        if (curprogress < 100)
        {
          sp = " ";
        }
        else
        {
          sp = "";
        };
        _logger->printf(" [%s%d%%]\r\n", sp, curprogress);
      }
    }
  }
};

// -----------------------------------------------------------------------------
// Base class for the two loggers using an OLED display
// -----------------------------------------------------------------------------

OtaDisplay::OtaDisplay(TOtaCallback otaCallback)
    : OtaLogger(otaCallback){};

void OtaDisplay::drawMessage(const char *message)
{
}

void OtaDisplay::onStart()
{
  OtaLogger::onStart();
  drawMessage("OTA Update...");
}

void OtaDisplay::onEnd()
{
  OtaLogger::onEnd();
  drawMessage("Rebooting...");
}

void OtaDisplay::onError(ota_error_t error)
{
  OtaLogger::onError(error);
  drawMessage(OtaLogger::getErrorMessage(error).c_str());
}

#if defined(DISPLAY_SSD1306)
// -----------------------------------------------------------------------------
// OTA info via SSD1306 128x64 OLED Display using the esp8266-oled-ssd1306 library
// -----------------------------------------------------------------------------

OtaDisplaySSD1306::OtaDisplaySSD1306(OLEDDisplay &display, TOtaCallback otaCallback)
    : OtaDisplay(otaCallback), _display(display){};

void OtaDisplaySSD1306::setup(uint16_t port, const char *password)
{
  _logger->println("• OTA - OtaDisplaySSD1306");
  OtaLogger::setup(port, password);
}

void OtaDisplaySSD1306::drawMessage(const char *message)
{
  _display.clear();
  _display.setTextAlignment(TEXT_ALIGN_CENTER);
  _display.setFont(cMediumFont);
  _display.drawString(64, 10, message);
  _display.display();
}

void OtaDisplaySSD1306::onProgress(unsigned int progress, unsigned int total)
{
  OtaLogger::onProgress(progress, total);
  _display.drawProgressBar(2, 30, 124, 8, progress / (total / 100));
  _display.display();
};

#elif defined(DISPLAY_U8G2)
// -----------------------------------------------------------------------------
// OTA info via OLED Display using the u8g2 library
// -----------------------------------------------------------------------------

OtaDisplayU8G2::OtaDisplayU8G2(U8G2 &display, TOtaCallback otaCallback)
    : OtaDisplay(otaCallback), _display(display)
{
  _height = _display.getHeight();
  _width = _display.getWidth();
  if (_height > 32)
  {
    _baseLine = _height / 2;
  }
  else
  {
    _baseLine = 32;
  }
  _baseLine -= 8; // subtract space for bar
  _progress = 0;
};

void OtaDisplayU8G2::setup(uint16_t port, const char *password)
{
  _logger->println("• OTA - OtaDisplayU8G2");
  OtaLogger::setup(port, password);
}

void OtaDisplayU8G2::drawMessage(const char *message)
{
  _display.clearBuffer();
  _display.setDrawColor(1);
  _display.setFontPosBottom();
  _display.setFont(cMediumFont);

  uint8_t _strWidth = _display.getStrWidth(message);
  _display.drawUTF8((_width - _strWidth) / 2, _baseLine - 2, message);

  _display.sendBuffer();
}

void OtaDisplayU8G2::onProgress(unsigned int progress, unsigned int total)
{
  OtaLogger::onProgress(progress, total);

  uint8_t curProgress = (progress * _width) / total;

  if (curProgress > _progress)
  {
    _progress = curProgress;
    _display.drawBox(0, _height - 8, _progress, 6);
    _display.sendBuffer();
  }
};
#endif