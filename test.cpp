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

uint32_t start = 0;
uint32_t next_task = 0;

void set_backlight(bool turnOn) {
  if (backlight_on != turnOn) {
    digitalWrite(BACKLIGHT_PIN, turnOn ? HIGH : LOW);
    backlight_on = turnOn;
  }
}
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
bool fill_not_rect = false;
const char* rot_names[] = {
  "0:Port", "1:Land", "2!Port", "3!Land"};
void loop() {
  uint32_t now = millis();
  if (now >= next_task) {
    // You can speed it up or slow it down here
    // Teensy 4.0 can do this wicket fast :)
    next_task = now + 1500; // Run this every 1.5 seconds
    tft.setRotation(0);
    if (fill_not_rect) {
      tft.fillScreen(ST77XX_BLUE);
      tft.setRotation(cur_rot);
      tft.fillScreen(ST77XX_BLACK);
    } else {
      tft.fillScreen(ST77XX_BLACK);
      delay(10);
      tft.setRotation(cur_rot);
      tft.fillScreen(ST77XX_GREEN); // I'm worried that this won't expose other
                                    // issues...
      tft.fillRect(1, 1, tft.width() - 2, tft.height() - 2, ST77XX_BLACK);
    }
    int16_t x, y;
    uint16_t w, h;
    tft.setCursor(0, 0);
    const char* str = rot_names[cur_rot];
    tft.getTextBounds(str, 0, 0, &x, &y, &w, &h);
    uint16_t xx = (tft.width() - w) / 2;
    uint16_t yy = (tft.height() + h) / 2;
    tft.setCursor(xx, yy);
    tft.fillRoundRect(xx + x - 10, yy + y - 10, w + 21, h + 21, 5, ST77XX_RED);
    tft.fillRoundRect(xx + x - 3, yy + y - 3, w + 6, h + 6, 2, ST77XX_BLACK);
    tft.setTextColor(ST77XX_WHITE);
    tft.print(str);
    cur_rot = (cur_rot + 1) & 3;
    if (!cur_rot) {
      fill_not_rect = !fill_not_rect;
    }
  }
  // Turn off the backlight after 1 minute
  if (now - start > 1 * 60 * 1000) {
    set_backlight(false);
  }
}