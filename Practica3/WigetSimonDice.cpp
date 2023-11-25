#include "WigetSimonDice.h"
#include <QRandomGenerator>
#include <QPalette>
#include <QDebug>

WigetSimonDice::WigetSimonDice(QWidget *parent) : QLabel(parent) {
}

void WigetSimonDice::secuenciaColor(){                  //LLamada del boton inicio de partida
    resetVar();                         //Limpiar variables en caso de que se haya jugado antes
    emit blockButton(true);             //Bloquea boton de inicio de partida
    startSecuencia();                   //Iniciar secuencia
}

void WigetSimonDice::resetVar(){                        //Limpia varibales generales
    //Limpia secuencias
    secuencia={};
    secuenciaMuestra={};

    //Limpia variables de control
    indexSec=0;
    punt = 0;
    jugando=true;
    indexCreate= 0;
    
    emit blockButtonColor(false);       //Desbloquea botones
    emit puntaje(punt);                 //Reinicia puntaje
}

void WigetSimonDice::startSecuencia(){                  //Inicia secuencia
    emit mensaje("Espere que termine la secuencia");                        //Mensaje de espera animacion de secuencia
    emit blockButtonColor(true);                                            //Bloquea botones de acciones   
    timer = new QTimer(this);                                               //Crea timer   
    connect(timer, SIGNAL(timeout()), this, SLOT(changeBackground()));      //Conecta el timer con el cambio de color

    if(secuencia.size()>0){                                                 //Si la secuencia ya ha sido iniciada      
        generateColorSequence();
    }else{                                                                  //Si es la primera vez que se inicia la secuencia
        inicioSec();
    }
    
    generarSecMuestraa();                                                   //Genera la secuencia a mostrar (con los grises entre medio)
    currentColorIndex = 0;                                                  //Inicializa la secuencia a mostrar

    timer->start(1000);                                                     // Establecer el intervalo a 1 segundo
}

void WigetSimonDice::changeBackground() {               //Cambio de fondo Label
    if (currentColorIndex < secuenciaMuestra.size()) {                       //Verifica que no haya finalizado la secuencia
        QColor color = secuenciaMuestra[currentColorIndex];                  //Obtiene el color de la secuencia   
        QString css = QString("QLabel { background-color: %1 ; }").arg(color.name()); //Establece String de css
        setStyleSheet(css);                                                 //Manda css al Style Sheet                                  
        currentColorIndex++;                                                //Incrementa el index de la secuencia
    } else {
        timer->stop();                                                      //Detiene el timer
        currentColorIndex = 0;                                              //Reset para la próxima secuencia
        if(secuencia.size()>0){                                             //Verifica que la secuencia tenga valores (en caso de reinicio)
            emit mensaje("Juegue");                                         //Mensaje de inicio de juego (pulsacion de secuencia)             
        }
        emit blockButtonColor(false);                                       //Desbloquea botones de secuencia
    }
}

void WigetSimonDice::generateColorSequence() {          //Genera siguiente color de secuencia                  
    QColor newColor;                                                        //Inicializa color nuevo
    QColor lastColor = secuencia[indexCreate];                              //Obtiene el ultimo color de la secuencia

    do {
        int index = QRandomGenerator::global()->bounded(colors.size());     //Genera un color random
        newColor = colors[index];                                           //ColorNuevo = color random
    } while(newColor == lastColor);                                         //Mientras el color nuevo sea igual al ultimo color de la secuencia, buesca otro color
    
    secuencia.append(newColor);                                             //Agrega el color a la secuencia
    indexCreate++;                                                          //Incrementa el index de la secuencia creada   
}

void WigetSimonDice::inicioSec(){                       //Inicia la secuencia
    int index = QRandomGenerator::global()->bounded(colors.size());         //Genera un index ramdon dentro del largo de los colores establecidos
    QColor newColor = colors[index];                                        //Obtiene el color correspondiente al index generado
    secuencia.append(newColor);                                             //Agrega el color a la secuencia
}

void WigetSimonDice::generarSecMuestraa(){              //Genera secuencia a mostrar
    secuenciaMuestra={};                                                    //Limpia la secuencia a mostrar
    for (int i = 0; i < secuencia.size(); ++i) {                            //Recorre la secuencia
        secuenciaMuestra.append(secuencia[i]);                              //Agrega el color correspondiente de la secuencia
        secuenciaMuestra.append(QColor("grey"));                            //Agrega el color gris para intercalar en la secuencia
    }
}

void WigetSimonDice::machSecuencia(int colorIndex){     //Ve el mach de los botones con la secuencia
    if(jugando){                                                                        //Verifica que no haya perdido
        if(secuencia.size()==0){                                                        //Si no hay secuencia   
            emit mensaje("Bienvenido, inicie partida para comenzar");                   //Envia mendaje de inicio de juego
        }
        else if(secuencia[indexSec] == colors[colorIndex-1]){                           //Si hay secuencia, verifica el color del boton con la secuencia 
                
                punt+=1;                                                                //Incrementa el puntaje
                indexSec+=1;                                                            //Incrementa el index de la secuencia
                
                emit puntaje(punt);                                                     //Envia el puntaje obtenido
                if(secuencia.size()==indexSec){
                    if(secuencia.size()==largoSec){
                        jugando=false;                                                  //Establece la perdida del juego
                        emit mensaje("Has ganado");                                     //Envia mensaje de ganador
                        emit blockButton(false);                                        //Desbloquea boton de inicio de partida
                    }else{
                        indexSec=0;                                                     //Reinicia el index de la secuencia
                        startSecuencia(); 
                    }                                                                   //Comienza una nuva secuencia (agrega un color más)
                }
            }else{
                jugando=false;                                                          //Establece la perdida del juego
                emit mensaje("Has perdido");                                            //Envia mensaje de perdida
                emit blockButton(false);                                                //Desbloquea boton de inicio de partida
            }
    }else{
        emit mensaje("Juego terminado, juega una nueva partida para continuar");        //Envia mensaje de perdida y reinicio de juego
        emit blockButton(false);                                                        //Desbloquea boton de inicio de partida
    }
    
}

void WigetSimonDice::reset(){                           //Resetea el juego

    QString css = QString("QLabel { background-color: %1 ; }").arg(QColor("grey").name());      //Establece String de css con color de fondo gris
    setStyleSheet(css);                                                                         //Manda css al Style Sheet

    resetVar();                                                                                 //Limpia varibales generales
    emit blockButton(false);                                                                    //Desbloquea boton de inicio de partida
    emit mensaje("Bienvenido, inicie partida para comenzar");
    largoSec=10;
    emit largoSecuencia(largoSec);                                   //Envia mensaje de inicio de juego
}

void WigetSimonDice::changeLargoSecuencia(int largo){   //Cambia el largo de la secuencia
    largoSec=largo;                                     //Resetea el juego
}