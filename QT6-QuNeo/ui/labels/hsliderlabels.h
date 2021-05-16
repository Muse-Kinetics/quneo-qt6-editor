#ifndef HSLIDERLABELS_H
#define HSLIDERLABELS_H

#include <QObject>
#include <QWidget>
//#include <QtGui>
#include <QtWidgets> // was QT Gui

class HSliderLabels : public QWidget
{
    Q_OBJECT
public:
    explicit HSliderLabels(QWidget *mainWindowAddress, QVariantMap *hSliderPresetValues, QWidget *parent = 0);

    QWidget *mainWindow;    //points to mainWindow for using findchild
    QVariantMap *presetValues;  //points to pad's presetMapCopy

    //ui stuff for parameter displaying on top of the QuNeo background
    QLabel *hSliderLabelBank;
    QLabel *hSliderLabelType;
    QLabel *hSliderLabelValue;
    QString hSliderDisplayNonNote;
    QString hSliderDisplayParamName;
    QString hSliderDisplayBank;
    QString hSliderDisplayType;

    //these are for functions that reformat the displayed parameters (for notes and off states)
    QString noteNumberToName(int noteNumber);
    QString numberToOffState(int numberToOff);

    int hSliderDisplayValue;

signals:

public slots:

    void slotDisplayParameter(QString objectName, int componentNum, int preset);

};

#endif // HSLIDERLABELS_H
