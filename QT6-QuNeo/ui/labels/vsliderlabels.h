#ifndef VSLIDERLABELS_H
#define VSLIDERLABELS_H

#include <QObject>
#include <QWidget>
//#include <QtGui>
#include <QtWidgets> // was QT Gui

class VSliderLabels : public QWidget
{
    Q_OBJECT
public:
    explicit VSliderLabels(QWidget *mainWindowAddress, QVariantMap *vSliderPresetValues, QWidget *parent = 0);

    QWidget *mainWindow;    //points to mainWindow for using findchild
    QVariantMap *presetValues;  //points to pad's presetMapCopy

    //ui stuff for parameter displaying on top of the QuNeo background
    QLabel *vSliderLabelBank;
    QLabel *vSliderLabelType;
    QLabel *vSliderLabelValue;
    QString vSliderDisplayNonNote;
    QString vSliderDisplayParamName;
    QString vSliderDisplayBank;
    QString vSliderDisplayType;

    //these are for functions that reformat the displayed parameters (for notes and off states)
    QString noteNumberToName(int noteNumber);
    QString numberToOffState(int numberToOff);

    int vSliderDisplayValue;

signals:

public slots:

    void slotDisplayParameter(QString objectName, int componentNum, int preset);

};

#endif // VSLIDERLABELS_H
