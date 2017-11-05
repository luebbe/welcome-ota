#include <Homie.h>
#include "homie-node-collection.h"

#define FW_VERSION "1.0.2"

// Uncomment the line corresponding to the OLED driver you want to use to
// show the OTA progress on the OLED display. Or none if you want just serial logging
#define DISPLAY_SSD1306
// #define DISPLAY_U8G2

const int PIN_SDA = 5;
const int PIN_SCL = 4;

// ======================================================================
// Setup OTA logging via SSD1306 OLED display driver and Homie logger
//
#if defined(DISPLAY_SSD1306)
#define FW_NAME "ota-ssd1306"
#include <SSD1306.h>

// Put your settings for the I2C bus here
const int I2C_DISPLAY_ADDRESS = 0x3c;

SSD1306Wire display(I2C_DISPLAY_ADDRESS, PIN_SDA, PIN_SCL);
OtaDisplaySSD1306 ota(display, NULL);

void showStartupInfo()
{
  // initlalize display
  display.init();
  display.clear();
  display.flipScreenVertically();
  display.setColor(WHITE);
  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, DISPLAY_HEIGHT / 3 - 8, (String)FW_NAME);
  display.drawString(64, DISPLAY_HEIGHT * 2 / 3 - 8, (String)FW_VERSION);
  display.display();
}

// ======================================================================
// Setup OTA logging via U8G2 OLED display driver and Homie logger
//
#elif defined(DISPLAY_U8G2)
#define FW_NAME "ota-u8g2"
#include <u8g2lib.h>

const int PIN_RESET = U8X8_PIN_NONE; // The reset pin of the SD1306 is not connected

// Using a 128x32 display
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, PIN_RESET, PIN_SCL, PIN_SDA);
OtaDisplayU8G2 ota(u8g2, NULL);

void showStartupInfo()
{
  u8g2.begin();

  u8g2.setDrawColor(1);
  u8g2.setFont(u8g2_font_helvR10_tf);

  uint8_t _strWidth = u8g2.getStrWidth(FW_NAME);
  u8g2.drawUTF8((128 - _strWidth) / 2, 12, FW_NAME);

  _strWidth = u8g2.getStrWidth(FW_VERSION);
  u8g2.drawUTF8((128 - _strWidth) / 2, 30, FW_VERSION);

  u8g2.sendBuffer();
}

// ======================================================================
// Setup OTA logging via Homie logger
//
#else
#define FW_NAME "ota-basic-serial"
OtaLogger ota;

void showStartupInfo()
{
  // nothing to show if no display is attached
}

#endif

void setupHandler()
{
  // This is called after the MQTT_CONNECTED event
  ota.setup();
}

void loopHandler()
{
  // This is only called by Homie when WiFi is connected
  ota.loop();
}

void setup()
{
  Serial.begin(SERIAL_SPEED);
  Serial << endl
         << endl;

  // show state and last reset reason
  welcome();

  showStartupInfo();

  Homie_setFirmware(FW_NAME, FW_VERSION);

  // before Homie.setup()
  Homie.disableResetTrigger();
  Homie.disableLedFeedback();

  Homie.setSetupFunction(setupHandler);
  Homie.setLoopFunction(loopHandler);

  Homie.setup();
}

void loop()
{
  Homie.loop();
}
