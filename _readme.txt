Typ:    ATmega1284P
Frequenz:   18.432 MHz

Pinbelegung:

PB0 DISP D4
PB1 DISP D5
PB2 DISP D6
PB3 DISP D7
PB4 /SDSEL
PB5 MOSI
PB6 MISO
PB7 SCK

PD0 RXD
PD1 TXD
PD2 DISP RS
PD3 DISP E
PD4 /Button Play
PD5 /Button Break
PD6 /Button Rec
PD7 /Button Dir

PC  DATA 0..7

PA0 /WE
PA1 /OE
PA2 RECORD
PA3 AUX
PA4 PLAY
PA5 /CTS
PA6 Counter Reset
PA7 Counter


Fuses:
LFUSE   FF
HFUSE   D9
EFUSE   FF

Fuses mit Bootloader (4096 words @ $F000):
LFUSE   FF
HFUSE   D8
EFUSE   FF


Tapeformat
1 Byte: Blocknummer (beginnend bei 1 (Vorblock), letzer Block = 0xff)
128 Byte: Nutzdaten
1 Byte: Prüfsumme über die Daten (einfach addiert Modulo 256)

Bei BASIC-Dateien gilt ein anderer Aufbau: Hier enthält der erste Block eine 1-Byte-Block-Nummer, eine 3-Byte-Typ- und eine 8-Byte-Namensinformation. Ab dem 13. Byte des ersten Blockes sind Daten enthalten.
