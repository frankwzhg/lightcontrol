#include <nRF24L01.h>
#include <Mirf.h>
#include <MirfHardwareSpiDriver.h>
#include <MirfSpiDriver.h>
int incoming[2];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
 

  Mirf.cePin = 9;
  Mirf.csnPin = 10;
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();

  Mirf.setTADDR((byte *)"Send01");
  Mirf.payload = sizeof(int);

  Mirf.channel = 3;
  Mirf.config();

  Serial.println("I'm Sender ...");

}
unsigned int adata = 0;
void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()>=2){
    //fill array by python
        for( int i = 0; i<2; i++){
          incoming[i] = Serial.read();
           };
  }
  int adata = 1;
  Serial.println(adata);
//  byte data[Mirf.payload];
//  if(!Mirf.isSending() && Mirf.dataReady()){
//    Mirf.setTADDR((byte *)"Rec01");
    Mirf.send((byte *)&adata);
//    delay(1000);
//    }
//  data[0] = adata & 0xFF;
//  data[1] = adata >> 8;

//  Mirf.setTADDR((byte *)"Rec01");
//  Mirf.send(data);
//  while(Mirf.isSending()){}
//  delay(2000);

}
