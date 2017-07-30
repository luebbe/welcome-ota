#include <Homie.h>
#include "homie-node-collection.h"

#define FW_VERSION "1.0.0"

// Uncomment this line if you want to see the OTA progress
// on the OLED display
//#define DISPLAY_LOGGER

#ifdef DISPLAY_LOGGER
  #define FW_NAME "ota-basic-display"
  // Setup OTA logging via OLED dislpay and Homie logger
  #include <SSD1306.h>
  // Put your settings for the I2C bus here
  const int I2C_DISPLAY_ADDRESS = 0x3c;
  const int SDA_PIN = 12;
  const int SCL_PIN = 13;

  SSD1306Wire display(I2C_DISPLAY_ADDRESS, SDA_PIN, SCL_PIN);
  OtaDisplay ota(&display);
#else
  #define FW_NAME "ota-basic-serial"
  // Setup OTA logging via Homie logger
  OtaLogger ota;
#endif

#define SERIAL_SPEED 115200

void setupHandler() {
  // This is called after the MQTT_CONNECTED event
  ota.setup();
}

void loopHandler() {
  // This is only called by Homie when WiFi is connected
  ota.loop();
}

void setup() {
  Serial.begin(SERIAL_SPEED);
  Serial << endl << endl;

  // show state and last reset reason
  welcome();

#ifdef DISPLAY_LOGGER
  // initlalize display
  display.init();
  display.clear();
  display.flipScreenVertically();
#endif

  Homie_setFirmware(FW_NAME, FW_VERSION);

  // before Homie.setup()
  Homie.disableResetTrigger();
  Homie.disableLedFeedback();

  Homie.setSetupFunction(setupHandler);
  Homie.setLoopFunction(loopHandler);

  Homie.setup();
}

void loop() {
   Homie.loop();
}
