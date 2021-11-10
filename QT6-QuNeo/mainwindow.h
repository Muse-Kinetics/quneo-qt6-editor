#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include "padbutton.h"
#include "vsliderbutton.h"
#include "hsliderbutton.h"
#include "rotarybutton.h"
#include "lsliderbutton.h"
#include "buttonbutton.h"
#include "presethandler.h"
#include "midideviceaccess.h"
#include "sysexformat.h"
#include "copypastehandler.h"
#include "datavalidator.h"

// midi overhaul
#include "kmi_ports.h"
#include "KMI_mdm.h"
#include "RtMidi.h"
#include "KMI_DevData.h"
#include "midi.h"
#include <fwupdate.h>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QByteArray applicationVersion, thisFw;

    // ------ midi overhaul --------------------------------------------------------

    // kmiPorts handles MIDI I/O changes
    KMI_Ports *kmiPorts;

    // create KMI devices
    MidiDeviceManager* QuNeo;

    // create a virtual port on MacOS, iOS, and Linux. Not supported on Windows.
#ifndef Q_OS_WIN
    MidiDeviceManager* virtualMidiPort;
#endif

    // MIDI aux inputs and outputs are defined here. For products like SoftStep, you would define 8 inputs for controllers
    // and one output for hosted mode. For other editors you would likely define one output port for to mirror the
    // incoming MIDI from the controller, as a workaround for Windows not sharing ports.
    // For KMI_Central we are using these for the input/output dropdowns as a simple MIDI route demo.
    MidiDeviceManager* midiAuxOut;

    // ------ quneo ------------------------------------------------------------


    //filter main window function for objects with event filter installed
    bool eventFilter(QObject *, QEvent *);
    //intercept exit app signal
    void closeEvent(QCloseEvent *event);

    //Data Validator
    DataValidator* dataValidator;

    // Firmware Update Window
    fwUpdate* fwUpdateWindow;

    //highlight button instances
    PadButton* padButton[16];
    VSliderButton* vSliderButton[4];
    HSliderButton* hSliderButton[4];
    RotaryButton* rotaryButton[2];
    LSliderButton* lSliderButton[1];
    ButtonButton* buttonButton[11];

    //preset stuff
    PresetHandler *presetHandler;
    int maybeSave();
    CopyPasteHandler *copyPasteHandler;

    //*****MIDI*****//
    MidiDeviceAccess* midiDeviceAccess;
    SysExFormat* sysExFomat;
    QComboBox* deviceMenu;

    //-------------- Rogue warning
    QMessageBox rogueWarning;
    bool rogueQueryDevice;

    //** FW Update UI Stuff **//
    QMessageBox* msgBox;
    QPushButton* firmwareUpdate;
    QProgressDialog *progress;
    QMessageBox updateComplete;
    int totalFwBytes;
    int remainingFwBytes;

    bool fwUpdateDialogOpen;
    QMessageBox fwUpdateDialogAuto;
    QMessageBox fwUpdateDialogManual;

    QMessageBox updateAllPresetsCompleteMsgBox;
    QProgressDialog* updateAllPresetsProgressDialog;

    QAction* toNextSensor;
    QAction* toPrevSensor;

    //variables for shiftclick select multiple
    int currentSensorClicked;
    QString modifiedSensorString;
    int eLimit;
    int eStart;

    bool connected;

    // ------ midi overhaul --------------------------------------------------------

    // version strings for console and about window
    QString deviceBootloaderVersionString();
    QString deviceFirmwareVersionString();
    QString applicationFirmwareVersionString();

    // ------ end midi overhaul --------------------------------------------------------


signals:
    //emits main window events
    void signalEvents(QString);
    void sigVersions(QString, QString);
    void sigFwProgressDialogOpen(bool);

public slots:
   // void slotPopulateDeviceMenu(QList<QString>);
//    int firmwareUpdateDialogMenu(bool);
//    int firmwareUpdateDialog(bool);
//    void progressDialog(void);
//    void firmwareUpdateCompleteDialog(void);

    void centerWidgetOnScreen(QWidget*);
//    void slotUpdateFwProgressDialog(int);

    void slotUpdateAllPresetsProgress(int);
    void slotGoToNextSensor();
    void slotGoToPrevSensor();
    void slotRogueWarning();

    void slotShowUpdateAllDialog();
//    void slotShowFWUpdateDialog();

    // ------ midi overhaul --------------------------------------------------------
    void slotMIDIPortChange(QString, uchar, uchar, int); // handles changes to MIDI i/o
    void slotBootloaderMode(bool fwUpdateRequested);
    void slotFwUpdateSuccessCloseDialog(bool);
    void slotForceFirmwareUpdate();
    void slotFirmwareDetected(MidiDeviceManager *thisMDM, bool);
    void slotQuNeoConnected(bool state);
    void slotUpdateMIDIaux();

    // ------ end midi overhaul --------------------------------------------------------


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
