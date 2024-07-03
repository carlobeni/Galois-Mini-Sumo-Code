#include "GestorEstados.h"

GestorEstados::GestorEstados(ControlMovimiento& controlMov, DetectorEnemigo& detE, DetectorLinea& detL)
    : controlMov(controlMov), detE(detE), detL(detL), estadoActual(INICIO), estadoAnterior(INICIO), tiempo(0) {}

void GestorEstados::begin() {
    detE.begin();
    detL.begin();
    controlMov.begin();
}

void GestorEstados::stop() {
     estadoActual = INICIO;
     controlMov.stop(60);
}

void GestorEstados::update() {
    detE.update();
    detL.update();

    bool* sensorE = detE.getEstados();
    bool* sensorL = detL.getEstados(0, 1000);

    switch (estadoActual) {
        case INICIO:
            controlMov.adelante(0);
            if (sensorE[2] == 1 || sensorE[3] == 1){
                estadoActual = ADELANTE;
                estadoAnterior = INICIO;
                tiempo = millis();
            }

            else if (sensorE[0] == 1 || sensorE[1] == 1 && sensorE[2] == 0 && sensorE[3] == 0){
                estadoActual = GIRO_IZQUIERDA;
                estadoAnterior = INICIO;
                tiempo = millis();
            }  

            else if (sensorE[4] == 1 || sensorE[5] == 1 && sensorE[2] == 0 && sensorE[3] == 0){
                estadoActual = GIRO_DERECHA;
                estadoAnterior = INICIO;
                tiempo = millis();
            }

            else if (millis() - tiempo > 10000 ){
                estadoActual = BUSQUEDA;
                estadoAnterior = INICIO;
                tiempo = millis();
            }
        break;

        case ADELANTE:
            controlMov.adelante(40);

            if (sensorL[0] == 1 || sensorL[1] == 1){
                estadoActual = ATRAS;
                estadoAnterior = ADELANTE;
            }

            else if (sensorE[0] == 0 && sensorE[1] == 0 && sensorE[2] == 0 && sensorE[3] == 0 && sensorE[4] == 0 && sensorE[5] == 0){
                estadoActual = BUSQUEDA;
                estadoAnterior = ADELANTE;
                tiempo = millis();
            }

            else if ((sensorE[0] == 1 || sensorE[1] == 1) && sensorE[2] == 0 && sensorE[3] == 0){
                estadoActual = GIRO_IZQUIERDA;
                estadoAnterior = ADELANTE;
                tiempo = millis();
            }  

            else if ((sensorE[4] == 1 || sensorE[5] == 1) && sensorE[2] == 0 && sensorE[3] == 0 ){
                estadoActual = GIRO_DERECHA;
                estadoAnterior = ADELANTE;
                tiempo = millis();
            }

            else if (millis() - tiempo> 50000 ){
                estadoActual = ADELANTE_RAPIDO;
                estadoAnterior = ADELANTE;
                tiempo = millis();
            }            
        break;

        case ADELANTE_RAPIDO:
            controlMov.adelante(20);
            if (sensorL[0] == 1 && sensorL[1] == 1){
                estadoActual = ATRAS;
                estadoAnterior = ADELANTE_RAPIDO;
            }

            else if (sensorE[0] == 0 && sensorE[1] == 0 && sensorE[2] == 0 && sensorE[3] == 0 && sensorE[4] == 0 && sensorE[5] == 0){
                estadoActual = BUSQUEDA;
                estadoAnterior = ADELANTE_RAPIDO;
            }

            else if ((sensorE[0] == 1 || sensorE[1] == 1) && sensorE[4] == 0 && sensorE[5] == 0 && sensorE[6] == 0){
                estadoActual = GIRO_IZQUIERDA;
                estadoAnterior = ADELANTE_RAPIDO;
                tiempo = millis();
            }  

            else if ((sensorE[4] == 1 || sensorE[5] == 1) && sensorE[0] == 0 && sensorE[1] == 0 && sensorE[2] == 0){
                estadoActual = GIRO_DERECHA;
                estadoAnterior = ADELANTE_RAPIDO;
                tiempo = millis();
            }
        break;
      
            case GIRO_IZQUIERDA:

              controlMov.giroIzquierdo(velocidadGiro);
              
              if (sensorL[0] == 1 || sensorL[1] == 1){
                  estadoActual = ATRAS;
                  estadoAnterior = GIRO_IZQUIERDA;
              }
              
              else if (sensorE[2] == 1 && sensorE[3] == 1){
                  estadoActual = ADELANTE;
                  estadoAnterior = GIRO_IZQUIERDA;
                  tiempo = millis();
              }

              else if (sensorE[4] == 1 && sensorE[5] == 1){
                  estadoActual = GIRO_DERECHA;
                  estadoAnterior = GIRO_IZQUIERDA;
                  tiempo = millis();
              }

              else if (sensorE[0] == 0 && sensorE[1] == 0 && sensorE[2] == 0 && sensorE[3] == 0 && sensorE[4] == 0 && sensorE[5] == 0){
                estadoActual = BUSQUEDA;
                estadoAnterior = ADELANTE;
                tiempo = millis();
            }
        break;

            case GIRO_DERECHA:
            controlMov.giroDerecho(velocidadGiro);
            if (sensorL[0] == 1 || sensorL[1] == 1){
                estadoActual = ATRAS;
                estadoAnterior = GIRO_DERECHA;
            }

            else if (sensorE[2] == 1 && sensorE[3] == 1){
                estadoActual = ADELANTE;
                estadoAnterior = GIRO_DERECHA;
                tiempo = millis();
            }

            else if (sensorE[0] == 1 && sensorE[1] == 1){
                estadoActual = GIRO_IZQUIERDA;
                estadoAnterior = GIRO_DERECHA;
                tiempo = millis();
            }

            else if (sensorE[0] == 0 && sensorE[1] == 0 && sensorE[2] == 0 && sensorE[3] == 0 && sensorE[4] == 0 && sensorE[5] == 0){
                estadoActual = BUSQUEDA;
                estadoAnterior = ADELANTE;
                tiempo = millis();
            }

        break;

            case ATRAS:
            controlMov.atras(velocidadAtras);
            if(sensorL[0] == 0 && sensorL[1] == 0 && (estadoAnterior == ADELANTE || estadoAnterior == ADELANTE_RAPIDO)){
                estadoActual = GIRO_DERECHA;
                estadoAnterior = ATRAS;
                tiempo = millis();
            }
            
            else if(sensorL[0] == 0 && sensorL[1] == 0 && estadoAnterior == GIRO_IZQUIERDA){
                estadoActual = GIRO_DERECHA;
                estadoAnterior = ATRAS;
                tiempo = millis();
            }

            else if(sensorL[0] == 0 && sensorL[1] == 0){
                estadoActual = GIRO_IZQUIERDA;
                estadoAnterior = ATRAS;
                tiempo = millis();
            }
        break;


        case TALADRO:

            if (sensorL[0] == 1 || sensorL[1] == 1 && sensorE[0] == 0 && sensorE[1] == 0 && sensorE[2] == 0 && sensorE[3] == 0 && sensorE[4] == 0 && sensorE[5] == 0){
                estadoActual = ATRAS;
                estadoAnterior = TALADRO;
                tiempo = millis();
            }

            else if (sensorE[0] == 0 && sensorE[1] == 0 && (sensorE[2] == 1 && sensorE[3] == 1) && sensorE[4] == 0 && sensorE[5] == 0){
                estadoActual = ADELANTE;
                estadoAnterior = TALADRO;
                tiempo = millis();
            }

            else if (sensorE[0] == 0 && sensorE[1] == 0 && sensorE[2] == 0 && (sensorE[3] == 1 || sensorE[4] == 1 || sensorE[5] == 1)){
                estadoActual = GIRO_IZQUIERDA;
                estadoAnterior = TALADRO;
                tiempo = millis();
            }

            else if ((sensorE[0] == 1 && sensorE[1] == 1 && sensorE[2] == 1) && sensorE[3] == 0 && sensorE[4] == 0 && sensorE[5] == 0){
                estadoActual = GIRO_DERECHA;
                estadoAnterior = TALADRO;
                tiempo = millis();
            }


           else  if (millis() - tiempo < 500) {
                controlMov.adelante(velocidadAdelanteNormal); // Mover hacia adelante
            } 
            else if (millis() - tiempo < 500) {
                controlMov.atras(velocidadAtras); // Mover hacia atrás
            } 
            else {
                tiempo = millis(); // Reiniciar el ciclo de búsqueda
            }

            break;


        case BUSQUEDA:

            if (sensorL[0] == 1 || sensorL[1] == 1){
                estadoActual = ATRAS;
                estadoAnterior = BUSQUEDA;
                tiempo = millis();
            }


            else if (sensorE[2] == 1 || sensorE[3] == 1){
                estadoActual = ADELANTE;
                estadoAnterior = BUSQUEDA;
                tiempo = millis();
            }

            else if (sensorE[0] == 1 || sensorE[1] == 1 && sensorE[2] == 0 && sensorE[3] == 0){
                estadoActual = GIRO_IZQUIERDA;
                estadoAnterior = BUSQUEDA;
                tiempo = millis();
            }  

            else if (sensorE[4] == 1 || sensorE[5] == 1 && sensorE[2] == 0 && sensorE[3] == 0){
                estadoActual = GIRO_DERECHA;
                estadoAnterior = BUSQUEDA;
                tiempo = millis();
            }
           else  if (millis() - tiempo < 1000) {
                controlMov.adelante(velocidadAdelanteNormal); // Mover hacia adelante
            } 
            else if (millis() - tiempo < 2000) {
                controlMov.atras(velocidadAtras); // Mover hacia atrás
            } 
            else {
                tiempo = millis(); // Reiniciar el ciclo de búsqueda
            }

            break;

    }
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
