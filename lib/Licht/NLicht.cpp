/**
 * @file NLicht.cpp
 * @brief Funktionen für Neopixiel-Licht
 * @version 1.1
 * @date 26 Jan 2022 13 Dez 23 6 4 3 2 Nov 31 30 Okt 2021
 * @author Dr. Burkhard Borys, Zeller Ring 15, 34246 Vellmar, Deutschland
 * @copyright Copyright (c) 2021-2022 B. Borys
 */
#include "Licht.h"
/**
 * @brief Construct a new Neopixel Licht object
 * Strip-Objekt in Pixel-Adresse übernehmen, Farbe auf Weiß
 * @param strip 
 * @param id 
 */
cNLicht::cNLicht(Adafruit_NeoPixel *strip, int id) : cLicht(id)
{
    Strip = strip;
    aus();
    Farbe =NEOWEISS;
}
cNLicht::cNLicht(Adafruit_NeoPixel *strip, int id, long pFarbe) : cLicht(id)
{
    Strip = strip;
    aus();
    Farbe = pFarbe;
}
/**
 * @brief Neopixel ausschalten
 * 
 */
void cNLicht::aus()
{
#ifndef NDEBUG
    Serial.printf("cNLicht::aus(ID=%d)\n", LichtID);
#endif
    Strip->setPixelColor(LichtID, 0);
    istAn = false;
}
void cNLicht::ein()
{
    //DbgOut("cNLicht::ein");
    Strip->setPixelColor(LichtID, Farbe);
    istAn = true;
}
cNLicht::~cNLicht()
{
}
/**
 * @brief nichts weiter als eine funzelige Schreibtischlampe 
 * @param strip der Neopixel-Strip
 * @param id Adresse der einen LED
 */
cSchreibLicht::cSchreibLicht(Adafruit_NeoPixel *strip, int id) : cNLicht(strip, id)
{
    Farbe = strip->Color(255, 255, 20);
}
/**
 * @brief Construct a new Lichterkette object
 * 
 * @param strip 
 * @param id 
 * @param pAnzahl 
 * @param Farbe 
 */
cKetteLicht::cKetteLicht(Adafruit_NeoPixel *strip, int id, int pAnzahl, uint32_t Farbe) : cNLicht(strip, id, Farbe)
{
    Anzahl = pAnzahl;
}
void cKetteLicht::aus()
{
    for (size_t i = 0; i < Anzahl; i++)
    {
        Strip->setPixelColor(LichtID + i, 0);
    }
}
void cKetteLicht::ein(uint32_t pFarbe)
{
    if (pFarbe > 0)
        Farbe = pFarbe;
    for (size_t i = 0; i < Anzahl; i++)
        Strip->setPixelColor(LichtID + i, Farbe);
}
/**
 * @brief Construct a new zweifarbige Lichterkette object
 * 
 * @param strip 
 * @param Start-ID 
 * @param Anzahl 
 * @param Farbe1 (Rot)
 * @param Farbe2 (Gelb)
 */
cKette2Licht::cKette2Licht(Adafruit_NeoPixel *strip, unsigned int id, unsigned int pAnzahl, uint32_t pFarbe1, uint32_t pFarbe2)
    : cKetteLicht(strip, id, pAnzahl, pFarbe1)
{
    Farbe2 = pFarbe2;
}
void cKette2Licht::ein(unsigned int pAbstand, uint32_t pFarbe1, uint32_t pFarbe2)
{
  unsigned  int j;
    if (pFarbe1 > 0)
        Farbe = pFarbe1;
    if (pFarbe2 > 0)
        Farbe2 = pFarbe2;
    j = 0;
    for (size_t i = 0; i < Anzahl; i++)
    {
        if (j < pAbstand)
            Strip->setPixelColor(LichtID + i, Farbe);
        else if (j < 2 * pAbstand)
            Strip->setPixelColor(LichtID + i, Farbe2);
        if (++j >= 2 * pAbstand)
            j = 0;
    }
}
/**
 * @brief Construct a new Stroboskop object
 * 
 * @param strip 
 * @param id 
 * @param pAnzahl 
 * @param Farbe (normalerweise Weiß = 0xFFFFFF)
 */
cStrobeLicht::cStrobeLicht(Adafruit_NeoPixel *strip, int id, int pAnzahl, uint32_t Farbe) : cKetteLicht(strip, id, pAnzahl, Farbe)
{
    Stat = stAus;
    zeiten();
}
void cStrobeLicht::aus()
{
    Stat = stAus;
    cKetteLicht::aus();
}
void cStrobeLicht::ein(uint32_t pFarbe)
{
    warten = millis() + 166;
    Stat = stEinDunkel;
    cKetteLicht::ein(pFarbe);
}
/**
 * @brief Zeiten für Stroboskop-Licht einstellen
 * 
 * @param auszeit in ms, Default 75
 * @param einzeit in ms, Default 25
 */
void cStrobeLicht::zeiten(int auszeit , int einzeit )
{
    Auszeit = auszeit;
    Einzeit = einzeit;
}

void cStrobeLicht::check()
{
    uint32_t z;
    if (Stat == stAus)
        return;
    jetzt = millis();
    if (jetzt > warten)
    {
        if (Stat == stEinHell)
        {
            z = 0;
            Stat = stEinDunkel;
            warten = jetzt + Auszeit;
        }
        else
        {
            z = Farbe;
            Stat = stEinHell;
            warten = jetzt + Einzeit;
        }
        for (size_t i = 0; i < Anzahl; i++)
            Strip->setPixelColor(LichtID + i, z);
    }
}
