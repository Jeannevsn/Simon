#include <Arduino.h>

const int bt_yellow = 4, bt_green = 17, bt_black = 16, bt_blue = 13;      // buttons initiation
const int led_yellow = 19, led_green = 26, led_blue = 23, led_black = 25; // leds initation
int state_led_black, state_led_yellow, state_led_blue, state_led_green;   // state leds initation

void update_leds();

void setup()
{
  // pins initiation
  pinMode(bt_yellow, INPUT);
  pinMode(led_yellow, OUTPUT);
  pinMode(bt_green, INPUT);
  pinMode(led_green, OUTPUT);
  pinMode(bt_black, INPUT);
  pinMode(led_black, OUTPUT);
  pinMode(bt_blue, INPUT);
  pinMode(led_blue, OUTPUT);
  // turn off the devil led
  pinMode(5, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(14, OUTPUT);
}

void loop()
{
  if (digitalRead(bt_black) == LOW)
    state_led_black = HIGH;
  else
    state_led_black = LOW;
  if (digitalRead(bt_yellow) == LOW)
    state_led_yellow = HIGH;
  else
    state_led_yellow = LOW;
  if (digitalRead(bt_green) == LOW)
    state_led_green = HIGH;
  else
    state_led_green = LOW;
  if (digitalRead(bt_blue) == LOW)
    state_led_blue = HIGH;
  else
    state_led_blue = LOW;

  update_leds();
}

void update_leds()
{
  digitalWrite(led_black, state_led_black);
  digitalWrite(led_yellow, state_led_yellow);
  digitalWrite(led_green, state_led_green);
  digitalWrite(led_blue, state_led_blue);
}