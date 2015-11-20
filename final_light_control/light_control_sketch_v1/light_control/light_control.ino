#include <VarSpeedServo.h>

int PsrPin = 7; //set HC-sr501 sensor input to 3 pin
int PsrPin1 = 8; // set HC-sr501_1 in bath room 
int BSled = 3; // blue led is connect to 13, controled by sr501 high
int LightPin = A0; //ser light sensor input to 8 pin
int Bath_light = 5; //relay switch connect to light in bath room
int MainLight = 9 ; //main lights pin
int val_light; //light sensor 
int val_psr;  // pir sensor
int incoming[2]; //web input parameter arrary
int state ; // web input parameter value
int light_state; // each condition light state
int lightstate; // web light function return value
int envirlightstate; // envirment function retun value
int RLled = 4;
int GSled = 11;
int val_l;
VarSpeedServo rotPsr; //object servo to rot psr
void setup() {
  // put your setup code here, to run once:
 Serial.begin(9600);
 pinMode(BSled, OUTPUT); //
 pinMode(PsrPin, INPUT);
 pinMode(LightPin, INPUT);
 pinMode(PsrPin1, INPUT);
 pinMode(Bath_light, OUTPUT);
 pinMode(MainLight, OUTPUT);
 pinMode(RLled, OUTPUT);
 pinMode(GSled, OUTPUT);
 rotPsr.attach(12); 
 
}

void loop() {
  val_light = analogRead(LightPin);
  val_psr = digitalRead(PsrPin);
  Serial.println(analogRead(val_light));
  if (val_light >= 200){
    val_l = HIGH;
    }else{
      val_l = LOW;
      }
  digitalWrite(Bath_light, digitalRead(PsrPin1));
  digitalWrite(GSled, digitalRead(PsrPin1));
  digitalWrite(BSled, digitalRead(PsrPin));
  digitalWrite(RLled, val_l);
  
  // put your main code here, to run repeatedly:
 
// rotPsr.write(160, 100, true);
  while(val_psr == 1){
    digitalWrite(Bath_light, digitalRead(PsrPin1));
    digitalWrite(GSled, digitalRead(PsrPin1));
    envirlightstate = envir_Light_state();
    digitalWrite(MainLight, envirlightstate);
    rot_psr();
    }
  lightstate = web_Light_state();
  digitalWrite(MainLight, lightstate);
//  Serial.println(digitalRead(LightPin));
 
}

int rot_psr(){
 rotPsr.write(120, 50, true);
 delay(5000);
 rotPsr.write(20, 50, true);
 delay(5000);
 rotPsr.write(120, 50, true);
// delay(5000);
// rotPsr.write(10, 50, true);
// delay(5000);
  }
int envir_Light_state(){
    val_light = analogRead(LightPin);
    val_psr = digitalRead(PsrPin);
    if((val_psr == HIGH) && (val_light >= 200)){
    light_state = 1;
    }else{
      light_state = 0 ;
      }; 
    return light_state;
  }

int web_Light_state(){
  
   if(Serial.available()>=2){
    //fill array by python
    for( int i = 0; i<2; i++){
      incoming[i] = Serial.read();
      };
    state = incoming[0];
//    Serial.println(state);
    if(state == 1){
      light_state = 1;
//       return light_state;
      }
    else if (state == 2){
     light_state = 0;
//       return light_state;
    }
  }else{
    light_state = digitalRead(MainLight);
    }
  return light_state;
  }
