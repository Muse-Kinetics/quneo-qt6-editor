#ifndef COPYPASTEHANDLER_H
#define COPYPASTEHANDLER_H

#include <QObject>
#include <QVariant>
//#include <QtGui>
#include <QtWidgets> // was QT Gui
#include <QDesktopServices>
#include <QUrl>
#include "presethandler.h"
#include "midideviceaccess.h"
#include "datavalidator.h"

class MainWindow;

class CopyPasteHandler : public QObject
{
    Q_OBJECT
public:
    explicit CopyPasteHandler(PresetHandler *presetHandle, MidiDeviceAccess *mda, QWidget *mainWindow, DataValidator* dataValidator, QObject *parent = 0);

    PresetHandler *handlerOfPresets;

    // pointers to functions that return QStrings
    QString deviceBootloaderVersionString;
    QString deviceFirmwareVersionString;
    QString applicationFirmwareVersionString;
    QString applicationVersionString;

    int copiedFrom;

    MidiDeviceAccess *midiDeviceAccess;

    QPushButton *copyButton;
    QPushButton *pasteButton;

    QVariantMap presetCopiedMap;
    //Maps to store individual copied sensor data
    QVariantMap padSensorCopiedMap;
    QVariantMap hSliderSensorCopiedMap;
    QVariantMap vSliderSensorCopiedMap;
    QVariantMap lSliderSensorCopiedMap;
    QVariantMap rotarySensorCopiedMap;
    QVariantMap transportSensorCopiedMap;
    QVariantMap leftrightSensorCopiedMap;
    QVariantMap rhombusSensorCopiedMap;
    QVariantMap updownSensorCopiedMap;

    QString currentSensorType;//plural: Pads
    QString copiedSensorType;
    QString currentSensor; //with number: Pad0
    QString currentSensorNumber; //to be used inside current sensor string

    /*****JSON I/O*****/
    QFile *presetFile;

//    QJson::Parser parser;
//    QJson::Serializer serializer;
    bool ok;

    DataValidator* dataValidator;

    QMenuBar *menuBar;
    QWidget* mWindow;


    //----- FILE MENU ----//
    QMenu *fileMenu;
    QAction *importPreset;
    QAction *exportPreset;
    QAction *exportAllPresets;
    QAction *savePreset;
    QAction *saveAllPresets;
    QAction *openAppDataDir;

    QVariantMap importedPresetMap;

    QByteArray presetByteArray;
    QDataStream* dataStream;
    //location for slotExportAllPResets to save into
    QString foldername;

    //----- EDIT MENU -----//
    QMenu *editMenu;
    QAction *copyAct;
    QAction *pasteAct;

    QAction *clearPreset;
    QString pathBlankPreset;
    QAction *copySensorAct;
    QAction *pasteSensorAct;
    QAction *revertFactoryAct;
    QAction *revertAllPresets;

    //------ HARDWARE MENU ------//
    QMenu *hardwareMenu;
    QAction *swapPadLeds;
    QAction *updateFirmware;
    QAction *toggleProgramChangeOut;
    QAction *toggleProgramChangeIn;

    //----- HELP MENU -----//
    QMenu *helpMenu;
    QAction *quNeoManual;
    QAction *aboutQuNeo;
    QAction *updateCheckAct;
    QAction* troubleShooting;

    QMessageBox aboutScreen;

    QFile *documentationLinkFile;

    //---- DRUM STYLES ----//
    QMenu *padSensitivityMenu;
    QAction *padSensitivityDefault;
    QAction *padSensitivityLow;
    QAction *padSensitivityMedium;
    QAction *padSensitivityHigh;
    QAction *padSensitivityExtreme;

    bool fwProgressOpen;

signals:
    void sigUpdateFirmware();
    void sigUpdateAboutMenuVersions();

public slots:

    void slotOpenPresetDirectory();
    void copyPreset();
    void pastePreset();
    void slotExportPreset();
    void slotExportAllPresets();
    void slotImportPreset();
    void slotImportBlankPreset();
    void slotOpenAbout();
    void slotOpenDocumentation();
    void slotCopySensor();
    void slotPasteSensor();
    void slotSetCurrentSensor(QString sensorTypeName);
    void slotLoadFactoryAll();
    void slotLoadFactoryCurrent();
    void slotUpdateFirmware();

    void slotQuNeoConnected(bool);
    void slotProgressDialogOpen(bool);
    void slotTogglePCout();
    void slotTogglePCin();


};

#endif // COPYPASTEHANDLER_H
