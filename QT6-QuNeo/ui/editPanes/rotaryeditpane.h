#ifndef ROTARYEDITPANE_H
#define ROTARYEDITPANE_H

#include <QObject>
#include <QWidget>
//#include <QtGui>
#include <QtWidgets> // was QT Gui
#include <rotarylabels.h>

class RotaryEditPane : public QWidget
{
    Q_OBJECT
public:
    explicit RotaryEditPane(QVariantMap *variantMap, QVariantMap *variantMapCopy, QWidget *mainWindowAddress, QWidget *parent = 0);
    bool eventFilter(QObject *, QEvent *);

    QWidget *mainWindow;
    QVariantMap *presetMap;
    QVariantMap *presetMapCopy;
    QLabel *paneLabel;
    QDial *sensitivityDial;
    QWidget *focusCheck;
    QSpinBox *uiObject;
    QString prevObjectToLabel;
    QWidget *clicked;

    QSpinBox *rB1speed;
    QSpinBox *rB2speed;
    QSpinBox *rB3speed;
    QSpinBox *rB4speed;

    //pointers for velocity check boxes
    QCheckBox *rB1Velocity;
    QCheckBox *rB2Velocity;
    QCheckBox *rB3Velocity;
    QCheckBox *rB4Velocity;

    QCheckBox *localLEDControl;

    //pointers for direction check boxes
    QCheckBox *rB1DirectionEnable;
    QCheckBox *rB2DirectionEnable;
    QCheckBox *rB3DirectionEnable;
    QCheckBox *rB4DirectionEnable;

    QSpinBox *rB1Direction;
    QSpinBox *rB2Direction;
    QSpinBox *rB3Direction;
    QSpinBox *rB4Direction;

    QSpinBox *rB1Location;
    QSpinBox *rB2Location;
    QSpinBox *rB3Location;
    QSpinBox *rB4Location;

    QComboBox *modeMenuB1;
    QComboBox *modeMenuB2;
    QComboBox *modeMenuB3;
    QComboBox *modeMenuB4;

    QSpinBox *ledInputChannel;
    QSpinBox *onThresh;
    QSpinBox *offThresh;

    //these make preset saving work
    QVariantMap rotaryMap;
    QStringList rotaryParamList;//list of parameter names
    QStringList rotaryUiBlacklist;
    QMap<QString, QSpinBox*> spinBoxMap; //map of spin box address by param name in rotaryParamList

    void findUiElements();
    void displayRotaryValues();

    int currentPreset;
    int currentRotary;
    int numDouble;
    int currentBank;

    RotaryLabels *rotaryLabel[2];

signals:
    void signalValueChanged(int presetNum, QString settingsType, QString paramType, int typeNum, QString paramName, int paramValue);
    void signalToLabels(QString);
    void signalTriggerCopy();
    void signalTriggerPaste();

public slots:
    void slotEvents(QString);
    void slotRecallPreset(int);
    void slotValueChanged(int);
    void slotValueChanged(double);
    void slotFocusParam(QWidget* oldObject, QWidget* nowObject);
    void slotToLabels(QString);
    void slotDirectionOrLocation(int);
    void slotCalculateOffThreshFromOn(int onThreshChanged);
    void slotCheckOffThreshAgainstOn(int offThreshChanged);
};

#endif // ROTARYEDITPANE_H
