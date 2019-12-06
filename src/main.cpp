#include <Arduino.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <PMserial.h>

#include "alex.h"
#include "plots.h"

const int ledPin13 = 13;
const int ledPin12 = 12;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET -1

#define DEBUG 1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const uint8_t PMS_RX=16, PMS_TX=17;
SerialPM pms(PMS7003, Serial2);


void showSplash();

void setup() {
 	Serial.begin(9600);
	Serial.printf("\nProgram: %s\n", __FILE__);
  Serial.printf("Build: %s %s\n", __DATE__, __TIME__);
  Serial.printf("PMS7003 on RX:GPIO%02d, TX:GPIO%02d\n", PMS_RX, PMS_TX);

	display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  
	showSplash();
	pms.init();
	display.clearDisplay();
}

void loop() {
  pms.read();
  if(pms){  
    Serial.printf("PM1.0 %2d, PM2.5 %2d, PM10 %2d [ug/m3]; ", pms.pm01,pms.pm25,pms.pm10);
    Serial.printf("N0.3 %4d, N0.5 %3d, N1.0 %2d, N2.5 %2d, N5.0 %2d, N10 %2d [#/100cc]\n",
      pms.n0p3,pms.n0p5,pms.n1p0,pms.n2p5,pms.n5p0,pms.n10p0);

    showPM(pms.pm, display, 5000); // particulate matter, wait 5 sec
    plotPM(pms.pm, display, 5000);
    showNC(pms.nc, display, 5000); // number concentration, wait 5 sec
    plotNC(pms.nc, display, 5000);
  } else {
    switch (pms.status) {
    case pms.OK: // should never come here
      break;     // included to compile without warnings
    case pms.ERROR_TIMEOUT:
      Serial.println(F(PMS_ERROR_TIMEOUT));
      break;
    case pms.ERROR_MSG_UNKNOWN:
      Serial.println(F(PMS_ERROR_MSG_UNKNOWN));
      break;
    case pms.ERROR_MSG_HEADER:
      Serial.println(F(PMS_ERROR_MSG_HEADER));
      break;
    case pms.ERROR_MSG_BODY:
      Serial.println(F(PMS_ERROR_MSG_BODY));
      break;
    case pms.ERROR_MSG_START:
      Serial.println(F(PMS_ERROR_MSG_START));
      break;
    case pms.ERROR_MSG_LENGTH:
      Serial.println(F(PMS_ERROR_MSG_LENGTH));
      break;
    case pms.ERROR_MSG_CKSUM:
      Serial.println(F(PMS_ERROR_MSG_CKSUM));
      break;
    case pms.ERROR_PMS_TYPE:
      Serial.println(F(PMS_ERROR_PMS_TYPE));
      break;
    }
  }
  delay(200);
}

void showSplash() {
	display.clearDisplay();
	display.drawBitmap(0, 0,  Alex, 128, 64, WHITE);
	display.display();
}