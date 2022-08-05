#include "MemoriaEEPROM.h"
//Funciones para EEPROM

void cargarValores(){
  //variables constantes para la direccion de los valores
  temp_high = EEPROM.read(dir_temp_high);
  temp_low = EEPROM.read(dir_temp_low);
  luz = EEPROM.get(dir_luz, luz);
  sonido = EEPROM.read(dir_sonido);
}
void reiniciarValores(){
  temp_high=25;
  EEPROM.write(dir_temp_high,temp_high);
  temp_low=18;
  EEPROM.write(dir_temp_low,temp_low);
  luz = 300;
  EEPROM.put(dir_luz,luz);
  sonido = 30;
  EEPROM.write(dir_sonido,sonido);
}
