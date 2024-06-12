#include "GestorEstados.h"

void GestorEstados::update() {
    detE.update();
    detL.update();

    bool* estadosE = detE.getEstados();
    bool* estadosL = detL.getEstados(0, 1000);

    if (estadosL[0] || estadosL[1]) {
        estadoActual = DETECCION_LINEA;
    }

    switch (estadoActual) {
        case BUSQUEDA:
            controlMov.giroDerecho(100);
            if (estadosE[0] == 1 || estadosE[1] == 1 || estadosE[2] == 1 || estadosE[3] == 1 || estadosE[4] == 1 || estadosE[5] == 1 || estadosE[6] == 1 || estadosE[7] == 1) {
                estadoActual = ALINEACION;
            }
            break;

        case ALINEACION:
            if (estadosE[2] == 1 || estadosE[3] == 1) {
                estadoActual = AVANCE;
            } else if (estadosE[0] == 1 || estadosE[1] == 1) {
                controlMov.giroIzquierdo(50);
            } else if (estadosE[4] == 1 || estadosE[5] == 1) {
                controlMov.giroDerecho(50);
            } else {
                estadoActual = BUSQUEDA;
            }
            break;

        case AVANCE:
            if (estadosE[2] == 1 || estadosE[3] == 1) {
                controlMov.adelante(150);
                if (estadosE[2] == 1 && estadosE[3] == 1) {
                    estadoActual = ATAQUE_RAPIDO;
                }
            } else {
                estadoActual = ALINEACION;
            }
            break;

        case ATAQUE_RAPIDO:
            controlMov.adelante(255);
            if (!(estadosE[2] == 1 && estadosE[3] == 1)) {
                estadoActual = BUSQUEDA;
            }
            break;

        case DETECCION_LINEA:
            if (estadosL[0] && estadosL[1]) {
                controlMov.atras(150);
                delay(500);
                estadoActual = BUSQUEDA;
            } else if (estadosL[0]) {
                controlMov.giroDerecho(150);
                delay(500);
                estadoActual = BUSQUEDA;
            } else if (estadosL[1]) {
                controlMov.giroIzquierdo(150);
                delay(500);
                estadoActual = BUSQUEDA;
            }
            break;
    }
}
