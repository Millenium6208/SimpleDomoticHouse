/*
  Author:   R. Faraoni
  GitRepo:  https://github.com/Millenium6208/SimpleDomoticHouse
  File:     SimpleDomoticHouse.ino
  Desc:     The house always (unless you turn off the display) shows the outdoor temperature (°C or °F) and humidity (%).
            During the day you can manually control the house by opening/closing the door, turning on/off the display 
            and convert Celsius degrees to Fahrenheit and viceversa. When the night is detected the door closes and the 
            lights turn on automatically, the door control is disabled as a security but you can still control the display. 
            When the day comes again the "night mode" gets disabled and all controls are available again.
  Ver:      3.0
  Date:     2025-05-30
*/

// Uncomment the following line of code to enable serial print and tests
//#define TEST 1

/*
  PINOUT ON ARDUINO
*/

// Light Sensor (with Interrupt)
// Previously the A0 pin was used to get brightness level, it was modified to digital pin 2
// in order to use interrupt on change, so it can only check wether it's day or night.
#define PHOTO_RESISTOR_PIN 2

// LCD
#define D7 3
#define D6 4
#define D5 5
#define D4 6
#define E 7
#define RS 8

/* LCD Pin Setup in Breadboard
VSS   ->  GND
VDD   ->  5V
V0    ->  Potentiometer
RW    ->  GND
D0-D3 ->  not connected
A     ->  5V
K     ->  GND
*/

// Components
#define TEMP_HUM_PIN 9
#define DOOR_SERVO_PIN 10
#define IR_RECEIVER_PIN 11
#define GARDEN_LEDS_PIN 12


/* 
  LIBRARIES
*/

#include <LiquidCrystal.h>
#include <Servo.h>
#include "IRremote.h"
#include <dht_nonblocking.h>


/* 
  CONSTANTS AND VARIABLES
*/

volatile bool isDay = true; // Flag used in interrupt
bool isCelsius = true; // Displays temp in celsius when true, in fahrenheit when false
#define DOOR_POSITION_CLOSED 25
#define DOOR_POSITION_OPENED 130
#define DHT_SENSOR_TYPE DHT_TYPE_11
bool display = true;
decode_results remote;


/*
  LIBRARIES' OBJECTS
*/

LiquidCrystal lcd(RS, E, D4, D5, D6, D7);
Servo doorServo;
IRrecv Receiver(IR_RECEIVER_PIN);
DHT_nonblocking tempHum(TEMP_HUM_PIN, DHT_SENSOR_TYPE);


/*
  FUNCTIONS' DECLARATION
*/

void openDoor();
void closeDoor();
void gardenOn();
void gardenOff();
int brightness();
int translateIR();
void displayTemp();
void night();

void setup() {
  // Pin and components initial setting
  attachInterrupt(digitalPinToInterrupt(PHOTO_RESISTOR_PIN), night, FALLING);
  pinMode(GARDEN_LEDS_PIN, OUTPUT);
  doorServo.attach(DOOR_SERVO_PIN);
  Receiver.enableIRIn();
  lcd.begin(16, 2); // Sets a 16 cols by 2 rows display
  
  // Initialize the state of the house when first turned on
  openDoor();
  gardenOff();
  lcd.setCursor(0, 0);
  lcd.print("DomoticHouse");
  lcd.setCursor(0, 1);
  lcd.print("R. Faraoni");

  #ifdef TEST
    Serial.begin(9600);
  #endif
}


void loop() {
  if (isDay) { // Executed when it's day
    #ifdef TEST
      // Plot brightess and 
      Serial.println(brightness());
      Serial.print("IR: ");Serial.println(translateIR());
    #endif
    
    // Display Temp and Hum
    displayTemp();
    
    // Listens any event on the IR remote sensor
    if (Receiver.decode(&remote)) {
      switch (translateIR()) {
        case 0: // Power button on remote
          if (display) {
            lcd.noDisplay(); // Turn off display
          }
          else {
            lcd.display(); // Turn off display
          }
          display = !display; // Negate display state
        break;
        
        case 6: // Down arrow on remote
          closeDoor();
        break;

        case 8: // Up arrow on remote
          openDoor();
        break;

        case 9: // 0 on remote
          isCelsius = !isCelsius;
          displayTemp();
        break;
      }

      // If none of the previous, read another value
      Receiver.resume();
      delay(100); // Little delay to ensure correct reading
  }
  } else { // Executed when it's night
    #ifdef TEST
      // Plot brightess and 
      Serial.println(brightness());
      Serial.print("IR: ");Serial.println(translateIR());
    #endif
    
    closeDoor();
    gardenOn(); // Turn on garden lights

    while (digitalRead(2) == 0) { // Wait for the day
      #ifdef TEST
        Serial.println("Night while loop.");
      #endif
      if (Receiver.decode(&remote)) {
        switch (translateIR()) {
          case 0: // Power button on remote
            if (display) {
              lcd.noDisplay(); // Turn off display
            }
            else {
              lcd.display(); // Turn off display
            }
            display = !display; // Negate display state
          break;

          case 9: // 0 on remote
            isCelsius = !isCelsius;
            displayTemp();
          break;
        }
  
        // If none of the previous, read another value
        Receiver.resume();
        delay(100); // Little delay to ensure correct reading
      }
      
      displayTemp(); // Display Temp and Hum
    }

    // When day comes, open door, turn off lights and go back to loop()
    openDoor(); // Open the door
    gardenOff(); // Turn off garden lights
    delay(20);
  }
}

void openDoor() {
  doorServo.write(DOOR_POSITION_OPENED);
}

void closeDoor() {
  doorServo.write(DOOR_POSITION_CLOSED);
}

void gardenOn() {
  digitalWrite(GARDEN_LEDS_PIN, HIGH);
}

void gardenOff() {
  digitalWrite(GARDEN_LEDS_PIN, LOW);
}

// Returns brightness from PHOTO_RESISTOR_PIN in range 0 to 127.
int brightness() {
  return map(analogRead(A0), 0, 1023, 0, 127);
}

int translateIR()
{
  switch(remote.value)
  {
    case 0xFFA25D: return 0; break;
    case 0xFF629D: return 1; break;
    case 0xFFE21D: return 2; break;
    case 0xFF22DD: return 3; break;
    case 0xFF02FD: return 4; break;
    case 0xFFC23D: return 5; break;
    case 0xFFE01F: return 6; break;
    case 0xFFA857: return 7; break;
    case 0xFF906F: return 8; break;
    case 0xFF9867: return 10; break;
    case 0xFFB04F: return 11; break;
    // Digits
    case 0xFF6897: return 9; break;
    case 0xFF30CF: return 12; break;
    case 0xFF18E7: return 13; break;
    case 0xFF7A85: return 14; break;
    case 0xFF10EF: return 15; break;
    case 0xFF38C7: return 16; break;
    case 0xFF5AA5: return 17; break;
    case 0xFF42BD: return 18; break;
    case 0xFF4AB5: return 19; break;
    case 0xFF52AD: return 20; break;
    // Repeat
    case 0xFFFFFFFF: return 30; break;  

    // Other Button
    default: return 100; break;
  }
}

//
static bool environmentTempHum(float *temperature, float *humidity) {
  static unsigned long counter = millis();

  // Return every 2 seconds
  if (millis() - counter > 2000ul) {
    if (tempHum.measure(temperature, humidity) == true) {
      counter = millis(); // Update timer
      return true;
    }
  }

  return false;
}

void displayTemp() {
  // DHT11 (Hum and Temp) sensor variables
  float temperature, humidity;
  if (environmentTempHum(&temperature, &humidity) == true) {
    lcd.clear(); // Clear lcd before printing

    // Temperature on row 1 [0]
    lcd.setCursor(0, 0);
    lcd.print("Temp:");
    lcd.setCursor(6, 0);
    if (isCelsius) { // Print the temperature in Celsius degrees
      lcd.print(round(temperature));
      lcd.setCursor(9, 0);
      lcd.print("C");
    } else { // Print the temperature in Fahrenheit degrees
      lcd.print(round(temperature) * 9/5 + 32);
      lcd.setCursor(9, 0);
      lcd.print("F");
    }

    // Humidity on row 2 [1]
    lcd.setCursor(0, 1);
    lcd.print("Hum:");
    lcd.setCursor(6, 1);
    lcd.print(round(humidity));
    lcd.setCursor(9, 1);
    lcd.print("%");
  }
}

void night() { // Activated on pin 2 interrupt on FALLING EDGE
               // When the night comes, the function turns on lights and closes the door
               // Due to problems with PWM used in doorServo, everything is done in loop()
  isDay = false;
}
