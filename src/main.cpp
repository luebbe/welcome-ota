#include <Homie.h>
#include "homie-node-collection.h"

#define FW_VERSION "1.0.1"

// Uncomment the line corresponding to the OLED driver you want to use to 
// show the OTA progress on the OLED display. Or none if you want just serial logging
// #define DISPLAY_SSD1306
#define DISPLAY_U8G2

const int PIN_SDA = 5;
const int PIN_SCL = 4;

#if defined(DISPLAY_SSD1306)
//
// Setup OTA logging via SSD1306 OLED display driver and Homie logger
//
#define FW_NAME "ota-display-ssd1306"
#include <SSD1306.h>

// Put your settings for the I2C bus here
const int I2C_DISPLAY_ADDRESS = 0x3c;

SSD1306Wire display(I2C_DISPLAY_ADDRESS, PIN_SDA, PIN_SCL);
OtaDisplaySSD1306 ota(display, NULL);

#elif defined(DISPLAY_U8G2)
//
// Setup OTA logging via U8G2 OLED display driver and Homie logger
//
#define FW_NAME "ota-display-u8g2"
#include <u8g2lib.h>

const int PIN_RESET = U8X8_PIN_NONE; // The reset pin of the SD1306 is not connected

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, PIN_RESET, PIN_SCL, PIN_SDA);
OtaDisplayU8G2 ota(u8g2, NULL);

#else
#define FW_NAME "ota-basic-serial"
//
// Setup OTA logging via Homie logger
//
OtaLogger ota;
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

#if defined(DISPLAY_SSD1306)
  // initlalize display
  display.init();
  display.clear();
  display.flipScreenVertically();
  display.setColor(WHITE);
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, DISPLAY_HEIGHT / 2, (String)FW_NAME + " " + (String)FW_VERSION);
  display.display();
#elif defined(DISPLAY_U8G2)
  u8g2.begin();
#else
#endif

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
