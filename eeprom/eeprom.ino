//
//  eeprom.ino
//
//  Author:          Michael Bernhard
//  Available from:  https://github.com/mchlbrnhrd/mbArduinoExample
//
//  Description:     Example code for using EEPROM
//
// GNU General Public License v3.0
//
// Copyright (C) 2022  Michael Bernhard
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.


// ===Brief description===
//
// ---run demo---
// load scetch to your Arduino and open Serial Monitor from Arduino IDE to see what happens.
// (bautrate: 9600). No additional hardware is required.
// 
// ---what is done?---
// In this example an user defined struct is stored in EEPROM and loaded from EEPROM
// For safety reason also a check sum is calculated. In case loading values from
// EEPROM went wrong then pre defined values are used. This will happens
// when code will run for the very first time - or when an error in EEPROM occured.
//
// ---functions---
// The examples shows:
// * example struct
// * function to store struct into EEPROM
// * function to load struct from EEPROM
// * function to calculate check sum
//
// ---demo---
// Example demo first load settings from EEPROM and print out one example value via Serial.
// Then an value is changed and saved into EEPROM.
// After reboot of Arudino the new value should be printed out.

#include "EEPROM.h"

// example struct which will be stored in EEPROM
struct ExampleSettingStruct_st {
    uint16_t ExampleValueA_ui16;
    uint16_t ExampleValueB_ui16;
    uint8_t ExampleValueC_ui8;
    uint8_t ExampleValueD_ui8;
    uint8_t ValueArray_pui8[8];

    // check value (will be calculated)
    uint8_t CheckSum_ui8;
};


ExampleSettingStruct_st myExampleSetting_st;

//============================
// setup
//============================
void setup() 
{
  Serial.begin(9600);
  Serial.println(F("Setup is called."));

  bool error=false;
  error=loadSettings();
  if (error) {
    Serial.println(F("Loading settings from EEPROM failed."));
  } else {
    Serial.println(F("Loading settings from EEPROM succeed."));
  }

  // print out an example value:
  Serial.print(F("previous ExampleValueA: "));
  Serial.println(myExampleSetting_st.ExampleValueA_ui16);
  Serial.print(F("CheckSum: "));
  Serial.println((uint16_t)myExampleSetting_st.CheckSum_ui8);
  

  // change value here:
  myExampleSetting_st.ExampleValueA_ui16 = 123;

  Serial.print(F("new ExampleValueA: "));
  Serial.println(myExampleSetting_st.ExampleValueA_ui16);
  Serial.print(F("CheckSum: "));
  Serial.println((uint16_t)myExampleSetting_st.CheckSum_ui8);

  // save new value
  saveSettings();

  Serial.println(F(""));
  Serial.println(F("Reboot Arduino to check if new value was stored in EEPROM"));
  Serial.println(F(""));
  Serial.println(F(""));
}


//============================
// loop
//============================
void loop()
{
  

}


//============================
// saveSettings
//============================
void saveSettings() {
  char l_Buffer_pc[sizeof(ExampleSettingStruct_st)];
  memcpy(&l_Buffer_pc[0], &myExampleSetting_st, sizeof(ExampleSettingStruct_st));
  for (uint16_t l_Ctr_ui16=0; l_Ctr_ui16 < sizeof(ExampleSettingStruct_st); ++l_Ctr_ui16) {
    EEPROM.update(l_Ctr_ui16, l_Buffer_pc[l_Ctr_ui16]);
  }
}


//============================
// loadSettings
//============================
bool loadSettings()
{
  bool ErrorInSettingStruct_bl=false;
  
  char l_Buffer_pc[sizeof(ExampleSettingStruct_st)];
  // read values into buffer
  for (uint16_t l_Ctr_ui16=0; l_Ctr_ui16 < sizeof(ExampleSettingStruct_st); ++l_Ctr_ui16) {
    l_Buffer_pc[l_Ctr_ui16] = EEPROM.read(l_Ctr_ui16);
  }

  // copy/convert buffer into myExampleSetting_st
  memcpy(&myExampleSetting_st, &l_Buffer_pc[0], sizeof(ExampleSettingStruct_st));

  // check if settings are valid
  const uint8_t CheckSum_ui8 = calculateSettingCheckSum_ui8(myExampleSetting_st);
  if (myExampleSetting_st.CheckSum_ui8 != CheckSum_ui8) {
    // reading settings from EEPROM went wrong
    // use pre defined settings and save them in EEPROM
    
    // error
    ErrorInSettingStruct_bl=true;

    
    // set pre defined data (initial data)
    myExampleSetting_st.ExampleValueA_ui16 = 100;
    myExampleSetting_st.ExampleValueB_ui16 = 15000;
    myExampleSetting_st.ExampleValueC_ui8 = 4;
    myExampleSetting_st.ExampleValueD_ui8 = 6;
    for (int k=0; k < 8; ++k) {
      myExampleSetting_st.ValueArray_pui8[k] = k;
    }
    
    saveSettings();
  }

  return ErrorInSettingStruct_bl;
}


//============================
// calculateSettingCheckSum
//============================
uint8_t calculateSettingCheckSum_ui8(const ExampleSettingStruct_st& f_Setting_st)
{
  uint8_t l_CheckSum_ui8=0;
  char l_Buffer_pc[sizeof(ExampleSettingStruct_st)];
  memcpy(&l_Buffer_pc[0], &f_Setting_st, sizeof(ExampleSettingStruct_st));
  for (uint16_t k=0; k < sizeof(ExampleSettingStruct_st)-1;++k) {
    l_CheckSum_ui8+=l_Buffer_pc[k];
  }
  return  l_CheckSum_ui8;
}
