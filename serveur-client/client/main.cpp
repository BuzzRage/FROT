#include "clientfrm.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ClientFrm w;
    w.show();

    return a.exec();
}
