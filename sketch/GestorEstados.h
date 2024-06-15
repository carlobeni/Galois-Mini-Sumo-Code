#ifndef GESTOR_ESTADOS_H
#define GESTOR_ESTADOS_H

#include "ControlMovimiento.h"
#include "DetectorEnemigo.h"
#include "DetectorLinea.h"

enum Estado {
    INICIO,
    BUSQUEDA,
    ALINEACION,
    AVANCE,
    ATAQUE_RAPIDO,
    DETECCION_LINEA,
    GIRO_Y_ATAQUE
};

class GestorEstados {
public:
    GestorEstados(ControlMovimiento& controlMov, DetectorEnemigo& detE, DetectorLinea& detL);
    void begin();
    void update();

private:
    ControlMovimiento& controlMov;
    DetectorEnemigo& detE;
    DetectorLinea& detL;
    Estado estadoActual;
    unsigned long tiempoBusquedaInicio;
    const unsigned long duracionBusqueda = 1000; // Duración de cada movimiento de búsqueda (en milisegundos)
};

#endif
