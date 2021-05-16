#ifndef MODELABELS_H
#define MODELABELS_H

#include <QObject>
#include <QWidget>
//#include <QtGui>
#include <QtWidgets> // was QT Gui

class ModeLabels : public QWidget
{
    Q_OBJECT
public:
    explicit ModeLabels(QWidget *mainWindowAddress, QVariantMap *modePresetValues, QWidget *parent = 0);

    QWidget *mainWindow;
    QVariantMap *presetValues;

    //ui stuff for parameter displaying on top of the QuNeo Background
    QLabel *modeDisplay;
    QString modeDisplayNote;
    QString modeDisplayNonNote;

    //these are for functions that reformat the displayed parameters (for notes and off states).
    QString noteNumberToName(int noteNumber);
    QString numberToOffState(int numberToOff);

    int modeDisplayValue;
    
signals:
    
public slots:

    void slotDisplayParameter(QString objectName, int componentNum, int preset);
    
};

#endif // MODELABELS_H
