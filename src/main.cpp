#include <Arduino.h>
int x;
int y;
int z;
# define relay_pin 23
# define iR1_pin 2
# define iR2_pin 3
# define Msensor_pin 5  // NPN mode 
int count=0;
void on_relay(){digitalWrite(relay_pin,HIGH);}
void off_relay(){digitalWrite(relay_pin,LOW);}

bool sort_1(){
  int sort= digitalRead(iR2_pin);
  return (sort== HIGH);
}
bool sort_2(){
  int sort= digitalRead(Msensor_pin);
  return (sort== HIGH);
}

void feeder(){
  int part_Detected = digitalRead(iR1_pin);
  if(part_Detected==LOW){       //  parts infront of the sensor 
      on_relay();                           // on relay     
      Serial.println("relay is on");         
      count++;                               // count 1
  }
  else if(part_Detected==HIGH){              // off relay  
      off_relay();
      count++;
  }

}

void setup() {
  Serial.begin(115200);
  pinMode(relay_pin,OUTPUT);
  pinMode(iR1_pin,INPUT);
  pinMode(iR2_pin,INPUT);
  pinMode(Msensor_pin,INPUT);
}

void loop() {
  on_relay();    // as defult on 
  feeder();  
  if(count==4){
    off_relay();  // relay back to his own place
  }                      

  Serial.println("relay is off");
  // sorting
  if (sort_1()){Serial.println("White");}   
  else {Serial.println("Black");}
  if (sort_2()){Serial.println("Metal");}
  else{Serial.println("not metal");}
}

