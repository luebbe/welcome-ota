#include "welcome.hpp"

// -----------------------------------------------------------------------------
// Base class using just serial logging
// -----------------------------------------------------------------------------

Welcome::Welcome(const char *fw_name, const char *fw_version)
    : _fw_name(fw_name), _fw_version(fw_version)
{
}

void Welcome::show()
{
  delay(1000);
  Serial.println();
  Serial.printf("Device: %s\r\n", (char *)WiFi.hostname().c_str());
  Serial.printf("ChipID: %06X\r\n", ESP.getChipId());
  Serial.printf("Last reset reason: %s\r\n", (char *)ESP.getResetReason().c_str());
  Serial.printf("Memory size: %d bytes\r\n", ESP.getFlashChipSize());
  Serial.printf("Free heap: %d bytes\r\n", ESP.getFreeHeap());
  FSInfo fs_info;
  if (SPIFFS.info(fs_info))
  {
    Serial.printf("File system total size: %d bytes\r\n", fs_info.totalBytes);
    Serial.printf("            used size : %d bytes\r\n", fs_info.usedBytes);
    Serial.printf("            block size: %d bytes\r\n", fs_info.blockSize);
    Serial.printf("            page size : %d bytes\r\n", fs_info.pageSize);
    Serial.printf("            max files : %d\r\n", fs_info.maxOpenFiles);
    Serial.printf("            max length: %d\r\n", fs_info.maxPathLength);
  }
  Serial.println();
  Serial.println();
}

// -----------------------------------------------------------------------------
// Welcome message via 128x64 OLED Display using the esp8266-oled-ssd1306 library
// -----------------------------------------------------------------------------
#if defined(DISPLAY_SSD1306)
WelcomeSSD1306::WelcomeSSD1306(OLEDDisplay &display, const char *fw_name, const char *fw_version)
    : Welcome(fw_name, fw_version), _display(display)
{
}

void WelcomeSSD1306::show()
{
  Welcome::show();

  // initialize display
  _display.init();
  _display.clear();
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
