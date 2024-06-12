#include "DetectorEnemigo.h"

DetectorEnemigo::DetectorEnemigo(uint8_t pinSensores[], uint8_t numSensores) {
  _pinSensores = pinSensores;
  _numSensores = numSensores;
  _estados = new bool[numSensores];
}

void DetectorEnemigo::begin() {
  for (uint8_t i = 0; i < _numSensores; i++) {
    pinMode(_pinSensores[i], INPUT);
  }
}

void DetectorEnemigo::update() {
  for (uint8_t i = 0; i < _numSensores; i++) {
    _estados[i] = digitalRead(_pinSensores[i]);
  }
}

bool* DetectorEnemigo::getEstados() {
  return _estados;
}
