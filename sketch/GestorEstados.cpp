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

    bool* sensorE = detE.getEstados();
    bool* sensorL = detL.getEstados(0, 1000);

    switch (estadoActual) {
        case INICIO:
            if (sensorL[0] || sensorL[1]) {
                estadoActual = DETECCION_LINEA;
            }else if (sensorE[2] == 1 || sensorE[3] == 1 || sensorE[0] == 1 || sensorE[1] == 1 || sensorE[4] == 1 || sensorE[5] == 1) {
                estadoActual = ALINEACION;
            } else if (sensorE[6] == 1) {
                estadoActual = GIRO_Y_ATAQUE;
            } else {
                estadoActual = BUSQUEDA;
                tiempoBusquedaInicio = millis(); // Inicializa el ciclo de búsqueda
            }
            break;

        case BUSQUEDA:
            if (millis() - tiempoBusquedaInicio < duracionBusqueda) {
                controlMov.adelante(20); // Mover hacia adelante
            } else if (millis() - tiempoBusquedaInicio < 2 * duracionBusqueda) {
                controlMov.atras(20); // Mover hacia atrás
            } else {
                tiempoBusquedaInicio = millis(); // Reiniciar el ciclo de búsqueda
            }
            
            //OBS: No se vuelve al estado INICIO porque el tiempo de busqueda podria reiniciarse 
            if (sensorL[0] || sensorL[1]) {
                estadoActual = DETECCION_LINEA;
            }else if (sensorE[2] == 1 || sensorE[3] == 1 || sensorE[0] == 1 || sensorE[1] == 1 || sensorE[4] == 1 || sensorE[5] == 1) {
                estadoActual = ALINEACION;
            } else if (sensorE[6] == 1) {
                estadoActual = GIRO_Y_ATAQUE;
            }
            break;

        case ALINEACION:
            if (sensorE[2] == 1 || sensorE[3] == 1) {
                estadoActual = AVANCE;
            } else if (sensorE[0] == 1 || sensorE[1] == 1) {
                controlMov.giroIzquierdo(15);
            } else if (sensorE[4] == 1 || sensorE[5] == 1) {
                controlMov.giroDerecho(15);
            } else {
                estadoActual = INICIO;
            }
            break;

        case AVANCE:
            if (sensorE[2] == 1 || sensorE[3] == 1) {
                controlMov.adelante(20);
                if (sensorE[2] == 1 && sensorE[3] == 1) {
                    estadoActual = ATAQUE_RAPIDO;
                }
            } else {
                estadoActual = ALINEACION;
            }
            break;

        case ATAQUE_RAPIDO:
            controlMov.adelante(25);
            if (!(sensorE[2] == 1 && sensorE[3] == 1)) {
                estadoActual = INICIO;
            }
            break;

        case DETECCION_LINEA:
            if (sensorL[0] && sensorL[1]) {
                controlMov.atras(20);
                //delay(500);
            } else if (sensorL[0]) {
                controlMov.giroDerecho(20);
                //delay(500);
            } else if (sensorL[1]) {
                controlMov.giroIzquierdo(20);
                //delay(500);
            }else{
                estadoActual = INICIO;
            }
            break;

        case GIRO_Y_ATAQUE:
            controlMov.giroDerecho(20);
            //delay(1000); // Ajusta este valor según sea necesario para un giro de 180 grados
            controlMov.adelante(20); // Ataque rápido después de girar
            //delay(500); // Duración del ataque
            estadoActual = INICIO;
            break;
    }

    //delay(50);  
}

void GestorEstados::string(char* buffer, size_t bufferSize) {
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
