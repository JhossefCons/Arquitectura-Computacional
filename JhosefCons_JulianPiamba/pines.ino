#include "pines.h"

//pines usados para el keypad
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[ROWS] = {44, 42, 40, 38}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {51, 49, 47}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);

//pines para pantalla 16x2
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);


//funcion para RGB con parametros
void RGB(int R, int G, int B) {
  digitalWrite(RGB_RED, R);
  digitalWrite(RGB_GREEN, G);
  digitalWrite(RGB_BLUE, B);
}
