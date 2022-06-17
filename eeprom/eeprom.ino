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


//============================
// setup
//============================
void setup() 
{
  delay(3000);
  Serial.begin(9600);
  delay(3000);
  Serial.println(F("\nSetup is called."));

  bool l_Error_bl=false;
  ExampleSettingStruct_st l_MyExampleSetting_st;
  
  l_Error_bl=loadSettings_bl(l_MyExampleSetting_st);
  if (l_Error_bl) {
    Serial.println(F("Error: Loading settings from EEPROM failed."));
  } else {
    Serial.println(F("Loading settings from EEPROM succeed."));
  }

  // print out an example value:
  Serial.print(F("previous ExampleValueA: "));
  Serial.println(l_MyExampleSetting_st.ExampleValueA_ui16);
  Serial.print(F("CheckSum: "));
  Serial.println((uint16_t)l_MyExampleSetting_st.CheckSum_ui8);
  
  // change value here and update check sum
  l_MyExampleSetting_st.ExampleValueA_ui16 = 123;
  l_MyExampleSetting_st.CheckSum_ui8 = calculateSettingCheckSum_ui8(l_MyExampleSetting_st);

  Serial.print(F("new ExampleValueA: "));
  Serial.println(l_MyExampleSetting_st.ExampleValueA_ui16);
  Serial.print(F("CheckSum: "));
  Serial.println((uint16_t)l_MyExampleSetting_st.CheckSum_ui8);

  // save new value
  saveSettings_vd(l_MyExampleSetting_st);

  Serial.println(F("\nReboot Arduino to check if new value was stored in EEPROM\n\n"));
}


//============================
// loop
//============================
void loop()
{
  // do nothing in this example
}


//============================
// saveSettings
//============================
void saveSettings_vd(const ExampleSettingStruct_st& f_MyExampleSetting_st) {
  const uint8_t* const l_StructPointer_pui8=  (const uint8_t*)&f_MyExampleSetting_st; // cast to 8 bit pointer
  for (uint16_t l_Ctr_ui16=0; l_Ctr_ui16 < sizeof(ExampleSettingStruct_st); ++l_Ctr_ui16) {
    EEPROM.update(l_Ctr_ui16, l_StructPointer_pui8[l_Ctr_ui16]); // save byte by byte
  }
}


//============================
// loadSettings
//============================
bool loadSettings_bl(ExampleSettingStruct_st& f_MyExampleSetting_st)
{
  bool l_ErrorInSettingStruct_bl=false;
  uint8_t* l_StructPointer_pui8 = (uint8_t*)&f_MyExampleSetting_st; // get address of location
  
  // read values into buffer
  for (uint16_t l_Ctr_ui16=0; l_Ctr_ui16 < sizeof(ExampleSettingStruct_st); ++l_Ctr_ui16) {
    l_StructPointer_pui8[l_Ctr_ui16] = EEPROM.read(l_Ctr_ui16);
  }

  // check if settings are valid
  const uint8_t l_CheckSum_ui8 = calculateSettingCheckSum_ui8(f_MyExampleSetting_st);
  if (f_MyExampleSetting_st.CheckSum_ui8 != l_CheckSum_ui8) {
    // reading settings from EEPROM went wrong
    // use pre defined settings and save them in EEPROM
    
    // error
    l_ErrorInSettingStruct_bl=true;

    // set pre defined data (initial data)
    f_MyExampleSetting_st.ExampleValueA_ui16 = 100;
    f_MyExampleSetting_st.ExampleValueB_ui16 = 15000;
    f_MyExampleSetting_st.ExampleValueC_ui8 = 4;
    f_MyExampleSetting_st.ExampleValueD_ui8 = 6;
    for (uint8_t k=0; k < 8; ++k) {
      f_MyExampleSetting_st.ValueArray_pui8[k] = k;
    }
    // update checksum
    f_MyExampleSetting_st.CheckSum_ui8 = calculateSettingCheckSum_ui8(f_MyExampleSetting_st);

    // save default value into EEPROM
    saveSettings_vd(f_MyExampleSetting_st);
  }

  return l_ErrorInSettingStruct_bl;
}


//============================
// calculateSettingCheckSum
//============================
uint8_t calculateSettingCheckSum_ui8(const ExampleSettingStruct_st& f_Setting_st)
{
  uint8_t l_CheckSum_ui8=1; // init value not 0: otherwise empty EEPROM will be detected as valid
  const uint8_t* const l_StructPointer_pui8 = (const uint8_t*)&f_Setting_st; // get 8 bit pointer of struct

  // loop over buffer but do not consider already existing checksum value into calculation
  // therefore: sizeof() - 1, because last entry is check sum
  for (uint16_t k=0; k < sizeof(ExampleSettingStruct_st)-1;++k) {
    l_CheckSum_ui8+=l_StructPointer_pui8[k];
  }
  return  l_CheckSum_ui8;
}
