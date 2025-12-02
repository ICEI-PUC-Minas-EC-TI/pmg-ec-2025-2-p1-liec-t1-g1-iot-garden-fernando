#include "display.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

static Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
static bool displayReady = false;

void initDisplay() {
  Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);

  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED: failed");
    displayReady = false;
    return;
  }

  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(0, 0);
  oled.println("IoT Garden");
  oled.println("Starting...");
  oled.display();

  displayReady = true;
  Serial.println("OLED: ready");
}

void updateDisplay(const DisplayData& data) {
  if (!displayReady) return;

  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);

  // Line 1: Temperature
  oled.setCursor(0, 0);
  oled.print("Temp: ");
  if (data.tempValid) {
    oled.print(data.temperature, 1);
    oled.print("C");
  } else {
    oled.print("--");
  }

  // Line 2: Humidity
  oled.setCursor(0, 12);
  oled.print("Humidity: ");
  if (data.humValid) {
    oled.print(data.humidity, 0);
    oled.print("%");
  } else {
    oled.print("--");
  }

  // Line 3: Light
  oled.setCursor(0, 24);
  oled.print("Light: ");
  if (data.lightValid) {
    oled.print(data.light ? "DAY" : "NIGHT");
  } else {
    oled.print("--");
  }

  // Line 4: Soil
  oled.setCursor(0, 36);
  oled.print("Soil: ");
  if (data.soilValid) {
    oled.print(data.soil);
  } else {
    oled.print("--");
  }

  // Line 5: Active actuator (with divider)
  oled.drawLine(0, 48, SCREEN_WIDTH, 48, SSD1306_WHITE);
  oled.setCursor(0, 52);
  oled.print("Active: ");
  switch (data.activeActuator) {
    case ACTUATOR_SPRINKLER:
      oled.print("SPRINKLER");
      break;
    case ACTUATOR_FAN:
      oled.print("FAN");
      break;
    case ACTUATOR_LIGHT:
      oled.print("LIGHT");
      break;
    default:
      oled.print("NONE");
      break;
  }

  oled.display();
}
