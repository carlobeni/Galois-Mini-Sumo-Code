#ifndef GESTOR_ESTADOS_BETHA_H
#define GESTOR_ESTADOS_BETHA_H

#include "ControlMovimiento.h"
#include "DetectorEnemigo.h"
#include "DetectorLinea.h"

enum Estado {
    DISPARO_INIT,
    INICIO,
    BUSQUEDA,
    ALINEACION,
    ROTACION_ORB,
    AVANCE,
    ATAQUE_BAJO,
    ATAQUE_ALTO,
    DETECCION_DE_LINEA,
    RETROCESO,
    GIRO_DERECHO_180,
    GIRO_IZQUIERDO_180,
    GIRO_EN_ATAQUE,
    TALADRO
};

class GestorEstadosBetha {
public:
    GestorEstadosBetha(ControlMovimiento& controlMov, DetectorEnemigo& detE, DetectorLinea& detL);
    void begin();
    void update();
    void stop();
    void string(char* buffer, size_t bufferSize);

private:
    ControlMovimiento& controlMov;
    DetectorEnemigo& detE;
    DetectorLinea& detL;
    Estado estadoActual;

    const unsigned long duracionDisparoInit = 20; // Duración máxima del giro en milisegundos (ejemplo)
    const unsigned int potenciaDisparoInit = 100;    // Potencia actual de ataque

    const unsigned int potenciaBusqueda = 15;
    const unsigned int potenciaGiro = 20;
    const unsigned int potenciaAvance = 20;

    unsigned long tiempoAtaqueBajo;
    const unsigned int potenciaAtaqueBajo= 100;
    const unsigned long duracionAtaqueBajo = 100; 

    unsigned long tiempoAtaqueAlto;
    const unsigned int potenciaAtaqueAlto= 200;
    const unsigned long duracionAtaqueAlto = 500; 

    unsigned long tiempoUltimoIncremento; // Tiempo del último incremento de potencia
    unsigned int potenciaActualAtaque;    // Potencia actual de ataque

    unsigned long tiempoGiroInicio;  // Tiempo de inicio del giro
    const unsigned int potenciaGiro180 = 60;
    const unsigned long duracionGiro180 = 150; // Duración máxima del giro en milisegundos (ejemplo)

    unsigned long tiempoGiroAtaqueInicio;  // Tiempo de inicio del giro
    const unsigned int potenciaGiroAtaque = 100;
    const unsigned long duracionGiroAtaque = 20; // Duración máxima del giro en milisegundos (ejemplo)

    unsigned long tiempoRetrocesoInicio; // Tiempo de inicio de retroceso
    const unsigned int potenciaRetroceso = 40;
    const unsigned long duracionRetroceso = 200; // Duración del retroceso en milisegundos (ejemplo)

    const char* nombresEstados[14] = {
        "DISPARO INIT",
        "INICIO",
        "BUSQUEDA",
        "ALINEACION",
        "ROTACION ORBITAL",
        "AVANCE",
        "ATAQUE BAJO",
        "ATAQUE ALTO",
        "DETECCION DE LINEA",
        "RETROCESO",
        "GIRO DERECHO 180",
        "GIRO IZQUIERDO 180",
        "GIRO EN ATAQUE",
        "TALADRO"
    };
};

#endif
