#include "ReceptorIR.h"

ReceptorIR::ReceptorIR(int recvPin)
  : RECV_PIN(recvPin),
    irrecv(recvPin),
    currentState(STOPPED),
    lastState(STOPPED) {}

void ReceptorIR::begin() {
  irrecv.enableIRIn();
}

void ReceptorIR::loop() {
  if (irrecv.decode(&results)) {
    handleIRCode(results.value);
    irrecv.resume();
  }
}

ReceptorIR::SumoState ReceptorIR::getCurrentState() const {
  return currentState;
}

ReceptorIR::SumoState ReceptorIR::getLastState() const {
  return lastState;
}

void ReceptorIR::handleIRCode(unsigned long code) {
  switch (code) {
    case BUTTON_READY:
      Serial.println("Click Ready");
      currentState = READY;
      break;

    case BUTTON_START:
      if (currentState == READY) {
        Serial.println("Click Start");
        currentState = RUNNING;
      }
      break;

    case BUTTON_STOP:
      Serial.println("Click Stop");
      currentState = STOPPED;
      break;

    default:
      Serial.println("Unknown button click");
      break;
  }
  lastState = currentState;
}
