/**
 * @file Baustellenlicht.cpp
 * @brief Funktionen zur Steuerung von Blinklichtern an Baustellen
 * @version 1.0
 * @date 25 Jan 2022 1 Dez 23 4 3 2 1 Nov 31 Okt 2021
 * @author Dr. Burkhard Borys, Zeller Ring 15, 34246 Vellmar, Deutschland
 * @copyright Copyright (c) 2021-2022 B. Borys
 * 
 */

#include "Licht.h"
///  ---------------------------------------------------------------------
/**
 * @brief Construct a new einfaches Baustellen-Blinklicht object
 * einfaches Baustellen-Blinklicht
 * @param strip Adresse Neopixel
 * @param id Nummer des Pixels
 * @param pTan An-Zeit in ms, default 2,5 s
 * @param pTaus Aus-Zeit in ms, default 1 s
 */
cBauLicht::cBauLicht(Adafruit_NeoPixel *strip, int id,int pTan, int pTaus) : cBLicht(strip, id, blaGelb, pTan, pTaus)
{
    Farbe = strip->Color(222, 50, 0);
}
/// ---------------------------------------------------------------------
/**
 * @brief Construct a new Lauf-Blinklicht object
 * Lauf-Blinklicht für Absperrungen
 * @param strip Adresse Neopixel
 * @param id Nummer des Pixels
 * @param Anzahl der Lichter
 */
cLaufLicht::cLaufLicht(Adafruit_NeoPixel *strip, int id, int pAnzahl, int pTan) : cBauLicht(strip, id, pTan)
{
    Anzahl = pAnzahl;
    farbe(111, 25, 0);
}
void cLaufLicht::farbe(uint8_t r, uint8_t g, uint8_t b)
{
    Farbe = Strip->Color(r, g, b);
    FarbeDunkler = Strip->Color(r/3, g/4, b/3);
    FarbeDunkel = Strip->Color(r/16, g/16, b/16);
}
void cLaufLicht::blinken(bool pAn)
{
    if (pAn)
    {
        iLicht = 0;
        iLauf = 0;
        istAn = true;
        Stat = stLauf1;
        wechsel = 0;
    }
    else
    {
        Stat = stAus;
        aus();
    }
}
/**
 * @brief alle Neopixel der Kette ausschalten
 * 
 */
void cLaufLicht::aus()
{
    for (size_t i = 0; i < Anzahl; i++)
    {
        Strip->setPixelColor(LichtID + i, 0);
    }
    istAn = false;
}
void cLaufLicht::check()
{
    unsigned long jetzt;

    jetzt = millis();
    switch (Stat)
    {
    case stLauf1:
        if (jetzt > wechsel)
        {
            Strip->setPixelColor((LichtID + (iLicht + 4) % Anzahl), Farbe);        //der erste
            //Strip->setPixelColor((LichtID + (iLicht + 3) % Anzahl), Farbe);        //der erste
            Strip->setPixelColor((LichtID + (iLicht + 2) % Anzahl), FarbeDunkler); //der folgende
            //Strip->setPixelColor((LichtID + (iLicht + 1) % Anzahl), FarbeDunkler); //der folgende
            //Strip->setPixelColor((LichtID + iLicht % Anzahl), FarbeDunkler);       //der dritte
            wechsel = jetzt + Blitzzeit;
            Stat = stLauf2;
        }
        break;

    case stLauf2:
        if (jetzt > wechsel)
        {
            wechsel = jetzt + Tan;
            Strip->setPixelColor((LichtID + iLicht % Anzahl), FarbeDunkel);   //der dritte auf ganz dunkel
            iLicht++;
            if (iLicht < Anzahl)
            {
                Stat = stLauf1;
            }
            else
            {
                iLicht = 0;
                iLauf++;
                if (iLauf < nLauf)
                {
                    Stat = stLauf1;
                }
                else
                {
                    iLauf = 0;
                    iBlink = 0;
                    Stat = stLauf3;
                }
            }
        }
        break;
    case stLauf3:
        if (jetzt > wechsel)
        {
            for (size_t i = 0; i < Anzahl; i++)
                Strip->setPixelColor(LichtID + i, Farbe);
            wechsel = jetzt + Blitzzeit;
            Stat = stLauf4;
        }
        break;
    case stLauf4:
        if (jetzt > wechsel)
        {
            for (size_t i = 0; i < Anzahl; i++)
                Strip->setPixelColor(LichtID + i, Farbe);
            iBlink++;
            if (iBlink < nBlink)
            {
                wechsel = jetzt + Taus;
                Stat = stLauf3;
            }
            else
            {
                Stat = stLauf1;
                wechsel = jetzt + Tan;
            }
        }
        break;

    default:
        Stat = stLauf1;
        wechsel = 0;
        break;
    }
}
