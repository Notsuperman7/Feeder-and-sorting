#include <Arduino.h>
#include "Sender.h"
#include "Reciver.h"
#define relay_pin 23
#define iR1_pin 2
#define iR2_pin 3
#define Msensor_pin 5 // NPN mode
#define Switch_pin 6

#define forwardTime 1000
#define BackwardTime 1000

class part
{
public:
  bool isWhite;
  bool isBase;
};

part currentPart;

void sort(void *parameter)
{
  while (1)
  {
    if (digitalRead(Switch_pin) == LOW)
    {
      currentPart.isBase = (digitalRead(Msensor_pin) == HIGH);
      currentPart.isWhite = (digitalRead(iR2_pin) == HIGH);
    }
    // sorting
    if (currentPart.isWhite)
    {
      Serial.println("White");
      Send("White");
    }
    else
    {
      Serial.println("Black");
      Send("Black");
    }
    if (currentPart.isBase)
    {
      Serial.println("Base");
      Send("Base");
    }
    else
    {
      Serial.println("Lid");
      Send("Lid");
    }
    delay(100);
  }
}

void feed(void *parameter)
{
  while (1)
  {
    String got = Receive();
    if(got =="F"|| got =="S"){            // F as finished postining, S as start in the beging to feed
      bool part_Detected = (digitalRead(iR1_pin) == LOW);
      bool no_part_Sorting = (digitalRead(Switch_pin) == HIGH);
      if (no_part_Sorting && part_Detected)
      {
        digitalWrite(relay_pin, LOW);
        delay(BackwardTime);
        digitalWrite(relay_pin, HIGH);
        delay(forwardTime);
      }
      if(!part_Detected&& !no_part_Sorting)    // if no part & no soerting means no parts
      {
        Serial.println("no parts");
        Send("no parts");
      }

    }
    
  }
}

void setup()
{
  Serial.begin(115200);
  pinMode(relay_pin, OUTPUT);
  digitalWrite(relay_pin, HIGH);

  pinMode(iR1_pin, INPUT);
  pinMode(iR2_pin, INPUT);
  pinMode(Msensor_pin, INPUT);
  delay(forwardTime);
  Serial.println("ready");
  Sender_Init();
  Receiver_Init(); 

  xTaskCreate(
      feed,   // Function name of the task
      "feed", // Name of the task (e.g. for debugging)
      2048,   // Stack size (bytes)
      NULL,   // Parameter to pass
      1,      // Task priority
      NULL    // Task handle
  );
  xTaskCreate(
      sort,   // Function name of the task
      "sort", // Name of the task (e.g. for debugging)
      2048,   // Stack size (bytes)
      NULL,   // Parameter to pass
      1,      // Task priority
      NULL    // Task handle
  );
}

void loop()
{
}
