#ifndef LEFTRIGHTLABELS_H
#define LEFTRIGHTLABELS_H

#include <QObject>
#include <QWidget>
//#include <QtGui>
#include <QtWidgets> // was QT Gui

class LeftrightLabels : public QWidget
{
    Q_OBJECT
public:
    explicit LeftrightLabels(QWidget *mainWindowAddress, QVariantMap *leftrightPresetValues, QWidget *parent = 0);

    QWidget *mainWindow;
    QVariantMap *presetValues;

    //ui stuff for parameter displaying on top of the QuNeo background
    QLabel *leftrightLDisplay;
    QLabel *leftrightRDisplay;
    QString leftrightDisplayNote;
    QString leftrightDisplayNonNote;

    //these are for functions that reformat the displayed parameters (for notes and off states)
    QString noteNumberToName(int noteNumber);
    QString numberToOffState(int numberToOff);

    int leftrightDisplayValue;
    int leftrightRDisplayValue;
    int leftrightLDisplayValue;
    
signals:
    
public slots:

    void slotDisplayParameter(QString objectName, int componentNum, int preset);
    
};

#endif // LEFTRIGHTLABELS_H
