#ifndef GESTOR_ESTADOS_H
#define GESTOR_ESTADOS_H

#include "ControlMovimiento.h"
#include "DetectorEnemigo.h"
#include "DetectorLinea.h"

enum Estado {
        INICIO,
        BUSQUEDA,
        ADELANTE,
        ADELANTE_RAPIDO,
        ATRAS,
        GIRO_IZQUIERDA,
        TALADRO,
        GIRO_DERECHA
};

class GestorEstados {
public:
    GestorEstados(ControlMovimiento& controlMov, DetectorEnemigo& detE, DetectorLinea& detL);
    void begin();
    void update();
    void stop();
    void string(char* buffer, size_t bufferSize);

private:
    ControlMovimiento& controlMov;
    DetectorEnemigo& detE;
    DetectorLinea& detL;
    Estado estadoActual;
    Estado estadoAnterior;
    int bandera = 0;
    unsigned long tiempo;
    const unsigned long duracionEsperaEnemigo = 5000; 
    const unsigned long duracionBusqueda = 1000; 
    const unsigned int velocidadAdelanteRapido = 40;      
    const unsigned int velocidadAdelanteNormal = 20;      
    const unsigned int velocidadAtras = 30;               
    const unsigned int velocidadGiro = 20;
    const char* nombresEstados[8] = {
        "INICIO",
        "BUSQUEDA",
        "ADEALNTE",
        "ADEALNTE_RAPIDO",
        "ATRAS",
        "TALADRO",
        "GIRO_IZQUIERDA",
        "GIRO_DERECHA"
    };
};

#endif