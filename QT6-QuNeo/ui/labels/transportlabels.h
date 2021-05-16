#ifndef TRANSPORTLABELS_H
#define TRANSPORTLABELS_H

#include <QObject>
#include <QWidget>
//#include <QtGui>
#include <QtWidgets> // was QT Gui

class TransportLabels : public QWidget
{
    Q_OBJECT
public:
    explicit TransportLabels(QWidget *mainWindowAddress, QVariantMap *transportPresetValues, QWidget *parent = 0);

    QWidget *mainWindow;
    QVariantMap *presetValues;

    //ui stuff for parameter displaying on top of the QuNeo background
    QLabel *transportDisplay;
    QString transportDisplayNote;
    QString transportDisplayNonNote;

    //these are for functions that reformat the displayed parameters (for notes and off states)
    QString noteNumberToName(int noteNumber);
    QString numberToOffState(int numberToOff);

    int transportDisplayValue;
    
signals:
    
public slots:

    void slotDisplayParameter(QString objectName, int componentNum, int preset);
    
};

#endif // TRANSPORTLABELS_H
