#include "qtTCP.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qtTCP w;
    w.show();
    return a.exec();
}
