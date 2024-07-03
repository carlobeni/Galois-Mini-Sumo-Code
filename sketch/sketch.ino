#include "DetectorEnemigo.h"
#include "DetectorLinea.h"
#include "ControlMovimiento.h"
#include "GestorEstadosAlpha.h"
#include <IRremote.h>

const int RECV_PIN = 13;
IRrecv irrecv(RECV_PIN);
decode_results results;

// Array de códigos Sony en hexadecimal
/*
0 910
1 10
2 810
3 410
4 C10
5 210
6 A10
7 610
8 E10
9 110
OFF A90 (posicion 10)
*/
const unsigned long sonyCodes[] = {0x910, 0x10, 0x810, 0x410, 0xC10, 0x210, 0xA10, 0x610, 0xE10, 0x110, 0xA90};
const int numCodes = sizeof(sonyCodes) / sizeof(sonyCodes[0]);

// Definición de constantes para los códigos
const unsigned long codeReady = sonyCodes[1];
const unsigned long codeStart = sonyCodes[2];
const unsigned long codeStop = sonyCodes[3];

enum StateIR
{
  Init,
  Ready,
  Start,
  Stop
};

StateIR currentStateIR = Init;

// Definición de pines para los motores
// Derecho
#define M1_INA 5  // VERDE
#define M1_INB 16 // LILA
#define M1_PWM 17 // AZUL

// Izquierdo
#define M2_INA 18 // VERDE
#define M2_INB 21 // LILA
#define M2_PWM 19 // AZUL

ControlMovimiento controlMov(M1_PWM, M1_INA, M1_INB, M2_PWM, M2_INA, M2_INB);

// Conector 1: 3 sensores de Enemigo
// Conector 2: 3 sensores de Enemigo
// Conector 3: sensor enemigo trasero + 2 sensores QT

const uint8_t numSensoresE = 7;
uint8_t pinSensoresE[numSensoresE] = {25, 33, 32, 26, 27, 14, 34}; // Define pines de los sensores
DetectorEnemigo detE(pinSensoresE, numSensoresE);

const uint8_t numSensoresL = 2;
uint8_t pinSensoresL[numSensoresL] = {39, 35};
DetectorLinea detL(pinSensoresL, numSensoresL);

GestorEstadosAlpha gestorEstados(controlMov, detE, detL);

void setup()
{
  Serial.begin(115200);
  detE.begin();
  detL.begin();
  controlMov.begin();
  gestorEstados.begin();
  irrecv.enableIRIn();
}

void loop()
{
  if (irrecv.decode(&results))
  {
    if (results.decode_type == SONY)
    {
      unsigned long code = results.value;
      handleIRCode(code);
    }
    irrecv.resume();
  }
  // Activacion/Desactivacion del Gestor de Estados
  if (currentStateIR == Start)
  {
    gestorEstados.update();
    char buffer[300];
    gestorEstados.string(buffer, sizeof(buffer));
    Serial.println(buffer);
  }
  else
  {
    gestorEstados.stop();
  }
}

void handleIRCode(unsigned long code)
{
  switch (currentStateIR)
  {
  case Init:
    if (code == codeReady)
    {
      currentStateIR = Ready;
      Serial.println("Estado: Ready");
    }
    break;

  case Ready:
    if (code == codeStart)
    {
      currentStateIR = Start;
      Serial.println("Estado: Start");
    }
    break;

  case Start:
    if (code == codeStop)
    {
      currentStateIR = Stop;
      Serial.println("Estado: Stop");
    }
    break;

  case Stop:
    if (code == codeStart)
    {
      currentStateIR = Start;
      Serial.println("Estado: Start");
    }
    break;
  }
}
