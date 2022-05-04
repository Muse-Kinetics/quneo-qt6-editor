//#include <QtGui/QApplication>
#include <QtWidgets/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{

    Q_INIT_RESOURCE(Resources);


    // set highdpi scale factor rounding
//    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#ifdef Q_OS_IOS
    // up-scale the app for iOS
//    QApplication* temp = new QApplication(argc, argv);
//    QScreen *screen = QGuiApplication::primaryScreen();
//    QRect  screenGeometry = screen->availableGeometry();
//    double width = screenGeometry.width();
//    // assumes that the default desktop resolution is 720p (scale of 1)
//    int minWidth = 1080;
//    delete temp;
//
//    double scale = width / minWidth;
    //qDebug() << "scale: " << scale;
    double scale = 1.1;
    std::string scaleAsString = std::to_string(scale);
    QByteArray scaleAsQByteArray(scaleAsString.c_str(), (int)scaleAsString.length());
    qputenv("QT_SCALE_FACTOR", scaleAsQByteArray);
    //qputenv("QT_SCALE_FACTOR", "2");
    qputenv("QT_SCALE_FACTOR_ROUNDING_POLICY", "Round");
#endif

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
#ifndef Q_OS_IOS
    w.setMinimumHeight(678);
    w.setMinimumWidth(1080);
    w.setMaximumHeight(678);
    w.setMaximumWidth(1080);
#endif
    #else
    w.setMinimumHeight(678);
    w.setMinimumWidth(1080);
    w.setMaximumHeight(678);
    w.setMaximumWidth(1080);
    #endif

    w.show();

    return a.exec();
}
