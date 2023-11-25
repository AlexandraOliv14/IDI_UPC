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
    QList<QColor> secuencia;
    QList<QColor> secuenciaMuestra;

    int currentColorIndex;
    int indexSec=0, largoSec=5;
    int punt = 0;
    bool wins = true;
    int indexCreate= 0;

    
private slots:
    void changeBackground();
    void secuenciaColor();
    void machSecuencia(int);
    void reset();

signals: 
    void puntaje(int);
    void fallo(bool);
    void mensaje(QString);
    void blockButton(bool);
};
