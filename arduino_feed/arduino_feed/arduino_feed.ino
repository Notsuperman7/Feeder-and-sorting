#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

// Set the LCD address (0x27 or 0x3F are common), number of columns and rows
LiquidCrystal_I2C lcd(0x27, 16, 2); // Adjust address and size if needed

#define relay_pin 19
#define iR1_pin 18
#define iR2_pin 5
#define Msensor_pin 17 // NPN mode
#define Switch_pin 16

#define forwardTime 1500
#define BackwardTime 500

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
    delay(200);
  }
}

void feed(void *parameter)
{
  while (1)
  {
    bool part_Detected = (digitalRead(iR1_pin) == HIGH);
    bool no_part_sorting =(digitalRead(Switch_pin)==HIGH);
    if (part_Detected && no_part_sorting)
    {
      digitalWrite(relay_pin, HIGH);
      delay(BackwardTime);
      digitalWrite(relay_pin, LOW);
      delay(forwardTime);
    }
    else
    {
      digitalWrite(relay_pin, LOW);
    }
  }
}

// void comms(void *parameter)
// {
// if (currentPart.isWhite)
//     {
//       lcd.clear();
//       lcd.setCursor(0, 0);
//       lcd.print("White");
//     }
//     else
//     {
//       lcd.clear();
//       lcd.setCursor(0, 0);
//       lcd.print("Black");
//     }
//     if (currentPart.isBase)
//     {
//       lcd.setCursor(0, 1);
//       lcd.print("Base");
//     }
//     else
//     {
//       lcd.setCursor(0, 1);
//       lcd.print("Base");
//     }
//     delay(200);
// }


void setup()
{

  pinMode(relay_pin, OUTPUT);

  pinMode(iR1_pin, INPUT);
  pinMode(iR2_pin, INPUT);
  pinMode(Msensor_pin, INPUT);
  delay(forwardTime);

  xTaskCreate(
      feed,   // Function name of the task
      "feed", // Name of the task (e.g. for debugging)
      4096,   // Stack size (bytes)
      NULL,   // Parameter to pass
      2,      // Task priority
      NULL    // Task handle
  );
  xTaskCreate(
      sort,   // Function name of the task
      "sort", // Name of the task (e.g. for debugging)
      4096,   // Stack size (bytes)
      NULL,   // Parameter to pass
      1,      // Task priority
      NULL    // Task handle
  );
    // xTaskCreate(
    //   comms,   // Function name of the task
    //   "comms", // Name of the task (e.g. for debugging)
    //   4096,   // Stack size (bytes)
    //   NULL,   // Parameter to pass
    //   1,      // Task priority
    //   NULL    // Task handle
  //);
}
// LOoOOOL
void loop()
{
}
