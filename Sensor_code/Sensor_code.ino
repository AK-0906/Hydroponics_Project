#define BLYNK_TEMPLATE_ID "TMPLLKKDe96S"
#define BLYNK_TEMPLATE_NAME "Intelligent Hydroponics"
#define BLYNK_AUTH_TOKEN "LsnVHD63CAEbiD_e1wL6zd0xezPbJrBR"
#define BLYNK_PRINT Serial
#define ESP8266_BAUD 115200

#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <DHT.h>
#include <SoftwareSerial.h>
SoftwareSerial EspSerial(20,21);


char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "4GWIFI_69976";
char pass[] = "12345678";

ESP8266 wifi(&EspSerial);
BlynkTimer timer;

#define DHTPIN 23          
#define DHTTYPE DHT11    
DHT dht(DHTPIN, DHTTYPE);

#define Liquid_Detection_Pin 27
int Liquid_detection_val;


#define TDS_pin A2
int TDS_read;
double TDS_DUMB;
double TDS_Value;

#define PH_pin A3
int PH_read;
double PH_Value;



#define flowPin 25  
double flowRate;  
volatile int count; 



void sendSensor(){
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (digitalRead(Liquid_Detection_Pin)) {
    Liquid_detection_val = 0;
  }
  else {
    Liquid_detection_val = 1;
  }


  TDS_read = analogRead(TDS_pin);
  TDS_DUMB = 0.00147928*(TDS_read)*(TDS_read)-0.462*(TDS_read)+244.854;
  if(TDS_DUMB>100){
TDS_Value = TDS_DUMB;
  }

  PH_read = analogRead(PH_pin);
  PH_Value = 0.001340381*(PH_read) + 1.44001;



  count = 0;      
  interrupts();   
  delay (1000);   
  noInterrupts(); 
  flowRate = (count * 2.25);        
  flowRate = flowRate * 60;         
  flowRate = flowRate / 1000;
  flowRate = 445.182*flowRate + 1.888712;
 
  Blynk.virtualWrite(V1, h);
  Blynk.virtualWrite(V2, t);
  Blynk.virtualWrite(V3, Liquid_detection_val); 
  Blynk.virtualWrite(V4, TDS_Value);
  Blynk.virtualWrite(V5, PH_Value);
  Blynk.virtualWrite(V6, flowRate);


}

void setup(){
  Serial.begin(9600);
  EspSerial.begin(ESP8266_BAUD);
  delay(10);
  dht.begin();
  pinMode(Liquid_Detection_Pin, INPUT);
  pinMode(TDS_pin, INPUT);
  pinMode(PH_pin, INPUT);
  pinMode(flowPin, INPUT);           
  attachInterrupt(0, Flow, RISING);
  Blynk.begin(auth, wifi, ssid, pass);
  timer.setInterval(1000, sendSensor);
}



void loop()
{
  Blynk.run();
  timer.run();
  
}

void Flow(){
   count++;
}  

