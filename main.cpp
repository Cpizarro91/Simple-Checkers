#include "checkers1.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Checkers1 w;
    w.show();

    return a.exec();
}
//main calls constructor in checkers1.cpp file, checkers1::checkers1
