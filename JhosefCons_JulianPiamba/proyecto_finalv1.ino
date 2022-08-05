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

void accionInit();
void accionConfig();
void accionRun();
void accionAlarm();

// tareas asincronicas
AsyncTask asyncTaskInit(10, true, accionInit);
AsyncTask asyncTaskConfig(100, true, accionConfig);
AsyncTask asyncTaskRun(10, true, accionRun);
AsyncTask asyncTaskAlarm(10, true, accionAlarm);

//caracter ñ
byte caracter[8] = {
  0b00000,
  0b01110,
  0b00000,
  0b10110,
  0b11001,
  0b10001,
  0b10001,
  0b00000
};
//buzzer 
boolean activaralarma= false;
float sinVal;
int toneVal;


//fotosensor
int foto_value = 0;
int luz;
void fotoresistor(void);

//temperatura

byte temp_high;
byte temp_low;
void temperatura(void);

//Sonido
byte sonido;
int valor=-1;
int bandera = 0;//hace parte del menu
//int opc = 0;
int encoder(int encoderValX, int rango);

// flechita
byte flechita[8] = {
  0b00000,
  0b00100,
  0b00010,
  0b11111,
  0b00010,
  0b00100,
  0b00000,
  0b00000
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
  // stateMachine.AddTransition(iniciar, iniciar, []() {return input == Reset;});

  //segundo ciclo de estado
  //stateMachine.AddTransition(configuracion, iniciar, []() {return input == Backward;});
  stateMachine.AddTransition(configuracion, ejecutar, []() {return input == Forward;});
  stateMachine.AddTransition(configuracion, iniciar, []() {return input == Reset;});

  //tercer ciclo de estado
  stateMachine.AddTransition(ejecutar, configuracion, []() {return input == Backward;});
  stateMachine.AddTransition(ejecutar, alarma, []() {return input == Forward;});
  //stateMachine.AddTransition(ejecutar, iniciar, []() {return input == Reset;});

  //cuarto ciclo de estado
  stateMachine.AddTransition(alarma, configuracion, []() {return input == Forward;});
  //stateMachine.AddTransition(alarma, iniciar, []() {return input == Reset;});

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
  Serial.println("Starting State Machine...");
  setupStateMachine();
  Serial.println("Start Machine Started");

  //inicializar hardware
  lcd.begin(16, 2); // inicializa
  lcd.setCursor(0, 0);
  lcd.print("  contrase");
  lcd.createChar(1, caracter);
  lcd.setCursor(10, 0);
  lcd.write((byte)1);
  lcd.print("a:");

  pinMode (RGB_RED, OUTPUT);
  pinMode (RGB_GREEN, OUTPUT);
  pinMode (RGB_BLUE, OUTPUT);
 
  //inicializacion del encoder
  pinMode(clkPin, INPUT);
  pinMode(dtPin, INPUT);
  pinMode(swPin, INPUT);
  digitalWrite(swPin, HIGH);

  asyncTaskTemp.Start();
  asyncTaskFoto.Start();
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
  asyncTaskInit.Start();
}

void outputB()
{
  asyncTaskAlarm.Stop();
  asyncTaskInit.Stop();
  asyncTaskConfig.Start();
}

void outputC()
{
  asyncTaskConfig.Stop();
  asyncTaskRun.Start();
}

void outputD()
{
  asyncTaskRun.Stop();
  asyncTaskAlarm.Start();
}

//acciones que realiza la maquina de estado
void accionInit() {
  seguridad() ;
}
void accionConfig() {
  input = Input::Unknown;
  menu();
}
void accionRun() {
   input = Input::Unknown;
   clear_down();
   //asyncTaskTemp.Update();
   delay(3000);
   asyncTaskFoto.Update();
}
void accionAlarm() {
  input = Input::Unknown;
  Buzzer();
}

//actualizacion de la maquina de estado
void actualizarEstado() {
  int currentState = stateMachine.GetState();

  switch (currentState) {
    case iniciar:
      accionInit();
      break;
    case configuracion:
      accionConfig();
      break;
    case ejecutar:
      accionRun();
      break;
    case alarma:
      accionAlarm();
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
