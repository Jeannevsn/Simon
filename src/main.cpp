#include <Arduino.h>

const int bt_jaune = 4, bt_vert = 17, bt_noir = 16;
const int led_jaune = 21, led_verte = 26;

void setup()
{
  pinMode(bt_jaune, INPUT);
  pinMode(led_jaune, OUTPUT);
  pinMode(bt_vert, INPUT);
  pinMode(led_verte, OUTPUT);
  pinMode(bt_noir, INPUT);
}

void loop()
{
  if (digitalRead(bt_noir) == LOW)
  {
    digitalWrite(led_jaune, LOW);
    digitalWrite(led_verte, LOW);
  }

  else if (digitalRead(bt_jaune) == LOW)
  {
    digitalWrite(led_jaune, HIGH);
    delay(300);
    digitalWrite(led_verte, HIGH);
    delay(500);
    digitalWrite(led_jaune, LOW);
    digitalWrite(led_verte, LOW);
  }
  
  else if (digitalRead(bt_vert) == LOW)
  {
    digitalWrite(led_jaune, HIGH);
    digitalWrite(led_verte, HIGH);
  }
}