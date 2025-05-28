# Simple Domotic House
A simple domotic house made with Arduino UNO R3.

The house always (unless you turn off the display) shows the outdoor temperature (°C) and humidity (%).

You can manually control the house by opening/closing the door and turning on/off the display.

When the night has been detected (by a photoresistor), a routine (handled with interrupt) closes the door and turns on all the lights. When the day comes , you can once again control the house. 

## Board schematic
![Schematic](https://github.com/Millenium6208/SimpleDomoticHouse/blob/main/SimpleDomoticHouse_sch.png)

---

## House designed using AutoCAD
You can find the image file and the AutoCAD project in data folder.
![Schematic](https://github.com/Millenium6208/SimpleDomoticHouse/blob/main/SimpleDomoticHouse_cad.bmp)
