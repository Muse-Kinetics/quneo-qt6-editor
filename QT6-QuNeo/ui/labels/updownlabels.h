#ifndef UPDOWNLABELS_H
#define UPDOWNLABELS_H

#include <QObject>
#include <QWidget>
//#include <QtGui>
#include <QtWidgets> // was QT Gui

class UpdownLabels : public QWidget
{
    Q_OBJECT
public:
    explicit UpdownLabels(QWidget *mainWindowAddress, QVariantMap *updownPresetValues, QWidget *parent = 0);

    QWidget *mainWindow;
    QVariantMap *presetValues;

    //ui stuff for parameter displaying on top of the QuNeo background
    QLabel *updownUDisplay;
    QLabel *updownDDisplay;
    QString updownDisplayNote;
    QString updownDisplayNonNote;

    //these are for functions that reformat the displayed parameters (for notes and off states)
    QString noteNumberToName(int noteNumber);
    QString numberToOffState(int numberToOff);

    int updownDisplayValue;
    int updownUDisplayValue;
    int updownDDisplayValue;
    
signals:
    
public slots:

    void slotDisplayParameter(QString objectName, int componentNum, int preset);

};

#endif // UPDOWNLABELS_H
