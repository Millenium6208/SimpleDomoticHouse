# Simple Domotic House
A simple domotic house project made with Arduino UNO R3.

### Description
The house always (unless you turn off the display) shows the outdoor temperature (°C) and humidity (%).

During the day you can manually control the house by opening/closing the door, turning on/off the display and convert Celsius degrees to Fahrenheit and viceversa.
When the night is detected the door closes and the lights turn on automatically, the door control is disabled as a security but you can still control the display.
When the day comes again the "night mode" gets disabled and all controls are available again.

### Components used
If you want to recreate this project you'll need the following components:
- Arduino UNO R3 (or similar)
- Resistors (possibly 330Ω and 10kΩ, the second one can be modified to adapt the amount of luminosity needed to enter "night mode", or even use a potentiometer)
- Photoresistor
- LCD Display 16x2 (16 pins) with a potentiometer (or use a display serial module so you'll only have 4 pins to connect).
- IR Receiver (with remote controller)
- LED strip and a single LED (used in the garden and inside the house)
- Temperature and Humidity sensor
- Servomotor (used to open and close the door)

### Code explanation

---

## Board schematic
![Schematic](https://github.com/Millenium6208/SimpleDomoticHouse/blob/main/resources/SimpleDomoticHouse_sch.png)

---

## AutoCAD design
You can find the image file and the AutoCAD project in data folder.

![CAD Schematic](https://github.com/Millenium6208/SimpleDomoticHouse/blob/main/resources/SimpleDomoticHouse_cad.png)
