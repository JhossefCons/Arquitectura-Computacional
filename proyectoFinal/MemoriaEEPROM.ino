#include "MemoriaEEPROM.h"
//Funciones para EEPROM

void cargarValores(){
  //variables constantes para la direccion de los valores
  temperaturaAlta = EEPROM.read(dirTempAlta);
  temperaturaBaja = EEPROM.read(dirTempBaja);
  luz = EEPROM.get(dirLuz, luz);
  sonido = EEPROM.read(dirSonido);
}
void reiniciarValores(){
  temperaturaAlta=25;
  EEPROM.write(dirTempAlta,temperaturaAlta);
  temperaturaBaja=18;
  EEPROM.write(dirTempBaja,temperaturaBaja);
  luz = 300;
  EEPROM.put(dirLuz,luz);
  sonido = 30;
  EEPROM.write(dirSonido,sonido);
}
