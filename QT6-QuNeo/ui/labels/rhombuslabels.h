#ifndef RHOMBUSLABELS_H
#define RHOMBUSLABELS_H

#include <QObject>
#include <QWidget>
//#include <QtGui>
#include <QtWidgets> // was QT Gui

class RhombusLabels : public QWidget
{
    Q_OBJECT
public:
    explicit RhombusLabels(QWidget *mainWindowAddress, QVariantMap *rhombusPresetValues, QWidget *parent = 0);

    QWidget *mainWindow;
    QVariantMap *presetValues;

    //ui stuff for parameter displaying on top of the QuNeo background
    QLabel *rhombusDisplay;
    QString rhombusDisplayNote;
    QString rhombusDisplayNonNote;

    //these are for functions that reformat the displayed parameters (for notes and off states)
    QString noteNumberToName(int noteNumber);
    QString numberToOffState(int numberToOff);

    int rhombusDisplayValue;
    
signals:
    
public slots:

    void slotDisplayParameter(QString objectName, int componentNum, int preset);
    
};

#endif // RHOMBUSLABELS_H
