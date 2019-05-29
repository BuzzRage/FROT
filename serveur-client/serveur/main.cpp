#include "serverfrm.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ServerFrm w;
    w.show();

    return a.exec();
}
