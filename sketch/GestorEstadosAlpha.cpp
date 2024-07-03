#include "GestorEstadosAlpha.h"

GestorEstadosAlpha::GestorEstadosAlpha(ControlMovimiento& controlMov, DetectorEnemigo& detE, DetectorLinea& detL)
    : controlMov(controlMov), detE(detE), detL(detL), estadoActual(INICIO), tiempoBusquedaInicio(0), tiempoGiroInicio(0), tiempoDeteccionLineaInicio(0) {}

void GestorEstadosAlpha::begin() {
    detE.begin();
    detL.begin();
    controlMov.begin();
}

void GestorEstadosAlpha::stop() {
    estadoActual = INICIO;
    controlMov.stop(60);
}

void GestorEstadosAlpha::update() {
    detE.update();
    detL.update();

    bool* sensorE = detE.getEstados();
    bool* sensorL = detL.getEstados(0, 1000);

    switch (estadoActual) {
        case INICIO:
            if (sensorL[1] || sensorL[1]) {
                estadoActual = DETECCION_DE_LINEA;
                tiempoDeteccionLineaInicio = millis(); // Inicializar el tiempo de detección de línea
            }else if (sensorE[2] == 1 || sensorE[3] == 1 || sensorE[0] == 1 || sensorE[1] == 1 || sensorE[4] == 1 || sensorE[5] == 1) {
                estadoActual = ALINEACION;
            } else if (sensorE[6] == 1) {
                estadoActual = GIRO_180;
                tiempoGiroInicio = millis(); // Inicializar el tiempo de giro
            } else {
                estadoActual = BUSQUEDA;
                tiempoBusquedaInicio = millis(); // Inicializar el ciclo de búsqueda
            }
            break;

        case BUSQUEDA:
            if (millis() - tiempoBusquedaInicio < duracionLoop) {
                controlMov.adelante(potenciaAdBusc); // Mover hacia adelante
            } else if (millis() - tiempoBusquedaInicio < 2 * duracionLoop) {
                controlMov.atras(potenciaAtBusc); // Mover hacia atrás
            } else {
                tiempoBusquedaInicio = millis(); // Reiniciar el ciclo de búsqueda
            }
            
            if (sensorL[0] || sensorL[1]) {
                estadoActual = DETECCION_DE_LINEA;
                tiempoDeteccionLineaInicio = millis(); // Inicializar el tiempo de detección de línea
            } else if (sensorE[2] == 1 || sensorE[3] == 1 || sensorE[0] == 1 || sensorE[1] == 1 || sensorE[4] == 1 || sensorE[5] == 1) {
                estadoActual = ALINEACION;
            } else if (sensorE[6] == 1) {
                estadoActual = GIRO_180;
                tiempoGiroInicio = millis(); // Inicializar el tiempo de giro
            }
            break;

        case ALINEACION:
            if (sensorE[2] == 1 || sensorE[3] == 1) {
                estadoActual = AVANCE;
            } else if (sensorE[0] == 1 || sensorE[1] == 1) {
                controlMov.giroIzquierdo(potenciaGiro);
            } else if (sensorE[4] == 1 || sensorE[5] == 1) {
                controlMov.giroDerecho(potenciaGiro);
            } else {
                estadoActual = INICIO;
            }
            break;

        case AVANCE:
            if (sensorE[2] == 1 || sensorE[3] == 1) {
                controlMov.adelante(potenciaAdelante);
                if (sensorE[2] == 1 && sensorE[3] == 1) {
                    estadoActual = ATAQUE_RAPIDO;
                }
            } else {
                estadoActual = ALINEACION;
            }
            break;

        case ATAQUE_RAPIDO:
            controlMov.adelante(potenciaAdelanteMax);
            if (!(sensorE[2] == 1 && sensorE[3] == 1)) {
                estadoActual = INICIO;
            }
            break;

        case DETECCION_DE_LINEA:
            if (millis() - tiempoDeteccionLineaInicio < duracionDeteccionLinea) {
                if (sensorL[0] && sensorL[1]) {
                    controlMov.atras(20);
                    estadoActual = GIRO_180;
                    tiempoGiroInicio = millis(); // Inicializar el tiempo de giro
                } else if (sensorL[0]) {
                    estadoActual = GIRO_DERECHO_90;
                    tiempoGiroInicio = millis(); // Inicializar el tiempo de giro
                } else if (sensorL[1]) {
                    estadoActual = GIRO_IZQUIERDO_90;
                    tiempoGiroInicio = millis(); // Inicializar el tiempo de giro
                }
            } else {
                estadoActual = INICIO;
            }
            break;

        case GIRO_180:
            if (millis() - tiempoGiroInicio < duracionGiro180) {
                controlMov.giroDerecho(potenciaGiro);
            } else {
                estadoActual = INICIO;
            }
            break;

        case GIRO_DERECHO_90:
            if (millis() - tiempoGiroInicio < duracionGiro180/2) {
                controlMov.giroDerecho(potenciaGiro);
            } else {
                estadoActual = INICIO;
            }
            break;

        case GIRO_IZQUIERDO_90:
            if (millis() - tiempoGiroInicio < duracionGiro180/2) {
                controlMov.giroIzquierdo(potenciaGiro);
            } else {
                estadoActual = INICIO;
            }
            break;
    }
}

void GestorEstadosAlpha::string(char* buffer, size_t bufferSize) {
    bool* sensorE = detE.getEstados();
    bool* sensorL = detL.getEstados(0, 1000);

    char bufferSE[50];
    detE.string(bufferSE, sizeof(bufferSE));

    char bufferSL[50];
    detL.string(bufferSL, sizeof(bufferSL));

    char bufferMotores[50];
    controlMov.string(bufferMotores, sizeof(bufferMotores));

    size_t index = 0;
    index += snprintf(buffer + index, bufferSize - index, "-------------------------------\n");
    index += snprintf(buffer + index, bufferSize - index, "ESTADO: %s\n", nombresEstados[estadoActual]);
    index += snprintf(buffer + index, bufferSize - index, "SE: %s\n", bufferSE);
    index += snprintf(buffer + index, bufferSize - index, "SL: %s\n", bufferSL);
    index += snprintf(buffer + index, bufferSize - index, "MOTORES: %s\n", bufferMotores);
    index += snprintf(buffer + index, bufferSize - index, "-------------------------------\n\n");
    buffer[index] = '\0'; // Null terminator
}
