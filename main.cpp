#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    MainWindow w;
    w.show();
    return a.exec();
}
