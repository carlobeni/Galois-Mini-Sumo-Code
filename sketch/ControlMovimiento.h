#ifndef CONTROLMOVIMIENTO_H
#define CONTROLMOVIMIENTO_H

#include <Arduino.h>

class MotorDriver {
public:
    MotorDriver(uint8_t pwmPin, uint8_t inaPin, uint8_t inbPin);
    void begin();
    void setSpeed(int speed);
    int getLastSpeed() const;
    void stop(int brake);

private:
    uint8_t _pwmPin;
    uint8_t _inaPin;
    uint8_t _inbPin;
    bool _forward = true;
    int _lastSpeed = 0;
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
    void giroAntihorarioAxisM1(int potencia); 
    void giroHorarioAxisM2(int potencia); 
    void stop(int brake);

    void setMotor1Speed(int potencia);
    void setMotor2Speed(int potencia);
    void setBothMotorsSpeed(int potencia1, int potencia2);
    void string(char* buffer, size_t bufferSize) const;

private:
    MotorDriver motor1;
    MotorDriver motor2;
};

#endif