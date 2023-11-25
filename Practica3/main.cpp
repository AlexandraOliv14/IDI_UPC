//main.cpp
#include <QApplication>
#include "Simon.h"

int main(int argc, char ** argv)
{
    QApplication app(argc, argv);
    Simon form;
    form.show();
    return app.exec();
}
