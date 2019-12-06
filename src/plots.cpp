#include "plots.h"
#include <Arduino.h>

#include <SPI.h>
#include <Wire.h>

void plotPM(uint16_t *pm, Adafruit_SSD1306 &display, const uint16_t msec){
  const uint8_t barWidth=8, nbar=WIDTH/barWidth, nbin=3;
  static uint8_t buffer[nbar*nbin], pos=0, scale=0;
  uint8_t n, bin, bar;

  // rescale buffer, if necessary
  uint8_t rescale=0;
  while ((pm[nbin-1]>>scale)>HEIGHT) {
    scale++;
    rescale++;
#ifdef DEBUG
    Serial.printf("\n  scale:%d, rescale:%d",scale,rescale);
#endif
  }
  if (rescale>0) {
    for (n=0; n<nbar*nbin; n++) {
      buffer[n] >>= rescale;
    }
  }
 
  // buffer new value
  pos%=nbar; // ensure we are inside the buffer
  for (bin=0, n=pos*nbin; bin<nbin; bin++, n++){
     buffer[n] = pm[bin]>>scale;
  }
  pos++;     // next position on the buffer, first to plot

  // plot buffer
  display.clearDisplay();
  for (n=0; n<nbar*nbin; n++){
    bar=(nbar-pos+n/nbin)%nbar;
    display.drawRect(X0+barWidth*bar, Y1-buffer[n], barWidth-2, buffer[n], WHITE);
#ifdef DEBUG
    if(bar == nbar-1)
      Serial.printf("\n  pos:%d, bar:%d, val:%d",pos,bar,buffer[n]);
    display.display();
    delay(50);
#endif
  }
  display.display();
  delay(msec);
}

void plotNC(uint16_t *nc, Adafruit_SSD1306 &display, const uint16_t msec){
  const uint8_t nbin=6, barWidth=WIDTH/nbin;
  uint8_t bin, scale, barHeight;

  // rescale input, if necessary
  scale=0;
  for (bin=0; bin<nbin; bin++){
    while ((nc[bin]>>scale)>HEIGHT) {
      scale++;
    }
  }

  // plot input
  display.clearDisplay();
  for (bin=0; bin<nbin; bin++){
    barHeight=nc[bin]>>scale;
    display.fillRect(X0+barWidth*bin, Y1-barHeight, barWidth-2, barHeight, WHITE);
#ifdef DEBUG
    display.display();
    delay(150);
#endif
  }
  display.display();
  delay(msec);
}


static char *uint2str (const char *fmt, uint16_t var){
  // wrap sprintf as a function with a pre-deffined buffer
  const uint8_t chlen = 32;
  static char buffer[chlen];
  sprintf(buffer, fmt, var);
  return buffer;
}

void drawString(Adafruit_SSD1306 &display, uint8_t x, uint8_t y, const char *msg) {
	display.setTextSize(1);
	display.setTextColor(WHITE);
	display.setCursor(x,y);
	display.println(msg);
}

void showPM(uint16_t *pm, Adafruit_SSD1306 &display, const uint16_t msec){
  display.clearDisplay();

  drawString(display, XM, 14, "[ug/m3]");
  drawString(display, X0, 24, "PM1.0");
  drawString(display, X0, 34, "PM2.5");
  drawString(display, X0, 44, "PM10");
  drawString(display, X1, 24, uint2str("%9d",pm[0]));//PM1.0
  drawString(display, X1, 34, uint2str("%9d",pm[1]));//PM2.5
  drawString(display, X1, 44, uint2str("%9d",pm[2]));//PM10

  display.display();
  delay(msec);
}

void showNC(uint16_t *nc, Adafruit_SSD1306 &display, const uint16_t msec){
  display.clearDisplay();
  
  drawString(display, XM, 14, "[#/100cc]");
  drawString(display, X0, 24, "N0.3");
  drawString(display, X0, 34, "N1.0");
  drawString(display, X0, 44, "N2.5");
  drawString(display, X0, 54, "N10");
  drawString(display, X1, 24, uint2str("%9d",nc[0]));//N0.3
  drawString(display, X1, 34, uint2str("%9d",nc[2]));//N1.0
  drawString(display, X1, 44, uint2str("%9d",nc[3]));//N2.5
  drawString(display, X1, 54, uint2str("%9d",nc[5]));//N10
  display.display();
  delay(msec);
}
