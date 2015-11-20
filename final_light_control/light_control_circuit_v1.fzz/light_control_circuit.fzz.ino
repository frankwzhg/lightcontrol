int WaterPin = 3; //fill wate into pot when HIGH 
int PumpPin = 4; //pump start
int CoffPin = 5; // switch coffe 
int MilkPin = 6; // switch milk
int StartPin = 7; // start programe
int WaterTime;
int CoffeeTime;
int MilkTime;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(WaterPin, OUTPUT);
  pinMode(PumpPin, OUTPUT);
  pinMode(CoffPin, OUTPUT);
  pinMode(MilkPin, OUTPUT);
  pinMode(StartPin, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(digitalRead(StartPin));
  if(digitalRead(StartPin) == HIGH){
    Make_Coffee();
    }
  
}

void Make_Coffee(){
  digitalWrite(WaterPin, HIGH);
  delay(2000);
  digitalWrite(WaterPin, LOW);
  digitalWrite(PumpPin, HIGH);
  digitalWrite(CoffPin, HIGH);
  delay(5000);
  digitalWrite(CoffPin, LOW);
  digitalWrite(MilkPin, HIGH);
  delay(2000);
  digitalWrite(MilkPin, LOW);
  digitalWrite(PumpPin, LOW);
  
  }
