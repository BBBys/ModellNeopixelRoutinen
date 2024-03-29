/**
 * @file LichtfunkTest.cpp
 * @brief Testprogramm für alle Lichtfunktionen
 * @version 1.1
 * @date 29 28 27 26 25 18 Jan 2022 3 Dez 7 6 4 3 2 1 Nov 31 30 29 28 Okt 2021
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
// Blinklicht
cBLicht Bll(&strip, 15);
// Blaulicht, 2 Autos
cBlauLicht Bla(&strip, 13), Blb(&strip, 14, cBLicht::blaPolizei,10,100);
// Blinklichter für 4 Autos. Damit sie nicht gleich aussehen, sind Farben und Zeiten variiert
c2BlauLicht 
BL2d(&strip, 0,cBLicht::blaPolizei), // 1. Polizeiauto
BL2c(&strip, 2, cBLicht::blaBlau,31,110),                                                    // 2. Polizeiauto, Farbe und Zeiten anders
    BL2b(&strip, 4, cBLicht::blaGelb,35), BL2a(&strip, 6,cBLicht::blaAbschleppwagen,31,89); // 2 Abschleppwagen, ebenso
// Schreibtischlampe
cSchreibLicht Stl(&strip, 8);
// Leuchtstoffröhre
cLRLicht Lr(&strip, 9);
// Fernseher
cTVLicht Tv(&strip, 10);
// Blinklichter für Baustellen
cBauLicht Bau(&strip, 11), Bau2(&strip, 12, 2200, 1100);
cLaufLicht Lauf(&strip, 20, 10);
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
  strip.begin(); 
  delay(300);
  Serial.println("show()");
  strip.show();           
  strip.setBrightness(80);
  art = 0;

  Stl.ein();
  Lr.ein();
  Tv.ein();
  Bau.blinken();
  Bau2.blinken();
  Lauf.blinken();
  Bll.blinken();
  Bla.blinken();
  Blb.blinken();

  anfang = millis();
  ende = anfang + (long int)(1000 * dauer);
  delay(300);
  pinMode(NEOPIN, OUTPUT);
}

void loop()
{
  long int jetzt;
  jetzt = millis();
  if (jetzt > ende)
  {
    anfang = jetzt;
    ende = anfang + (long int)(1000 * dauer);
    if(Lr.istEin())
      Lr.aus();
      else
        Lr.ein();
  }
  Bll.check();
  Bla.check();
  Blb.check();
  BL2a.check();
  BL2b.check();
  BL2c.check();
  BL2d.check();
  Lr.check();
  Tv.check();
  Bau2.check();
  Bau.check();
  Lauf.check();
  delay(5);
  strip.show(); 
}
