#ifndef DetectorEnemigo_h
#define DetectorEnemigo_h

#include <Arduino.h>

class DetectorEnemigo {
  public:
    DetectorEnemigo(uint8_t pinSensores[], uint8_t numSensores);
    void begin();
    void update();
    bool* getEstados();

  private:
    uint8_t* _pinSensores;
    uint8_t _numSensores;
    bool* _estados;
};

#endif
