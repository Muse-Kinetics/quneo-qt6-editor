#ifndef MODEEDITPANE_H
#define MODEEDITPANE_H

#include <QObject>
#include <QWidget>
//#include <QtGui>
#include <QtWidgets> // was QT Gui
#include "modelabels.h"

class ModeEditPane : public QWidget
{
    Q_OBJECT
public:
    explicit ModeEditPane(QVariantMap *variantMap, QVariantMap *variantMapCopy, QWidget *mainWindowAddress, QWidget *parent = 0);

    QWidget *mainWindow;
    QVariantMap *presetMap;
    QVariantMap *presetMapCopy;
    QWidget *focusCheck;
    QDial *sensitivityDial;
    QCheckBox *enableMIDI;
    QWidget *clicked;
    QString prevObjectToLabel;

    void findUiElements();
    void displayModeValues();

    int currentPreset;
    int currentMode;

    QVariantMap modeMap;
    QStringList modeParamList; //list of parameter names
    QStringList modeUiBlacklist;
    QMap<QString, QSpinBox*> spinBoxMap; //map of spin box address by param name in modeParamList

    ModeLabels *modeLabel[1];

    //pointers for velocity check boxes
    QCheckBox *modeVelocity;
    QCheckBox *localLEDControl;

signals:
    void signalValueChanged(int presetNum, QString settingsType, QString paramType, int typeNum, QString paramName, int paramValue);
    void signalToLabels(QString);
    
public slots:
    void slotEvents(QString);
    void slotRecallPreset(int);
    void slotValueChanged(int);
    void slotEnable(int);
    void slotToLabels(QString);
    void slotFocusParam(QWidget* oldObject, QWidget* nowObject);
};

#endif // MODEEDITPANE_H
