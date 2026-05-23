# Glassboard

a transparent NFC business card that lights up when you tap your phone to it. no battery. no charging. ever.

![3D render of Glassboard (Front)](images/3d_front.png)
![3D render of Glassboard (Back)](images/3d_back.png)

---

## what it is

Glassboard is a credit card sized transparent PCB that stores my contact info and portfolio link in NFC memory. when someone taps their phone, two things happen at the same time — their phone opens my portfolio, and the card animates three green LEDs.

the whole thing is powered by energy harvested from the phone's own NFC field. the card is completely passive. there's no battery, no charging port, nothing. it just works.

it's made on JLCPCB's transparent FPC substrate so you can see every trace, every component, and the antenna spiral through the card.

---

## why i made it

i kept handing people paper business cards at hackathons and programs. felt wrong. i'm a hardware builder — the card should prove it the second someone touches it.

so i built one that does.

---

## how it works

**NFC chip:** NT3H2111 from NXP. stores my URL in 888 bytes of EEPROM. harvests energy from the phone's 13.56 MHz RF field and outputs it on VOUT pin (~3V at up to 5mA). when a field is detected, the FD pin goes LOW.

**MCU:** ATtiny202 sitting in deep sleep at 0.1µA. the FD pin triggers a pin-change interrupt, MCU wakes in microseconds, runs the LED animation (~2 seconds), goes back to sleep.

**antenna:** rectangular spiral coil, 5 turns, 0.5mm trace, 0.3mm spacing, generated directly on F.Cu via a custom KiCad footprint wizard script. tuned to 13.56 MHz with a 39pF capacitor.

**LEDs:** three green 0402 LEDs (Würth 150040GS73240). green because blue/white LEDs need ~3V forward voltage which is too close to VOUT floor. green runs at 2.1V Vf, leaving clean margin. current limited to 0.3mA each via 3.3kΩ resistors — visible through transparent substrate even at low current.

**total power budget when active:** ~1.9mA (MCU + 3 LEDs). comfortably within NT3H2111 VOUT 5mA limit.

---

## how to use it

tap any NFC-enabled phone to the component side of the card. your phone opens the stored URL. the LEDs animate.

to reprogram the URL: download NXP TagWriter app (free, iOS and Android). tap phone to card. write new NDEF record. done — no programmer needed.

to reprogram the MCU: connect a CH340 USB-UART adapter to the three test pads on the bottom edge (VCC, GND, UPDI). use megaTinyCore in Arduino IDE. select ATtiny202, UPDI programmer.

---

## PCB specs

| parameter | value |
|-----------|-------|
| substrate | transparent FPC (PET) |
| layers | 2 |
| thickness | 0.2mm |
| dimensions | 85.6 × 54mm (standard credit card) |
| surface finish | ENIG |
| manufacturer | JLCPCB |

![PCB layout screenshot](images/pcb.png)
![schematic screenshot](images/schematic.png)

---

## bill of materials

see `BOM.csv` for full BOM with links and quantities.

| component | part | qty |
|-----------|------|-----|
| NFC chip | NT3H2111W0FTT (TSSOP8) | 1 |
| MCU | ATtiny202-SSF (SOIC8) | 1 |
| green LED 0402 | Würth 150040GS73240 | 3 |
| 220nF 0402 cap | generic | 1 |
| 100nF 0402 cap | generic | 1 |
| 39pF 0402 cap | generic | 1 |
| 10pF 0402 cap (DNP) | generic | 1 |
| 10kΩ 0402 resistor | generic | 4 |
| 3.3kΩ 0402 resistor | generic | 3 |
| 1kΩ 0402 resistor | generic | 1 |
| transparent FPC PCB | JLCPCB | 1 |

**total BOM cost: ~$32 for 5 boards including shipping**

---

## firmware

`firmware/glassboard.ino` — ATtiny202 sketch using megaTinyCore

install megaTinyCore via Arduino IDE board manager. select ATtiny202, 5MHz internal clock, UPDI programmer.

---

## files

```
glassboard/
├── README.md
├── BOM.csv
├── firmware/
│   └── glassboard.ino
├── hardware/
│   ├── glassboard.kicad_pro
│   ├── glassboard.kicad_sch
│   ├── glassboard.kicad_pcb
│   └── gerbers/
├── Glassboard_Zine.pdf
├── images/
│   ├── 3d_back.png
│   ├── 3d_front.png
│   ├── pcb.png
│   ├── schematic.png
│   └── zine.png
```

---

## zine page

![zine page](images/zine.png)

---

made by Yakshith Kommineni, 17, Brampton Ontario Canada
