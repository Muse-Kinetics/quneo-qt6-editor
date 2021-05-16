#ifndef HSLIDEREDITPANE_H
#define HSLIDEREDITPANE_H

#include <QObject>
#include <QWidget>
//#include <QtGui>
#include <QtWidgets> // was QT Gui
#include "hsliderlabels.h"

class HSliderEditPane : public QWidget
{
    Q_OBJECT
public:
    explicit HSliderEditPane(QVariantMap *variantMap, QVariantMap *variantMapCopy, QWidget *widget, QWidget *parent = 0);
    bool eventFilter(QObject *, QEvent *);

    QWidget *mainWindow;
    QVariantMap *presetMap;
    QVariantMap *presetMapCopy;
    QLabel *paneLabel;
    QDial *sensitivityDial;
    QWidget *focusCheck;
    QSpinBox *uiObject;
    QWidget *clicked;
    QString prevObjectToLabel;
    QComboBox *modeMenuB1;
    QComboBox *modeMenuB2;
    QComboBox *modeMenuB3;
    QComboBox *modeMenuB4;

    //these make preset saving work
    QVariantMap hSliderMap;
    QStringList hSliderParamList;//list of parameter names
    QStringList hSliderUiBlackList;
    QMap<QString, QSpinBox*> spinBoxMap; //map of spin box address by param name in padParamList

    void findUiElements();
    void displayHSliderValues();

    int currentPreset;
    int currentHSlider;
    int currentBank;

    HSliderLabels *hSliderLabel[4];

    //pointers for velocity check boxes
    QCheckBox *hB1Velocity;
    QCheckBox *hB2Velocity;
    QCheckBox *hB3Velocity;
    QCheckBox *hB4Velocity;
    QCheckBox *localLEDControl;

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
    void slotToLabels(QString);
    void slotFocusParam(QWidget* oldObject, QWidget* nowObject);
    void slotCalculateOffThreshFromOn(int onThreshChanged);
    void slotCheckOffThreshAgainstOn(int offThreshChanged);
};

#endif // HSLIDEREDITPANE_H
