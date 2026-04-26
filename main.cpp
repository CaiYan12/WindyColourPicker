#include "WindyColourPicker.h"
#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/favicon.png"));
    WindyColourPicker w;
    w.show();
    return a.exec();
}