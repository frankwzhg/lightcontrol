#include <SPI.h>

#include <nRF24L01.h>
#include <Mirf.h>
#include <MirfHardwareSpiDriver.h>

int incoming[2];
short int message;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
 
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();

  Mirf.payload = sizeof(int);
  Mirf.channel = 20;
  Mirf.setRADDR((byte *)"serv1");
  Mirf.config();
  
//  Serial.println("Listening..");
  
  
  message = 0;
//  Serial.println("I'm Sender ...");

}
void loop() {
  // put your main code here, to run repeatedly:
  byte data[Mirf.payload];
  if(Mirf.dataReady()&&!Mirf.isSending()){
      Mirf.getData((byte *)&data);
     unsigned int adata = ((data[1]<<8) | data[0]);
      Serial.println(adata);
//    Serial.print("receive data:");
//      Serial.println(data[0]);
//      Serial.println(data[1]);
    
//    delay(1000);
    }
//    delay(2000);
  if(Serial.available()>=2){
    //fill array by python
        for( int i = 0; i<2; i++){
          incoming[i] = Serial.read();
           };

  if(incoming[0] == 1){
   message = 1;
    };
  if(incoming[0] == 2){
    message = 2;
    };
  if(incoming[0] == 3){
    message = 3;
    };
  if(incoming[0] == 4){
    message = 4;
    }
  };
  int data2 = 7;
  int data3 = 99;
  int message_list[]={message, data2};
 Mirf.setTADDR((byte *)"clie1");
  send_data(message_list[0]);
//  Mirf.send((byte *)&message);
//  while(Mirf.isSending()){};
  delay(10);
  Mirf.setTADDR((byte *)"clie9");
  delay(10);
  Mirf.setTADDR((byte *)"clie2");
  send_data(message_list[1]);
//  Mirf.send((byte *)&data2);
//  while(Mirf.isSending()){};
  delay(10);
  Mirf.setTADDR((byte *)"clie9"); 
}

int send_data(int data){
  byte abc[Mirf.payload];
  abc[0] = data & 0xFF;
  abc[1] = data >> 8 ;
  Mirf.send((byte *)&abc);
  while (Mirf.isSending()){};
  }
