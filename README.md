# Simple Domotic House
A simple domotic house made with Arduino UNO R3.

The house always (unless you turn off the display) shows the outdoor temperature (°C) and humidity (%).

You can control the house by opening/closing the door, turning on/off the display and turning on/off the indoor light.

When the night has been detected (by a photoresistor), a routine (handled with interrupt) closes the door and turns on all the lights, when the day comes again, another routine opens the door once again and turns 
off all the lights.
