#include <Homie.h>
#include "homie-node-collection.h"

#define FW_VERSION "1.0.1"

// Uncomment this line if you want to see the OTA progress
// on the OLED display
#define DISPLAY_SSD1306

#ifdef DISPLAY_SSD1306
  #define FW_NAME "ota-display-ssd1306"
  // #define DISPLAY_HEIGHT 32

  // Setup OTA logging via OLED dislpay and Homie logger
  #include <SSD1306.h>

  // Put your settings for the I2C bus here
  const int I2C_DISPLAY_ADDRESS = 0x3c;
  const int SDA_PIN = 5;
  const int SCL_PIN = 4;

  SSD1306Wire display(I2C_DISPLAY_ADDRESS, SDA_PIN, SCL_PIN);
  OtaDisplaySSD1306 ota(display, NULL);
#else
  #define FW_NAME "ota-basic-serial"
  // Setup OTA logging via Homie logger
  OtaLogger ota;
#endif

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

#ifdef DISPLAY_SSD1306
  // initlalize display
  display.init();
  display.clear();
  display.flipScreenVertically();
  display.setColor(WHITE);
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, DISPLAY_HEIGHT/2, (String)FW_NAME + " " + (String)FW_VERSION);
  display.display();
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
