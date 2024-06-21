#include "GestorEstados.h"

GestorEstados::GestorEstados(ControlMovimiento& controlMov, DetectorEnemigo& detE, DetectorLinea& detL)
    : controlMov(controlMov), detE(detE), detL(detL), estadoActual(INICIO), tiempoBusquedaInicio(0) {}

void GestorEstados::begin() {
    detE.begin();
    detL.begin();
    controlMov.begin();
}

void GestorEstados::update() {
    detE.update();
    detL.update();

    bool* estadosE = detE.getEstados();
    bool* estadosL = detL.getEstados(0, 1000);

    // Prioridad más alta: Detección de línea
    if (estadosL[0] || estadosL[1]) {
        estadoActual = DETECCION_LINEA;
    }

    switch (estadoActual) {
        case INICIO:
            controlMov.giroDerecho(100);
            if (estadosE[2] == 1 || estadosE[3] == 1 || estadosE[0] == 1 || estadosE[1] == 1 || estadosE[4] == 1 || estadosE[5] == 1) {
                estadoActual = ALINEACION;
            } else if (estadosE[6] == 1) {
                estadoActual = GIRO_Y_ATAQUE;
            } else {
                estadoActual = BUSQUEDA;
                tiempoBusquedaInicio = millis(); // Inicializa el ciclo de búsqueda
            }
            break;

        case BUSQUEDA:
            if (millis() - tiempoBusquedaInicio < duracionBusqueda) {
                controlMov.adelante(100); // Mover hacia adelante
            } else if (millis() - tiempoBusquedaInicio < 2 * duracionBusqueda) {
                controlMov.atras(100); // Mover hacia atrás
            } else {
                tiempoBusquedaInicio = millis(); // Reiniciar el ciclo de búsqueda
            }
            
            // Chequear si se detecta el enemigo durante la búsqueda
            if (estadosE[2] == 1 || estadosE[3] == 1 || estadosE[0] == 1 || estadosE[1] == 1 || estadosE[4] == 1 || estadosE[5] == 1) {
                estadoActual = ALINEACION;
            } else if (estadosE[6] == 1) {
                estadoActual = GIRO_Y_ATAQUE;
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
                estadoActual = INICIO;
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
                estadoActual = INICIO;
            }
            break;

        case DETECCION_LINEA:
            if (estadosL[0] && estadosL[1]) {
                controlMov.atras(150);
                delay(500);
                estadoActual = DETECCION_LINEA;
            } else if (estadosL[0]) {
                controlMov.giroDerecho(150);
                delay(500);
                estadoActual = DETECCION_LINEA;
            } else if (estadosL[1]) {
                controlMov.giroIzquierdo(150);
                delay(500);
                estadoActual = DETECCION_LINEA;
            }else{
                estadoActual = INICIO;
            }
            break;

        case GIRO_Y_ATAQUE:
            controlMov.giroDerecho(255);
            delay(1000); // Ajusta este valor según sea necesario para un giro de 180 grados
            controlMov.adelante(255); // Ataque rápido después de girar
            delay(500); // Duración del ataque
            estadoActual = INICIO;
            break;
    }

    //delay(50);  
}

void GestorEstados::string(char* buffer, size_t bufferSize) {
    bool* estadosE = detE.getEstados();
    bool* estadosL = detL.getEstados(0, 1000);

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
