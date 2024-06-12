#ifndef DetectorLinea_h
#define DetectorLinea_h

#include <Arduino.h>
#include <QTRSensors.h>

class DetectorLinea {
  public:
    DetectorLinea(uint8_t* pins, uint8_t num_sensores);
    void begin();
    void update();
    uint16_t* getSensorValues();
    bool* getEstados(uint16_t minWhite, uint16_t maxWhite);

  private:
    QTRSensors qtr;
    uint8_t num_sensores;
    uint16_t* sensorValues;
};

#endif
