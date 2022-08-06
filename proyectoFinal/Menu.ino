#include "Menu.h"
#include "MemoriaEEPROM.h"
int opc;
String arrayMenu[] = {"Temp HIGH", "Temp LOW", "Luz", "Sonido", "Reset", "Avanzar", "Atras"};

//Menu de configuracion
void mostrar_caracter(int pos) {
  lcd.createChar(1, guion);
  lcd.setCursor(0, pos);
  lcd.write((byte)1);
}
void menu() {
  //lcd.clear();
  if (bandera == 0) {
    opc = encoder(opc,-1);
  }
  btn_encoder();
 
  //pantalla0
  if (opc == 0 || opc == 1) {
    pantalla0();
  }else if (opc == 2 || opc == 3) {//cambia a pantalla1
    pantalla1();
  } else if (opc == 4 || opc == 5) {//cambia a pantalla2
    pantalla2();
  }else if (opc == 6 || opc == 7) {//cambia a pantalla3
    pantalla3();
  }else if (opc == 8 || opc == 9) {//cambia a pantalla3
    pantalla4();
  }
}
//Pantallas
void pantalla0(){
  if (bandera == 1) { // seleccionado
      if (opc == 0) {
        clear_down();
        configurarTempAlta();
      } else if (opc == 1) {
        clear_up();
        configurarTempAlta();
      }
      clear_down();
    } else { // NO seleccionado bandera = 0
      clear_up();
      if (opc == 0) {
        lcd.setCursor(0, 2);
        lcd.print(" ");
        mostrar_caracter(0);
      } else if (opc == 1) {
        lcd.setCursor(0, 0);
        lcd.print(" ");
        mostrar_caracter(2);
      }
      lcd.setCursor(1, 0);
      lcd.print(arrayMenu[0]);
      lcd.setCursor(1, 1);
      lcd.print(arrayMenu[1]);
    }
}
void pantalla1(){
  if (bandera == 1) { // seleccionado
      if (opc == 2) {
        clear_down();
        configurarTempBaja();
      } else if (opc == 3) {
        clear_up();
        lcd.setCursor(0, 0);
        mostrar_caracter(0);
        lcd.print(" Luz");
        clear_down();
        Luz();
      }

    } else { // NO seleccionado b = 0
      lcd.setCursor(0, 0); //arriba
      lcd.print("                ");
      if (opc == 2) {
        lcd.setCursor(0, 2);
        lcd.print(" ");
        mostrar_caracter(0);
      } else if (opc == 3) {
        lcd.setCursor(0, 0);
        lcd.print(" ");
        mostrar_caracter(2);
      }
      lcd.setCursor(1, 0);
      lcd.print(arrayMenu[1]);
      lcd.setCursor(1, 1);
      lcd.print("                ");
      lcd.setCursor(1, 1);
      lcd.print(arrayMenu[2]);
    }
}
void pantalla2(){
  if (bandera == 1) { // seleccionado
      if (opc == 4) {
        clear_down();
        Luz();

      } else if (opc == 5) {
        clear_up();

        lcd.setCursor(0, 0);
        mostrar_caracter(0);
        lcd.print("Sonido ");
        clear_down();
        Sonido();
      }

    } else { // NO seleccionado b = 0
      clear_up();
      if (opc == 4) {
        lcd.setCursor(0, 2);
        lcd.print(" ");
        mostrar_caracter(0);
      } else if (opc == 5) {
        lcd.setCursor(0, 0);
        lcd.print(" ");
        mostrar_caracter(2);
      }
      lcd.setCursor(1, 0);
      lcd.print(arrayMenu[2]);
      lcd.setCursor(1, 1);
      lcd.print("                ");
      lcd.setCursor(1, 1);
      lcd.print(arrayMenu[3]);
    }
}
void pantalla3(){
  if (bandera == 1) { // seleccionado
      if (opc == 6) {
        clear_down();
        Sonido();

      } else if (opc == 7) {
        clear_up();
        lcd.setCursor(0, 0);
        mostrar_caracter(0);
        lcd.print("Reset");
        lcd.setCursor(0, 1);
        lcd.print("       ");
        lcd.setCursor(0, 1);
        lcd.print("   COMPLETADO!");
        reiniciarValores();
      }

    } else { // NO seleccionado bandera = 0
      clear_up();
      if (opc == 6) {
        lcd.setCursor(0, 2);
        lcd.print(" ");
        mostrar_caracter(0);
      } else if (opc == 7) {
        lcd.setCursor(0, 0);
        lcd.print(" ");
        mostrar_caracter(2);
      }
      lcd.setCursor(1, 0);
      lcd.print(arrayMenu[3]);
      lcd.setCursor(1, 1);
      lcd.print("                ");
      lcd.setCursor(1, 1);
      lcd.print(arrayMenu[4]);
    }
}
void pantalla4(){
  if (bandera == 1) { // seleccionado
      if (opc == 8) {
        clear_down();
        reiniciarValores();

      } else if (opc == 9) {
        clear_up();
        input = Input::Forward;
        bandera=0;
      }

    } else { // NO seleccionado bandera = 0
      clear_up();
      if (opc == 8) {
        lcd.setCursor(0, 2);
        lcd.print(" ");
        mostrar_caracter(0);
      } else if (opc == 9) {
        lcd.setCursor(0, 0);
        lcd.print(" ");
        mostrar_caracter(2);
      }
      lcd.setCursor(1, 0);
      lcd.print(arrayMenu[4]);
      lcd.setCursor(1, 1);
      lcd.print("                ");
      lcd.setCursor(1, 1);
      lcd.print(arrayMenu[5]);
    }
}
