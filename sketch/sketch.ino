#include "DetectorEnemigo.h"
#include "DetectorLinea.h"
#include "ControlMovimiento.h"
#include "GestorEstados.h"

// Definición de pines para los motores
#define M1_INA 18 // VERDE
#define M1_PWM 19 // AZUL
#define M1_INB 21 // LILA

#define M2_INB 16 // LILA
#define M2_PWM 17 // AZUL
#define M2_INA 5  // VERDE

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

// GestorEstados gestorEstados(controlMov, detE, detL);

void setup()
{
  Serial.begin(115200);
  detE.begin();
  detL.begin();
  controlMov.begin();
}

void loop()
{

  //Detector de Enemigo
  char bufferE[300];
  detE.string(bufferE, sizeof(bufferE));
  Serial.println(bufferE);

  //Detector de Linea
  char bufferL[300];
  detL.string(bufferL, sizeof(bufferL));
  Serial.println(bufferL);

  //Cargado por Serial
  /*
  if (Serial.available() > 0) {
    int potencia = Serial.parseInt();
    controlMov.setBothMotorsSpeed(potencia, potencia);
    char buffer[50];
    controlMov.string(buffer, sizeof(buffer));
    Serial.println(buffer);
    delay(10*1000);
  }
  */

  /*
  gestorEstados.update();
  char buffer[300];
  gestorEstados.string(buffer, sizeof(buffer));
  Serial.println(buffer);
  */
}
