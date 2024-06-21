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
  
  _estados[_numSensores-1]=!_estados[_numSensores-1];
}

bool* DetectorEnemigo::getEstados() {
  return _estados;
}

void DetectorEnemigo::string(char* buffer, size_t bufferSize) {
  size_t index = 0;
  for (uint8_t i = 0; i < _numSensores; i++) {
    if (index < bufferSize - 2) { 
      buffer[index++] = _estados[i] ? '1' : '0';
      if (i < _numSensores - 1 && index < bufferSize - 2) {
        buffer[index++] = ',';
      }
    } else {
      break; 
    }
  }
  buffer[index] = '\0'; // Null terminator
}
