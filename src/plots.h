#include <Arduino.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const uint8_t 
  WIDTH=96, HEIGHT=48,
  X0=WIDTH/2, X1=WIDTH-X0, XM=(X0+X1)/2,
  Y1=HEIGHT, Y0=Y1-HEIGHT, YM=(Y0+Y1)/2;


void plotPM(uint16_t *pm, Adafruit_SSD1306 &display, const uint16_t msec);

void plotNC(uint16_t *nc, Adafruit_SSD1306 &display, const uint16_t msec);


void showPM(uint16_t *pm, Adafruit_SSD1306 &display, const uint16_t msec);

void showNC(uint16_t *nc, Adafruit_SSD1306 &display, const uint16_t msec);