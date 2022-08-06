//Librerias utilizadas
#include <Wire.h>
#include "StateMachineLib.h"
#include "AsyncTaskLib.h"
#include "MemoriaEEPROM.h"
#include "pines.h"
#include "Seguridad.h"
#include "Menu.h"
#include "Configuracion.h"
#include "Sistema.h"

void arrancar();
void configurar();
void runn();
void encenderAlarma();

// tareas asincronicas
AsyncTask asyncTaskArrancar(10, true, arrancar);
AsyncTask asyncTaskConfig(100, true, configurar);
AsyncTask asyncTaskrunn(10, true, runn);
AsyncTask asyncTaskAlarm(10, true, encenderAlarma);

//buzzer 
float sinVal;
int toneVal;
boolean activarAlarma= false;

//fotosensor
void fotoresistor(void);
int foto_value = 0;
int luz;

//temperatura
float temperaturaVal = 0.0;
byte temperaturaAlta;
byte temperaturaBaja;
void temperatura(void);

//Sonido
byte sonido;
int valor=-1;
int bandera = 0;//hace parte del menu
//int opc = 0;
int encoder(int encoderValX, int rango);

// guion
byte guion[8] = {
  0b00000,
  0b11111,
};

//Tareas asíncronas
AsyncTask asyncTaskTemp(2000, true, temperatura);
AsyncTask asyncTaskFoto(1000, true, fotoresistor);
/**************** configuracion de la maquina de estado ****************/
// state Alias
enum State
{
  iniciar = 0,
  configuracion = 1,
  ejecutar = 2,
  alarma = 3
};

// Input Alias
enum Input
{
  Reset = 0,
  Forward = 1,
  Backward = 2,
  Unknown = 3,
};

// Create new StateMachine
StateMachine stateMachine(4, 7);

// Stores last user input
Input input;

// Setup the State Machine
void setupStateMachine() {
  // Add transitions

  //primer ciclo de estado
  stateMachine.AddTransition(iniciar, configuracion, []() {return input == Forward;});

  //segundo ciclo de estado
  stateMachine.AddTransition(configuracion, ejecutar, []() {return input == Forward;});
  stateMachine.AddTransition(configuracion, iniciar, []() {return input == Reset;});

  //tercer ciclo de estado
  stateMachine.AddTransition(ejecutar, configuracion, []() {return input == Backward;});
  stateMachine.AddTransition(ejecutar, alarma, []() {return input == Forward;});

  //cuarto ciclo de estado
  stateMachine.AddTransition(alarma, configuracion, []() {return input == Forward;});

  // Add actions
  stateMachine.SetOnEntering(iniciar, outputA);
  stateMachine.SetOnEntering(configuracion, outputB);
  stateMachine.SetOnEntering(ejecutar, outputC);
  stateMachine.SetOnEntering(alarma, outputD);

  stateMachine.SetOnLeaving(iniciar, []() {Serial.println("Saliendo de iniciar");});
  stateMachine.SetOnLeaving(configuracion, []() {Serial.println("Saliendo de configuracion");});
  stateMachine.SetOnLeaving(ejecutar, []() {Serial.println("Saliendo de ejecutar");});
  stateMachine.SetOnLeaving(alarma, []() {Serial.println("Saliendo de alarma");});
}

void setup() {

  //inicializar consola
  Serial.begin(9600);
  setupStateMachine();

  //inicializar hardware
  lcd.begin(16, 2); // inicializa
  lcd.setCursor(0, 0);
  lcd.print("Password:");

  pinMode (RGB_RED, OUTPUT);
  pinMode (RGB_GREEN, OUTPUT);
  pinMode (RGB_BLUE, OUTPUT);
 
  //inicializacion del encoder
  pinMode(clkPin, INPUT);
  pinMode(dtPin, INPUT);
  pinMode(swPin, INPUT);
  digitalWrite(swPin, HIGH);
  asyncTaskFoto.Start();
  asyncTaskTemp.Start();
  cargarValores();

  // Initial state
  stateMachine.SetState(iniciar, false, true);
}

void loop() {

  // Update State Machine
  stateMachine.Update();
  actualizarEstado();
}

void outputA()
{
  asyncTaskArrancar.Start();
}

void outputB()
{
  asyncTaskAlarm.Stop();
  asyncTaskArrancar.Stop();
  asyncTaskConfig.Start();
}

void outputC()
{
  asyncTaskConfig.Stop();
  asyncTaskrunn.Start();
}

void outputD()
{
  asyncTaskrunn.Stop();
  asyncTaskAlarm.Start();
}

//acciones que realiza la maquina de estado
void arrancar() {
  seguridad() ;
}
void configurar() {
  input = Input::Unknown;
  menu();
}
void runn() {
   input = Input::Unknown;
   clear_down();
   lcd.setCursor(0, 0);
   asyncTaskTemp.Update();
   lcd.clear();
   lcd.setCursor(1, 1);
   asyncTaskFoto.Update();
}
void encenderAlarma() {
  input = Input::Unknown;
  Buzzer();
}

//actualizacion de la maquina de estado
void actualizarEstado() {
  int currentState = stateMachine.GetState();

  switch (currentState) {
    case iniciar:
      arrancar();
      break;
    case configuracion:
      configurar();
      break;
    case ejecutar:
      runn();
      break;
    case alarma:
      encenderAlarma();
      break;
  }
}


//Funciones del Encoder
void btn_encoder() {

  if (digitalRead(swPin) == LOW) {
    if (bandera == 0) {
      bandera = 1;
    } else {
      bandera = 0;
    }
  }
}

int encoder(int encoderValX, int rango) {
  int change = getEncoderTurn();
  encoderValX = encoderValX + change;

  if (rango == -1) {
    if (encoderValX >= 10) {
      encoderValX = 9;
    } else if (encoderValX <= -1) {
      encoderValX = 0;
    }
   
  }else if(rango == 0){ // TEMP HIGH
    if (encoderValX >= 51) {
      encoderValX = 50;
    } else if (encoderValX <= 24) {
        encoderValX = 25;      
    }
  }else if(rango == 1){ // TEMP LOW
    if (encoderValX >= 19) {
      encoderValX = 18;
    } else if (encoderValX <= -1) {
      encoderValX = 0;
    }
  }else if(rango == 2){ // LUZ
    if (encoderValX >= 301) {
      encoderValX = 300;
    } else if (encoderValX <= -1) {
      encoderValX = 0;
    }
  }else if(rango == 3){ // SONIDO
    if (encoderValX >= 31) {
      encoderValX = 30;
    } else if (encoderValX <= -1) {
      encoderValX = 0;
    }
  }

  return encoderValX;
}

int getEncoderTurn(void)
{
  static int oldA = HIGH;
  static int oldB = HIGH;
  int result = 0;
  int newA = digitalRead(clkPin);
  int newB = digitalRead(dtPin);
  if (newA != oldA || newB != oldB)
  {
    // something has changed
    if (oldA == HIGH && newA == LOW)
    {
      result = (oldB * 2 - 1);
    }
  }
  oldA = newA;
  oldB = newB;
  return result;
}
//Funciones del lcd
void clear_up() {
  lcd.setCursor(0, 0);
  lcd.print("                 ");
}
void clear_down() {
  lcd.setCursor(0, 1);
  lcd.print("                 ");
}
void limpiarMsj() {
  lcd.setCursor(0, 1);
  lcd.print("                ");
}
