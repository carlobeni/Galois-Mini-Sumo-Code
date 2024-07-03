#include "GestorEstadosAlpha.h"

GestorEstadosAlpha::GestorEstadosAlpha(ControlMovimiento& controlMov, DetectorEnemigo& detE, DetectorLinea& detL)
    : controlMov(controlMov), detE(detE), detL(detL), estadoActual(INICIO), tiempoAtaqueInicio(0), tiempoGiroInicio(0), tiempoRetrocesoInicio(0) {}

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
            } else if (sensorE[2] == 1 || sensorE[3] == 1 || sensorE[0] == 1 || sensorE[1] == 1 || sensorE[4] == 1 || sensorE[5] == 1) {
                estadoActual = ALINEACION;
            } else if (sensorE[6] == 1) {
                estadoActual = GIRO_DERECHO_180;
                tiempoGiroInicio = millis(); // Inicializar el tiempo de giro
            } else {
                estadoActual = BUSQUEDA;
            }
            break;

        case BUSQUEDA:
            controlMov.adelante(potenciaBusqueda);
            estadoActual = INICIO;
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
                controlMov.adelante(potenciaAvance);
                if (sensorE[2] == 1 && sensorE[3] == 1) {
                    estadoActual = ATAQUE;
                    tiempoAtaqueInicio = millis();
                }
            } else {
                estadoActual = ALINEACION;
            }
            break;

        case ATAQUE:
            if (millis() - tiempoAtaqueInicio < duracionLoopAtaque) {
                controlMov.adelante(potenciaActualAtaque); // Mover hacia adelante
            } else if (millis() - tiempoAtaqueInicio < 2 * duracionLoopAtaque) {
                controlMov.atras(potenciaAtaqueAtras); // Mover hacia atrás
            } else {
                tiempoAtaqueInicio = millis(); // Reiniciar el ciclo de ataque
            }

            if (millis() - tiempoUltimoIncremento > periodoIncrementoPotencia) {
                potenciaActualAtaque += incrementoPotenciaAtaque;
                if (potenciaActualAtaque > potenciaAtaqueAdelanteMax) {
                    potenciaActualAtaque = potenciaAtaqueAdelanteMin; // Reiniciar a potencia mínima
                }
                tiempoUltimoIncremento = millis(); // Reiniciar el tiempo del último incremento
            }

            if (sensorL[0] || sensorL[1]) {
                estadoActual = DETECCION_DE_LINEA;
                tiempoRetrocesoInicio = millis(); // Inicializar el tiempo de retroceso
            } else if (sensorE[2] == 1 || sensorE[3] == 1 || sensorE[0] == 1 || sensorE[1] == 1 || sensorE[4] == 1 || sensorE[5] == 1) {
                estadoActual = ALINEACION;
            } else if (sensorE[6] == 1) {
                estadoActual = GIRO_DERECHO_180;
                tiempoGiroInicio = millis(); // Inicializar el tiempo de giro
            }
            if (!(sensorE[2] == 1 && sensorE[3] == 1)) {
                estadoActual = INICIO;
            }
            break;

        case DETECCION_DE_LINEA:
            controlMov.stop(60);
            tiempoRetrocesoInicio = millis(); // Inicializar el tiempo de retroceso
            estadoActual = RETROCESO;
            break;

        case RETROCESO:
            if (millis() - tiempoRetrocesoInicio < duracionRetroceso) {
                controlMov.atras(potenciaRetroceso);
            } else {
                if (sensorL[0] && sensorL[1]) {
                    tiempoGiroInicio = millis();
                    estadoActual = GIRO_DERECHO_180;
                } else if (sensorL[0]) {
                    tiempoGiroInicio = millis();
                    estadoActual = GIRO_DERECHO_180;
                } else if (sensorL[1]) {
                    tiempoGiroInicio = millis();
                    estadoActual = GIRO_IZQUIERDO_180;
                } else {
                    estadoActual = INICIO;
                }
            }
            break;

        case GIRO_DERECHO_180:
            if (millis() - tiempoGiroInicio < duracionGiro180) {
                controlMov.giroDerecho(potenciaGiro180);
            } else {
                estadoActual = INICIO;
            }
            break;

        case GIRO_IZQUIERDO_180:
            if (millis() - tiempoGiroInicio < duracionGiro180) {
                controlMov.giroIzquierdo(potenciaGiro180);
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
