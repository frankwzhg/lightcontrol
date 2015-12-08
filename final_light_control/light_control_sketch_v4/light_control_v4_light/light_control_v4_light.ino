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

// include library
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
int incoming[2]; // python import parameter
int clie1_Main_LightSensor; //clie1 light sensor 
int clie1_Main_LSwitch; // clie1 light status in main room
int clie1_Bath_LSwitch; // clie1 light status in bath room
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
  digitalWrite(Main_LSwitchPin, Main_Psr);
 Main_LightSensor = analogRead(Main_LightPin); // variable for main light sensor
 Main_LSwitch = digitalRead(Main_LSwitchPin);  // variable for Main room ligth status

const unsigned long PerMinutes = 0.2 * 60 * 1000UL;  // define a period time to do somethin 0.2 minutes
static unsigned long lastSampleTime = 0 - PerMinutes; // define time stamp
unsigned long now = millis(); // program run time from starting

if(now - lastSampleTime >= PerMinutes){ // do somethin every period time
  lastSampleTime += PerMinutes;
  clie1_Main_LightSensor = 10000 + Main_LightSensor;  //10000 means analog input of main room light sensor
  clie1_Main_LSwitch = 110 + Main_LSwitch; // 110 means digital status of main room light 
  clie1_Bath_LSwitch = 120 + Bath_LSwitch; // 120 means digital status of bath romm light
  int clie1_data[] = {clie1_Main_LightSensor, clie1_Main_LSwitch, clie1_Bath_LSwitch}; // define send data array
  Mirf.setTADDR((byte *)"serv1");
  send_data(clie1_data[0]);
  send_data(clie1_data[1]);
  send_data(clie1_data[2]);
//  for(i=0; i<3; i=i+1){
//    Mirf.send((byte *)&send_data); // send data by byte
//    }
////  Mirf.send((byte *)&send[1]);
//  while(Mirf.isSending()){};
//  Serial.print("data sent");
//  Serial.println(send_data[0]);  // send first data in data_list
//  Serial.println(send_data[1]);
  Mirf.setTADDR((byte *)"clie9"); // setup send address to non 
//  delay(1000);
  }

 byte message[Mirf.payload];
  if(Mirf.dataReady()&& !Mirf.isSending()){ // waiting for get data
    Mirf.getData(message); // get data
//    delay(3000);
//    Serial.print("message[0]: ");
//    Serial.println(message[0]);
    if(Main_LightSensor >= 150){  // when light sensor more than 150, light will open
      // if light sensor more 150 and psr is high, main light will be controled
      // by PSR
      if(Main_Psr == HIGH){ 
         digitalWrite(Main_LSwitchPin, HIGH);
//         Serial.println("test");
        }
        // if light sensor more than 150 and psr is low, main light will be
        // control message from control pc
        if(Main_Psr == LOW){ 
//          Serial.println(Main_Psr);
          // if your mobile in internet, message = 1 will be send, means main
          // light will be open
          if(message[0] == 1){  
            digitalWrite(Main_LSwitchPin, HIGH);
           };
           // your mobile is not in internet, message = 1 will be send, means
           // light will be close
          if(message[0] == 2){
            digitalWrite(Main_LSwitchPin, LOW);
           };
            };      
          }
       // message = 3 or 4, means this message come from web, this message
       // will force to open or close light what ever status of light sensor
      if(message[0] == 3){ // 3 means to open light
        digitalWrite(Main_LSwitchPin, HIGH);
        };
      if(message[0] == 4){ // 4 means to close light
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

//int Main_PYState(){
//  byte message[Mirf.payload];
//  if(Mirf.dataReady()&& !Mirf.isSending()){
//   
//    Mirf.getData(message);
//    Serial.print("message: ");
//    Serial.println(message[0]);
//    if(message[0] == 1){
//       Main_PYlightState = HIGH;
//       Serial.println("test1");
//      };
//      if(message[0] == 2){
//        Main_PYlightState = LOW;
//        Serial.println("test2");
//        };
//      }
//      
////      else{
////            Main_WeblightState = digitalRead(Main_LSwitchPin);
////         }
//      return Main_PYlightState;   
//  }

  

