#include <LiquidCrystal.h>
#include <Keypad.h>

//Initialize LiquidCrystal
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//Initialize Keypad
#define ROWS 4
#define COLS 3
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {22, 24, 26, 28}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {23, 25, 27}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

//Initialize SYSTEM SECURITY variables
int pos=0;
int intentos = 0;
//Password
char clave[7] = {'8','7','6','1','8','9','#'};
char tecla[7]={'0','0','0','0','0','0','#'};

//Pin connection for RGB LED (Always in the POWER field)
#define REDPIN 43
#define GREENPIN 45
#define BLUEPIN 41

void setup(){
  //Initialize RGB LED
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);
  
  lcd.begin(16, 2);
  mostrar(0,0,"Bienvenido!");
  delay(3000);
  lcd.clear();
}
void loop(){
    sistemaSeguridad();
}
void sistemaSeguridad(){
    if(intentos < 3){ 
        introducirClave();
    }
    else{
        sistemaBloqueado();
    }
}
void introducirClave(){
  mostrar(0,0,"Clave:");
  char key = keypad.getKey();
  if(key){
    if(key=='#'||pos==7){
      verificar();
    }else{
      leerClave(key); 
    }
  }  
}
void leerClave(char key){
  mostrar(pos,1,"*");
  tecla[pos]=key;
  pos++;
}
void verificar(){
    for(pos=0;pos<7;pos++){
      if(tecla[pos]!=clave[pos]){
        claveIncorrecta();
        return;
      }
    }
    claveCorrecta();
}
void claveIncorrecta(){
  limpiarAbajo();
  mostrar(0,1,"Error!");
  tecla[6]='#';
  asignarColor(255,120,0,2000); //Orange and wait 2 seconds.
  if(intentos<2){
    mostrar(0,1," intente de nuevo!");
    asignarColor(0,0,0,2000); //Led turn off...
    }
  lcd.clear();
  pos = 0;
  intentos++;
}
void claveCorrecta(){
  mostrar(0,1,"Correcto");
  delay(1500);
  lcd.clear();
  mostrar(0,0,"   Bienvenido");
  asignarColor(0,255,0,2000);  //Green
  asignarColor(0,0,0,2000);
  desbloquearSistema();
  lcd.clear();
}
void sistemaBloqueado(){
  mostrar(0,0,"Sistema Bloqueado");
  asignarColor(255,0,0,5000);  //Red
  asignarColor(0,0,0,2000);
  pos = intentos = 0;
  lcd.clear();
}
void desbloquearSistema(){
    asignarColor(120,0,255,1000);
    asignarColor(255,0,120,1000);
    asignarColor(255,0,255,1000);
    asignarColor(255,0,120,1000);
    asignarColor(255,0,255,1000);
    asignarColor(255,0,120,1000);
    asignarColor(120,0,255,1000);
    asignarColor(255,0,255,1000);
    asignarColor(255,0,120,1000);
    asignarColor(120,0,255,1000);
    pos=intentos=0;
}
void cambiarColor(int red, int green, int blue){
  analogWrite(REDPIN,red);
  analogWrite(GREENPIN,green);
  analogWrite(BLUEPIN,blue);
}
void asignarColor(int r, int g, int b, int time_ms){
  cambiarColor(r,g,b);
  delay(time_ms);
}
void mostrar(int c, int f, char message[]){
  
  lcd.setCursor(c, f);
  lcd.print(message);
}
void limpiarAbajo(){
  
  lcd.setCursor(0, 1);
  lcd.print("                       ");
}
