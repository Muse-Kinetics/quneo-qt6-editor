//#include <QtGui/QApplication>
#include <QtWidgets/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{

#ifdef Q_OS_WIN
    double scale = 1.0;
    std::string scaleAsString = std::to_string(scale);
    QByteArray scaleAsQByteArray(scaleAsString.c_str(), (int)scaleAsString.length());
    qputenv("QT_SCALE_FACTOR", scaleAsQByteArray);
    qputenv("QT_SCALE_FACTOR_ROUNDING_POLICY", "Round");
#endif

    QApplication a(argc, argv);
    MainWindow w;


    #ifdef Q_OS_MAC
    w.setMinimumHeight(678);
    w.setMinimumWidth(1080);
    w.setMaximumHeight(678);
    w.setMaximumWidth(1080);
    #else
    w.setMinimumHeight(678);
    w.setMinimumWidth(1080);
    w.setMaximumHeight(678);
    w.setMaximumWidth(1080);
    #endif

    w.show();

    return a.exec();
}
