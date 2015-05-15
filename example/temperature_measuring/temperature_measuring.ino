#include <Bridge.h>

#include <OneWire.h>            
#include <DallasTemperature.h>  

#define DS18B20_PIN 2   // define DS18B20 pin on Arduino D2

OneWire oneWire(DS18B20_PIN);          // set oneWire Refernece
DallasTemperature sensors(&oneWire);   // initialize DS18B20

// define Temperature Sensor ID's
DeviceAddress Boiler_O = { 0x28, 0x61, 0x02, 0xBB, 0x03, 0x00, 0x00, 0x88 };
DeviceAddress Boiler_M = { 0x28, 0x11, 0xC8, 0xBA, 0x03, 0x00, 0x00, 0x00 };
DeviceAddress Boiler_U = { 0x28, 0x44, 0xF4, 0xBA, 0x03, 0x00, 0x00, 0x60 };



int i=0;

void setup() {
  pinMode(13,OUTPUT);  // to signal the update
  
  Bridge.begin();
  
  sensors.begin();
  sensors.setResolution(Boiler_O, 9);
  sensors.setResolution(Boiler_M, 9);
  sensors.setResolution(Boiler_U, 9);
  
  CallUpdate();    // generate initial readout
}

void loop() {

  if (i == 1200){ // every minute
    CallUpdate(); 
  }
  
  delay(50);
  i++;
  
}

void CallUpdate(){
  
  digitalWrite(13, HIGH);  // update start
  
  sensors.requestTemperatures();
  float boiler_o = sensors.getTempC(Boiler_O);  // temp value with comma
  int boiler_m = sensors.getTempC(Boiler_M);  // temp value without comma
  int boiler_u = sensors.getTempC(Boiler_U);

  Bridge.put("boiler_oben", String(boiler_o));  // put values to bridge
  Bridge.put("boiler_mitte", String(boiler_m));  // put values to bridge
  Bridge.put("boiler_unten", String(boiler_u));  // put values to bridge
  
  i=0;
  
  digitalWrite(13, LOW);  // update end
}
