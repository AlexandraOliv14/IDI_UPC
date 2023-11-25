#include <QPushButton>


class WigetSimonDiceButton : public QPushButton {
    Q_OBJECT

public:
    WigetSimonDiceButton(QWidget *parent);

public slots:
    void textoComoEntero();

signals: 
    void numeroMach(int valor);
};