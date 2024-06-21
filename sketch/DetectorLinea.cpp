#include "DetectorLinea.h"

DetectorLinea::DetectorLinea(uint8_t* pins, uint8_t numSensores) : _numSensores(numSensores) {
  _qtr.setTypeAnalog();
  _qtr.setSensorPins(pins, numSensores);
  _sensorValues = new uint16_t[numSensores];
  _estados = new bool[numSensores];
}

void DetectorLinea::begin() {
  _qtr.setTypeAnalog();
  _qtr.setEmitterPin(2); // Si el pin del emisor no está conectado, omitir esta línea
  _qtr.calibrate(); // Calibración automática
}

void DetectorLinea::update() {
  _qtr.read(_sensorValues);
}

uint16_t* DetectorLinea::getSensorValues() {
  return _sensorValues;
}

bool* DetectorLinea::getEstados(uint16_t minWhite, uint16_t maxWhite) {
  for (uint8_t i = 0; i < _numSensores; i++) {
    _estados[i] = (_sensorValues[i] >= minWhite && _sensorValues[i] <= maxWhite);
  }
  return _estados;
}

void DetectorLinea::string(char* buffer, size_t bufferSize) {
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
