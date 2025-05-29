# Simple Domotic House
A simple domotic house project made with Arduino UNO R3.

You can find a short video demonstrating this project here: https://youtu.be/EtnrSQTw_zk

### Description
The house always (unless you turn off the display) shows the outdoor temperature (°C or °F) and humidity (%).

During the day you can manually control the house by opening/closing the door, turning on/off the display and convert Celsius degrees to Fahrenheit and viceversa.
When the night is detected the door closes and the lights turn on automatically, the door control is disabled as a security but you can still control the display.
When the day comes again the "night mode" gets disabled and all controls are available again.

To control the house, use the following buttons on remote:
- Power Button: turn on/off display
- Down Arrow: close the door
- Up Arrow: open the door
- "0" Button: change temperature from Celsius to Fahrenheit and viceversa

The schematics and the CAD models can be found in "resources" folder.

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
The first part of the code is dedicated to variables, constants and objects declaration.
In the main loop the isDay variable gets constantly checked, and it is modified by Night() function, called by an interrupt on change (IoC)
on pin 2 (photoresistor as a brightness control), when the value falls to 0 (falling edge), the variable isDay is mutated to false and the instructions of "night mode" in loop() are executed. Once the pin 2 value turns to 1, the variable isDay is mutated once again to true and the house can be fully controlled again.

The IoC function (Night()) was supposed to control the whole "night mode", but it wasn't possible because of millis() function (and similar) used in the Servo library, that won't work in the Interrupt function. In a nutshell, the interrupt only acts as a flag modifier: it mutates the isDay variable (flag) that controls the flow of the instructions in loop().

The Temperature and Humidity sensor and the IR sensor are controlled by non-blocking functions (these functions are using millis() and some calculations instead of delays, which block the code's flow for all the duration of the delay).

---


## Board schematic


![Schematic](https://github.com/Millenium6208/SimpleDomoticHouse/blob/main/resources/SimpleDomoticHouse_sch.png)


## AutoCAD design


![CAD Schematic](https://github.com/Millenium6208/SimpleDomoticHouse/blob/main/resources/SimpleDomoticHouse_cad.png)
