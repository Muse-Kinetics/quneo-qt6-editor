#ifndef PRESETHANDLER_H
#define PRESETHANDLER_H

#include <QObject>
#include <QVariant>
#include <QFile>
//#include "qjson/src/parser.h"
//#include "qjson/src/serializer.h"
#include "padeditpane.h"
#include "rotaryeditpane.h"
#include "hslidereditpane.h"
#include "vslidereditpane.h"
#include "lslidereditpane.h"
#include "transporteditpane.h"
#include "modeeditpane.h"
#include "leftrighteditpane.h"
#include "rhombuseditpane.h"
#include "updowneditpane.h"
#include "globalparameters.h"
#include "updateindicator.h"

class PresetHandler : public QObject
{
    Q_OBJECT
public:
    explicit PresetHandler(QWidget *widget, QObject *parent = 0);

    QWidget *mainWindow;

    /*****JSON I/O*****/
    QFile *jsonFile;
    QByteArray jsonByteArray;
    QVariantMap jsonMasterMap;

//    QJson::Parser parser;
//    QJson::Serializer serializer;
    bool ok;

    /*****Edit Panes*****/
    PadEditPane *padEditPane;
    RotaryEditPane *rotaryEditPane;
    HSliderEditPane *hSliderEditPane;
    VSliderEditPane *vSliderEditPane;
    LSliderEditPane *lSliderEditPane;
    TransportEditPane *transportEditPane;
    //ModeEditPane *modeEditPane;
    LeftrightEditPane *leftrightEditPane;
    RhombusEditPane *rhombusEditPane;
    UpdownEditPane *updownEditPane;
    GlobalParameters *globalParameters;
    QStackedWidget *editPanes;

    /*****Data Maps*****/
    QVariantMap presetMaps;
    QVariantMap presetMapsCopy;

    QVariantMap presetNumMap;
    QVariantMap settingsTypeMap;
    QVariantMap paramTypeMap;
    QVariantMap typeNumMap;

    QVariantMap sensPresetNumMap;
    QVariantMap sensSettingsTypeMap;
    QVariantMap sensParamTypeMap;
    QVariantMap sensTypeNumMap;

    /*****Saving Components*****/
    QComboBox *presetMenu;
    bool currentPresetModified;

    QPushButton *saveButton;
    QPushButton *revertButton;
    int currentPreset;
    UpdateIndicator *updateIndicator;
    int alreadyOn;
    int alreadyShown;

    //list of currently clicked sensors for shift click
    QMap<QString, int> clickedSensors;
    QStringList sensorList;

    //presetName validator
    QString currentPresetName;
    QString otherPresetName;
    int x;//number to append if a duplicate name is found.
    bool isPresetNameValidatorRunning;
signals:

    void signalPresetModified(bool);
    //void signalValueChanged(int presetNum, QString settingsType, QString paramType, int typeNum, QString paramName, int paramValue);

public slots:
    void slotSave();
    void slotSaveAllPresets();
    void slotCheckPresets();
    void slotValueChanged(int presetNum, QString settingsType, QString paramType, int typeNum,
                          QString paramName, int paramValue);
    //this slot is called at end of slotValueChanged to change values in all selected sensors
    void slotPropogateValuesSelectMultiple(int presetNum, QString settingsType, QString paramType, int typeNum, QString paramName, int paramValue);
    void slotPresetNameChanged(QString);
    void slotRecallPreset(QString);
    void slotRevertPreset();
    void slotEvents(QString);
    void slotConnectDisconnectDisplayAllLabels(bool connectDisconnect);
    void slotDisplayAllLabels(QString sourceObject);

    //this function saves both the table ID and the table array into the json file.
    void slotPadVelocityTableChanged(int presetNum, QString selectedTableID, QList<QVariant> selectedTableValues);
    void slotPresetNameValidator();
};

#endif // PRESETHANDLER_H
