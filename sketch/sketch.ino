#include "DetectorEnemigo.h"
#include "DetectorLinea.h"
#include "ControlMovimiento.h"
#include "GestorEstados.h"
#include <IRremote.h>

// Define del pin IR y los códigos de los botones
#define RECV_PIN 11
#define BUTTON_READY 0xA90
#define BUTTON_START 0xA91
#define BUTTON_STOP 0xA92
enum ControllState
{
  STOPPED,
  READY,
  RUNNING
};
decode_results results;
ControllState currentState = STOPPED;
IRrecv irrecv(RECV_PIN);

// Definición de pines para los motores
#define M1_PWM 0 // AZUL
#define M1_INA 4 // VERDE
#define M1_INB 2 // LILA

#define M2_PWM 17 // AZUL
#define M2_INA 5  // VERDE
#define M2_INB 16 // LILA

ControlMovimiento controlMov(M1_PWM, M1_INA, M1_INB, M2_PWM, M2_INA, M2_INB);

// Conector 1: 3 sensores de Enemigo
// Conector 2: 3 sensores de Enemigo
// Conector 3: sensor enemigo trasero + 2 sensores QT

const uint8_t numSensoresE = 7;
uint8_t pinSensoresE[numSensoresE] = {25, 33, 32, 26, 27, 14, 39}; // Define pines de los sensores
DetectorEnemigo detE(pinSensoresE, numSensoresE);

const uint8_t numSensoresL = 2;
uint8_t pinSensoresL[numSensoresL] = {34, 36};
DetectorLinea detL(pinSensoresL, numSensoresL);

GestorEstados gestorEstados(controlMov, detE, detL);

// Función para manejar los códigos recibidos del control remoto
void handleIRCode(unsigned long code)
{
  switch (code)
  {
  case BUTTON_READY:
    Serial.println("Click Ready");
    currentState = READY;
    break;

  case BUTTON_START:
    if (currentState == READY)
    {
      Serial.println("Click Start");
      currentState = RUNNING;
    }
    break;

  case BUTTON_STOP:
    Serial.println("Click Stop");
    currentState = STOPPED;
    break;

  default:
    Serial.println("Unknown button click");
    break;
  }
}

void setup()
{
  Serial.begin(115200);
  irrecv.enableIRIn();
  detE.begin();
  detL.begin();
  controlMov.begin();
}

void loop()
{

  if (irrecv.decode(&results)) {
    // Mostrar el valor recibido en formato HEX
    Serial.print("IR code: 0x");
    Serial.println(results.value, HEX);

    // Manejar el código IR recibido
    handleIRCode(results.value);

    // Continuar escuchando para el siguiente código IR
    irrecv.resume();
  }

  // Actualizar el estado del sumo según currentState
  switch (currentState)
  {
  case STOPPED:
    controlMov.setBothMotorsSpeed(0, 0);
    break;

  case READY:
    break;

  case RUNNING:
    gestorEstados.update();
    break;
  }
}
