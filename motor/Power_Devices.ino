#define BLYNK_TEMPLATE_ID "TMPLLKKDe96S"
#define BLYNK_TEMPLATE_NAME "Intelligent Hydroponics"
#define BLYNK_AUTH_TOKEN "LsnVHD63CAEbiD_e1wL6zd0xezPbJrBR"
#define BLYNK_PRINT Serial
#define ESP8266_BAUD 115200

#include <ESP8266_Lib.h>
#include <ezButton.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <SoftwareSerial.h>
SoftwareSerial EspSerial(12,13);

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "4GWIFI_69976";
char pass[] = "12345678";

ESP8266 wifi(&EspSerial);
BlynkTimer timer;

const int stepPin = 8;
const int dirPin = 9;
ezButton limitSwitch_up(11);  
ezButton limitSwitch_down(10);

int Light_pin = 3;
int Pump_pin = 4;
int Oxygen_Pump_pin = 5;



void setup() {
Serial.begin(9600);
EspSerial.begin(ESP8266_BAUD);
delay(10);
pinMode(stepPin,OUTPUT);
pinMode(dirPin,OUTPUT);
limitSwitch_up.setDebounceTime(50); 
limitSwitch_down.setDebounceTime(50);
pinMode(Light_pin, OUTPUT);
pinMode(Pump_pin,OUTPUT);
pinMode(Oxygen_Pump_pin,OUTPUT);
Blynk.begin(auth, wifi, ssid, pass);

}

BLYNK_WRITE(V0){
  limitSwitch_up.loop();
  limitSwitch_down.loop();
  int isData = 0;
  while (!isData){
    limitSwitch_up.loop();
    limitSwitch_down.loop();
    if (limitSwitch_up.getState() == LOW){
      digitalWrite(dirPin, HIGH);
      while(limitSwitch_down.getState() == HIGH){
        limitSwitch_up.loop();
        limitSwitch_down.loop();
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(1000);
        digitalWrite(stepPin, LOW);
        delayMicroseconds(2);
      }
    }
    else if (limitSwitch_down.getState() == LOW){
      delay(60000);
      isData = 1;
      digitalWrite(dirPin, LOW); 
      for (int x = 0; x < 2000; x++){ 
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(1000);
        digitalWrite(stepPin, LOW);
        delayMicroseconds(2);
      }
    }
 else{
      digitalWrite(dirPin, HIGH);
      while(limitSwitch_down.getState() == HIGH){
        limitSwitch_up.loop();
        limitSwitch_down.loop();
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(1000);
        digitalWrite(stepPin, LOW);
        delayMicroseconds(2);
      }
    }
    Blynk.virtualWrite(V0,0);
  }


}



BLYNK_WRITE(V8)
 {
 if(param.asInt()==1)
  {
     digitalWrite(Oxygen_Pump_pin, HIGH);
  }
  else
  {
     digitalWrite(Oxygen_Pump_pin, LOW);
  }
 }

BLYNK_WRITE(V7)
 {
 if(param.asInt()==1)
  {
     digitalWrite(Pump_pin, HIGH);
  }
  else
  {
     digitalWrite(Pump_pin, LOW);
  }
 }


BLYNK_WRITE(V9)
 {
 if(param.asInt()==1)
  {
     digitalWrite(Light_pin, HIGH);
  }
  else
  {
     digitalWrite(Light_pin, LOW);
  }
 }


void loop()
{
  Blynk.run();
  timer.run();
}
