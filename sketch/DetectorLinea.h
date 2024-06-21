#ifndef DetectorLinea_h
#define DetectorLinea_h

#include <Arduino.h>
#include <QTRSensors.h>

class DetectorLinea {
public:
    DetectorLinea(uint8_t* pins, uint8_t numSensores);
    void begin();
    void update();
    uint16_t* getSensorValues();
    bool* getEstados(uint16_t minWhite, uint16_t maxWhite);
    void string(char* buffer, size_t bufferSize);

private:
    QTRSensors _qtr;
    uint8_t _numSensores;
    uint16_t* _sensorValues;
    bool* _estados;
};

#endif
