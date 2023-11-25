#include <QLabel>
#include <QTimer>
#include <QList>
#include <QColor>
#include <QPushButton>
#include <QVBoxLayout>


class WigetSimonDice : public QLabel {
    Q_OBJECT

public:
    WigetSimonDice(QWidget *parent);
    QList<QColor> colors = {QColor("red"), QColor("green"), QColor("blue"), QColor("yellow")};
    
    void generateColorSequence();
    void generarSecMuestraa();
    void inicioSec();
    void startSecuencia();
    void resetVar();

    QTimer *timer;
    QList<QColor> secuencia;        //Secuencia de colores
    QList<QColor> secuenciaMuestra; //Secuencia a mostrar con los grises entre medio

    int currentColorIndex;          //Indice de color actual a mostrar
    int indexSec=0;                 //Indice de secuencia actual
    int largoSec=10;                 //Largo de secuencia
    int punt = 0;                   //Puntaje
    bool jugando = true;            //Verifica fin de juego
    int indexCreate= 0;             //Indice de creacion de secuencia

    
private slots:
    void changeBackground();
    void secuenciaColor();
    void machSecuencia(int);
    void reset();
    void changeLargoSecuencia(int);

signals: 
    void puntaje(int);
    void mensaje(QString);
    void blockButton(bool);
    void blockButtonColor(bool);
    void largoSecuencia(int);
};
