#include "DetectorEnemigo.h"
#include "DetectorLinea.h"
#include "ControlMovimiento.h"
#include "GestorEstados.h"

// Definición de pines para los 

#define M1_PWM 0 //AZUL
#define M1_INA 4 //VERDE
#define M1_INB 2 //LILA

#define M2_PWM 17 //AZUL
#define M2_INA 5 //VERDE
#define M2_INB 16 //LILA

ControlMovimiento controlMov(M1_PWM, M1_INA, M1_INB, M2_PWM, M2_INA, M2_INB);

const uint8_t numSensoresE = 8;
uint8_t pinSensoresE[numSensoresE] = {32, 33, 25, 26, 27, 14, 12};
DetectorEnemigo detE(pinSensoresE, numSensoresE);

const uint8_t numSensoresL = 2;
uint8_t pinSensoresL[numSensoresL] = {34, 12};
DetectorLinea detL(pinSensoresL, numSensoresL);

GestorEstados gestorEstados(controlMov, detE, detL);

void setup() {
  Serial.begin(115200);
  detE.begin();
  detL.begin();
  controlMov.begin();
}

void loop() {
  //gestorEstados.update();
  for(int i=0; i<=150 ; i+=20)
    controlMov.setBothMotorsSpeed(i,i);
}
