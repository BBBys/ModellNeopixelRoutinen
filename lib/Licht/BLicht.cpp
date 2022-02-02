/**
 * @file BLicht.cpp
 * @brief Funktionen zur Steuerung von Blinklichtern
 * @version 1.2
 * @date 29 28 27 26 25 Jan 2022 1 Dez 23 7 6 5 4 3 2 1 Nov 31 30 29 28 Okt 2021
 * @author Dr. Burkhard Borys, Zeller Ring 15, 34246 Vellmar, Deutschland
 * @copyright Copyright (c) 2021-2022 B. Borys
 */
#include "Licht.h"

///  ---------------------------------------------------------------------
void cBLicht::set(int pTan, int pTaus)
{
    set(pTan, pTaus, 0);
}
/**
 * @brief Parameter für Blinklicht, alle in ms
 * 
 * @param pTan An-Zeit
 * @param pTaus Aus-Zeit
 * @param pW Zeit bis Start
 */
void cBLicht::set(int pTan, int pTaus, int pW)
{
    Tan = pTan;
    Taus = pTaus;
    wechsel = millis() + pW;
    istAn = false;
    setzen(istAn);
}
/**
 * @brief Construct a new Blinklicht object
 * einfaches Blinklicht, Oberklasse für andere Blinklichter
 * @param strip Adresse Neopixel
 * @param id Nummer des Pixels
 * @param pTan An-Zeit in ms
 * @param pTaus Aus-Zeit in ms
 */
cBLicht::cBLicht(Adafruit_NeoPixel *strip, int id, BlArt art , int pTan , int pTaus ) : cNLicht(strip, id)
{
    Tan = pTan;
    Taus = pTaus;
    wechsel = 0;
    switch (art)
    {
    case blaPolizei:
    case blaBlau:
        Farbe = NEOBLAU; //= 0x0000ff
        break;
    case blaRot:
        Farbe = NEOROT;
        break;
    case blaAmpel:
        Farbe = 0xff9F00;
        break;
    case blaAbschleppwagen:
        Farbe = 0xfe3F00;
        break;
    case blaGelb:
    default:
        Farbe = NEOGELB; // = 0xff5F00
        break;
    }
    Stat = stAus;
}
void cBLicht::blinken(bool pAn)
{
    if (pAn)
        Stat = stBlink;
    else
    {
        Stat = stAus;
        aus();
    }
}
void cBLicht::check()
{
    if (Stat == stBlink)
    {
        unsigned long jetzt;
        jetzt = millis();
        if (jetzt > wechsel)
        {
            istAn = !istAn;
            setzen(istAn);
            wechsel = istAn ? jetzt + Tan : jetzt + Taus;
        }
    } 
}
///  -------------------------------------------------------
/**
 * @brief Construct a new Blaulicht object
 * Blaulicht, ein Pixel
 * @param strip Adresse Neopixel
 * @param id Nummer des Pixels
 * @param art 
 * @param pTan 
 * @param pTaus 
 */
cBlauLicht::cBlauLicht(Adafruit_NeoPixel *strip, int id, BlArt art , int pTan, int pTaus) : cBLicht(strip, id, art, pTan, pTaus) {}

// void cBlauLicht::blinken(bool pAn)
// {
//     if (pAn){
//         wechsel = 0;
//         Stat = stBlink1;}
//     else
//     {
//         Stat = stAus;
//         cNLicht::aus();
//     }
// }
// void cBlauLicht::check()
// {
//     unsigned long jetzt;
//     jetzt = millis();
//     Serial.printf("cBla:check %d ", Stat);
//     switch (Stat)
//     {
//     case stEin:
//     case stBlink1:
//         Serial.println("blink1");
//         if (jetzt > wechsel)
//         {
//             Serial.println("wechsel");
//             istAn = !istAn;
//             setzen(istAn);
//         }
//         break;
//     case stBlink2:
//     case stBlink3:
//     case stBlink4:
//         Stat = stBlink1;//nur dieses soll sein
//     case stAus:
//     default:
//         break;
//     } //switch
// }
///  -------------------------------------------------------
/**
 * @brief Construct a new Fernseher object
 * Fernseher-Simulation, Farbwechsel, mal schneller, mal langsamer
 * @param strip Adresse Neopixel
 * @param id Nummer des Pixels
 */
cTVLicht::cTVLicht(Adafruit_NeoPixel *strip, int id) : cBLicht(strip, id)
{
}
/**
 * @brief Fernseher aus
 */
void cTVLicht::aus()
{
    Stat = stAus;
    cBLicht::aus();
}
/**
 * @brief Fernseher ein
 * danach regelmäßig cTVLicht::check() aufrufen!
 */
void cTVLicht::ein()
{
    cBLicht::ein();
    wechsel = 0;
    wechsel2 = 0;
    Stat = stBlink1;
}
/**
 * @brief Fernseher-Licht ändern
 */
void cTVLicht::check()
{
    unsigned long jetzt;
    unsigned int r, g, b;
    jetzt = millis();
    if (jetzt > wechsel)
    {
        //zufällige Farbe, wenig rot, viel Blau
        r = random(63);
        b = random(255);
        g = random(130);
        Strip->setPixelColor(LichtID, r, g, b);
        switch (Stat)
        {
        case stBlink1: //schnelle Wechsel
            wechsel = jetzt + 100;
            if (jetzt > wechsel2)
            {
                Stat = stBlink2;
                wechsel2 = jetzt + random(6000, 18000);
            }
            break;
        case stBlink2: //langsame Wechsel
            wechsel = jetzt + random(1000, 3000);
            if (jetzt > wechsel2)
            {
                Stat = stBlink1;
                wechsel2 = jetzt + random(1000, 4000);
            }
            break;

        default:
            break;
        }
    }
}
///  -------------------------------------------------------
/**
 * @brief Construct a new zwei-Blaulichter object
 * zwei Blaulichter, gleichzeitig und abwechselnd blinken. Für Modellautos: Polizei, Feuerwehr, Abschleppwagen
 * @param strip Adresse Neopixel
 * @param id Nummer des Pixels
 * @param art Art des Blinklichts: blaPolizei, blaBlau, blaAbschleppwagen, blaGelb, blaAmpel, blaRot
 * @param pTan An-Zeit in ms
 * @param pTaus Aus-Zeit in ms
 */
c2BlauLicht::c2BlauLicht(Adafruit_NeoPixel *strip, int id, BlArt art , int pTan, int pTaus) : cBlauLicht(strip, id, art, pTan, pTaus) {}
void c2BlauLicht::blinken(bool pAn)
{
    if (pAn)
        Stat = stBlink1;
    else
    {
        Stat = stAus;
        aus(0);
        aus(1);
    }
}
void c2BlauLicht::check()
{
    unsigned long jetzt;
    jetzt = millis();
    switch (Stat)
    {
    case stBlink1:
        if (jetzt > wechsel)
        {
            istAn[1] = istAn[0] = !istAn[0];
            setzen(istAn[0], istAn[1]);
            wechsel = istAn[0] ? jetzt + Tan : jetzt + Taus;
            if (!istAn[0])
            {
                iBlink++;
                if (iBlink > nBlink)
                {
                    Stat = stBlink2;
                    wechsel = jetzt + 2 * Taus;
                }
            }
        }
        break;
    case stBlink2:
        if (jetzt > wechsel)
        {
            if (++iFolge > nFolge)
            {
                Stat = stBlink3;
                iFolge = 0;
            }
            else
            {
                Stat = stBlink1;
            }
            iBlink = 0;
            wechsel = jetzt + Tan;
        }
        break;
    case stBlink3:
        if (jetzt > wechsel)
        {
            istAn[0] = !istAn[0];
            istAn[1] = !istAn[0];
            setzen(istAn[0], istAn[1]);
            wechsel = jetzt + 3 * Taus;
            if (!istAn[0])
            {
                iBlink++;
                if (iBlink > nBlink)
                {
                    if (++iFolge > nFolge)
                    {
                        Stat = stBlink1;
                        iFolge = 0;
                    }
                    else
                        Stat = stBlink1;
                    wechsel = jetzt + Tan;
                }
            }
            break;
        case stBlink4:
        default:
            if (jetzt > wechsel)
            {
                Stat = stBlink3;
                iBlink = 0;
            }
            break;
        }

    } //switch
}
/**
 * @brief ein-oder ausschalten
 * 
 * @param p aus, wenn false; default: ein
 */
void c2BlauLicht::setzen(bool p0, bool p1)
{
    //   DbgOut("c2BlauLicht::setzen");
    if (p1)
        ein(1);
    else
        aus(1);
    if (p0)
        ein(0);
    else
        aus(0);
}
/**
 * @brief Neopixel ausschalten
 * 
 */
void c2BlauLicht::aus(int i)
{
    Strip->setPixelColor(LichtID + i, 0);
    istAn[i] = false;
}
void c2BlauLicht::ein(int i)
{
    Strip->setPixelColor(LichtID + i, Farbe);
    istAn[i] = true;
}
/**
 * @brief Construct a new Partylicht object
 * 
 * @param *strip    Adresse Neopixel-Objekt
 * @param id        Nummer Startpixel
 * @param pAnzahl   Anzahl Pixel
 * @param pnFarben  Anzahl Farben (max. 60)
 * @param pTan      An-Zeit
 * @param pTaus     Aus-Zeit
 * @param pTBlitz   Blitz-Zeit
 */
cPartyLicht::cPartyLicht(Adafruit_NeoPixel *strip, unsigned int id, unsigned int pAnzahl, unsigned int pnFarben, unsigned int pTan, unsigned int pTBlitz)
    : cLaufLicht(strip, id, pAnzahl, pTan)
{
    Blitzzeit = pTBlitz;
    if (pnFarben < 1)
        pnFarben = pAnzahl / 2;
    nFarben = min(MAXFARBEN, max(2U, pnFarben));
    for (size_t i = 0; i < nFarben; i++)
    {
        Farben[i] = strip->ColorHSV(i * 65535 / nFarben);
    }
    FarbIndex = random(0, nFarben);
}
/*
void cPartyLicht::blinken(bool pAn)
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
*/
/**
 * @brief Patylichterkette ändern
 * 
 */
void cPartyLicht::check()
{
    unsigned long jetzt;
    jetzt = millis();
    switch (Stat)
    {
    case stLauf1: ///Kette mit zufälligen Farben füllen
        if (jetzt > wechsel)
        {
            int x;
            //Strip->setPixelColor(LichtID + iLicht, Farben[random(0, nFarben)]);
            Strip->setPixelColor(LichtID + iLicht, Farben[FarbIndex]);
            x = (int)FarbIndex + (random(0, 5) - 2);
            FarbIndex = x % nFarben;
            wechsel = jetzt + 1000 / Anzahl; //die Kette in 1 s füllen
            iLicht++;
            Stat = (iLicht < Anzahl) ? stLauf1 : stLauf2;
        }
        break;
    case stLauf2: ///Kette mehrfach mit anderen zufälligen Farben füllen
        if (jetzt > wechsel)
        {
            wechsel = jetzt + Tan;
            iLicht = 0;
            iLauf++;
            if (iLauf < nLauf)
            {
                Stat = stLauf1; //noch einmal zufällige Farben
                FarbIndex = random(0, nFarben);
            }
            else
            {
                iLauf = 0;
                iBlink = 0;
                nBlink = random(9, 15);
                nLauf = random(2, 10);
                Stat = stLauf3;
            }

        } //if (jetzt > wechsel)
        break;
    case stLauf3: /// Kette mit 1...10 ähnlichen Farben füllen
        if (jetzt > wechsel)
        {
            int i1 = random(0, nFarben - 10);
            for (size_t i = 0; i < Anzahl; i++)
                Strip->setPixelColor(LichtID + i, Farben[i1 + random(0, 10)]);
            wechsel = jetzt + Blitzzeit;
            Stat = stLauf4;
        }
        break;
    case stLauf4: /// und mit diesen ähnlichen Farben blitzen lassen
        if (jetzt > wechsel)
        {
            for (size_t i = 0; i < Anzahl; i++)
                Strip->setPixelColor(LichtID + i, 0);
            iBlink++;
            if (iBlink < nBlink)
            {
                wechsel = jetzt + Blitzzeit;
                // Taus;
                Stat = stLauf3;
            }
            else
            {
                iBlink = 0;
                Stat = stLauf5;
                wechsel = jetzt + Tan;
            }
        }
        break;
    case stLauf5: /// Kette mit allen Farben füllen
        for (size_t i = 0; i < Anzahl; i++)
            Strip->setPixelColor(LichtID + i, Farben[(i + iBlink) % nFarben]);
        wechsel = jetzt + 2000 / Anzahl;
        //Blitzzeit;
        Stat = stLauf6;
        break;
    case stLauf6: /// und diese alle Farben durchlaufen lassen
        if (jetzt > wechsel)
        {
            iBlink++;
            Stat = stLauf5;
            if (iBlink > 20 * nBlink)
            {
                wechsel = jetzt + Tan;
                iBlink = 0;
                Stat = stLauf1;
            }
        }
        break;
    default:
        Stat = stLauf1;
        wechsel = 0;
        break;
    }
}
///  ---------------------------------------------------------------------
/**
 * @brief Construct a new Leuchtstoffröhre object
 * Leuchtstoffröhre
 * @param strip Adresse Neopixel
 * @param id Nummer des Pixels
 */
cLRLicht::cLRLicht(Adafruit_NeoPixel *strip, int id) : cBLicht(strip, id)
{
    wechsel = 0;
    Stat = stAus;
    FarbeLR = strip->Color(200, 200, 180);
    FarbeZuenden = strip->Color(45, 30, 0);
}
/**
 * @brief Leuchtstoffröhre einschalten
 * damit nich alle gleichzeitig angehen, kann man eine Verzögerungszeit angeben
 * @param Wartensekunden starten nach so viel Sekunden
 */
void cLRLicht::ein(unsigned int Wartensekunden)
{
    if (Wartensekunden < 1)
    {
        Stat = stStart1;
        nZuenden = random(1, 3);
        iZuenden = 0;
        istAn = true;
    }
    else
    {
        wechsel = millis() + 1000UL * (unsigned long)Wartensekunden;
        Stat = stEinWarten;
    }
}
/**
 * @brief Leuchtstoffröhre ausschalten
 * damit nich alle gleichzeitig ausgehen, kann man eine Verzögerungszeit angeben
 * @param Wartensekunden aus nach so viel Sekunden
 */
void cLRLicht::aus(unsigned int Wartensekunden)
{
    if (Wartensekunden < 1)
    {
        cNLicht::aus();
        Stat = stAus;
        istAn = false;
    }
    else
    {
        wechsel = millis() + 1000UL * (unsigned long)Wartensekunden;
        Stat = stAusWarten;
    }
}
void cLRLicht::check()
{
    unsigned long jetzt;
    jetzt = millis();
    switch (Stat)
    {
    case stEinWarten:
        if (jetzt > wechsel)
            ein();
        else
            break;
    case stStart1:
        wechsel = jetzt + Blitzzeit * random(8, 16);
        Strip->setPixelColor(LichtID, FarbeZuenden);
        Stat = stStart2;
        break;
    case stStart2:
        if (jetzt > wechsel)
        {
            wechsel = jetzt + Blitzzeit * random(4, 8);
            Strip->setPixelColor(LichtID, FarbeLR);
            Stat = stStart3;
        }
        break;
    case stStart3:
        if (jetzt > wechsel)
        {
            iZuenden++;
            if (iZuenden > nZuenden)
            {
                Stat = stEin;
            }
            else
            {
                Stat = stStart4;
                Strip->setPixelColor(LichtID, 0);
                wechsel = jetzt + Blitzzeit * random(8, 12);
            }
        }
        break;
    case stStart4:
        if (jetzt > wechsel)
        {
            Stat = stStart1;
        }
        break;
    case stAusWarten:
        if (jetzt > wechsel)
            aus(0);
        break;
    case stEin:
    case stAus:
        break;
    default:
        Stat = stAus;
        break;
    }
}
