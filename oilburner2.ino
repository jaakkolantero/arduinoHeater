#include <Wire.h>

/*
[1] Check thermostat status
      If temperature under set value from thermostat:
      -Start burner
      -Start cooling fan
      When wanted temperature is reached:
      -Stop burner
      -Stop cooling fan after certain time (to cool down oil burner)
[2] 2-modes for cooling fan
      Automatic
      Manual
[3] adjust cooling time with potentiometer
*/

//const int fanDelay = 300000; //delay in millis (300 000 = 5Minutes)

const int fanPin = 2;
const int burnerPin = 3;
const int thermostatPin = 4;

typedef struct _Heater{
  int fan;
  int burner;
  int fanDelay;
  int burnerLock;
  unsigned long heaterTimer;
} Heater;

void thermostat(struct _Heater *heater);
void fanDelay(struct _Heater *heater);
void burner(struct _Heater *heater);
void fan(struct _Heater *heater);

void setup() {}

void loop() {
  Wire.begin();
  Serial.begin(9600);
  
  pinMode(fanPin, OUTPUT);
  pinMode(burnerPin, OUTPUT);
  pinMode(thermostatPin, INPUT_PULLUP);
  digitalWrite(fanPin, HIGH);
  digitalWrite(burnerPin, HIGH);

  Heater heater;
  heater.fan = 0;
  heater.burner = 0;
  heater.fanDelay = 3000;
  heater.burnerLock = 5000;
  heater.heaterTimer = 0;
  
  while(1) {

  Serial.print("PIPAPO: ");
  Serial.println(digitalRead(fanPin));
    
  thermostat(&heater);
  fan(&heater);
  burner(&heater);
  Serial.print("PIPAPO2: ");
  Serial.println(heater.fan);
    
  }  
}


void thermostat (struct _Heater *heater) {
  if ( !digitalRead(thermostatPin) && heater->heaterTimer==0 ) { //if thermostat connected
    heater->fan = HIGH;
    heater->burner = HIGH;
    heater->heaterTimer = millis();
  }
  if ( digitalRead(thermostatPin) && millis() - heater->heaterTimer >= heater->burnerLock) { //burner shutdown
    heater->burner = LOW;   
  }
  if ( digitalRead(thermostatPin) && millis() - heater->heaterTimer >= heater->fanDelay + heater->burnerLock) { //fan shutdown
    heater->fan = LOW;
    heater->heaterTimer = 0;
  }
}

void fan(struct _Heater *heater) {
  
  if (heater->fan && digitalRead(fanPin))
    digitalWrite(fanPin,LOW);
    
  if (!heater->fan && !digitalRead(fanPin))
    digitalWrite(fanPin,HIGH);
}
void burner(struct _Heater *heater) {
  
  if (heater->burner && digitalRead(burnerPin))
    digitalWrite(burnerPin,LOW);  
    
  if (!heater->burner && !digitalRead(burnerPin))
    digitalWrite(burnerPin,HIGH);
}
void fanDelay(struct _Heater *heater) {
  //Read potentiometer and adjust cooling
}


