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
  Homie.getLogger()
      << endl
      << endl
      << "Device: " << WiFi.hostname() << endl
      << "ChipID: " << ESP.getChipId() << endl
      << "Last reset reason: " << ESP.getResetReason() << endl
      << "Memory size: " << ESP.getFlashChipSize() << " bytes" << endl
      << "Free heap: " << ESP.getFreeHeap() << " bytes" << endl;

  FSInfo fs_info;
  if (SPIFFS.info(fs_info))
  {
    Homie.getLogger()
        << "File system total size: " << fs_info.totalBytes << " bytes" << endl
        << "            used size : " << fs_info.usedBytes << " bytes" << endl
        << "            block size: " << fs_info.blockSize << " bytes" << endl
        << "            page size : " << fs_info.pageSize << " bytes" << endl
        << "            max files : " << fs_info.maxOpenFiles << endl
        << "            max length: " << fs_info.maxPathLength << endl;
  }
  Homie.getLogger() << endl
                    << endl;
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
