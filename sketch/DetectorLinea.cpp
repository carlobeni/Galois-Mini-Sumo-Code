#include "DetectorLinea.h"

DetectorLinea::DetectorLinea(uint8_t* pins, uint8_t num_sensores) : num_sensores(num_sensores) {
  qtr.setTypeAnalog();
  qtr.setSensorPins(pins, num_sensores);
  sensorValues = new uint16_t[num_sensores];
}


void DetectorLinea::begin() {
  qtr.setTypeAnalog();
  qtr.setEmitterPin(2); // Si el pin del emisor no está conectado, omitir esta línea
  qtr.calibrate(); // Calibración automática
}

void DetectorLinea::update() {
  qtr.read(sensorValues);
}

uint16_t* DetectorLinea::getSensorValues() {
  return sensorValues;
}

bool* DetectorLinea::getEstados(uint16_t minWhite, uint16_t maxWhite) {
  bool* estados = new bool[num_sensores];
  for (uint8_t i = 0; i < num_sensores; i++) {
    Serial.print(i);
    Serial.print(") ");
    Serial.print(sensorValues[i]);
    Serial.print(" ");
    estados[i] = (sensorValues[i] >= minWhite && sensorValues[i] <= maxWhite) ? 1 : 0;
  }
  return estados;
}
