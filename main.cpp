#include "MainWidget.h"
#include "patient.h"
#include <QApplication>
#include <QDebug>
#include <QString>

int main(int argc, char *argv[])
{
//    Patient tmp;
//    tmp.print_info();
//    qDebug()<<tmp.get_info();

    QApplication a(argc, argv);
    MainWidget t;
    t.show();

    return a.exec();
}
