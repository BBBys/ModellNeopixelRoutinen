/**
 * @file Licht.cpp
 * @brief Funktionen zur Steuerung von einfachen Lichtern
 * @version 1.0
 * @date 26 Jan 2022 23 4 3 2 1 Nov 31 30 29 28 26 Okt 2021
 * @author Dr. Burkhard Borys, Zeller Ring 15, 34246 Vellmar, Deutschland
 * @copyright Copyright (c) 2021-2022 B. Borys
 */
#include "Licht.h"
/**
 * @brief Construct a new allgemeines Licht object
 * ID (=Pinnummer oder Pixel-ID) übernehmen, Status=aus
 * @param pPin 
 */
cLicht::cLicht(int pPin)
{
    LichtID = pPin;
    istAn = false;
}
/**
 * @brief true, wenn eingeschaltet
 * @return true 
 * @return false 
 */
bool cLicht::istEin()
{
    return istAn;
}
/**
 * @brief ein-oder ausschalten
 * 
 * @param p aus, wenn false; default: ein
 */
 void cLicht::setzen(bool p)
{
  //DbgOut("cLicht::setzen");
    if (p)
        ein();
    else
        aus();
}
void cLicht::aus()
{
    istAn = false;
}
void cLicht::ein()
{
    istAn = true;
}
void cDLicht::weg()
{
    int d;
    for (d = 500; d > 50; d = d - 20)
    {
        aus();
        delay(75);
        ein();
        delay(d);
    }
    aus();
}
void cDLicht::hin()
{
    int d;
    for (d = 50; d < 500; d = d + 20)
    {
        ein();
        delay(d);
        aus();
        delay(75);
    }
    ein();
}
cDLicht::cDLicht(int id) : cLicht(id)
{
    pinMode(id, OUTPUT);
    aus();
}
void cDLicht::aus()
{
    cLicht::istAn = false;
    digitalWrite(LichtID, LOW);
}
void cDLicht::ein()
{
    digitalWrite(LichtID, HIGH);
    cLicht::istAn = true;
}
cDLicht::~cDLicht()
{
}
