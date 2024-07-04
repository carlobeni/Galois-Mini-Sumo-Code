#ifndef GESTOR_ESTADOS_ALPHA_H
#define GESTOR_ESTADOS_ALPHA_H

#include "ControlMovimiento.h"
#include "DetectorEnemigo.h"
#include "DetectorLinea.h"

enum Estado {
    INICIO,
    BUSQUEDA,
    ALINEACION,
    AVANCE,
    ATAQUE,
    DETECCION_DE_LINEA,
    RETROCESO,
    GIRO_DERECHO_180,
    GIRO_IZQUIERDO_180
};

class GestorEstadosAlpha {
public:
    GestorEstadosAlpha(ControlMovimiento& controlMov, DetectorEnemigo& detE, DetectorLinea& detL);
    void begin();
    void update();
    void stop();
    void string(char* buffer, size_t bufferSize);

private:
    ControlMovimiento& controlMov;
    DetectorEnemigo& detE;
    DetectorLinea& detL;
    Estado estadoActual;

    const unsigned int potenciaBusqueda = 15;
    const unsigned int potenciaGiro = 20;
    const unsigned int potenciaAvance = 20;

    unsigned long tiempoAtaqueInicio;
    const unsigned int potenciaAtaqueAdelanteMax = 150;
    const unsigned int potenciaAtaqueAdelanteMin = 20;
    const unsigned int potenciaAtaqueAtras = 25;
    const unsigned long duracionLoopAtaque = 150;
    const unsigned int incrementoPotenciaAtaque = 10;
    const unsigned long periodoIncrementoPotencia = 4000; // Periodo de incremento de potencia en milisegundos

    unsigned long tiempoUltimoIncremento; // Tiempo del último incremento de potencia
    unsigned int potenciaActualAtaque;    // Potencia actual de ataque

    unsigned long tiempoGiroInicio;  // Tiempo de inicio del giro
    const unsigned int potenciaGiro180 = 60;
    const unsigned long duracionGiro180 = 150; // Duración máxima del giro en milisegundos (ejemplo)

    unsigned long tiempoRetrocesoInicio; // Tiempo de inicio de retroceso
    const unsigned int potenciaRetroceso = 40;
    const unsigned long duracionRetroceso = 500; // Duración del retroceso en milisegundos (ejemplo)

    const char* nombresEstados[10] = {
        "INICIO",
        "BUSQUEDA",
        "ALINEACION",
        "AVANCE",
        "ATAQUE",
        "DETECCION DE LINEA",
        "RETROCESO",
        "GIRO DERECHO 180",
        "GIRO IZQUIERDO 180"
    };
};

#endif
