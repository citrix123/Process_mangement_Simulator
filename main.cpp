#include <QtGui/QApplication>
#include "os_sim.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    OS_Sim w;
    w.show();

    return a.exec();
}
