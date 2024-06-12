#ifndef GESTOR_ESTADOS_H
#define GESTOR_ESTADOS_H

#include "ControlMovimiento.h"
#include "DetectorEnemigo.h"
#include "DetectorLinea.h"

class GestorEstados {
public:
    enum Estado { BUSQUEDA, ALINEACION, AVANCE, ATAQUE_RAPIDO, DETECCION_LINEA };
    
    GestorEstados(ControlMovimiento& controlMov, DetectorEnemigo& detE, DetectorLinea& detL)
        : controlMov(controlMov), detE(detE), detL(detL), estadoActual(BUSQUEDA) {}

    void update();

private:
    ControlMovimiento& controlMov;
    DetectorEnemigo& detE;
    DetectorLinea& detL;
    Estado estadoActual;
};

#endif