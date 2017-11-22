#define FW_VERSION "1.0.2"

// Default OTA Progress is shown on the serial console.
// Uncomment the line corresponding to the progress class you want to use
// in platformio.ini

#include <Homie.h>
#include "ota.hpp"
#include "welcome.hpp"

const int PIN_SDA = 5;
const int PIN_SCL = 4;

// ======================================================================
// Setup OTA logging via SSD1306 OLED display driver and Homie logger
//
#if defined(DISPLAY_SSD1306)
#define FW_NAME "ota-ssd1306"

// Put your settings for the I2C bus here
const int I2C_DISPLAY_ADDRESS = 0x3c;

SSD1306Wire display(I2C_DISPLAY_ADDRESS, PIN_SDA, PIN_SCL);
OtaDisplaySSD1306 ota(display, NULL);
WelcomeSSD1306 welcome(display, FW_NAME, FW_VERSION);

// ======================================================================
// Setup OTA logging via U8G2 OLED display driver and Homie logger
//
#elif defined(DISPLAY_U8G2)
#define FW_NAME "ota-u8g2"

const int PIN_RESET = U8X8_PIN_NONE; // The reset pin of the SD1306 is not connected

// Using a 128x32 display
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, PIN_RESET, PIN_SCL, PIN_SDA);
OtaDisplayU8G2 ota(u8g2, NULL);
WelcomeU8G2 welcome(u8g2, FW_NAME, FW_VERSION);

// ======================================================================
// Setup OTA logging via Homie logger
//
#else
#define FW_NAME "ota-basic-serial"
OtaLogger ota;
Welcome welcome(FW_NAME, FW_VERSION);
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

  Homie_setFirmware(FW_NAME, FW_VERSION);

  // show state and last reset reason
  welcome.show();

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
