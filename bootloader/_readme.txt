Angepasst an KC-Recorder mit ATmega1284P (Boert, 05/2015)


Verwendung:
Wird die rechte Taste des KC-Recorder beim Einschalten gedrückt gehalten, wird der Bootloader aktiviert.
Dies erkennt man an der blinkenden AUfnahme-LED und der Ausschrift 'bootmode' im Display.

In diesem Zustand wird der KC-Recorder von außen von verschiedenen Programmen (AVRStudio, avrdude) als Programmiergerät erkannt.
Als Protokoll wird das 'STK500v2'-Protokoll verwendet. Die Bitrate beträgt 115200 bps.

Der Aufruf von avrdude erfolgt z.B. so:
avrdude -p atmega1284p -P /dev/tty.usbserial   -c stk500v2   -b115200   -U flash:w:main.hex

Die verwendete Schnittstelle muß an die eigenen Gegebenheiten angepasst werden.

Ohne Druck auf die rechte Taste wird das eingespielte Programm gestartet.
Ist kein Programm vorhanden, wird der Bootloader angesprungen.



Kompilieren und einspielen:
- Im Makefile den verwendeten Programmer anpassen (AVRDUDE_PROGRAMMER und AVRDUDE_PORT)
- kompilieren mit 'make'
- Flashen mit 'make program'
- Fuses richtig setzen mit 'make fuses' (nur einmalig nötig)


Source-Link:
http://homepage.hispeed.ch/peterfleury/avr-software.html

HFUSE für Bootsize
0xde	512 words
0xdc   1024 words
0xda   2048 words
0xd8   4096 words
