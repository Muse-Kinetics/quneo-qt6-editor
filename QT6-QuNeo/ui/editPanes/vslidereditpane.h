#ifndef VSLIDEREDITPANE_H
#define VSLIDEREDITPANE_H

#include <QObject>
#include <QWidget>
//#include <QtGui>
#include <QtWidgets> // was QT Gui

#include <vsliderlabels.h>

class VSliderEditPane : public QWidget
{
    Q_OBJECT
public:
    explicit VSliderEditPane(QVariantMap *variantMap, QVariantMap *variantMapCopy, QWidget *widget, QWidget *parent = 0);
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
    QVariantMap vSliderMap;
    QStringList vSliderParamList;//list of parameter names
    QStringList vSliderUiBlackList;
    QMap<QString, QSpinBox*> spinBoxMap; //map of spin box address by param name in padParamList

    void findUiElements();
    void displayVSliderValues();

    int currentPreset;
    int currentVSlider;
    int currentBank;

    VSliderLabels *vSliderLabel[16];

    //pointers for velocity check boxes
    QCheckBox *vB1Velocity;
    QCheckBox *vB2Velocity;
    QCheckBox *vB3Velocity;
    QCheckBox *vB4Velocity;
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

#endif // VSLIDEREDITPANE_H
