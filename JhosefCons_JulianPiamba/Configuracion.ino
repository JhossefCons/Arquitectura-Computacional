#include "Configuracion.h"



//funciones de configuracion
void temp_High(){
        valor=temp_high;
        valor = encoder(valor,0);
        lcd.setCursor(12, 0);
        lcd.print( valor );
        lcd.setCursor(14, 0);
        lcd.print("C");
        temp_high = valor;
        EEPROM.write(dir_temp_high, temp_high );
}
void temp_Low(){
        lcd.setCursor(0, 0);
        mostrar_caracter(0);
        lcd.print(" Temp LOW ");
        valor = temp_low;
        valor = encoder(valor,1);
        lcd.setCursor(12, 0);
        lcd.print( valor );
        lcd.setCursor(14, 0);
        lcd.print("C");
        temp_low = valor;
        EEPROM.write(dir_temp_low, temp_low);
}
void Luz(){
        valor = luz;
        valor = encoder(valor,2);
        lcd.setCursor(6, 0);
        lcd.print( valor );
        lcd.setCursor(9, 0);
        lcd.print(" lm");
        luz = valor;
        EEPROM.put(dir_luz, luz);
}
void Sonido(){
        valor = sonido;
        valor = encoder(valor,3);
        lcd.setCursor(9, 0);
        lcd.print( valor );
        lcd.setCursor(11, 0);
        lcd.print(" dB");
        sonido = valor;
        EEPROM.write(dir_sonido, sonido);
}
