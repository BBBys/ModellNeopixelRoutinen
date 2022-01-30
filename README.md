# ModellNeopixelRoutinen
 Ansteuerung von Neopixel-Ketten für Modelleisenbahn- und Modellbau-Zwecke (Häuser, Autos, Signale usw.)

## Blinklichter
- cBLicht:     einfaches Blinklicht, Oberklasse für andere
- cBaulicht:   gelbes Blinklicht für Baustellen
- cLaufLicht:  Lauf-Blinklicht für Absperrungen: gelbes Licht, das die Kette entlangläuft. Farbe kann geändert werden.
- cBlaulicht:  einfaches Blaulicht, Polizei, Feuerwehr ...
- c2BlauLicht: Zwei Blinklichter, blau oder gelb, die gleichzeitig und abwechselnd blinken. Für Modellautos: Polizei, Feuerwehr, Abschleppwagen ...

## Beleuchtung
- cSchreibLicht: nichts weiter als eine funzelige Schreibtischlampe 
- cLRLicht:      Leuchtstoffröhre, mit Zündvorgang

## Aktivitäten
- cTVLicht: Fernseher-Simulation, Farbwechsel, mal schneller, mal langsamer

# einzelne LEDs (keine Neopixel)
- cDLicht: Licht am Digitalausgang, eine einzelne LED an einem PWM-Ausgang. Einschalten, ausschalten, auf- und abblenden ist implementiert.

# Basisklasse
- cLicht: speichert ID (=Pinnummer für LED-Anschluss oder Pixel-ID für Neopixel) und Status (an/aus)