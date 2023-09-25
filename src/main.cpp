#define FW_VERSION "2.0.0"

// Default OTA Progress is shown on the serial console.
// Uncomment the line corresponding to the progress class you want to use
// in platformio.ini

#include <ESP8266WiFi.h>
#include "ota.hpp"
#include "welcome.hpp"

// Create a secrets.h that contains your Wifi credentials
#include "secrets.h"

#ifndef WIFI_SSID
#define WIFI_SSID "myssid"
#define WIFI_PASS "mypass"
#endif

const int PIN_SDA = 4; // =D2 on Wemos
const int PIN_SCL = 5; // =D1 on Wemos

// ======================================================================
// Setup OTA logging via SSD1306 OLED display driver and logger
//
#if defined(DISPLAY_SSD1306)
#define FW_NAME "ota-ssd1306"

// Put your settings for the I2C bus here
const int I2C_DISPLAY_ADDRESS = 0x3c;

SSD1306Wire display(I2C_DISPLAY_ADDRESS, PIN_SDA, PIN_SCL);
OtaDisplaySSD1306 ota(display, NULL);
WelcomeSSD1306 welcome(display, FW_NAME, FW_VERSION);

// ======================================================================
// Setup OTA logging via U8G2 OLED display driver and logger
//
#elif defined(DISPLAY_U8G2)
#define FW_NAME "ota-u8g2"

const int PIN_RESET = U8X8_PIN_NONE; // The reset pin of the SD1306 is not connected

// Using a 128x32 display
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, PIN_RESET, PIN_SCL, PIN_SDA);
OtaDisplayU8G2 ota(u8g2, NULL);
WelcomeU8G2 welcome(u8g2, FW_NAME, FW_VERSION);

// ======================================================================
// Setup OTA logging via logger
//
#else
#define FW_NAME "ota-basic-serial"
OtaLogger ota;
Welcome welcome(FW_NAME, FW_VERSION);
#endif


void setup()
{
  Serial.begin(SERIAL_SPEED);

  // show info about the chip (on serial)zk,.
  welcome.show();

  // connect to Wifi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  ota.setup();
}

void loop()
{
  ota.loop();
}
