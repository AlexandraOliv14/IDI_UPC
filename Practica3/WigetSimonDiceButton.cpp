#include "WigetSimonDiceButton.h"

WigetSimonDiceButton::WigetSimonDiceButton(QWidget *parent) : QPushButton(parent) {
    connect(this, SIGNAL(clicked()), this, SLOT(textoComoEntero()));   // connect(this, SIGNAL(QPushButton::clicked()), this,SLOT(textoComoEntero()));

}

void WigetSimonDiceButton::textoComoEntero() {
    bool ok;
    QString number = this->text();
    int num= number.split("&").last().toInt(&ok);

    if (ok) {
        emit numeroMach(num); // Emitir la señal con el número si la conversión fue exitosa
    }
    // Aquí podrías manejar el caso de no convertir correctamente
}
