/*
Rules :
This mini-game is a module of the video game "Keep Talking and Nobody Explodes".
The goal of the game is to reproduce a sequence of leds by pressing the corresponding buttons in the defusing manual.
The game features 4 buttons and 4 leds.
The yellow button starts the game. Then the game generates a random sequence of leds. The player must reproduce the sequence by pressing the corresponding buttons.
If the player succeeds in the sequence, he/she moves to the next level. If the player fails, he/she must repeat the sequence.
The game has 5 levels of difficulty.
Level 1 is the easiest and level 5 is the most difficult.
The game is over when the player reaches level 5.
*/

// library
#include <Arduino.h>
#include "rgb_lcd.h"
#include <ctime>
#include "bouton.h"

// object creation
rgb_lcd lcd; 
Bouton bt[4];

void setup_bt(int nb_bt); // initialization of the buttons
void read_bt(int nb_bt); // reading of the buttons

const int bt_yellow = 4, bt_green = 17, bt_black = 16, bt_blue = 13;      // buttons initiation
const int led_yellow = 19, led_green = 26, led_blue = 23, led_black = 25; // leds initation
int timer_random_led = 0;                                                 // timer to turn random leds on or off
int leds_counter = 0;                                                     // initiation of the led counter to go to the next led in the array
int buttons_counter = 0;                                                  // initiation of the counter of the player's button presses
int buttons_error = 0;                                                    // initiation of the counter of the player's errors
int state = 4;                                                            // initiation of my state variable for my state machine
int level = 0, level_max = 5;                                             // initiation of the level counter and level max (5)

unsigned long int timer[4] = {0};
unsigned long int timer_lcd = millis();                               // timer initiation
int led_table[4] = {led_yellow, led_blue, led_black, led_green};      // set leds table
int button_table[4] = {bt_yellow, bt_blue, bt_black, bt_green};       // set buttons table
int state_led_table[4];                                               // {state_led_yellow, state_led_blue, state_led_black, state_led_green}
int random_led_table[5];                                              // the leds are drawn at random and stored in this table
int answer_player_table[5];                                           // the leds lit by the player are stored in this table
int error_table[4][3] = {{3, 1, 2}, {2, 0, 3}, {1, 3, 1}, {0, 2, 0}}; // {0 : bt_yellow, 1 : bt_blue, 2 : bt_black, 3 : bt_green}

void update_leds();                                                                                   // turn on led with buttons function
void posting_lcd(int state_led_black, int state_led_yellow, int state_led_blue, int state_led_green); // led state on display function
void lcd_position(String text, int column, int line, bool clear);                                     // led state position on display function
void random_led();                                                                                    // random led function

void setup()
{
  setup_bt(4);

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
  for (int m = 0; m < 5; m++)
  {
    random_led_table[m] = random(0, 4);
    answer_player_table[m] = error_table[random_led_table[m]][buttons_error];
  }
  timer_random_led = millis();
}

void loop()
{

  read_bt(4);
  // log information
  /*if (millis() > timer_lcd + 200)
  {
    lcd.setCursor(0, 0);
    lcd.printf("T%d,%d,%d,%d,%d", random_led_table[0], random_led_table[1], random_led_table[2], random_led_table[3], random_led_table[4]);
    lcd.setCursor(0, 1);
    lcd.printf("T%d,%d,%d,%d,%d", answer_player_table[0], answer_player_table[1], answer_player_table[2], answer_player_table[3], answer_player_table[4]);
    lcd_position("c:" + String(buttons_counter), 13, 0, false);
    lcd_position("e:" + String(buttons_error), 13, 1, false);
    lcd_position("s:" + String(state), 10, 1, false);
    timer_lcd = millis();
  }*/

  switch (state)
  {
  case 0: // turn on or off the leds of the random array
    state_led_table[random_led_table[leds_counter]] = HIGH;

    if (millis() > timer_random_led + 1500)
    {

      for (int j = 0; j < 4; j++)
      {
        state_led_table[j] = LOW;
      }

      if (millis() > timer_random_led + 2000)
      {
        timer_random_led = millis();
        leds_counter++;
        if (leds_counter > level)
        {
          leds_counter = 0;
          state = 1;
        }
      }
    }
    update_leds();
    break;

  case 1: // turn the leds on or off depending on the buttons pressed
    for (int k = 0; k < 4; k++)
    {
      if (digitalRead(button_table[k]) == LOW)
      {
        digitalWrite(led_table[k], HIGH);
      }
      else
        digitalWrite(led_table[k], LOW);
    }

    for (int k = 0; k < 4; k++)
    {
      if (bt[k].click())
      {
        if (answer_player_table[buttons_counter] == k)
        {
          buttons_counter++;
          if (buttons_counter > level)
          {
            level++;
            if (level >= level_max)
              state = 3;
            else
            {
              buttons_counter = 0;
              timer_random_led = millis();
              state = 0;
            }
          }
        }

        else
        {
          state = 0;
          buttons_error++;
          buttons_counter = 0;
          timer_random_led = millis();
        }
      }

      if (buttons_error >= 3)
        state = 2;

      else
        for (int m = 0; m < 5; m++)
        {
          answer_player_table[m] = error_table[random_led_table[m]][buttons_error];
        }
    }
    break;

  case 2: // when you make three mistakes, all the leds are on and blinking
    if (millis() > timer_random_led + 300)
    {
      for (int o = 0; o < 4; o++)
      {
        state_led_table[o] = HIGH;
      }
      if (millis() > timer_random_led + 600)
        timer_random_led = millis();
    }
    
    else
    {
      for (int o = 0; o < 4; o++)
      {
        state_led_table[o] = LOW;
      }
    }
    update_leds();
    lcd_position("LOOSER,",0,0,true);
    lcd_position("YOU LOOSE :(",0,1,false);
    delay(150);
    break;

  case 3: // when you win, all the leds are on
    for (int o = 0; o < 4; o++)
    {
      state_led_table[o] = HIGH;
    }
    update_leds();
    lcd_position("BOSS,",0,0,true);
    lcd_position("YOU WIN :)",0,1,false);
    delay(150);
    break;

  case 4: // basic state, start of the game
    if (bt[0].click())
    {

      state_led_table[0] = HIGH;
      update_leds();
      delay(200);
      state_led_table[0] = LOW;
      update_leds();
      delay(200);
      timer_random_led = millis();
      leds_counter = 0;
      buttons_counter = 0;
      buttons_error = 0;
      state = 0;
      lcd_position("G M  I  P O R S ", 0, 0, true);
      lcd_position(" A E  N  R G E S ", 0, 1, false);
    }
    break;
  }
}

void setup_bt(int nb_bt)
{
  /*
    Input : nb_bt : number of buttons (int)
    Output : none
    Description: This function is used to initialize the buttons
  */
  for (int k = 0; k < nb_bt; k++)
  {
    bt[k].begin(button_table[k], HIGH, 200, 1500, 200);
  }
}

void read_bt(int nb_bt)
{
  /*
    Input : nb_bt : number of buttons (int)
    Output : none
    Description: This function is used to read the buttons
  */
  for (int k = 0; k < nb_bt; k++)
  {
    bt[k].read_Bt();
  }
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