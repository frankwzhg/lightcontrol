#include <nRF24L01.h>
#include <Mirf.h>
#include <MirfHardwareSpiDriver.h>
#include <MirfSpiDriver.h>


int Main_PsrPin = 8; //set main room PIR to pin 3
int Main_LightPin = A0; // set main room light sensor to pin A0 
int Main_LSwitchPin = 4; // main room light realy switch to pin 4
int Bath_PsrPin = 6; //relay switch connect to light in bath room
int Bath_LSwitchPin = 7 ; //bath room lights pin
int Main_LightSensor; //variable for main room light sensor value 
int Main_Psr;  // variable for main room PIR value
int Bath_Psr; // variable for bath room PIR value
int Main_LSwitch; // relay switch for Main room
int Bath_LSwitch; // ralay switch for Bath room
int Main_LightState; //main room light state
int incoming[2]; // python import parameter
int Main_WeblightState; // web control light 

unsigned int adata = 0, oldadata = 0;


void setup() {
  // put your setup code here, to run once:
 Serial.begin(9600);
 pinMode(Main_PsrPin, INPUT); //main room PIR input
 pinMode(Main_LSwitchPin, OUTPUT); //main room light relay switch
 pinMode(Main_LightPin, INPUT); // main room light sensor input
 pinMode(Bath_PsrPin, INPUT); // bath room PIR input
 pinMode(Bath_LSwitchPin, OUTPUT); // bath room light relay switch


 Mirf.cePin = 9;
 Mirf.csnPin = 10;
 Mirf.spi = &MirfHardwareSpi;
 Mirf.init();

 Mirf.setRADDR((byte *)"Rec01");
 Mirf.payload = sizeof(unsigned int);
 Mirf.channel = 3;
 Mirf.config();

 Serial.println("I'm Receiver ...");
}

void loop() {
  byte data[Mirf.payload];
  if(Mirf.dataReady()){
    Mirf.getData(data);
    adata = (unsigned int)((data[1] << 8) | data[0]);
    }
   Serial.print("send data= ");
   Serial.println(adata);
  
 Bath_Psr = digitalRead(Bath_PsrPin); //variable value for Bath pir
 digitalWrite(Bath_LSwitchPin, Bath_Psr); // Bath pir control bath light
 Main_Psr = digitalRead(Main_PsrPin); // varialbe value for Main PIR
 Main_LightState = Main_WebState(); // get web pass light state to variable
 Main_LightSensor = analogRead(Main_LightPin); // variable for main light sensor
 if(Main_LightSensor >= 200){
  digitalWrite(Main_LSwitchPin, Main_LightState);
  if(Main_Psr == HIGH){
 digitalWrite(Main_LSwitchPin, Main_Psr);
 }
// delay(2000);
  } 
}

int Main_WebState(){
  if(Serial.available()>=2){
    //fill array by python
        for( int i = 0; i<2; i++){
          incoming[i] = Serial.read();
           }
           if(incoming[0] == 1){
              Main_WeblightState = HIGH;
               }
            else if(incoming[0] == 2){
                Main_WeblightState = LOW;
                }
         }
         else{
            Main_WeblightState = digitalRead(Main_LSwitchPin);
         }
      return Main_WeblightState;   
  }

