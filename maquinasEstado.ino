#include "AsyncTaskLib.h"
#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 31, d5 = 32, d6 = 33, d7 = 34;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int LED_ROJO = 25;  
int LED_VERDE = 26; 
int LED_AZUL = 27;  

#define pin_photo A0
int photo_value = 0;
void read_photoresistor(void);

#define pin_temp A1
float temp_value = 0.0;
float temperatura = 0.0;
void read_temperature(void);

AsyncTask asyncTaskTemp(200, true, read_temperature);
AsyncTask asyncTaskPhoto(100, true, read_photoresistor);

enum estado
{
  estado_A,
  estado_B,
  estado_C,
  alarma,
};

estado currentState;

void read_photoresistor()
{
  photo_value = analogRead(pin_photo);
  lcd.setCursor(0, 0);
  lcd.print("PHOTO:");
  lcd.print(photo_value);
}

void read_temperature()
{
  temp_value = analogRead(pin_temp);
  float milivolts = temp_value * (5000 / 1024);
  temperatura = milivolts / 10;
  lcd.setCursor(0, 1);
  lcd.print("Temperatura:");
  lcd.print(temperatura);
}

void setup()
{
  Serial.begin(9600);
  Serial.println("Iniciando");
  lcd.begin(16, 2);
  lcd.clear();
  pinMode(LED_ROJO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AZUL, OUTPUT);

  currentState = estado_A;

  asyncTaskTemp.Start();
  asyncTaskPhoto.Start();
}

void loop()
{
  asyncTaskTemp.Update();
  asyncTaskPhoto.Update();
  actualizar_estado();
}

void actualizar_estado()
{
  switch (currentState)
  {
    case estado::estado_A: funcion_A();   break;
    case estado::estado_B: funcion_B();   break;
    case estado::estado_C: funcion_C();   break;
    case estado::alarma: arctivarAlarma();   break;
    default: break;
  }
}

void funcion_A() {
  digitalWrite(LED_AZUL, HIGH);
  digitalWrite(LED_ROJO, LOW);
  digitalWrite(LED_VERDE, LOW);
  
  if (temperatura > 18) {
    currentState = estado::estado_B;
  }
}

void funcion_B() {
  digitalWrite(LED_AZUL, LOW);
  digitalWrite(LED_ROJO, LOW);
  digitalWrite(LED_VERDE, HIGH);
  
  if (temperatura > 25) {
    currentState = estado::estado_C;
  }
  if (temperatura <= 18) {
    currentState = estado::estado_A;
  }
}

void funcion_C() {
  digitalWrite(LED_AZUL, LOW);
  digitalWrite(LED_ROJO, HIGH);
  digitalWrite(LED_VERDE, LOW);
  
  if (temperatura > 40) {
    currentState = estado::alarma;
  }
  if (temperatura <= 25) {
    currentState = estado::estado_B;
  }
}

void arctivarAlarma() {
  digitalWrite(LED_AZUL, HIGH);
  digitalWrite(LED_ROJO, HIGH);
  digitalWrite(LED_VERDE, LOW);

  delay(500);
  if (temperatura <= 25) {
    currentState = estado::estado_B;
  }
}
