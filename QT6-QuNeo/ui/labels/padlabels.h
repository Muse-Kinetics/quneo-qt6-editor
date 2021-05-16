#ifndef PADLABELS_H
#define PADLABELS_H

#include <QObject>
#include <QWidget>
//#include <QtGui>
#include <QtWidgets> // was QT Gui

class PadLabels : public QWidget
{
    Q_OBJECT
public:
    explicit PadLabels(QWidget *mainWindowAddress, QVariantMap *padPresetValues, QWidget *parent = 0);

    QWidget *mainWindow;    //points to mainWindow for using findchild
    QVariantMap *presetValues;  //points to pad's presetMapCopy

    //holds the string to be displayed on padDisplayLabel in the middle for both grid and drum modes.
    QString padParamName;
    //holds a copy of the object name which is then parsed to get the values for that object on each corner in grid mode.
    QString padGridString;

    QWidget *padDisplayWidget; //container for labels on each pad
    //pointers to the pad display qlabels
    //4 for grid mode corners param values
    QLabel *padNWDisplay;
    QLabel *padNEDisplay;
    QLabel *padSWDisplay;
    QLabel *padSEDisplay;

    //padDisplayLabel holds the parameter type name for both modes.
    QLabel *padDisplayLabel;
    //padDmDisplay holds current drum mode param value
    QLabel *padDmDisplay;
    //padDisplayLow holds 3rd CC parameter in drum mode
    QLabel *padDisplayLow;

    //holds values to be displayed on labels above.
    int padDisplayValue;
    int padDisplayValueNW;
    int padDisplayValueNE;
    int padDisplayValueSW;
    int padDisplayValueSE;
    int gridMode; //is current pad in grid mode or not?
    int presetNumber;
    int componentNumber;
    int xVal;
    int yVal;
    int pressVal;
    QString NoteNumberToName(int noteNumber);   //function for note number to note name
    QString numberToOffState(int numberToOff);
    QString numberToLatchState(int numberToOff);

signals:

public slots:

    void slotDisplayParameter(QString objectName, int componentNum, int preset);
    void slotDisplayGrid(QString objectName);
    void slotDisplayChannel(void);
    void slotDisplayDrum(QString objectName);
    void slotDisplayTranspose(QString objectName, int componentNum, int preset);

};

#endif // PADLABELS_H
