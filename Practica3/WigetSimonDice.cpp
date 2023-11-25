#include "WigetSimonDice.h"
#include <QRandomGenerator>
#include <QPalette>
#include <QDebug>

WigetSimonDice::WigetSimonDice(QWidget *parent) : QLabel(parent) {
}

void WigetSimonDice::changeBackground() {
    if (currentColorIndex < secuenciaMuestra.size()) {
        QColor color = secuenciaMuestra[currentColorIndex];
        QString css = QString("QLabel { background-color: %1 ; }").arg(color.name());
        setStyleSheet(css);
        currentColorIndex++;
    } else {
        timer->stop();
        currentColorIndex = 0; // Reset para la próxima secuencia
        emit mensaje("Juegue");
    }
}

void WigetSimonDice::secuenciaColor(){
    resetVar();
    startSecuencia();
}

void WigetSimonDice::generateColorSequence() {
    QColor newColor;
    QColor lastColor = secuencia[indexCreate];

    do {
        int index = QRandomGenerator::global()->bounded(colors.size());
        newColor = colors[index];
    } while(newColor == lastColor);
    
    secuencia.append(newColor);
    indexCreate++;   
}

void WigetSimonDice::startSecuencia(){
    emit mensaje("Espere que termine la secuencia");

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(changeBackground()));

    emit blockButton(true);
    if(secuencia.size()>0){
        generateColorSequence();
    }else{
        inicioSec();
    }
    
    generarSecMuestraa();
    currentColorIndex = 0;

    timer->start(1000); // Establecer el intervalo a 1 segundo
}

void WigetSimonDice::inicioSec(){
    int index = QRandomGenerator::global()->bounded(colors.size());
    QColor newColor = colors[index];
    secuencia.append(newColor);
}

void WigetSimonDice::machSecuencia(int colorIndex){
    if(wins){
        if(secuencia[indexSec] == colors[colorIndex-1]){
                
                punt+=1;
                indexSec+=1;
                emit puntaje(punt);
                if(secuencia.size()==indexSec){
                    indexSec=0;
                    startSecuencia();

                }
            }else{
                wins=false;
                emit mensaje("Has perdido");
                emit blockButton(false);
            }
    }else{
        emit mensaje("Has perdido, juega una nueva partida para continuar");
        emit blockButton(false);
    }
    
}

void WigetSimonDice::resetVar(){
    secuencia={};
    secuenciaMuestra={};

    indexSec=0;
    punt = 0;
    wins=true;
    indexCreate= 0;
    emit puntaje(punt);
}

void WigetSimonDice::reset(){
    resetVar();
    emit blockButton(false);
    emit mensaje("Inicie partida para comenzar");
}

void WigetSimonDice::generarSecMuestraa(){
    secuenciaMuestra={};
    for (int i = 0; i < secuencia.size(); ++i) {
        secuenciaMuestra.append(secuencia[i]);
        secuenciaMuestra.append(QColor("grey"));
    }
}