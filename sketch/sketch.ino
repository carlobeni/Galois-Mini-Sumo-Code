#include "DetectorEnemigo.h"
#include "DetectorLinea.h"
#include "ControlMovimiento.h"
//#include "GestorEstados.h"

// Definición de pines para los motores
#define M1_INB 15 // LILA
#define M1_PWM 2 // AZUL
#define M1_INA 4 // VERDE

#define M2_INB 18 // LILA
#define M2_PWM 19 // AZUL
#define M2_INA 21 // VERDE

ControlMovimiento controlMov(M1_PWM, M1_INA, M1_INB, M2_PWM, M2_INA, M2_INB);

// Conector 1: 3 sensores de Enemigo
// Conector 2: 3 sensores de Enemigo
// Conector 3: sensor enemigo trasero + 2 sensores QT

const uint8_t numSensoresE = 7;
uint8_t pinSensoresE[numSensoresE] = {14, 12, 13, 27, 26, 25, 32}; // Define pines de los sensores

DetectorEnemigo detE(pinSensoresE, numSensoresE);

const uint8_t numSensoresL = 2;
uint8_t pinSensoresL[numSensoresL] = {33, 35};
DetectorLinea detL(pinSensoresL, numSensoresL);

//GestorEstados gestorEstados(controlMov, detE, detL);

void setup()
{
  Serial.begin(115200);
  detE.begin();
  detL.begin();
  controlMov.begin();
}

void loop()
{
  
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

  delay(10);
  


  // Control de los motores basado en los sensores de enemigo
  // Motor 0: primero
  // Motor 1: segundo (feo)
  // controlMov.setBothMotorsSpeed(20, 20);
  // delay(100);
  
  for(int i=10; i<=60; i++){
    controlMov.setBothMotorsSpeed(i, i);
    Serial.println(i);
    delay(3000);
  }
  
  /*
  gestorEstados.update();
  char buffer[300];
  gestorEstados.string(buffer, sizeof(buffer));
  Serial.println(buffer);
  */
}
