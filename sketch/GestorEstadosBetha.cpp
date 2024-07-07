#include "GestorEstadosBetha.h"

GestorEstadosBetha::GestorEstadosBetha(ControlMovimiento& controlMov, DetectorEnemigo& detE, DetectorLinea& detL)
    : controlMov(controlMov), detE(detE), detL(detL), estadoActual(DISPARO_INIT), tiempoAtaqueBajo(0),tiempoAtaqueAlto(0), tiempoGiroInicio(0),tiempoGiroAtaqueInicio(0), tiempoRetrocesoInicio(0) {}

void GestorEstadosBetha::begin() {
    detE.begin();
    detL.begin();
    controlMov.begin();
}

void GestorEstadosBetha::stop() {
    estadoActual = DISPARO_INIT;
    controlMov.stop(60);
}

void GestorEstadosBetha::update() {
    detE.update();
    detL.update();

    bool* sensorE = detE.getEstados();
    bool* sensorL = detL.getEstados(0, 1000);

    switch (estadoActual) {
        case DISPARO_INIT:
            controlMov.setBothMotorsSpeed(potenciaDisparoInit,potenciaDisparoInit);
            delay(duracionDisparoInit);
            estadoActual = INICIO;
            break;

        case INICIO:
            if (sensorL[0] || sensorL[1]) {
                estadoActual = DETECCION_DE_LINEA;
            } else if (sensorE[2] == 1 || sensorE[3] == 1 || sensorE[0] == 1 || sensorE[1] == 1 || sensorE[4] == 1 || sensorE[5] == 1) {
                estadoActual = ALINEACION; //ROTACION_ORB;
            } else if (sensorE[6] == 1) {
                estadoActual = GIRO_DERECHO_180;
                tiempoGiroInicio = millis(); // Inicializar el tiempo de giro
            } else {
                estadoActual = BUSQUEDA;
            }
            break;

        case BUSQUEDA:
            //Version para rotacion Orbital
            //estadoActual = ROTACION_ORB;

            //Version con alineacion
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

        case ROTACION_ORB:
            controlMov.setBothMotorsSpeed(-10,5);
            break;

        case AVANCE:
            if (sensorE[2] == 1 || sensorE[3] == 1) {
                controlMov.adelante(potenciaAvance);
                if (sensorE[2] == 1 && sensorE[3] == 1) {
                    estadoActual = ATAQUE_BAJO;
                    tiempoAtaqueBajo = millis();
                }
            } else {
                estadoActual = ALINEACION; //ROTACION_ORB;
            }
            break;

        case ATAQUE_BAJO:
            if (millis() - tiempoAtaqueBajo < duracionAtaqueBajo) {
                controlMov.adelante(potenciaAtaqueBajo); // Mover hacia adelante
            } else {
                estadoActual = ATAQUE_ALTO;
                tiempoAtaqueAlto = millis();
            }
            if (sensorL[0] || sensorL[1]) {
                estadoActual = DETECCION_DE_LINEA;
                tiempoRetrocesoInicio = millis(); // Inicializar el tiempo de retroceso
            } else if ((sensorE[2] == 0 && sensorE[3] == 0) && (sensorE[0] == 1 || sensorE[1] == 1 || sensorE[4] == 1 || sensorE[5] == 1)) {
                estadoActual = ALINEACION; //ROTACION_ORB;
            } else if (sensorE[6] == 1) {
                estadoActual = GIRO_DERECHO_180;
                tiempoGiroInicio = millis(); // Inicializar el tiempo de giro
            }
            if (!(sensorE[2] == 1 && sensorE[3] == 1)) {
                estadoActual = INICIO;
            }
            break;
        
        case ATAQUE_ALTO:
            if (millis() - tiempoAtaqueAlto < duracionAtaqueAlto) {
                controlMov.adelante(potenciaAtaqueAlto); // Mover hacia adelante
            } else {
                tiempoGiroAtaqueInicio = millis(); // Inicializar el tiempo de giro
                estadoActual = GIRO_EN_ATAQUE;
            }
            if (sensorL[0] || sensorL[1]) {
                estadoActual = DETECCION_DE_LINEA;
                tiempoRetrocesoInicio = millis(); // Inicializar el tiempo de retroceso
            } else if ((sensorE[2] == 0 && sensorE[3] == 0) && (sensorE[0] == 1 || sensorE[1] == 1 || sensorE[4] == 1 || sensorE[5] == 1)) {
                estadoActual = ALINEACION; //ROTACION_ORB;
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

        case GIRO_EN_ATAQUE:
            if (millis() - tiempoGiroAtaqueInicio < duracionGiroAtaque) {
                controlMov.giroAntihorarioAxisM1(potenciaGiroAtaque);
            } else {
                estadoActual = ATAQUE_BAJO;
            }
            break;

        case GIRO_DERECHO_180:
            if (millis() - tiempoGiroInicio < duracionGiro180 * 0.7) {
                controlMov.giroDerecho(potenciaGiro180);
            } else {
                controlMov.stop(100);
                if (millis() - tiempoGiroInicio >= duracionGiro180) {
                    estadoActual = INICIO;
                }
            }
            break;

        case GIRO_IZQUIERDO_180:
            if (millis() - tiempoGiroInicio < duracionGiro180 * 0.5) {
                controlMov.giroIzquierdo(potenciaGiro180);
            } else {
                controlMov.stop(100);
                if (millis() - tiempoGiroInicio >= duracionGiro180) {
                    estadoActual = INICIO;
                }
            }
            break;
    }
}

void GestorEstadosBetha::string(char* buffer, size_t bufferSize) {
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
