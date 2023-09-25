#include "welcome.hpp"

// -----------------------------------------------------------------------------
// Base class using just serial logging
// -----------------------------------------------------------------------------

Welcome::Welcome(const char *fw_name, const char *fw_version)
    : _fw_name(fw_name), _fw_version(fw_version), _logger(&Serial)
{
}

void Welcome::show()
{
  _logger->print("\r\n");
  _logger->printf("Device: %s\r\n", WiFi.hostname().c_str());
  _logger->printf("ChipID: %d\r\n", ESP.getChipId());
  _logger->printf("Last reset reason: %s\r\n", ESP.getResetReason().c_str());
  _logger->printf("Flash size: %d bytes\r\n", ESP.getFlashChipSize());
  _logger->printf("Free heap: %d bytes\r\n", ESP.getFreeHeap());

#if defined(SHOW_SPIFFS_INFO) || defined(SHOW_LITTLEFS_INFO)
  FSInfo fs_info;
#if defined(SHOW_SPIFFS_INFO)
  if (!SPIFFS.info(fs_info))
    _logger->printf("No SPIFFS file system found");
  else
#else
  if (!LittleFS.info(fs_info))
    _logger->printf("No LittleFS file system found");
  else
#endif
  {
    _logger->printf("total size: %d bytes", fs_info.totalBytes);
    _logger->printf("used size : %d bytes", fs_info.usedBytes);
    _logger->printf("block size: %d bytes", fs_info.blockSize);
    _logger->printf("page size : %d bytes", fs_info.pageSize);
    _logger->printf("max files : %d bytes", fs_info.maxOpenFiles);
    _logger->printf("max length: %d bytes", fs_info.maxPathLength);
  }
#endif
  _logger->print("\r\n");
}

// -----------------------------------------------------------------------------
// Welcome message via 128x64 OLED Display using the esp8266-oled-ssd1306 library
// -----------------------------------------------------------------------------
#if defined(DISPLAY_SSD1306)
WelcomeSSD1306::WelcomeSSD1306(OLEDDisplay &display, const char *fw_name, const char *fw_version, bool flipScreen)
    : Welcome(fw_name, fw_version), _display(display), _flipScreen(flipScreen)
{
}

void WelcomeSSD1306::show()
{
  Welcome::show();

  // initialize display
  _display.init();
  _display.clear();
  if (_flipScreen)
    _display.flipScreenVertically();
  _display.setColor(WHITE);
  _display.setFont(cMediumFont);
  _display.setTextAlignment(TEXT_ALIGN_CENTER);
  _display.drawString(64, DISPLAY_HEIGHT / 3 - 8, (String)_fw_name);
  _display.drawString(64, DISPLAY_HEIGHT * 2 / 3 - 8, (String)_fw_version);
  _display.display();
}

// -----------------------------------------------------------------------------
// Welcome message via OLED Display using the u8g2 library
// -----------------------------------------------------------------------------
#elif defined(DISPLAY_U8G2)
WelcomeU8G2::WelcomeU8G2(U8G2 &display, const char *fw_name, const char *fw_version)
    : Welcome(fw_name, fw_version), _display(display)
{
}

void WelcomeU8G2::show()
{
  Welcome::show();

  // initialize display
  _display.begin();

  _display.setDrawColor(1);
  _display.setFont(cMediumFont);

  uint8_t _strWidth = _display.getStrWidth(_fw_name);
  _display.drawUTF8((128 - _strWidth) / 2, 14, _fw_name);

  _strWidth = _display.getStrWidth(_fw_version);
  _display.drawUTF8((128 - _strWidth) / 2, 32, _fw_version);

  _display.sendBuffer();
}
#endif
