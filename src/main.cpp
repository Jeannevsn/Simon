#include <Arduino.h>
#include "rgb_lcd.h"
#include <ctime>

rgb_lcd lcd; // object creation

const int bt_yellow = 4, bt_green = 17, bt_black = 16, bt_blue = 13;      // buttons initiation
const int led_yellow = 19, led_green = 26, led_blue = 23, led_black = 25; // leds initation
int state_led_black, state_led_yellow, state_led_blue, state_led_green;   // state leds initation
unsigned long int timer[4] = {0};                                         // timer initiation

int led_table[4] = {led_yellow, led_green, led_blue, led_black};                               // led table
int state_led_table[4] = {state_led_black, state_led_yellow, state_led_blue, state_led_green}; // state led table
int random_led_table[5];                                                                       // random led table

void update_leds();                                                                                   // turn on led with buttons
void posting_lcd(int state_led_black, int state_led_yellow, int state_led_blue, int state_led_green); // led state on display
void lcd_position(String text, int column, int line, bool clear);                                     // led state position on display
void random_led();                                                                                    // random led function

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

  lcd.begin(16, 2); // lcd initiation

  // fill in the led table randomly
  for (int i = 0; i < 4; i++)
  {
    random_led_table[i] = random(0, 4);
  }
}

void loop()
{
  random_led();

  //  ON or OFF leds with buttons
  if (digitalRead(bt_black) == LOW)
    timer[0] = millis();
  if (millis() < timer[0] + 500)
    state_led_black = HIGH;
  else
    state_led_black = LOW;
  if (digitalRead(bt_yellow) == LOW)
    timer[1] = millis();
  if (millis() < timer[1] + 500)
    state_led_yellow = HIGH;
  else
    state_led_yellow = LOW;
  if (digitalRead(bt_green) == LOW)
    timer[2] = millis();
  if (millis() < timer[2] + 500)
    state_led_green = HIGH;
  else
    state_led_green = LOW;
  if (digitalRead(bt_blue) == LOW)
    timer[3] = millis();
  if (millis() < timer[3] + 500)
    state_led_blue = HIGH;
  else
    state_led_blue = LOW;

  // display log
  update_leds();
  posting_lcd(state_led_black, state_led_yellow, state_led_blue, state_led_green);
  lcd.setCursor(5, 1);
  lcd.printf("T %d,%d,%d,%d,%d", random_led_table[0], random_led_table[1], random_led_table[2], random_led_table[3], random_led_table[4]);
  // led 0 : black, led 1 : yellow, led 2 :blue, led 3 : green
  delay(150);
}

void update_leds()
{
  digitalWrite(led_black, state_led_black);
  digitalWrite(led_yellow, state_led_yellow);
  digitalWrite(led_green, state_led_green);
  digitalWrite(led_blue, state_led_blue);
}

void lcd_position(String text, int column, int line, bool clear)
{
  if (clear)
    lcd.clear();
  lcd.setCursor(column, line);
  lcd.print(text);
}

void posting_lcd(int state_led_black, int state_led_yellow, int state_led_blue, int state_led_green)
{
  lcd_position("BK:" + String(state_led_black), 0, 0, true);
  lcd_position("YE:" + String(state_led_yellow), 6, 0, false);
  lcd_position("BL:" + String(state_led_blue), 12, 0, false);
  lcd_position("GR:" + String(state_led_green), 0, 1, false);
}

void random_led()
{
  if (millis() < timer[random_led_table[random(0, 4)]] + 500)
    state_led_table[random_led_table[random(0, 4)]] = HIGH;
  else
  {
    state_led_table[random_led_table[random(0, 4)]] = LOW;
    timer[random_led_table[random(0, 4)]] = millis();
  }
}