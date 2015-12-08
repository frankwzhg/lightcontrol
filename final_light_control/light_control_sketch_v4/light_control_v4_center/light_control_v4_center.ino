/*
 * this programe is main control progame, that control all sub boad 
 * attached to this boar. this programe recive all other client boad
 * information and centor pc command, and forward this commande to 
 * other client.
 * 
 * variable policy:(this policy only used by transfer information to 
 * centrol control pc, not be used send command to client)
 * all digital variable and single byte variable will add 100-900 as 
 * lable. 100-200 will be used by centrol controler. 110-119 used by 
 * light, for example 110 used by main light, 111 used by bath  light 
 * 
 * all analog varible will add 10000-20000 as lable. 10000 used by 
 * main room, 11000 used by bath room
 * 引脚接法：
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
#include <nRF24L01.h>
#include <Mirf.h>
#include <MirfHardwareSpiDriver.h>

// define variable incoming[2] for pc transfer to arduino data package
int incoming[2];
// variable message is arudino center transfer command to client
short int message;

// function for send int data, split 16 byte int into abc array
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
  Mirf.init();  //initial nrf2401
  // set size of send data, for a int is 2byte
  Mirf.payload = sizeof(int);
  Mirf.channel = 20; // send channel
  Mirf.setRADDR((byte *)"serv1"); //set local board receiver lable
  Mirf.config();
  
//  Serial.println("Listening..");
  
  
  message = 0;
//  Serial.println("I'm Sender ...");

}
void loop() {
  // define a byte array, to store data will be send
  byte data[Mirf.payload]; 
  if(Mirf.dataReady()&&!Mirf.isSending()){ //waiting for receive ready
      Mirf.getData((byte *)&data); //get data array
     // data[1] will be move to left 8 byte, and rebiuld data arry
     unsigned int adata = ((data[1]<<8) | data[0]);
      Serial.println(adata);
      }
  if(Serial.available()>=2){ // adjust if pc send data to arduino
    //fill array by python
        for( int i = 0; i<2; i++){  
          incoming[i] = Serial.read();
           };
// adjust what information send by pc to arduino
  if(incoming[0] == 1){  // 1 is open main light command from python
   message = 1;
    };
  if(incoming[0] == 2){  // 2 is close main light command from python
    message = 2;
    };
  if(incoming[0] == 3){ // 3 is open main light command form web
    message = 3;
    };
  if(incoming[0] == 4){  // 4 is close main light command from web
    message = 4;
    }
  };
  int data2 = 7;
//  Serial.print("message");
//  Serial.println(message);
  int message_list[]={message, data2}; // data array will send
  Mirf.setTADDR((byte *)"clie1"); // set send address
  send_data(message_list[0]);  // send first data of data array to clie1
//  Serial.print("message[0]");
//  Serial.println(message_list[0]);
  delay(10);
  // under single channe to send data, in order to void data cross forware
  // set send address to non client address
  Mirf.setTADDR((byte *)"clie9"); 
  delay(10);
  // send second message data to clie2
  Mirf.setTADDR((byte *)"clie2");
  send_data(message_list[1]);
  delay(10);
  Mirf.setTADDR((byte *)"clie9"); 
  delay(3000);  // avoid to interrupt each other 
}


