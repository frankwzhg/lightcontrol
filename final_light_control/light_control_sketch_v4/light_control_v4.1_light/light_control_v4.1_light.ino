/*
 * this program is a client program,this program receive command 
 * from center control arduino
 *  variable policy:(this policy only used by transfer information to 
 * centrol control pc, not be used send command to client)
 * all digital variable and single byte variable will add 100-900 as 
 * lable. 100-200 will be used by centrol controler. 110-119 used by 
 * light, for example 110 used by main light, 111 used by bath  light 
 * 
 * all analog varible will add 10000-20000 as lable. 10000 used by 
 * main room, 11000 used by bath room
 * * 引脚接法：
 * nRF24L01   Arduino UNO  
 * VCC        <->        3.3V
 * GND        <->        GND
 * CE        <->        D8
 * CSN        <->        D7
 * MOSI<->        D11
 * MISO<->        D12
 * SCK        <->        D13
 * IRQ        <->        不接
 */

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
int i;

// send data function to send data
void send_data(int data){   
  byte abc[Mirf.payload];
  abc[0] = data & 0xFF;  //move high 8 byte of int to abc[0]
  abc[1] = data >> 8 ;   // move low 8 byte of in to abc[1],
  Mirf.send((byte *)&abc);  // send a 16 byte int data
  while (Mirf.isSending()){};
  }

void setup() {
  // put your setup code here, to run once:
 Serial.begin(9600);
 Mirf.spi = &MirfHardwareSpi;
 Mirf.init();
 
 Mirf.setRADDR((byte *)"clie1"); //setup receive address
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

const unsigned long PerMinutes = 0.2 * 60 * 1000UL; //define period time
static unsigned long lastSampleTime = 0 - PerMinutes; // define time stamp
unsigned long now = millis(); //this programe run time from starting

if(now - lastSampleTime >= PerMinutes){  // do something every period
  lastSampleTime += PerMinutes;
  Mirf.setTADDR((byte *)"serv1");  // setup send address
  int send_data[] = {Main_LightSensor, Main_LSwitch, Bath_LSwitch};
  for(i=0; i<sizeof(send_data); i=i+1){ //data will be sent one by one
    Mirf.send((byte *)&send_data[i]);
    }
  Mirf.setTADDR((byte *)"clie9");
  }

 byte message[Mirf.payload];
  if(Mirf.dataReady()&& !Mirf.isSending()){
    Mirf.getData(message);
   
  Serial.print("message[0]");
  Serial.println(message[0]);
//   delay(1000);
   if(message[0] == 1 || message[0] == 2){
   if(Main_LightSensor >= 150){
      if(Main_Psr == HIGH){
         digitalWrite(Main_LSwitchPin, HIGH);
         Serial.println("test");
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
   }
      if(message[0] == 3){
        digitalWrite(Main_LSwitchPin, HIGH);
        };
      if(message[0] == 4){
        Serial.println("pass");
        digitalWrite(Main_LSwitchPin, LOW);
        } ;
    }
}
 
