#include "myqtapp.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyQtApp w;
    w.show();

    return a.exec();
}
