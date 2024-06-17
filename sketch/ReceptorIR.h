#ifndef RECEPTOR_IR_H
#define RECEPTOR_IR_H

#include <IRremote.h>

class ReceptorIR {
  private:
    const int RECV_PIN;
    IRrecv irrecv;
    decode_results results;

  public:
    enum SumoState { STOPPED, READY, RUNNING };

  private:
    SumoState currentState;
    SumoState lastState;

    static const unsigned long BUTTON_READY = 0xA90; // Código de botón READY
    static const unsigned long BUTTON_START = 0xA91; // Código de botón START
    static const unsigned long BUTTON_STOP = 0xA92;  // Código de botón STOP

  public:
    ReceptorIR(int recvPin);
    
    void begin();
    void loop();
    SumoState getCurrentState() const;
    SumoState getLastState() const;

  private:
    void handleIRCode(unsigned long code);
};

#endif // RECEPTOR_IR_H
