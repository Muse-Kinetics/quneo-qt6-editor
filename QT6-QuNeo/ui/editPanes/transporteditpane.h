#ifndef TRANSPORTEDITPANE_H
#define TRANSPORTEDITPANE_H

#include <QObject>
#include <QWidget>
//#include <QtGui>
#include <QtWidgets> // was QT Gui
#include "transportlabels.h"

class TransportEditPane : public QWidget
{
    Q_OBJECT
public:
    explicit TransportEditPane(QVariantMap *variantMap, QVariantMap *variantMapCopy, QWidget *widget, QWidget *parent = 0);
    bool eventFilter(QObject *, QEvent *);

    QWidget *mainWindow;
    QVariantMap *presetMap;
    QVariantMap *presetMapCopy;
    QLabel *paneLabel;
    QWidget *focusCheck;
    QDial *sensitivityDial;
    QWidget *clicked;
    QString prevObjectToLabel;

    void findUiElements();
    void displayTransportValues();

    int currentPreset;
    int currentTransport;

    QVariantMap transportMap;
    QStringList transportParamList;//list of parameter names
    QStringList transportUiBlacklist;
    QMap<QString, QSpinBox*> spinBoxMap; //map of spin box address by param name in transportParamList

    TransportLabels *transportLabel[3];

    //pointers for velocity check boxes
    QCheckBox *transportVelocity;
    QCheckBox *localLEDControl;

    QComboBox *modeMenu;

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
    void slotToLabels(QString);
    void slotFocusParam(QWidget* oldObject, QWidget* nowObject);
};

#endif // TRANSPORTEDITPANE_H
