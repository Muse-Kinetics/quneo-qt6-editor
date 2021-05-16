#ifndef ROTARYLABELS_H
#define ROTARYLABELS_H

#include <QObject>
#include <QWidget>
//#include <QtGui>
#include <QtWidgets> // was QT Gui

class RotaryLabels : public QWidget
{
    Q_OBJECT
public:
    explicit RotaryLabels(QWidget *mainWindowAddress, QVariantMap *hSliderPresetValues, QWidget *parent = 0);

    QWidget *mainWindow;    //points to mainWindow for using findchild
    QVariantMap *presetValues;  //points to pad's presetMapCopy

    //ui stuff for parameter displaying on top of the QuNeo background
    QLabel *rotaryLabelBank;
    QLabel *rotaryLabelType;
    QLabel *rotaryLabelValue;
    QString rotaryDisplayNote;
    QString rotaryDisplayNonNote;
    QString rotaryDisplaySpeed;
    QString rotaryDisplayParamName;
    QString rotaryDisplayBank;
    QString rotaryDisplayType;

    //these are for functions that reformat the displayed parameters (for notes and off states and speed)
    QString noteNumberToName(int noteNumber);
    QString numberToOffState(int numberToOff);
    QString numberSpeedFloat(int numberSpeed);

    int rotaryDisplayValue;
    bool directionEnabled;
signals:

public slots:

    void slotDisplayParameter(QString objectName, int componentNum, int preset);

};

#endif // ROTARYLABELS_H
