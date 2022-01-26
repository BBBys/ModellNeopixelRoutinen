/**
 * @file LichtfunkTest.cpp
 * @author BBB
 * @brief Testprogramm für alle Lichtfunktionen
 * @version 1.1
 * @date 25 18 Jan 2022 3 Dez 7 6 4 3 2 1 Nov 31 30 29 28 Okt 2021
 * @author Dr. Burkhard Borys, Zeller Ring 15, 34246 Vellmar, Deutschland
 * @copyright Copyright (c) 2021-2022 B. Borys
 * 
 */
#include <Arduino.h>
#include "Licht.h"

#define NEOLEDS 60
#define NEOHELL 1
Adafruit_NeoPixel strip(0);//NEOLEDS,99);

// die Lichter
// Blinklichter für 4 Autos. Damit sie nicht gleich aussehen, sind Farben und Zeiten variiert
c2BlauLicht 
BL2d(&strip, 0,cBLicht::blaPolizei), // 1. Polizeiauto
BL2c(&strip, 2, cBLicht::blaBlau,31,110),                                                    // 2. Polizeiauto, Farbe und Zeiten anders
    BL2b(&strip, 4, cBLicht::blaGelb,35), BL2a(&strip, 6,cBLicht::blaAbschleppwagen,31,89); // 2 Abschleppwagen, ebenso

int art = 0;
const int maxart = 1, dauer = 30;
long int anfang, ende;
void setup()
{
  pinMode(NEOPIN, INPUT);
  Serial.begin(115200);
  Serial.println("setup");
  delay(3000);
  strip.setPin(NEOPIN);
  pinMode(NEOPIN, INPUT);
  strip.updateLength(NEOLEDS);
  strip.updateType(NEO_GRB + NEO_KHZ800);
  delay(300);
  Serial.println("begin()");
  strip.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  delay(300);
  Serial.println("show()");
  strip.show();           
  strip.setBrightness(80);
  art = 0;
  anfang = millis();
  ende = anfang + (long int)(1000 * dauer);
  delay(300);
  Serial.println("ein()");
  pinMode(NEOPIN, OUTPUT);
}

void loop()
{
  long int jetzt;
  jetzt = millis();
  if (jetzt > ende)
  {
    art = (art + 1) % maxart;
    anfang = jetzt;
    ende = anfang + (long int)(1000 * dauer);
  }
  switch (art)
  {
    case 0:
      BL2a.check();
      BL2b.check();
      BL2c.check();
      BL2d.check();
    }
  delay(5);
  strip.show(); 
}
