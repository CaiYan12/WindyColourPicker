#include "WindyColourPicker.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WindyColourPicker w;
    w.show();
    return a.exec();
}