#include <SPI.h>

#include <Mirf.h>
#include <MirfHardwareSpiDriver.h>
#include <nRF24L01.h>


int Main_PsrPin = 9; //set main room PIR to pin 3
int Main_LightPin = A0; // set main room light sensor to pin A0 
int Main_LSwitchPin = 4; // main room light realy switch to pin 4
int Bath_PsrPin = 6; //relay switch connect to light in bath room
int Bath_LSwitchPin = 5 ; //bath room lights pin
int Main_LightSensor; //variable for main room light sensor value 
int Main_Psr;  // variable for main room PIR value
int Bath_Psr; // variable for bath room PIR value
int Main_LSwitch; // relay switch for Main room
int Bath_LSwitch; // ralay switch for Bath room
int Main_LightState; //main room light state
int incoming[2]; // python import parameter
int Main_PYlightState; // Python control light
int Main_WeblightState; // web control light 

void setup() {
  // put your setup code here, to run once:
 Serial.begin(9600);
 Mirf.spi = &MirfHardwareSpi;
 Mirf.init();
 
 Mirf.setRADDR((byte *)"clie1");
 Mirf.payload = sizeof(short int);
 Mirf.channel = 20;
 Mirf.config();
 
 Serial.println("I'm Receiver ...");
 
 pinMode(Main_PsrPin, INPUT); //main room PIR input
 pinMode(Main_LSwitchPin, OUTPUT); //main room light relay switch
 pinMode(Main_LightPin, INPUT); // main room light sensor input
 pinMode(Bath_PsrPin, INPUT); // bath room PIR input
 pinMode(Bath_LSwitchPin, OUTPUT); // bath room light relay switch
 
}

void loop() {
 Bath_Psr = digitalRead(Bath_PsrPin); //variable value for Bath pir
 digitalWrite(Bath_LSwitchPin, Bath_Psr); // Bath pir control bath light
 Main_Psr = digitalRead(Main_PsrPin); // varialbe value for Main PIR
// Main_LightState = Main_PYState(); // get web pass light state to variable
 Main_LightSensor = analogRead(Main_LightPin); // variable for main light sensor
 Main_LSwitch = digitalRead(Main_LSwitchPin);

const unsigned long fiveMinutes = 0.2 * 60 * 1000UL;
static unsigned long lastSampleTime = 0 - fiveMinutes;
unsigned long now = millis();

if(now - lastSampleTime >= fiveMinutes){
  lastSampleTime += fiveMinutes;
  Mirf.setTADDR((byte *)"serv1");
  Mirf.send((byte *)&Main_LSwitch);
  while(Mirf.isSending()){};
  Serial.println("data sent");
  Mirf.setTADDR((byte *)"clie9");
//  delay(1000);
  }

 byte message[Mirf.payload];
  if(Mirf.dataReady()&& !Mirf.isSending()){
    Mirf.getData(message);
//    delay(3000);
//    Serial.print("message[0]: ");
//    Serial.println(message[0]);
    if(Main_LightSensor >= 150){
      if(Main_Psr == HIGH){
         digitalWrite(Main_LSwitchPin, HIGH);
//         Serial.println("test");
        }
        if(Main_Psr == LOW){
//          Serial.println(Main_Psr);
          if(message[0] == 1){
            digitalWrite(Main_LSwitchPin, HIGH);
           };
          if(message[0] == 2){
            digitalWrite(Main_LSwitchPin, LOW);
           };
            };      
          }
      if(message[0] == 3){
        digitalWrite(Main_LSwitchPin, HIGH);
        };
      if(message[0] == 4){
//        Serial.println("pass");
        digitalWrite(Main_LSwitchPin, LOW);
        } ;
    }
//    else{
//      if(Main_LightSensor >= 150){
//        digitalWrite(Main_LSwitchPin, Main_Psr);
//        }
//      };
// d`elay(3000);

}
 
// Serial.print("main_psr: ");
// Serial.println(Main_Psr);
// if((message[0] == 1)||message[0] == 2)){
//   if(Main_LightSensor >= 150){
//  if((Main_LightState == HIGH) || (Main_LightState == LOW)){
//    digitalWrite(Main_LSwitchPin, Main_LightState);
//      if(Main_Psr == HIGH){
//        digitalWrite(Main_LSwitchPin, Main_Psr);
//        }
//  }
//  }
//  }
//  if((message[0] == 3)||(message[0] == 4)) {
//    if(Main_LightState == 3 ){
//    digitalWrite(Main_LSwitchPin, HIGH);
//    } 
//   if(Main_LightState == 4){
//    digitalWrite(Main_LSwitchPin, LOW);
//    }
//    } 
// }
// delay(2000);
//  } 

int Main_PYState(){
  byte message[Mirf.payload];
  if(Mirf.dataReady()&& !Mirf.isSending()){
   
    Mirf.getData(message);
    Serial.print("message: ");
    Serial.println(message[0]);
    if(message[0] == 1){
       Main_PYlightState = HIGH;
       Serial.println("test1");
      };
      if(message[0] == 2){
        Main_PYlightState = LOW;
        Serial.println("test2");
        };
      }
      
//      else{
//            Main_WeblightState = digitalRead(Main_LSwitchPin);
//         }
      return Main_PYlightState;   
  }

  

