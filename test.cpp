#include <Adafruit_ST7789.h>
#include <stdint.h>

#include "Fonts/FreeSans18pt7b.h"

// Switch these to whatever you need
const uint8_t BACKLIGHT_PIN = 17;
const uint8_t TFT_CS = 8;
const uint8_t TFT_DC = 15;
const uint8_t TFT_RST = 6;

Adafruit_ST7789 tft(TFT_CS, TFT_DC, TFT_RST);

boolean backlight_on = false;

const char* rot_names[] = {"0:Port", "1:Land", "2!Port", "3!Land"};

void set_backlight(bool turnOn) {
  if (backlight_on != turnOn) {
    digitalWrite(BACKLIGHT_PIN, turnOn ? HIGH : LOW);
    backlight_on = turnOn;
  }
}

// Display the name of the current orientation
void printOrientation(uint8_t rot) {
  int16_t x, y;
  uint16_t w, h;
  tft.setCursor(0, 0);
  const char* str = rot_names[rot];
  tft.getTextBounds(str, 0, 0, &x, &y, &w, &h);
  uint16_t xx = (tft.width() - w) / 2;
  uint16_t yy = (tft.height() + h) / 2;
  tft.setCursor(xx, yy);
  tft.fillRoundRect(xx + x - 10, yy + y - 10, w + 21, h + 21, 5, ST77XX_RED);
  tft.fillRoundRect(xx + x - 3, yy + y - 3, w + 6, h + 6, 2, ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  tft.print(str);
}

void clearScreenTest(uint8_t rot) {
  tft.fillScreen(ST77XX_BLUE);
  tft.setRotation(rot);
  tft.fillScreen(ST77XX_BLACK);
}

void rectFrameTest(uint8_t rot) {
  tft.fillScreen(ST77XX_BLACK);
  delay(10);
  tft.setRotation(rot);
  tft.fillScreen(ST77XX_GREEN); // I'm worried that this won't expose
                                // other issues...
  tft.fillRect(1, 1, tft.width() - 2, tft.height() - 2, ST77XX_BLACK);
}

void cornerDotsTest(uint8_t rot) {
  tft.fillScreen(ST77XX_BLACK);
  tft.setRotation(rot);
  tft.drawPixel(0, 0, ST77XX_WHITE);
  tft.drawPixel(0, tft.height() - 1, ST77XX_WHITE);
  tft.drawPixel(tft.width() - 1, 0, ST77XX_WHITE);
  tft.drawPixel(tft.width() - 1, tft.height() - 1, ST77XX_WHITE);
}

uint32_t start = 0;
uint32_t next_task = 0;
void setup() {
  pinMode(BACKLIGHT_PIN, OUTPUT);
  set_backlight(true);
  tft.init(135, 240);
  // This is the fastest speed that worked
  // (72mhz also worked, but seemed to be the same speed)
  tft.setSPISpeed(60000000);
  tft.setFont(&FreeSans18pt7b);
  start = millis();
  next_task = start + 10;
}

uint8_t cur_rot = 0;
uint8_t which_test = 0;
typedef void (*test)(uint8_t);
const test tests[] = {clearScreenTest, rectFrameTest, cornerDotsTest};
const uint8_t num_tests = sizeof(tests) / sizeof(tests[0]);

void loop() {
  uint32_t now = millis();
  if (now >= next_task) {
    // You can speed it up or slow it down here
    // Teensy 4.0 can do this wicked fast :)
    next_task = now + 1000; // Run a new task every 1 second
    tft.setRotation(0);
    // Run the test
    tests[which_test](cur_rot);
    // Print the current orientation in the middle
    printOrientation(cur_rot);
    // Go to a new orientation
    cur_rot = (cur_rot + 1) & 3;
    // If we've looped around, pick a new test
    if (!cur_rot) {
      which_test = (which_test + 1) % num_tests;
    }
  }
  // Turn off the backlight after 5 minutes
  if (now - start > 5 * 60 * 1000) {
    set_backlight(false);
  }
}