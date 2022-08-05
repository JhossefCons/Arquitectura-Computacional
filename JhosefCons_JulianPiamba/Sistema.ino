#include "Sistema.h"

//Funciones del sistema
void fotoresistor(void) {

  foto_value = analogRead(photocellPin);
  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 0);
  lcd.print("Photocell:");
  lcd.setCursor(10, 0);
  lcd.print("  ");
  lcd.setCursor(12, 0);
  lcd.print(foto_value);

  if (foto_value > luz) {
    RGB(1, 0, 0);
    //RGB(0, 1, 1);
    delay(500);
    RGB(0, 0, 0);
    input = Input::Forward;
    activaralarma = true;
  } else {
    RGB(0, 1, 0);
    delay(500);
    RGB(0, 0, 0);
  }

  delay(1000);
  lcd.setCursor(11, 0);
  lcd.print(" ");
}
void temperatura(void)
{
  //read thermistor value
  long a = 1023 - analogRead(analogPin);
  //the calculating formula of temperature
  float temp_value = beta / (log((1025.0 * 10 / a - 10) / 10) + beta / 298.0) - 273.0;
  float tempF = temp_value + 273.15;
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temp_value);
  lcd.print(" C");

  if (temp_value > temp_high) { // 25
    RGB(1, 0, 0);
    delay(500);
    RGB(0, 0, 0);
    input = Input::Forward;
    activaralarma = true;
  } else if (temp_value < temp_low) { //18
    RGB(0, 0, 1);
    delay(500);
    RGB(0, 0, 0);
    input = Input::Forward;
    activaralarma = true;
  } else if (temp_value < temp_high && temp_value > temp_low) {
    RGB(0, 1, 0);
    delay(1000);
    RGB(0, 0, 0);
  }

  delay(200); //wait for 100 milliseconds
}

//alarma
void Buzzer() {
    for(int x=0; x<180; x++){
            // convertimos grados en radianes para luego obtener el valor.
            sinVal = (sin(x*(3.1412/180)));
            // generar una frecuencia a partir del valor sin
            toneVal = 2000+(int(sinVal*1000));
            tone(buzzer, toneVal,3000);
            delay(2); 
     }  
      lcd.setCursor(0, 0);
      lcd.print("                ");
      lcd.setCursor(0, 0);
      lcd.print("   Alerta!!");
      delay(3000);
    input = Input::Forward;
}
