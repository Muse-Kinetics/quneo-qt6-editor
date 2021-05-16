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


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    //filter main window function for objects with event filter installed
    bool eventFilter(QObject *, QEvent *);
    //intercept exit app signal
    void closeEvent(QCloseEvent *event);

    //Data Validator
    DataValidator* dataValidator;


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

signals:
    //emits main window events
    void signalEvents(QString);
    void sigVersions(QString, QString);
    void sigFwProgressDialogOpen(bool);

public slots:
   // void slotPopulateDeviceMenu(QList<QString>);
    int firmwareUpdateDialogMenu(bool);
    int firmwareUpdateDialog(bool);
    void progressDialog(void);
    void firmwareUpdateCompleteDialog(void);
    void centerWidgetOnScreen(QWidget*);
    void slotUpdateFwProgressDialog(int);
    void slotUpdateAllPresetsProgress(int);
    void slotGoToNextSensor();
    void slotGoToPrevSensor();
    void slotRogueWarning();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
