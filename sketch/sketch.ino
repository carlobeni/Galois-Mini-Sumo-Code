#include "DetectorEnemigo.h"
#include "DetectorLinea.h"
#include "ControlMovimiento.h"
#include "GestorEstados.h"

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

void setup()
{
  Serial.begin(115200);
  detE.begin();
  detL.begin();
  controlMov.begin();
}

void loop()
{
  /* 
  //Actualización de los sensores de enemigo
  detE.update();
  bool *estadosE = detE.getEstados();
  Serial.print("Estados de los sensores de Enemigo: ");
  for (uint8_t i = 0; i < numSensoresE; i++)
  {
    Serial.print(estadosE[i]);
    Serial.print(" ");
  }
  Serial.println();
  */

  /*
  // Actualización de los sensores de línea
  detL.update();
  bool *estadosL = detL.getEstados(0, 1000);
  Serial.print("Estados de los sensores de Línea: ");
  for (uint8_t i = 0; i < numSensoresL; i++)
  {
    Serial.print(estadosL[i]);
    Serial.print(" ");
  }
  Serial.println();

  */
  gestorEstados.update();
  char buffer[300];
  gestorEstados.string(buffer, sizeof(buffer));
  Serial.println(buffer);

}
