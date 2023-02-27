#include <Arduino.h>
#include "rgb_lcd.h"
#include <ctime>

rgb_lcd lcd; // object creation

const int bt_yellow = 4, bt_green = 17, bt_black = 16, bt_blue = 13;      // buttons initiation
const int led_yellow = 19, led_green = 26, led_blue = 23, led_black = 25; // leds initation
unsigned long int timer[4] = {0};                                         // timer initiation
int counter = 0;                                                          // counter of the player's button presses
int timer_random_led;                                                     // timer to turn random leds on or off
int i;                                                                    // i initiation for loop

int led_table[4] = {led_yellow, led_green, led_blue, led_black}; // set led table
int state_led_table[4];                                          // {state_led_black, state_led_yellow, state_led_blue, state_led_green}
int random_led_table[5];                                         // the leds are drawn at random and stored in this table
int answer_player_table[5];                                      // the leds lit by the player are stored in this table

void update_leds();                                                                                   // turn on led with buttons function
void posting_lcd(int state_led_black, int state_led_yellow, int state_led_blue, int state_led_green); // led state on display function
void lcd_position(String text, int column, int line, bool clear);                                     // led state position on display function
void random_led();                                                                                    // random led function
int buttons_counter(int count);                                                                       // buttons counter function

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

  // fill in the leds table randomly
  for (i = 0; i < 5; i++)
  {
    random_led_table[i] = random(0, 4);
  }
  timer_random_led = millis();
  i = 0;
}

void loop()
{
  // turn on or off the leds of the random array
  while (1)
  {
    state_led_table[random_led_table[i]] = HIGH;
    update_leds();

    if (millis() > timer_random_led + 500)
    {
      lcd_position("i:" + String(i), 13, 0, true);
      i += 1;
      for (int j = 0; j < 4; j++)
      {
        state_led_table[j] = LOW;
      }

      if (i >= 5)
        i = 0;

      timer_random_led = millis();
    }

    lcd.setCursor(0, 0);
    lcd.printf("Tr %d,%d,%d,%d,%d", random_led_table[0], random_led_table[1], random_led_table[2], random_led_table[3], random_led_table[4]);
    lcd.setCursor(0, 1);
    lcd.printf("T %d,%d,%d,%d", state_led_table[0], state_led_table[1], state_led_table[2], state_led_table[3]);
    delay(150);
  }

  /*// random_led();
  counter = buttons_counter(counter);
  // ON or OFF leds with buttons
  if (digitalRead(bt_black) == LOW)
  {
    timer[0] = millis();
    answer_player_table[counter] = 0;
  }
  if (millis() < timer[0] + 500)
  {
    state_led_black = HIGH;
  }
  else
    state_led_black = LOW;
  if (digitalRead(bt_yellow) == LOW)
  {
    timer[1] = millis();
    answer_player_table[counter] = 1;
  }
  if (millis() < timer[1] + 500)
  {
    state_led_yellow = HIGH;
  }
  else
    state_led_yellow = LOW;
  if (digitalRead(bt_blue) == LOW)
  {
    timer[3] = millis();
    answer_player_table[counter] = 2;
  }
  if (millis() < timer[3] + 500)
  {
    state_led_blue = HIGH;
  }
  else
    state_led_blue = LOW;

  if (digitalRead(bt_green) == LOW)
  {
    timer[2] = millis();
    answer_player_table[counter] = 3;
  }
  if (millis() < timer[2] + 500)
  {
    state_led_green = HIGH;
  }
  else
    state_led_green = LOW;
*/
  // display log
  update_leds();
  // posting_lcd(state_led_black, state_led_yellow, state_led_blue, state_led_green);
}

void update_leds()
{
  digitalWrite(led_black, state_led_table[2]);
  digitalWrite(led_yellow, state_led_table[0]);
  digitalWrite(led_green, state_led_table[3]);
  digitalWrite(led_blue, state_led_table[1]);
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

/*int buttons_counter(int count)
{
  if (digitalRead(bt_black) == HIGH || digitalRead(bt_blue) == HIGH || digitalRead(bt_green) == HIGH || digitalRead(bt_yellow) == HIGH)
  {
    count += 1;
    if (count >= 5)
      count = 0;
    delay(200);
  }
  return count;
}*/