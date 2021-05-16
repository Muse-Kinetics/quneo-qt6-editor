#ifndef LSLIDERLABELS_H
#define LSLIDERLABELS_H

#include <QObject>
#include <QWidget>
//#include <QtGui>
#include <QtWidgets> // was QT Gui

class LSliderLabels : public QWidget
{
    Q_OBJECT
public:
    explicit LSliderLabels(QWidget *mainWindowAddress, QVariantMap *hSliderPresetValues, QWidget *parent = 0);

    QWidget *mainWindow;    //points to mainWindow for using findchild
    QVariantMap *presetValues;  //points to pad's presetMapCopy

    //ui stuff for parameter displaying on top of the QuNeo background
    QLabel *lSliderLabelBank;
    QLabel *lSliderLabelType;
    QLabel *lSliderLabelValue;
    QString lSliderDisplayNote;
    QString lSliderDisplayNonNote;
    QString lSliderDisplayParamName;
    QString lSliderDisplayBank;
    QString lSliderDisplayType;

    //these are for functions that reformat the displayed parameters (for notes and off states)
    QString noteNumberToName(int noteNumber);
    QString numberToOffState(int numberToOff);

    int lSliderDisplayValue;

signals:


public slots:

    void slotDisplayParameter(QString objectName, int componentNum, int preset);

};

#endif // LSLIDERLABELS_H
