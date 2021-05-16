#ifndef LSLIDEREDITPANE_H
#define LSLIDEREDITPANE_H

#include <QObject>
#include <QWidget>
//#include <QtGui>
#include <QtWidgets> // was QT Gui
#include "lsliderlabels.h"

class LSliderEditPane : public QWidget
{
    Q_OBJECT
public:
    explicit LSliderEditPane(QVariantMap *variantMap, QVariantMap *variantMapCopy, QWidget *widget, QWidget *parent = 0);
    bool eventFilter(QObject *, QEvent *);

    QWidget *mainWindow;
    QVariantMap *presetMap;
    QVariantMap *presetMapCopy;
    QDial *sensitivityDial;
    QWidget *focusCheck;
    QSpinBox *uiObject;
    QWidget *clicked;
    QString prevObjectToLabel;

    //these make preset saving work
    QVariantMap lSliderMap;
    QStringList lSliderParamList;
    QStringList lSliderUiBlacklist;
    QMap<QString, QSpinBox*> spinBoxMap; //map of spin box address by param name

    void findUiElements();
    void displayLSliderValues();

    int currentPreset;
    int lSliderDisplayValue;
    int currentBank;

    LSliderLabels *lSliderLabel[1];

    //pointers for velocity check boxes
    QCheckBox *lB1Velocity;
    QCheckBox *lB2Velocity;
    QCheckBox *lB3Velocity;
    QCheckBox *lB4Velocity;
    QCheckBox *localLEDControl;
    QComboBox *modeMenuB1;
    QComboBox *modeMenuB2;
    QComboBox *modeMenuB3;
    QComboBox *modeMenuB4;

    QSpinBox *ledInputChannel;
    QSpinBox *onThresh;
    QSpinBox *offThresh;

signals:
    void signalValueChanged(int presetNum, QString settingsType, QString paramType, int typeNum, QString paramName, int paramValue);
    void signalToLabels(QString);
    void signalTriggerCopy();
    void signalTriggerPaste();

public slots:

    void slotEvents(QString);
    void slotValueChanged(int);
    void slotRecallPreset(int);
    void slotFocusParam(QWidget* oldObject, QWidget* nowObject);
    void slotToLabels(QString);
    void slotCalculateOffThreshFromOn(int onThreshChanged);
    void slotCheckOffThreshAgainstOn(int offThreshChanged);
};

#endif // LSLIDEREDITPANE_H
