/**
 * @file Licht.h
 * @brief Funktionen zur Steuerung von Lichtern
 * @version 1.3
 * @date 29 28 26 25 15 Jan 2022 13 1 Dez 23 7 6 5 4 3 2 1 Nov 31 30 29 28 26 Okt 2021
 * @author Dr. Burkhard Borys, Zeller Ring 15, 34246 Vellmar, Deutschland <Human-Factors@gmx.net>
 * @copyright Copyright (c) 2021-2022 B. Borys
 * Info zu Neopixel: https://learn.adafruit.com/adafruit-neopixel-uberguide/ 
 */
#ifndef Licht_H
#define Licht_H
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

enum RGBFarben
{
    NEOROT = 0xff0000,
    NEOGELB = 0xff5F00,
    NEOGRUEN = 0x00ff00,
    NEOBLAU = 0x0000ff,
    NEOWEISS = 0xffffff
};
enum HueFarben
{
    HUEROT      = 0,
    HUEGELB     = 10923U,       // 65336/6
    HUEGRUEN    = 21845U,       // 65336/3
    HUEBLAU     = 43691U,       // 65336*2/3
    HUECYAN     = 32768U,       // 65336/2
    HUEMAGENTA  = 54613U        // 65336*5/6
};

/**
 * @brief Basisklasse Licht/Lampe/Beleuchtung
 * 
 */
class cLicht
{
private:
protected:
    int LichtID; // Pinnumer für LED oder Adresse für Neopixel
    bool istAn;

public:
    cLicht(int pPin1);
    virtual void setzen(bool p);
    virtual void ein();
    virtual void aus();
    virtual bool istEin();
};
/**
 * @brief Klasse für Licht am Digitalausgang
 * 
 */
class cDLicht : cLicht
{
private:
    /* data */
public:
    cDLicht(int id);
    ~cDLicht();
    void aus();
    void ein();
    void weg();
    void hin();
};
/**
 * @brief Klasse für Licht an NeoPixel-Kette
 * 
 */
class cNLicht : public cLicht
{
private:
protected:
    Adafruit_NeoPixel *Strip;
    uint32_t Farbe;

public:
    cNLicht(Adafruit_NeoPixel *strip, int id);
    cNLicht(Adafruit_NeoPixel *strip, int id, long pFarbe);
    virtual void aus();
    virtual void ein();
    ~cNLicht();
};
/**
 * @brief Klasse für mehrere Lichter einfarbig an NeoPixel-Kette
 * 
 */
class cKetteLicht : public cNLicht
{
private:
protected:
    unsigned int Anzahl;

public:
    cKetteLicht(Adafruit_NeoPixel *strip, int id, int Anzahl, uint32_t Farbe = NEOWEISS);
    virtual void aus();
    virtual void ein(uint32_t Farbe = NEOWEISS);
};
/**
 * @brief Klasse für mehrere Lichter zweifarbig an NeoPixel-Kette
 * Abstand der Farbwechsel wird im Befehl ein() gesetzt
 */
class cKette2Licht : public cKetteLicht
{
private:
    uint32_t Farbe2;

protected:
public:
    cKette2Licht(Adafruit_NeoPixel *strip, unsigned int id, unsigned int Anzahl, uint32_t Farbe1 = NEOROT, uint32_t Farbe2 = NEOGELB);
    virtual void ein(unsigned int nAbstand = 1, uint32_t Farbe1 = 0, uint32_t Farbe2 = 0);
};
/**
 * @brief Klasse für Stoboskop-Licht mit mehreren Lichtern an NeoPixel-Kette.
 * 
 */
class cStrobeLicht : public cKetteLicht
{
private:
    int Auszeit, Einzeit;
    unsigned long jetzt = 1, warten = 0;
    enum Status
    {
        stAus,      /// es blitzt nicht
        stEinHell,  /// es blitzt, derzeitig ein
        stEinDunkel /// es blitzt, derzeitig aus
    } Stat;

protected:
public:
    cStrobeLicht(Adafruit_NeoPixel *strip, int id, int Anzahl = 1, uint32_t Farbe = NEOWEISS);
    void check();
    void aus();
    void zeiten(int auszeit = 75,int einzeit = 25 );
    void ein(uint32_t Farbe = NEOWEISS);
};
/**
 * @brief Klasse für Blinklicht.
 * Oberklasse für andere blinkende Lichter
 */
class cBLicht : public cNLicht
{
private:
protected:
    enum Status
    {
        stAus,
        stBlink,
        stEin
    }Stat;
   unsigned long wechsel, Tan, Taus;
    unsigned long Blitzzeit = 100;
public:
    enum BlArt
    {
        blaPolizei,blaBlau,
        blaAbschleppwagen,blaGelb,blaAmpel,
        blaRot
    };
    void set(int pTan, int pTaus);
    void set(int pTan, int pTaus, int pW);
    cBLicht(Adafruit_NeoPixel *strip, int id, BlArt art = blaGelb, int pTan = 5000, int pTaus = 2000);
    virtual void blinken(bool pAn = true);
    virtual void check();
};
/**
 * @brief Klasse für Leuchtstoffröhre
 * 
 */
class cLRLicht : public cBLicht
{
private:
    enum Status
    {
        stAus,
        stStart1,
        stStart2,
        stStart3,
        stStart4,
        stEin,
        stAusWarten
    } Stat;
    unsigned int nZuenden, iZuenden;
    uint32_t FarbeZuenden, FarbeLR;

protected:
    //    unsigned long wechsel, Tan, Taus;
    //  const unsigned long Blitzzeit = 100;

public:
    //void set(int pTan, int pTaus);
    //void set(int pTan, int pTaus, int pW);
    cLRLicht(Adafruit_NeoPixel *strip, int id);
    //virtual void blinken(bool pAn = true);
    void ein();
    void aus(unsigned int WartenSekunden = 0);
    void check();
};
/**
 * @brief Klasse für einfaches gelbes Baustellen-Blinklicht
 * ein einzelnes Pixel
 * Zeit einstellbar mit set(...) aus cBLicht
 * Steuerung über check() aus cBLicht
 */
class cBauLicht : public cBLicht
{
private:
   cBLicht::Status Stat;
protected:
public:
    cBauLicht(Adafruit_NeoPixel *strip, int id,int pTan = 3000, int pTaus = 500);
};
/**
 * @brief Klasse für Absperrungs-Lauflicht
 * gelbes Licht, das die Kette entlangläuft
 */
class cLaufLicht : public cBauLicht
{
private:
    unsigned int iLicht, // Offset aktuelles Licht zu Start-LichtID
        iBlink, iLauf;
    const unsigned int nBlink = 4, //4 mal blinken,
    nLauf = 3;  //  3 mal durchlaufen
    uint32_t FarbeDunkler, FarbeDunkel;

protected:
    unsigned int Anzahl;
    enum Status
    {
        stAus,
        stLauf1,
        stLauf2,
        stLauf3,
        stLauf4,
        stLauf5,
        stLauf6,
        stEin
    } Stat;

public:
    cLaufLicht(Adafruit_NeoPixel *strip, int id, int Anzahl, int pTan = 100);
    void farbe(uint8_t r=111, uint8_t g=25, uint8_t b=0);
    void check();
    void blinken(bool an = true);
    void aus();
};
/**
 * @brief Klasse für blinkendes Blaulicht
 * 
 */
class cBlauLicht : public cBLicht
{
private:
protected:
    const int nBlink = 3;
    int iBlink = 0;
    enum Status
    {
        stAus,
        stBlink1,
        stBlink2,
        stBlink3,
        stBlink4,
        stEin
    } Stat;

public:
    cBlauLicht(Adafruit_NeoPixel *strip, int id, BlArt art = blaPolizei, int pTan = 66, int pTaus = 200);
    // virtual void blinken(bool pAn = true);
    // virtual void check();
    // void aus(int i);
    // void ein(int i);
};
/**
 * @brief Klasse für Fernseher-Simulation
 */
class cTVLicht : public cBLicht
{
private:
    unsigned long wechsel2 = 0;

protected:
    const int nBlink = 3;
    int iBlink = 0;
    enum Status
    {
        stAus,
        stBlink1,
        stBlink2,
        stBlink3,
        stBlink4,
        stEin
    } Stat;

public:
    cTVLicht(Adafruit_NeoPixel *strip, int id);
    virtual void check();
    void aus();
    void ein();
};
/**
 * @brief zwei blinkendes Blaulichter
 * 
 */
class c2BlauLicht : public cBlauLicht
{
private:
    bool istAn[2];
    const int nFolge = 6;
    int iFolge = 0;

public:
    c2BlauLicht(Adafruit_NeoPixel *strip, int id, BlArt art = blaPolizei, int pTan = 33, int pTaus = 100);
    void blinken(bool pAn = true);
    void check();
    void aus(int i);
    void ein(int i);
    void setzen(bool p0 = true, bool p1 = true);
};
/**
 * @brief Klasse für Party-Lauflicht
 * - Kette 2...10x mit zufälligen Farben füllen
 * - - mx. 60 Farben, gleichmäßig im Farbraum verteilt
 * - Kette mit 1...10 ähnlichen Farben füllen
 * - - und mit diesen ähnlichen Farben blitzen lassen
 * - Kette mit allen Farben füllen
 * - - und diese alle Farben durchlaufen lassen
 */
class cPartyLicht : public cLaufLicht
{
private:
    unsigned int iLicht, iBlink, nBlink = 4, nLauf = 3, iLauf;
    unsigned int nFarben, FarbIndex;
    const unsigned int MAXFARBEN = 60U;
    uint32_t Farben[60];

protected:
public:
    cPartyLicht(Adafruit_NeoPixel *strip, unsigned int StartId, unsigned int Anzahl, unsigned int nFarben = 5, unsigned int Tan = 500,
                unsigned int TBlitz = 75);
    void check();
};
/**
 * @brief Klasse für Schreibtischlampe an NeoPixel-Kette
 * 
 */
class cSchreibLicht : public cNLicht
{
private:
protected:
public:
    cSchreibLicht(Adafruit_NeoPixel *strip, int id);
};

#endif