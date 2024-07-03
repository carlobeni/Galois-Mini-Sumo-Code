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
    ATAQUE_RAPIDO,
    DETECCION_DE_LINEA,
    GIRO_180,
    GIRO_IZQUIERDO_90,
    GIRO_DERECHO_90
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
    unsigned long tiempoBusquedaInicio;            
    unsigned long tiempoGiroInicio;  // Tiempo de inicio del giro
    unsigned long tiempoDeteccionLineaInicio; // Tiempo de inicio de detección de línea
    const unsigned int potenciaGiro = 20;
    const unsigned int potenciaAdBusc = 20;
    const unsigned int potenciaAtBusc = 15;
    const unsigned int potenciaAdelante = 15;
    const unsigned int potenciaAdelanteMax = 30;
    const unsigned long duracionLoop = 150; // Duración de cada movimiento de búsqueda (en milisegundos)
    const unsigned long duracionGiro180 = 200; // Duración máxima del giro en milisegundos (ejemplo)
    const unsigned long duracionDeteccionLinea = 50; // Duración máxima de la detección de línea en milisegundos (ejemplo)
    const char* nombresEstados[9] = {
        "INICIO",
        "BUSQUEDA",
        "ALINEACION",
        "AVANCE",
        "ATAQUE RAPIDO",
        "DETECCION DE LINEA",
        "GIRO DE 180",
        "GIRO IZQUIERDO 90",
        "GIRO DERECHO 90"
    };
};

#endif
