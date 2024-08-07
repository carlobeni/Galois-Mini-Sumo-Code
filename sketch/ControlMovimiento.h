#ifndef CONTROLMOVIMIENTO_H
#define CONTROLMOVIMIENTO_H

#include <Arduino.h>

class MotorDriver {
public:
    MotorDriver(uint8_t pwmPin, uint8_t inaPin, uint8_t inbPin);
    void begin();
    void setSpeed(int speed);

private:
    uint8_t _pwmPin;
    uint8_t _inaPin;
    uint8_t _inbPin;
    bool _forward = true;
};

class ControlMovimiento {
public:
    ControlMovimiento(uint8_t pwm1, uint8_t ina1, uint8_t inb1,
                      uint8_t pwm2, uint8_t ina2, uint8_t inb2);

    void begin();
    void adelante(int potencia);
    void atras(int potencia);
    void giroDerecho(int potencia);
    void giroIzquierdo(int potencia);

    void setMotor1Speed(int potencia);
    void setMotor2Speed(int potencia);
    void setBothMotorsSpeed(int potencia1, int potencia2);

private:
    MotorDriver motor1;
    MotorDriver motor2;
};

#endif
