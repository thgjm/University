#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.resize(700, 550);
    w.setWindowIcon(QIcon(":/icons/resources/icons/timerIcon.png"));
    w.setWindowTitle("Smart Timer");
    w.show();
    return a.exec();
}
