#ifndef UPDOWNEDITPANE_H
#define UPDOWNEDITPANE_H

#include <QObject>
#include <QWidget>
//#include <QtGui>
#include <QtWidgets> // was QT Gui
#include "updownlabels.h"

class UpdownEditPane : public QWidget
{
    Q_OBJECT
public:
    explicit UpdownEditPane(QVariantMap *variantMap, QVariantMap *variantMapCopy, QWidget *widget, QWidget *parent = 0);
    bool eventFilter(QObject *, QEvent *);

    QWidget *mainWindow;
    QVariantMap *presetMap;
    QVariantMap *presetMapCopy;
    QLabel *paneLabel;
    QWidget *focusCheck;
    QLabel *bankDestination;
    QCheckBox *enableBank;
    QComboBox *bankControl;
    QDial *sensitivityDial;
    QWidget *clicked;
    QString prevObjectToLabel;
    QComboBox  *modeMenuU;
    QComboBox  *modeMenuD;

    void findUiElements();
    void displayUpdownValues();

    int currentPreset;
    int currentUpdown;

    QVariantMap updownMap;
    QStringList updownParamList;//list of parameter names
    QStringList updownUiBlacklist;
    QMap<QString, QSpinBox*> spinBoxMap; //map of spin box address by param name in updownParamList

    UpdownLabels *updownLabel[2];

    //pointers for velocity check boxes
    QCheckBox *updownUVelocity;
    QCheckBox *updownDVelocity;
    QCheckBox *localLEDControl;

    QSpinBox *ledInputChannel;

signals:
    void signalValueChanged(int presetNum, QString settingsType, QString paramType, int typeNum, QString paramName, int paramValue);
    void signalToLabels(QString);
    void signalTriggerCopy();
    void signalTriggerPaste();
    
public slots:
    void slotEvents(QString);
    void slotRecallPreset(int);
    void slotValueChanged(int);
    void slotEnable(int);
    void slotBankControl(int);
    void slotToLabels(QString);
    void slotFocusParam(QWidget* oldObject, QWidget* nowObject);
};

#endif // UPDOWNEDITPANE_H
