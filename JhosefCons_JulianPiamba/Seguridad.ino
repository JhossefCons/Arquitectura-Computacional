#include "Seguridad.h"

//configuracion de la contraseña
char contra[7];
char realcontra[7] = "123456";
byte index = 0;
int intentos = 0;

void seguridad() {
  char key = keypad.getKey(); //lee lo del teclado
  if (key) { // puede ser key
    lcd.setCursor(index, 1);
    lcd.print('*');
    contra[index] = key;
    index++;
  }

  if (index == 6) {
    byte validar = 0;
    for (int i = 0; i < 6; i++) {
      if (contra[i] == realcontra[i]) {
        validar++;
      }
    }

    if (validar == 6) {
      lcd.setCursor(0, 1);
      lcd.print("   Correcto!");
      RGB(0, 1, 0);
      delay(1000);
      RGB(0, 0, 0);
      intentos = 0;
      limpiarMsj();
      input = Input::Forward;

    } else {

      lcd.setCursor(0, 1);
      lcd.print("  incorrecto!");
      RGB(1, 1, 1);
      delay(1000);
      RGB(0, 0, 0);
      limpiarMsj();
      intentos++;
    }
    index = 0;
  }

  if (intentos == 3) {
    lcd.setCursor(0, 1);
    lcd.print("Sistema Bloqueado!");

    RGB(1, 0, 0);
    delay(5000);
    RGB(0, 0, 0);
    //input = Input::Forward;
    limpiarMsj();
    intentos = 0;
  }

}
