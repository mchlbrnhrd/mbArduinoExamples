# mbArduinoExamples
Example scetches for arduino microcontroller from real projects. Therefore practical examples for Arduino Uno, Arduino Nano, Arduino Nano Every, Arduino Yún and also other microconrollers like ESP8266 (NodeMCU) or ESP32.

## EEPROM
scetch: **eeprom/eeprom.ino**

In this example an user defined struct is stored in EEPROM (Electrically Erasable Programmable Read-only Memory)  and loaded from EEPROM.
For safety reason also a check sum is calculated. In case that loading values from
EEPROM went wrong then pre defined values are used. This will happens
when code will run for the very first time - or when an error in EEPROM occured.

### Functions
The examples shows:
* example struct
* function to store struct into EEPROM
* function to load struct from EEPROM
* function to calculate check sum

### Run Demo
Load scetch to your Arduino and open Serial Monitor from Arduino IDE to see what happens.
Set bautrate to 9600. No additional hardware is required.

#### Content of demo
Example demo first loads settings from EEPROM and print out one example value via Serial.
Then an value is changed and saved into EEPROM.
After reboot of Arudino the new value should be printed out.

### Test environment
Tested with Arduino Uno.
