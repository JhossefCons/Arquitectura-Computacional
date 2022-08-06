#include "Configuracion.h"

//funciones de configuracion
void configurarTempAlta(){
        valor=temperaturaAlta;
        valor = encoder(valor,0);
        lcd.setCursor(12, 0);
        lcd.print( valor );
        lcd.setCursor(14, 0);
        lcd.print(" C");
        temperaturaAlta = valor;
        EEPROM.write(dirTempAlta, temperaturaAlta );
}
void configurarTempBaja(){
        lcd.setCursor(0, 0);
        mostrar_caracter(0);
        lcd.print(" Temp LOW ");
        valor = temperaturaBaja;
        valor = encoder(valor,1);
        lcd.setCursor(12, 0);
        lcd.print( valor );
        lcd.setCursor(14, 0);
        lcd.print("C");
        temperaturaBaja = valor;
        EEPROM.write(dirTempBaja, temperaturaBaja);
}
void Luz(){
        valor = luz;
        valor = encoder(valor,2);
        lcd.setCursor(6, 0);
        lcd.print( valor );
        lcd.setCursor(9, 0);
        lcd.print(" lm");
        luz = valor;
        EEPROM.put(dirLuz, luz);
}
void Sonido(){
        valor = sonido;
        valor = encoder(valor,3);
        lcd.setCursor(9, 0);
        lcd.print( valor );
        lcd.setCursor(11, 0);
        lcd.print(" dB");
        sonido = valor;
        EEPROM.write(dirSonido, sonido);
}
