#include <LiquidCrystal.h>
#include <Keypad.h>
#include <EEPROM.h>


extern LiquidCrystal lcd;
extern Keypad keypad;

//Variables para EEPROM
#define dir_temp_high 0
#define dir_temp_low  1
#define dir_luz       2
#define dir_sonido    6

//temperatura
#define analogPin A0 //the thermistor attach to
#define beta 4090 //the beta of the thermistor
#define resistance 10 //the value of the pull-down resistor

//fotoresistor
#define photocellPin  A1

//RGB
#define RGB_GREEN 30
#define RGB_BLUE 32
#define RGB_RED 34

//decoder
#define clkPin 2
#define dtPin 3
#define swPin 4 //the number of the buttonint   click


//buzzer 
#define buzzer 13
