#include "ControlMovimiento.h"

MotorDriver::MotorDriver(uint8_t pwmPin, uint8_t inaPin, uint8_t inbPin)
  : _pwmPin(pwmPin), _inaPin(inaPin), _inbPin(inbPin) {}

void MotorDriver::begin() {
    pinMode(_pwmPin, OUTPUT);
    pinMode(_inaPin, OUTPUT);
    pinMode(_inbPin, OUTPUT);
}

void MotorDriver::setSpeed(int speed) {
    if (speed >= 0) {
        digitalWrite(_inaPin, HIGH);
        digitalWrite(_inbPin, LOW);
        _forward = true;
    } else {
        digitalWrite(_inaPin, LOW);
        digitalWrite(_inbPin, HIGH);
        speed = -speed;
        _forward = false;
    }
    analogWrite(_pwmPin, speed);
}

ControlMovimiento::ControlMovimiento(uint8_t pwm1, uint8_t ina1, uint8_t inb1,
                                     uint8_t pwm2, uint8_t ina2, uint8_t inb2)
  : motor1(pwm1, ina1, inb1), motor2(pwm2, ina2, inb2) {}

void ControlMovimiento::begin() {
    motor1.begin();
    motor2.begin();
}

void ControlMovimiento::adelante(int potencia) {
    motor1.setSpeed(potencia);
    motor2.setSpeed(potencia);
}

void ControlMovimiento::atras(int potencia) {
    motor1.setSpeed(-potencia);
    motor2.setSpeed(-potencia);
}

void ControlMovimiento::giroDerecho(int potencia) {
    motor1.setSpeed(potencia);
    motor2.setSpeed(-potencia);
}

void ControlMovimiento::giroIzquierdo(int potencia) {
    motor1.setSpeed(-potencia);
    motor2.setSpeed(potencia);
}

void ControlMovimiento::setMotor1Speed(int potencia) {
    motor1.setSpeed(potencia);
}

void ControlMovimiento::setMotor2Speed(int potencia) {
    motor2.setSpeed(potencia);
}

void ControlMovimiento::setBothMotorsSpeed(int potencia1, int potencia2) {
    motor1.setSpeed(potencia1);
    motor2.setSpeed(potencia2);
}
