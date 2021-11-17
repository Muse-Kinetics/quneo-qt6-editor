#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QEvent>
#include <QMouseEvent>
#include "KMI_FwVersions.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    QCoreApplication::setApplicationName("QuNeo Editor");
    QCoreApplication::setOrganizationName("Keith McMillen Instruments");
    QCoreApplication::setOrganizationDomain("keithmcmillen.com");


    // ---- MIDI OVERHAUL ------------------------------------------------


    // application version
    applicationVersion.resize(3);

    applicationVersion[0] = 2;
    applicationVersion[1] = 0;
    applicationVersion[2] = 0;

    betaVersion = "B"; // release, do not show beta string

    thisFw = QByteArray(reinterpret_cast<char*>(_fw_ver_quneo), sizeof(_fw_ver_quneo));


    // ******************************
    // KMI_Ports
    // ******************************

    // kmiPorts reports changes in MIDI i/o
    kmiPorts = new KMI_Ports(this);

#ifndef Q_OS_WIN
    //kmiPorts->slotCreateVirtualIn("QuNeo Editor");
    //kmiPorts->slotCreateVirtualOut("QuNeo Editor");
#endif

    // start polling at 100ms intervals
    kmiPorts->devicePoller->start(100);

    // connect kmiPorts to our handler
    connect(kmiPorts, SIGNAL(signalPortUpdated(QString, uchar, uchar, int)),
            this, SLOT(slotMIDIPortChange(QString, uchar, uchar, int)));

    //qDebug() << "end connect";

    // ******************************
    // create KMI device handlers
    // ******************************

    QuNeo = new MidiDeviceManager(this, PID_QUNEO, "QuNeo");

    // setup firmware image

    QString thisFwFile = QString(":/Quneo/sysex/resources/sysex/Quneo_Firmware_v%1.%2.%3.syx")
            .arg(uchar(thisFw.at(0)))
            .arg(uchar(thisFw.at(1)))
            .arg(uchar(thisFw.at(2)));

    //qDebug() << "fwFilename: " << thisFwFile;

    QuNeo->slotOpenFirmwareFile(thisFwFile);

    // connect firmware signals
    qDebug() << "connect signalFirmwareDetected";

    // setup MIDI aux output
    midiAuxOut = new MidiDeviceManager(this, PID_AUX, "MIDI Thru");

    // ******************************
    // end KMI_Ports and device handlers
    // ******************************

    connected = false;

    // ---- End MIDI Overhaul --------------------------------------------

    //*****set up the ui
    ui->setupUi(this);

    this->setWindowTitle("QuNeo Editor");

    //
    qDebug() << "new DataValidator";
    dataValidator = new DataValidator();

    qDebug() << "DONE";

    //*****Setup Preset Handler*****//
    qDebug() << "new presetHandler";
    presetHandler = new PresetHandler(this, 0);

    qDebug() << "setup buttons";
    //*****create all the component buttons with instance numbers, main window address, parent arg (always 0 for now)
    for(int i=0; i<16; i++){
        padButton[i] = new PadButton(presetHandler, presetHandler->padEditPane, i, this, 0);
    }
    for(int i=0; i<4; i++){
        vSliderButton[i] = new VSliderButton(presetHandler, presetHandler->vSliderEditPane, i, this, 0);
    }
    for(int i=0; i<4; i++){
        hSliderButton[i] = new HSliderButton(presetHandler, presetHandler->hSliderEditPane, i, this, 0);
    }
    for(int i=0; i<2; i++){
        rotaryButton[i] = new RotaryButton(presetHandler, presetHandler->rotaryEditPane, i, this, 0);
    }
    for(int i=0; i<1; i++){
        lSliderButton[i] = new LSliderButton(presetHandler, presetHandler->lSliderEditPane, i, this, 0);
    }
    for(int i=1; i<11; i++){
        buttonButton[i] = new ButtonButton(presetHandler, presetHandler->leftrightEditPane, presetHandler->updownEditPane, presetHandler->rhombusEditPane, presetHandler->transportEditPane, i, this, 0);
    }

    // EB TODO - implement this
    // connect dropdowns and connection status to MIDI aux ports
    //connect(ui->midi_outputs, SIGNAL(currentIndexChanged(int)), this, SLOT(slotUpdateMIDIaux()));
    //connect(QuNeo, SIGNAL(signalConnected(bool)), this, SLOT(slotUpdateMIDIaux()));


    // Firmware update Window
    fwUpdateWindow = new fwUpdate(this, "QuNeo", applicationFirmwareVersionString());
    //fwUpdateWindow->setStyleSheet(generalStylesString);

    //*****MIDI*****//


    qDebug() << "setup MIDI";

    // EB keep these as they are device specific
    midiDeviceAccess = new MidiDeviceAccess(&presetHandler->presetMapsCopy,this);
    connect(presetHandler->presetMenu, SIGNAL(currentTextChanged(QString)), midiDeviceAccess, SLOT(slotSetCurrentPreset(QString)));
    connect(midiDeviceAccess, SIGNAL(sigRogueWarning()), this, SLOT(slotRogueWarning()));

    // cph needs a pointer to mda for fw version strings
    copyPasteHandler = new CopyPasteHandler(presetHandler, midiDeviceAccess,this, dataValidator, this);
    connect(copyPasteHandler, SIGNAL(sigUpdateFirmware()), this, SLOT(slotForceFirmwareUpdate()));

    // allow cph to update the device firmware strings before opening the about menu
    connect(copyPasteHandler, SIGNAL(sigUpdateAboutMenuVersions()), this, SLOT(slotUpdateAboutMenuVersions()));

    // this was already commented out
    //connect(midiDeviceAccess, SIGNAL(sigSetVersions(QString,QString)), copyPasteHandler, SLOT(slotSetVersions(QString,QString)));


    // ----------------
    // MIDI Overhaul
    //-----------------

    connect(midiDeviceAccess, SIGNAL(signalSendSysExBA(QByteArray)), QuNeo, SLOT(slotSendSysExBA(QByteArray)));
    connect(midiDeviceAccess, SIGNAL(signalSendSysEx(unsigned char*, int)), QuNeo, SLOT(slotSendSysEx(unsigned char*, int)));

    // Firmware Out of Date Warning, Update Progress Bar, Update Complete

    // connect firmware detection
    connect(QuNeo, SIGNAL(signalFirmwareDetected(MidiDeviceManager*, bool)), this, SLOT(slotFirmwareDetected(MidiDeviceManager*, bool)));

    // connect firmware update window and midi device manager controls and messaging
    connect(fwUpdateWindow, SIGNAL(signalRequestFwUpdate()), QuNeo, SLOT(slotRequestFirmwareUpdate()));                   // request fw
    connect(QuNeo, SIGNAL(signalFwConsoleMessage(QString)), fwUpdateWindow, SLOT(slotAppendTextToConsole(QString)));      // messaging
    connect(QuNeo, SIGNAL(signalFwProgress(int)), fwUpdateWindow, SLOT(slotUpdateProgressBar(int)));                      // console
    connect(QuNeo, SIGNAL(signalFirmwareUpdateComplete(bool)), fwUpdateWindow, SLOT(slotFwUpdateComplete(bool)));         // Update Complete
    connect(fwUpdateWindow, SIGNAL(signalFwUpdateSuccess()), QuNeo, SLOT(slotFirmwareUpdateReset()));                   // stop timeout timers
    connect(fwUpdateWindow, SIGNAL(signalFwUpdateSuccessCloseDialog(bool)), this, SLOT(slotFwUpdateSuccessCloseDialog(bool)));      // close fw dialog and connect

    // EB TODO - confirm no globals to save/recall on QuNeo?
    //connect(QuNeo, SIGNAL(signalRequestGlobals()), this, SLOT(slotSendGlobalsRequest()));                                 // request globals
    //connect(sysExEncDecode, SIGNAL(signalGlobalsReceivedDoFwUd()), QuNeo, SLOT(slotRequestFirmwareUpdate()));                      // if fwupdate requested globals then alert that we've saved them
    //connect(sysExEncDecode, SIGNAL(signalGlobalsReceived()), this, SLOT(slotEnableGlobalsWindows()));                       // enable globals window when we receive this
    //connect(QuNeo, SIGNAL(signalRestoreGlobals()), this, SLOT(slotEncodeGlobals()));                                      // restore the globals after fw update

    // handle device unexpectedly in bootloader mode
    connect(QuNeo, SIGNAL(signalBootloaderMode(bool)), this, SLOT(slotBootloaderMode(bool)));

    // reset portlist after sending bootloader commands, catch changes to port names
    connect(QuNeo, SIGNAL(signalBeginBlTimer()), this, SLOT(slotRefreshConnection()));
    connect(QuNeo, SIGNAL(signalBeginFwTimer()), this, SLOT(slotRefreshConnection()));


    //---------------------------------- Connected Light
    connect(QuNeo, SIGNAL(signalConnected(bool)), this, SLOT(slotQuNeoConnected(bool)));
    //connect(QuNeo, SIGNAL(signalConnected(bool)), this, SLOT(slotEnableDisableMidiFunctions(bool)));

    // ---- END MIDI OVERHAUL ----------------------------------------

    qDebug() << "Button classes to copypaste";
    //button classes need a pointer to the copypastehandler, but since they are initialized before the copypastehandler
    //set the pointer this way. This allows you to enable the menu items after a sensor has been clicked.
    for(int i=0; i<16; i++){
        padButton[i]->copyPasteHandle = copyPasteHandler;
    }
    for(int i=0; i<4; i++){
        vSliderButton[i]->copyPasteHandle = copyPasteHandler;
    }
    for(int i=0; i<4; i++){
        hSliderButton[i]->copyPasteHandle = copyPasteHandler;
    }
    for(int i=0; i<2; i++){
        rotaryButton[i]->copyPasteHandle = copyPasteHandler;
    }
    for(int i=0; i<1; i++){
        lSliderButton[i]->copyPasteHandle = copyPasteHandler;
    }
    for(int i=1; i<11; i++){
        buttonButton[i]->copyPasteHandle = copyPasteHandler;
    }

    qDebug() << "Connect signals from button classes";
    //--connect signals from button classes to slot which sets current sensor type and number--//
    //--to be used by slotCopySensor and slotPasteSensor to look up preset values--------------//
    for(int i=0; i<4; i++){
        connect(hSliderButton[i], SIGNAL(signalCurrentSensor(QString)), copyPasteHandler, SLOT(slotSetCurrentSensor(QString)));
    }
    for(int i=0; i<4; i++){
        connect(vSliderButton[i], SIGNAL(signalCurrentSensor(QString)), copyPasteHandler, SLOT(slotSetCurrentSensor(QString)));
    }
    for(int i=0; i<16; i++){
        connect(padButton[i], SIGNAL(signalCurrentSensor(QString)), copyPasteHandler, SLOT(slotSetCurrentSensor(QString)));
    }
    for(int i=0; i<1; i++){
        connect(lSliderButton[i], SIGNAL(signalCurrentSensor(QString)), copyPasteHandler, SLOT(slotSetCurrentSensor(QString)));
    }
    for(int i=1; i<11; i++){
        connect(buttonButton[i], SIGNAL(signalCurrentSensor(QString)), copyPasteHandler, SLOT(slotSetCurrentSensor(QString)));
    }
    for(int i=0; i<2; i++){
        connect(rotaryButton[i], SIGNAL(signalCurrentSensor(QString)), copyPasteHandler, SLOT(slotSetCurrentSensor(QString)));
    }

    qDebug() << "Command arrow keys";
    //----Command Arrow Keys Actions------//
    toNextSensor = new QAction(tr("&Edit Next Sensor"), this);
    copyPasteHandler->editMenu->addAction(toNextSensor);
    toNextSensor->setShortcut(Qt::CTRL | Qt::Key_Right);
    connect(toNextSensor, SIGNAL(triggered()), this, SLOT(slotGoToNextSensor()));

    toPrevSensor = new QAction(tr("&Edit Prev Sensor"), this);
    copyPasteHandler->editMenu->addAction(toPrevSensor);
    toPrevSensor->setShortcut(Qt::CTRL | Qt::Key_Left);
    connect(toPrevSensor, SIGNAL(triggered()), this, SLOT(slotGoToPrevSensor()));

    qDebug() << "Connect panes to copypase/handler";

    //connect each edit pane's copy/paste signals to the copypastehandler.
    //This allows you to copy/paste while a spinbox is in focus.
    connect(presetHandler->padEditPane, SIGNAL(signalTriggerCopy()), copyPasteHandler, SLOT(slotCopySensor()));
    connect(presetHandler->padEditPane, SIGNAL(signalTriggerPaste()), copyPasteHandler, SLOT(slotPasteSensor()));
    connect(presetHandler->hSliderEditPane, SIGNAL(signalTriggerCopy()), copyPasteHandler, SLOT(slotCopySensor()));
    connect(presetHandler->hSliderEditPane, SIGNAL(signalTriggerPaste()), copyPasteHandler, SLOT(slotPasteSensor()));
    connect(presetHandler->vSliderEditPane, SIGNAL(signalTriggerCopy()), copyPasteHandler, SLOT(slotCopySensor()));
    connect(presetHandler->vSliderEditPane, SIGNAL(signalTriggerPaste()), copyPasteHandler, SLOT(slotPasteSensor()));
    connect(presetHandler->lSliderEditPane, SIGNAL(signalTriggerCopy()), copyPasteHandler, SLOT(slotCopySensor()));
    connect(presetHandler->lSliderEditPane, SIGNAL(signalTriggerPaste()), copyPasteHandler, SLOT(slotPasteSensor()));
    connect(presetHandler->rotaryEditPane, SIGNAL(signalTriggerCopy()), copyPasteHandler, SLOT(slotCopySensor()));
    connect(presetHandler->rotaryEditPane, SIGNAL(signalTriggerPaste()), copyPasteHandler, SLOT(slotPasteSensor()));
    connect(presetHandler->leftrightEditPane, SIGNAL(signalTriggerCopy()), copyPasteHandler, SLOT(slotCopySensor()));
    connect(presetHandler->leftrightEditPane, SIGNAL(signalTriggerPaste()), copyPasteHandler, SLOT(slotPasteSensor()));
    connect(presetHandler->updownEditPane, SIGNAL(signalTriggerCopy()), copyPasteHandler, SLOT(slotCopySensor()));
    connect(presetHandler->updownEditPane, SIGNAL(signalTriggerPaste()), copyPasteHandler, SLOT(slotPasteSensor()));
    connect(presetHandler->transportEditPane, SIGNAL(signalTriggerCopy()), copyPasteHandler, SLOT(slotCopySensor()));
    connect(presetHandler->transportEditPane, SIGNAL(signalTriggerPaste()), copyPasteHandler, SLOT(slotPasteSensor()));
    connect(presetHandler->rhombusEditPane, SIGNAL(signalTriggerCopy()), copyPasteHandler, SLOT(slotCopySensor()));
    connect(presetHandler->rhombusEditPane, SIGNAL(signalTriggerPaste()), copyPasteHandler, SLOT(slotPasteSensor()));

    // allow pad sensitivity menu to select pads for editing values
    connect(presetHandler->padEditPane, SIGNAL(signalCurrentSensor(QString)), copyPasteHandler, SLOT(slotSetCurrentSensor(QString)));

    //Save button
    // EB commented out to avoid crash
    qDebug() << "Connect PresetHandler save button";
    connect(presetHandler->saveButton, SIGNAL(clicked()), presetHandler, SLOT(slotSave()));

    qDebug() << "Firmware update stuff";
    //Firmware Updating Stuff ***********
//    firmwareUpdate = this->findChild<QPushButton *>(QString("updateFirmwareButton"));
//    firmwareUpdate->hide();

//    connect(midiDeviceAccess, SIGNAL(sigFirmwareCurrent(bool)), this, SLOT(firmwareUpdateDialog(bool)));
    centerWidgetOnScreen(this);

//    updateComplete.setText("Firmware Sent. \n\nAfter hitting OK please press Update All.");
//    updateComplete.move(this->width()/2, this->height()/2);

//    fwUpdateDialogAuto.setIcon(QMessageBox::Warning);
//    fwUpdateDialogAuto.addButton(QMessageBox::Cancel);
//    fwUpdateDialogAuto.addButton(QMessageBox::Ok);
//    fwUpdateDialogAuto.setText(QString("The firmware on the selected QuNeo is out of date.\n""Click OK to update your firmware."));
//    fwUpdateDialogAuto.move(this->width()/2, this->height()/2);

//    fwUpdateDialogManual.setIcon(QMessageBox::Warning);
//    fwUpdateDialogManual.addButton(QMessageBox::Cancel);
//    fwUpdateDialogManual.addButton(QMessageBox::Ok);
//    fwUpdateDialogManual.setText(QString("Click OK to update your firmware."));
//    fwUpdateDialogManual.move(this->width()/2, this->height()/2);

    qDebug() << "setup Update All Presets Dialog";
    // moved this into slotShowUpdateAllDialog()
    updateAllPresetsProgressDialog = new QProgressDialog("Updating All Presets...","Cancel", 0,16,this);
    //updateAllPresetsProgressDialog->setWindowModality(Qt::WindowModal);
    updateAllPresetsProgressDialog->setCancelButton(0);
    updateAllPresetsProgressDialog->close();
    //updateAllPresetsProgressDialog->setValue(4);

    qDebug() << "Connect midiDeviceAccess";
    connect(midiDeviceAccess, SIGNAL(sigUpdateAllPresetsCount(int)), this, SLOT(slotUpdateAllPresetsProgress(int)));
    connect(midiDeviceAccess, SIGNAL(sigShowUpdateAllPresetsDialog()), this, SLOT(slotShowUpdateAllDialog()));

    qDebug() << "populate clicked sensor map";
    //populate clicked sensor map with 0's for initial state of editmultiple
    for(int i=0; i < 16; i++) {
        presetHandler->clickedSensors.insert(QString("padButton%1").arg(i), 0);
        presetHandler->sensorList.append(QString("padButton%1").arg(i));
    }
    for(int i=0; i < 4; i++) {
        presetHandler->clickedSensors.insert(QString("hSliderButton%1").arg(i), 0);
        presetHandler->sensorList.append(QString("hSliderButton%1").arg(i));
    }
    for(int i=0; i < 4; i++) {
        presetHandler->clickedSensors.insert(QString("vSliderButton%1").arg(i), 0);
        presetHandler->sensorList.append(QString("vSliderButton%1").arg(i));
    }
    for(int i=0; i < 11; i++) {
        presetHandler->clickedSensors.insert(QString("buttonButton%1").arg(i), 0);
        presetHandler->sensorList.append(QString("buttonButton%1").arg(i));
    }
    for(int i=0; i < 2; i++) {
        presetHandler->clickedSensors.insert(QString("rotaryButton%1").arg(i), 0);
        presetHandler->sensorList.append(QString("rotaryButton%1").arg(i));
    }
    presetHandler->clickedSensors.insert(QString("lSliderButton0"), 0);
    presetHandler->sensorList.append(QString("lSliderButton0"));

    totalFwBytes = midiDeviceAccess->sysExFirmwareBytes.size();

    qDebug() << "mac/win progress bars";

    //mac progress bar...
#ifdef Q_OS_MAC

//    connect(midiDeviceAccess, SIGNAL(sigFwBytesLeft(int)), this, SLOT(slotUpdateFwProgressDialog(int)));
    //connect(midiDeviceAccess, SIGNAL(sigShowFWUpdateDialog()), this, SLOT(slotShowFWUpdateDialog()));
    // this is moved to slotShowFWUpdateDialog()
//    progress = new QProgressDialog("Updating Firmware...", "Cancel", 0, totalFwBytes, this);
    //progress->setWindowModality(Qt::WindowModal);
//    progress->setCancelButton(0);
    //progress->setValue(1000);
//    progress->close();

    //************ THIS SHOULD BE DONE LAST, AFTER APP IS "CONSTRUCTED" / LOADED ****************//
    //midiDeviceAccess->getSourcesDests(); //populate midi devices

#else
    //windows progress bar...

//    progress = new QProgressDialog("Updating Firmware...", "Cancel", 0, 0, this);
//    progress->setWindowModality(Qt::WindowModal);
//    progress->setCancelButton(0);

    //windows edit pane fonts
    QGroupBox* gb = this->findChild<QGroupBox *>(QString("groupBox"));
    gb->setStyleSheet("QLabel { font: 10px } QLineEdit { font: 8px } QCheckBox {font:9px} QSpinBox {font: 10px} QDoubleSpinBox {font: 10px} QComboBox {font: 10px}");
    QLineEdit* qle = this->findChild<QLineEdit *>(QString("presetName"));
    qle->setStyleSheet("QLineEdit {font:8px}");

#endif

    qDebug() << "setup Rogue warning";
    //rogueWarning.setWindowFlags(Qt::Widget);
    rogueWarning.setIcon(QMessageBox::Warning);
    rogueWarning.addButton(QMessageBox::Ok);
    rogueWarning.setText("Your QuNeo is either in Expander Mode or you are using a Rogue.");
    rogueWarning.setInformativeText("Please connect your QuNeo with a USB cable, and make sure it is not in \"expander\" mode.");

    // update version string
    QString versionLabelText = QString("%1.%2.%3%4").arg((uchar)applicationVersion[0]).arg((uchar)applicationVersion[1]).arg((uchar)applicationVersion[2]).arg(betaVersion);
    ui->QuNeoVersionLabel->setText(versionLabelText);


}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject *object, QEvent *event){
    if(event->type() == QEvent::WindowDeactivate){
        //When main window doesn't have focus, disconnect each component's slotFocusParam so labels aren't displayed when window comes back into focus.
        //Reconnected by each button class when the user clicks a parameter.
        disconnect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), presetHandler->hSliderEditPane, SLOT(slotFocusParam(QWidget*,QWidget*)));
        for(int i = 0; i < 4; i++){
            hSliderButton[i]->focusConnected = false;
        }

        disconnect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), presetHandler->lSliderEditPane, SLOT(slotFocusParam(QWidget*,QWidget*)));
        lSliderButton[0]->focusConnected = false;

        disconnect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), presetHandler->padEditPane, SLOT(slotFocusParam(QWidget*,QWidget*)));
        for(int p = 0; p < 16; p++){
            padButton[p]->focusConnected = false;
        }

        disconnect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), presetHandler->vSliderEditPane, SLOT(slotFocusParam(QWidget*,QWidget*)));
        for(int v = 0; v < 4; v++){
            vSliderButton[v]->focusConnected = false;
        }

        disconnect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), presetHandler->rotaryEditPane, SLOT(slotFocusParam(QWidget*,QWidget*)));
        for(int r = 0; r < 2; r++){
            rotaryButton[r]->focusConnected = false;
        }
        disconnect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), presetHandler->leftrightEditPane, SLOT(slotFocusParam(QWidget*,QWidget*)));
        for(int n = 4; n < 8; n++){
            buttonButton[n]->focusConnected = false;
        }
        disconnect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), presetHandler->updownEditPane, SLOT(slotFocusParam(QWidget*,QWidget*)));
        for(int n = 9; n < 11; n++){
            buttonButton[n]->focusConnected = false;
        }
        disconnect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), presetHandler->rhombusEditPane, SLOT(slotFocusParam(QWidget*,QWidget*)));
        buttonButton[8]->focusConnected = false;
        disconnect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), presetHandler->transportEditPane, SLOT(slotFocusParam(QWidget*,QWidget*)));
        for(int n = 1; n < 4; n++){
            buttonButton[n]->focusConnected = false;
        }

    }

    if(event->type() == QEvent::MouseButtonPress){
        disconnect(presetHandler, SIGNAL(signalPresetModified(bool)), presetHandler->updateIndicator, SLOT(slotPresetModified(bool)));
        emit signalEvents(QString("%1").arg(object->objectName()));
        connect(presetHandler, SIGNAL(signalPresetModified(bool)), presetHandler->updateIndicator, SLOT(slotPresetModified(bool)));
    }
    if(event->type() == QEvent::MouseButtonPress){
        //if there is a mouse press event check to see if it is a shift left click
        QMouseEvent *mouseEvent;
        mouseEvent = (QMouseEvent *)event;
        if (mouseEvent->button() == 1){
            if (mouseEvent->modifiers().testFlag(Qt::ShiftModifier)){
                //if it is then set the current sensor = 1 for clicked if it hasn't already been clicked.
                //if it has already been clicked, set it to 0 so that it is unclicked.
                if(presetHandler->clickedSensors.value(object->objectName()) == 0){
                    presetHandler->clickedSensors.insert(object->objectName(), 1);
                }
                else {
                    presetHandler->clickedSensors.insert(object->objectName(), 0);
                    //when it is unclicked, pick the first already selected sensor in that group to show on the edit pane.
                    eStart = 0;
                    if(object->objectName().contains("padButton")){
                        eLimit = 16;
                        modifiedSensorString = QString("padButton");
                    }
                    if(object->objectName().contains("hSlider")){
                        eLimit = 4;
                        modifiedSensorString = QString("hSliderButton");
                    }
                    if(object->objectName().contains("vSlider")){
                        eLimit = 4;
                        modifiedSensorString = QString("vSliderButton");
                    }
                    if(object->objectName().contains("rotary")){
                        eLimit = 2;
                        modifiedSensorString = QString("rotaryButton");
                    }
                    if(object->objectName().contains("lSlider")){
                        eLimit = 1;
                        modifiedSensorString = QString("lSliderButton");
                    }
                    if(object->objectName().contains("buttonButton")){
                        modifiedSensorString = QString("buttonButton");
                        if(object->objectName().contains("1")||object->objectName().contains("2")||object->objectName().contains("3")){
                            if(!object->objectName().contains("10")){
                                eLimit = 4;
                            }
                        }
                        if(object->objectName().contains("4")||object->objectName().contains("5")||object->objectName().contains("6")||object->objectName().contains("7")){
                            eLimit = 8;
                            eStart = 4;
                        }
                        if(object->objectName().contains("8")){
                            eLimit = 9;
                            eStart = 8;
                        }
                        if(object->objectName().contains("9")||object->objectName().contains("10")){
                            eLimit = 11;
                            eStart = 9;
                        }
                    }
                    for(int e = eStart; e < eLimit; e++){//scroll through current sensor group to find a highlighted one
                        qDebug()<<QString("%1%2").arg(modifiedSensorString).arg(e);
                        if(presetHandler->clickedSensors.value(QString("%1%2").arg(modifiedSensorString).arg(e))){
                            //if it has been highlighted select it by emitting signalEvents(objectName)
                            //and stop the for loop
                            emit signalEvents(QString("%1%2").arg(modifiedSensorString).arg(e));
                            break;
                        }
                    }
                }

                //clear other sensor types.
                if(object->objectName().contains("padButton")){
                    for(int i=0; i < 4; i++){
                        presetHandler->clickedSensors.insert(QString("hSliderButton%1").arg(i), 0);
                    }
                    for(int i=0; i < 4; i++){
                        presetHandler->clickedSensors.insert(QString("vSliderButton%1").arg(i), 0);
                    }
                    for(int i=0; i < 11; i++){
                        presetHandler->clickedSensors.insert(QString("buttonButton%1").arg(i), 0);
                    }
                    for(int i=0; i < 2; i++){
                        presetHandler->clickedSensors.insert(QString("rotaryButton%1").arg(i), 0);
                    }
                    presetHandler->clickedSensors.insert(QString("lSliderButton0"), 0);
                }
                if(object->objectName().contains("hSliderButton")){
                    for(int i=0; i < 16; i++){
                        presetHandler->clickedSensors.insert(QString("padButton%1").arg(i), 0);
                    }
                    for(int i=0; i < 4; i++){
                        presetHandler->clickedSensors.insert(QString("vSliderButton%1").arg(i), 0);
                    }
                    for(int i=0; i < 11; i++){
                        presetHandler->clickedSensors.insert(QString("buttonButton%1").arg(i), 0);
                    }
                    for(int i=0; i < 2; i++){
                        presetHandler->clickedSensors.insert(QString("rotaryButton%1").arg(i), 0);
                    }
                    presetHandler->clickedSensors.insert(QString("lSliderButton0"), 0);
                }
                if(object->objectName().contains("vSliderButton")){
                    for(int i=0; i < 16; i++){
                        presetHandler->clickedSensors.insert(QString("padButton%1").arg(i), 0);
                    }
                    for(int i=0; i < 4; i++){
                        presetHandler->clickedSensors.insert(QString("hSliderButton%1").arg(i), 0);
                    }
                    for(int i=0; i < 11; i++){
                        presetHandler->clickedSensors.insert(QString("buttonButton%1").arg(i), 0);
                    }
                    for(int i=0; i < 2; i++){
                        presetHandler->clickedSensors.insert(QString("rotaryButton%1").arg(i), 0);
                    }
                    presetHandler->clickedSensors.insert(QString("lSliderButton0"), 0);
                }
                if(object->objectName().contains("buttonButton")){
                    for(int i=0; i < 16; i++){
                        presetHandler->clickedSensors.insert(QString("padButton%1").arg(i), 0);
                    }
                    for(int i=0; i < 4; i++){
                        presetHandler->clickedSensors.insert(QString("vSliderButton%1").arg(i), 0);
                    }
                    for(int i=0; i < 4; i++){
                        presetHandler->clickedSensors.insert(QString("hSliderButton%1").arg(i), 0);
                    }
                    for(int i=0; i < 2; i++){
                        presetHandler->clickedSensors.insert(QString("rotaryButton%1").arg(i), 0);
                    }
                    presetHandler->clickedSensors.insert(QString("lSliderButton0"), 0);
                }

                if(object->objectName().contains("buttonButton1")||
                        object->objectName().contains("buttonButton2")||
                        object->objectName().contains("buttonButton3")){
                    if(!object->objectName().contains("buttonButton10")){
                        for(int i=4; i < 11; i++){
                            presetHandler->clickedSensors.insert(QString("buttonButton%1").arg(i), 0);
                        }
                    }
                }
                if(object->objectName().contains("buttonButton4")||
                        object->objectName().contains("buttonButton5")||
                        object->objectName().contains("buttonButton6")||
                        object->objectName().contains("buttonButton7")){
                    for(int i=0; i < 11; i++){
                        if(i<4 || i>7){
                            presetHandler->clickedSensors.insert(QString("buttonButton%1").arg(i), 0);
                        }
                    }
                }
                if(object->objectName().contains("buttonButton9")||
                        object->objectName().contains("buttonButton10")){
                    for(int i=0; i < 11; i++){
                        if(i<9 || i>10){
                            presetHandler->clickedSensors.insert(QString("buttonButton%1").arg(i), 0);
                        }
                    }
                }
                if(object->objectName().contains("buttonButton8")){
                    for(int i=0; i < 11; i++){
                        if(i != 8){
                            presetHandler->clickedSensors.insert(QString("buttonButton%1").arg(i), 0);
                        }
                    }
                }
                if(object->objectName().contains("rotaryButton")){
                    for(int i=0; i < 16; i++){
                        presetHandler->clickedSensors.insert(QString("padButton%1").arg(i), 0);
                    }
                    for(int i=0; i < 4; i++){
                        presetHandler->clickedSensors.insert(QString("vSliderButton%1").arg(i), 0);
                    }
                    for(int i=0; i < 11; i++){
                        presetHandler->clickedSensors.insert(QString("buttonButton%1").arg(i), 0);
                    }
                    for(int i=0; i < 4; i++){
                        presetHandler->clickedSensors.insert(QString("hSliderButton%1").arg(i), 0);
                    }
                    presetHandler->clickedSensors.insert(QString("lSliderButton0"), 0);
                }
                if(object->objectName().contains("lSliderButton")){
                    for(int i=0; i < 16; i++){
                        presetHandler->clickedSensors.insert(QString("padButton%1").arg(i), 0);
                    }
                    for(int i=0; i < 4; i++){
                        presetHandler->clickedSensors.insert(QString("vSliderButton%1").arg(i), 0);
                    }
                    for(int i=0; i < 11; i++){
                        presetHandler->clickedSensors.insert(QString("buttonButton%1").arg(i), 0);
                    }
                    for(int i=0; i < 4; i++){
                        presetHandler->clickedSensors.insert(QString("hSliderButton%1").arg(i), 0);
                    }
                    for(int i=0; i < 2; i++){
                        presetHandler->clickedSensors.insert(QString("rotaryButton%1").arg(i), 0);
                    }
                }
                //check to make sure that if a sensor just got unclicked and it was the only one highlighted, it gets rehighlighted.
                if(presetHandler->clickedSensors.value(object->objectName()) == 0){
                    int anySensorClicked = 0;
                    for(int j = 0; j < presetHandler->sensorList.count(); j++){
                        if(presetHandler->clickedSensors.value(QString(presetHandler->sensorList.at(j))) == 1){
                            anySensorClicked = 1;
                        }
                    }
                    if (anySensorClicked == 0){
                        presetHandler->clickedSensors.insert(object->objectName(), 1);
                    }
                }
                //now the clickedSensors map has been updated.
                //query clickedSensors about whether or not each sensor is in a clicked state.
                //if it is, highlight it. if it isn't unhighlight.
                for(int j = 0; j < presetHandler->sensorList.count(); j++){
                    currentSensorClicked = presetHandler->clickedSensors.value(QString(presetHandler->sensorList.at(j)));
                    QString currentSensor = QString("%1").arg(presetHandler->sensorList.at(j));
                    QPixmap sensorHighlight;
                    QPixmap sensorUnHighlight;
                    QLabel* sensorLabel;
                    sensorLabel = this->findChild<QLabel *>(QString("%1").arg(presetHandler->sensorList.at(j)));
                    if(currentSensor.contains("padButton")){
                        //if current sensor is a padButton
                        sensorHighlight.load(QString(":Quneo/images/resources/images/padHighlight.png"));
                        sensorUnHighlight.load(QString(":Quneo/images/resources/images/padTransparent.png"));
                        if(currentSensorClicked){
                            sensorLabel->setPixmap(sensorHighlight);
                        }
                        else {
                            sensorLabel->setPixmap(sensorUnHighlight);
                        }
                    }
                    if(currentSensor.contains("hSliderButton")){
                        sensorHighlight.load(QString(":Quneo/images/resources/images/horizontalSliderHighlight.png"));
                        sensorUnHighlight.load(QString(":Quneo/images/resources/images/hSliderTransparent.png"));
                        if(currentSensorClicked){
                            sensorLabel->setPixmap(sensorHighlight);
                        }
                        else {
                            sensorLabel->setPixmap(sensorUnHighlight);
                        }
                    }
                    if(currentSensor.contains("vSliderButton")){
                        sensorHighlight.load(QString(":Quneo/images/resources/images/vSliderHighlight.png"));
                        sensorUnHighlight.load(QString(":Quneo/images/resources/images/vSliderTransparent.png"));
                        if(currentSensorClicked){
                            sensorLabel->setPixmap(sensorHighlight);
                        }
                        else {sensorLabel->setPixmap(sensorUnHighlight);
                        }
                    }
                    if(currentSensor.contains("rotaryButton")){
                        sensorHighlight.load(QString(":Quneo/images/resources/images/rotaryHighlight.png"));
                        sensorUnHighlight.load(QString(":Quneo/images/resources/images/rotaryTransparent.png"));
                        if(currentSensorClicked){
                            sensorLabel->setPixmap(sensorHighlight);
                        }
                        else {
                            sensorLabel->setPixmap(sensorUnHighlight);
                        }
                    }
                    if(currentSensor.contains("buttonButton1")||
                            currentSensor.contains("buttonButton2")||
                            currentSensor.contains("buttonButton3")){
                        if(!currentSensor.contains("buttonButton10")){
                            QString buttonButtonNumber = currentSensor;
                            buttonButtonNumber = buttonButtonNumber.remove(0,12);
                            sensorHighlight.load(QString(":Quneo/images/resources/images/highlightButton%1.png").arg(buttonButtonNumber));
                            sensorUnHighlight.load(QString(":Quneo/images/resources/images/modeTransparent.png"));
                            if(currentSensorClicked){
                                sensorLabel->setPixmap(sensorHighlight);
                            }
                            else {sensorLabel->setPixmap(sensorUnHighlight);
                            }
                        }
                    }
                    if(currentSensor.contains("buttonButton4")||
                            currentSensor.contains("buttonButton5")||
                            currentSensor.contains("buttonButton6")||
                            currentSensor.contains("buttonButton7")){
                        sensorHighlight.load(QString(":Quneo/images/resources/images/highlightButton4.png"));
                        sensorUnHighlight.load(QString(":Quneo/images/resources/images/modeTransparent.png"));
                        if(currentSensorClicked){
                            sensorLabel->setPixmap(sensorHighlight);
                        }
                        else {
                            sensorLabel->setPixmap(sensorUnHighlight);
                        }
                    }
                    if(currentSensor.contains("buttonButton8")){
                        sensorHighlight.load(QString(":Quneo/images/resources/images/highlightButton8.png"));
                        sensorUnHighlight.load(QString(":Quneo/images/resources/images/modeTransparent.png"));
                        if(currentSensorClicked){
                            sensorLabel->setPixmap(sensorHighlight);
                        }
                        else {
                            sensorLabel->setPixmap(sensorUnHighlight);
                        }
                    }
                    if(currentSensor.contains("buttonButton9")||currentSensor.contains("buttonButton10")){
                        sensorHighlight.load(QString(":Quneo/images/resources/images/highlightButton9.png"));
                        sensorUnHighlight.load(QString(":Quneo/images/resources/images/modeTransparent.png"));
                        if(currentSensorClicked){
                            sensorLabel->setPixmap(sensorHighlight);
                        }
                        else {
                            sensorLabel->setPixmap(sensorUnHighlight);
                        }
                    }
                }
            }
            else{
                //if it's a left click without shift then clear clicked sensor list
                for(int i=0; i < 16; i++){
                    presetHandler->clickedSensors.insert(QString("padButton%1").arg(i), 0);
                }
                for(int i=0; i < 4; i++){
                    presetHandler->clickedSensors.insert(QString("hSliderButton%1").arg(i), 0);
                }
                for(int i=0; i < 4; i++){
                    presetHandler->clickedSensors.insert(QString("vSliderButton%1").arg(i), 0);
                }
                for(int i=0; i < 11; i++){
                    presetHandler->clickedSensors.insert(QString("buttonButton%1").arg(i), 0);
                }
                for(int i=0; i < 2; i++){
                    presetHandler->clickedSensors.insert(QString("rotaryButton%1").arg(i), 0);
                }
                presetHandler->clickedSensors.insert(QString("lSliderButton0"), 0);
                //then set the current sensor to clicked so that it is
                //included if the next click is a shift click on a different sensor
                qDebug()<<QString("leftclick no shift: %1").arg(object->objectName());
                presetHandler->clickedSensors.insert(object->objectName(), 1);
            }
        }
    }
    return 0;
}


void MainWindow::slotGoToNextSensor(){
    //when action is triggered, check the current sensor name and number and add 1.
    QString nextSensor = copyPasteHandler->currentSensor;
    int nextSensorNumber = (copyPasteHandler->currentSensorNumber.toInt() + 1);

    if(nextSensor.contains("Pad")){
        if(nextSensorNumber > 15){
            nextSensorNumber = 0;           //if you are out of pads, reset sensor number to 0
            nextSensor = QString("hSliderButton%1").arg(nextSensorNumber);  //and send the next sensor type
        }
        else nextSensor = QString("padButton%1").arg(nextSensorNumber);
    }
    if(nextSensor.contains("HSlider")){
        if(nextSensorNumber > 3){
            nextSensorNumber = 0;
            nextSensor = QString("rotaryButton%1").arg(nextSensorNumber);
        }
        else nextSensor = QString("hSliderButton%1").arg(nextSensorNumber);
    }
    if(nextSensor.contains("Rotary")){
        if(nextSensorNumber > 1){
            nextSensorNumber = 0;
            nextSensor = QString("vSliderButton%1").arg(nextSensorNumber);
        }
        else nextSensor = QString("rotaryButton%1").arg(nextSensorNumber);
    }
    if(nextSensor.contains("VSlider")){
        if(nextSensorNumber > 3){
            nextSensorNumber = 0;
            nextSensor = QString("lSliderButton%1").arg(nextSensorNumber);
        }
        else nextSensor = QString("vSliderButton%1").arg(nextSensorNumber);
    }
    if(nextSensor.contains("LongSlider")){
        if(nextSensorNumber > 0){
            nextSensorNumber = 1;
            nextSensor = QString("buttonButton%1").arg(nextSensorNumber);
        }
        else nextSensor = QString("lSliderButton%1").arg(nextSensorNumber);
    }
    if(nextSensor.contains("TransportButton")){
        nextSensor = QString("buttonButton%1").arg(nextSensorNumber);
    }
    if(nextSensor.contains("LeftRightButton")){
        nextSensor = QString("buttonButton%1").arg(nextSensorNumber + 4);
    }
    if(nextSensor.contains("RhombusButton")){
        nextSensor = QString("buttonButton%1").arg(nextSensorNumber);
    }
    if(nextSensor.contains("UpDownButton")){
        nextSensorNumber = nextSensorNumber + 9;
        if(nextSensorNumber > 10){
            nextSensorNumber = 0;
            nextSensor = QString("padButton%1").arg(nextSensorNumber);
        }
        else {
            nextSensor = QString("buttonButton%1").arg(nextSensorNumber);
        }
    }

    //if it's a left click without shift then clear clicked sensor list
    for(int i=0; i < 16; i++) {
        presetHandler->clickedSensors.insert(QString("padButton%1").arg(i),0);
    }
    for(int i=0; i < 4; i++) {
        presetHandler->clickedSensors.insert(QString("hSliderButton%1").arg(i),0);
    }
    for(int i=0; i < 4; i++) {
        presetHandler->clickedSensors.insert(QString("vSliderButton%1").arg(i),0);
    }
    for(int i=0; i < 11; i++) {
        presetHandler->clickedSensors.insert(QString("buttonButton%1").arg(i),0);
    }
    for(int i=0; i < 2; i++) {
        presetHandler->clickedSensors.insert(QString("rotaryButton%1").arg(i),0);
    }
    presetHandler->clickedSensors.insert(QString("lSliderButton0"), 0);
    //then set the current sensor to clicked so that it is
    //included if the next click is a shiftclick on a different sensor
    presetHandler->clickedSensors.insert(nextSensor, 1);


    //send the string of the next sensor to select just as in event filter.
    disconnect(presetHandler, SIGNAL(signalPresetModified(bool)), presetHandler->updateIndicator, SLOT(slotPresetModified(bool)));
    emit signalEvents(nextSensor);
    connect(presetHandler, SIGNAL(signalPresetModified(bool)), presetHandler->updateIndicator, SLOT(slotPresetModified(bool)));
}

void MainWindow::slotGoToPrevSensor(){
    //when action is triggered, check the current sensor name and number and add 1.
    QString nextSensor = copyPasteHandler->currentSensor;
    int nextSensorNumber = (copyPasteHandler->currentSensorNumber.toInt() - 1);

    if(nextSensor.contains("Pad")){
        if(nextSensorNumber < 0){
            nextSensorNumber = 10;           //if you are out of pads, reset sensor number to 0
            nextSensor = QString("buttonButton%1").arg(nextSensorNumber);  //and send the next sensor type
        }
        else nextSensor = QString("padButton%1").arg(nextSensorNumber);
    }
    if(nextSensor.contains("HSlider")){
        if(nextSensorNumber < 0){
            nextSensorNumber = 15;
            nextSensor = QString("padButton%1").arg(nextSensorNumber);
        }
        else nextSensor = QString("hSliderButton%1").arg(nextSensorNumber);
    }
    if(nextSensor.contains("Rotary")){
        if(nextSensorNumber < 0){
            nextSensorNumber = 3;
            nextSensor = QString("hSliderButton%1").arg(nextSensorNumber);
        }
        else nextSensor = QString("rotaryButton%1").arg(nextSensorNumber);
    }
    if(nextSensor.contains("VSlider")){
        if(nextSensorNumber < 0){
            nextSensorNumber = 1;
            nextSensor = QString("rotaryButton%1").arg(nextSensorNumber);
        }
        else nextSensor = QString("vSliderButton%1").arg(nextSensorNumber);
    }
    if(nextSensor.contains("LongSlider")){
        if(nextSensorNumber < 0){
            nextSensorNumber = 3;
            nextSensor = QString("vSliderButton%1").arg(nextSensorNumber);
        }
        else nextSensor = QString("lSliderButton%1").arg(nextSensorNumber);
    }
    if(nextSensor.contains("TransportButton")){
        if(nextSensorNumber < 1){
            nextSensorNumber = 0;
            nextSensor = QString("lSliderButton%1").arg(nextSensorNumber);
        }
        else {nextSensor = QString("buttonButton%1").arg(nextSensorNumber);}
    }
    if(nextSensor.contains("LeftRightButton")){
        nextSensor = QString("buttonButton%1").arg(nextSensorNumber + 4);
    }
    if(nextSensor.contains("RhombusButton")){
        nextSensor = QString("buttonButton%1").arg(nextSensorNumber);
    }
    if(nextSensor.contains("UpDownButton")){
        nextSensorNumber = nextSensorNumber + 9;
        if(nextSensorNumber < 9){
            nextSensorNumber = 8;
            nextSensor = QString("buttonButton%1").arg(nextSensorNumber);
        }
        else {nextSensor = QString("buttonButton%1").arg(nextSensorNumber);}
    }
    //if it's a left click without shift then clear clicked sensor list
    for(int i=0; i < 16; i++) {
        presetHandler->clickedSensors.insert(QString("padButton%1").arg(i), 0);
    }
    for(int i=0; i < 4; i++) {
        presetHandler->clickedSensors.insert(QString("hSliderButton%1").arg(i), 0);
    }
    for(int i=0; i < 4; i++) {
        presetHandler->clickedSensors.insert(QString("vSliderButton%1").arg(i), 0);
    }
    for(int i=0; i < 11; i++) {
        presetHandler->clickedSensors.insert(QString("buttonButton%1").arg(i), 0);
    }
    for(int i=0; i < 2; i++) {
        presetHandler->clickedSensors.insert(QString("rotaryButton%1").arg(i), 0);
    }
    presetHandler->clickedSensors.insert(QString("lSliderButton0"), 0);
    //then set the current sensor to clicked so that it is
    //included if the next click is a shift click on a different sensor
    presetHandler->clickedSensors.insert(nextSensor, 1);

    //send the string of the next sensor to select just as in event filter.
    disconnect(presetHandler, SIGNAL(signalPresetModified(bool)), presetHandler->updateIndicator, SLOT(slotPresetModified(bool)));
    emit signalEvents(nextSensor);
    connect(presetHandler, SIGNAL(signalPresetModified(bool)), presetHandler->updateIndicator, SLOT(slotPresetModified(bool)));
}

//void MainWindow::slotPopulateDeviceMenu(QList<QString> list){
//deviceMenu->clear();
//deviceMenu->insertItems(0,list);
//}

void MainWindow::closeEvent(QCloseEvent *event){

    switch(maybeSave()){
    case 0:{      //if user selects discard, return 0. don't save, accept close event.
        event->accept();
        break;}
    case 1:{      //if user selects cancel, return 1. ignore close event, don't save.
        event->ignore();
        break;}
    case 2:{      //if user selects save, return 2. save changes, then exit app.
        presetHandler->slotSaveAllPresets();
        event->accept();
        break;}
    case 3:{    //if there no unsaved changes, function returns 3. exit app.
        qDebug("No unsaved changes. Exiting...");
        event->accept();
        break;
    }
    }

}

int MainWindow::maybeSave()
{
    if (presetHandler->updateIndicator->slotCheckForUnsavedPresets()) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Application"),
                                   tr("You have unsaved data in your presets.\n"
                                      "Do you want to save your changes?"),
                                   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return 2;
        else if (ret == QMessageBox::Cancel)
            return 1;
        else if (ret == QMessageBox::Discard)
            return 0;
    }
//    else
//    {
        return 3;
//    }
    //if there are unsaved changes on close, open message box.
}

//int MainWindow::firmwareUpdateDialog(bool upToDate)
//{
//    //return 1; // debug
//    qDebug() << "fw update DIALOG";

//    rogueQueryDevice = false;

//    if(rogueWarning.isVisible())
//    {
//        rogueWarning.close();
//        //midiDeviceAccess->replyTimeout->stop();
//    }

//    if(!fwUpdateDialogAuto.isVisible())
//    {
//        if(!upToDate)
//        {
//            int ret = fwUpdateDialogAuto.exec();
//            qDebug() << "FW UPDATE CALLED";

//            if(ret == QMessageBox::Ok)
//            {

//                slotShowFWUpdateDialog();
//                midiDeviceAccess->slotUpdateFirmware();
//                progressDialog();

//                //firmwareUpdateCompleteDialog();
//                return 1;
//            }
//            else if(ret == QMessageBox::Cancel)
//            {
//                qDebug() << "CAncleelll";
//                return 0;
//            }
//        }
//        else
//        {
//            if(progress->isVisible())
//            {
//                progress->close();
//                firmwareUpdateCompleteDialog();
//            }
//            return 1; // added to clear warning

//        }
//    }
//    return 0; // added to clear warning
//}

//void MainWindow::progressDialog(){

//    emit sigFwProgressDialogOpen(true);
//    //progress->show(); // debug
//}

//void MainWindow::firmwareUpdateCompleteDialog(){



//    updateComplete.exec();

//    emit sigFwProgressDialogOpen(false);

//    midiDeviceAccess->slotLoadPreset();

//    //*************************//
//    // Here we will need to later instantiate preset acquisition
//    //and reformatting if versions differ instead of loading preset
//    //*************************//

//    //qDebug() << "update completed";
//}

//void MainWindow::slotUpdateFwProgressDialog(int val){

//    qDebug() << "called slotUpdateFwProgressDialog";

//    if(val != 0){
//        qDebug() << "setvalue: " << val;
//        progress->setValue(totalFwBytes - val);
//    } else {
//        progress->setValue(totalFwBytes - val);

//        firmwareUpdateCompleteDialog();
//    }
//}

void MainWindow::slotUpdateAllPresetsProgress(int val){

     qDebug() << "slot update all Presets Progress called" << val;


    if(val != 16){
#ifndef Q_OS_MAC
        updateAllPresetsProgressDialog->show();
#endif
        updateAllPresetsProgressDialog->show();
        updateAllPresetsProgressDialog->setValue(val);
        qDebug() << "update val: " << val;
    } else {
        updateAllPresetsProgressDialog->setValue(16);
        updateAllPresetsCompleteMsgBox.setText("Update All Presets Complete");
        updateAllPresetsCompleteMsgBox.exec();
    }

}

void MainWindow::centerWidgetOnScreen (QWidget * widget) {
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect  screenGeometry = screen->geometry();
//    int height = screenGeometry.height();
//    int width = screenGeometry.width();


//    QRect rect = QApplication::desktop()->availableGeometry();
//    widget->move(rect.center() - widget->rect().center());

    widget->move(screenGeometry.center() - widget->rect().center());
}

//int MainWindow::firmwareUpdateDialogMenu(bool upToDate)
//{
//    if(!fwUpdateDialogManual.isVisible())
//    {
//        if(!upToDate)
//        {
//            int ret = fwUpdateDialogManual.exec();
//            //qDebug() << "FW UPDATE CALLED";

//            if(ret == QMessageBox::Ok)
//            {
//                slotShowFWUpdateDialog();
//                midiDeviceAccess->slotUpdateFirmware();
//                progressDialog();

//                //firmwareUpdateCompleteDialog();
//                return 1;
//            }
//            else if(ret == QMessageBox::Cancel)
//            {
//                return 0;
//            }
//        }
//        else
//        {
//            if(progress->isVisible())
//            {
//                progress->close();
//                firmwareUpdateCompleteDialog();
//            }
//            return 1; // added to clear warning
//        }
//    }
//    return 0; // added to clear warning
//}

void MainWindow::slotRogueWarning()
{
    //qDebug() << "------ slotRogueWarning called";
    rogueQueryDevice = true;

    if(!rogueWarning.isVisible())
    {
        //if(!ui->deviceMenu->currentText().contains("None"))
        if (connected)
        {
            qDebug() << "pre exec";
            //rogueQueryDevice = true;
            int rtrn = rogueWarning.exec();
            qDebug() << "post exec";

            if(rtrn == QMessageBox::Ok && rogueQueryDevice)
            {
                qDebug() << "rogue query";
                // EB TODO - check firmware here?
                //midiDeviceAccess->slotCheckFirmwareVersion();
            }
        }
        else
        {
            qDebug() << "warning thrown with no devices connected";
        }
    }
}

void MainWindow::slotShowUpdateAllDialog()
{
    updateAllPresetsProgressDialog->open();
    updateAllPresetsProgressDialog->show();
}

//void MainWindow::slotShowFWUpdateDialog()
//{
//    /*
//    progress = new QProgressDialog("Updating Firmware...", "Cancel", 0, totalFwBytes, this);
//    progress->setWindowModality(Qt::WindowModal);
//    progress->setCancelButton(0);
//    */
//    progress->open();
//    progress->show();
//}


// --------------------------------------------------------------------------------------
// ------ midi overhaul -----------------------------------------------------------------
// --------------------------------------------------------------------------------------

void MainWindow::slotMIDIPortChange(QString portName, uchar inOrOut, uchar messageType, int portNum)
{
    //qDebug() << "slotMIDIPortChange - " << kmiPorts->mType[messageType] << kmiPorts->inOut[inOrOut] << " portName:" << portName << " messageType: " << " portNum: " << portNum << "\n";

    switch (messageType)
    {
    case PORT_CONNECT:

        // update dropdown
        if (inOrOut == PORT_OUT && !portName.toUpper().contains("QUNEO"))
        {
            // EB TODO - add this dropdown for the aux output
//            QComboBox *thisDropDown = ui->midi_outputs;

//            thisDropDown->addItem(portName); // update dropdown
//            slotFixDropDownWidth(thisDropDown);
        }

        // **** QuNeo connect *****************************************
        // use .toUpper() because firmware < 1.31 is uppercase, 1.31 and later are "QuNeo"
        if (portName.toUpper() == QString(QUNEO_IN_P1).toUpper() && inOrOut == PORT_IN)
        {
            QByteArray thisFw = QByteArray(reinterpret_cast<char*>(_fw_ver_quneo), sizeof(_fw_ver_quneo));

            QuNeo->slotSetExpectedFW(thisFw);
            //qDebug() << "qn deviceName: " << BopPad->deviceName << " curfw is: " << BopPad->currentFwVer;
            QuNeo->updatePortIn(portNum);
            fwUpdateWindow->slotAppendTextToConsole("\nQuNeo Connected\n");
        }
        else if (portName.toUpper() == QString(QUNEO_OUT_P1).toUpper() && inOrOut == PORT_OUT)
        {
            QuNeo->updatePortOut(portNum);
            QuNeo->slotStartPolling("slotMIDIPortChange"); // start polling when output port is added
        }

        break;
    case PORT_DISCONNECT:

        if (inOrOut == PORT_OUT)
        {
            // update dropdown EB TODO - implement this
//            if (ui->midi_outputs->currentText() == portName)
//                ui->midi_outputs->setCurrentIndex(0);

//            ui->midi_outputs->removeItem(ui->midi_outputs->findText(portName));
        }

        // **** QuNeo disconnect ***************************************
        if (portName.toUpper() == QString(QUNEO_IN_P1).toUpper())
        {
            // close ports and stop polling
            QuNeo->slotCloseMidiIn();
            QuNeo->slotCloseMidiOut();
            QuNeo->slotStopPolling("slotMIDIPortChange - disconnect");
            if (inOrOut == PORT_IN) fwUpdateWindow->slotAppendTextToConsole("\nQuNeo Disconnected\n");
        }

        break;
    case PORT_CHANGED:
        qDebug() << " PORT CHANGED - name: " << portName << portName << " inOrOut: " << kmiPorts->inOut[inOrOut] << " messageType: " << kmiPorts->mType[messageType] << " portNum: " << portNum << "\n";

        // **** QuNeo renumber *****************************************
        if (portName.toUpper() == QString(QUNEO_IN_P1).toUpper() && inOrOut == PORT_IN)
        {
            QuNeo->updatePortIn(portNum);
        }
        else if (portName.toUpper() == QString(QUNEO_OUT_P1).toUpper() && inOrOut == PORT_OUT)
        {
            QuNeo->updatePortOut(portNum);
        }

        break;
    default:
        break;
    }
}

// close and then reopen the QuNexus ports
// this is needed when the bootloader and app port names do not match
void MainWindow::slotRefreshConnection()
{
    qDebug() << "slotRefreshConnection called";
#ifndef Q_OS_WIN
    QuNeo->slotResetConnections(QUNEO_IN_P1, QUNEO_BL_PORT);
#endif
}

void MainWindow::slotBootloaderMode(bool fwUpdateRequested)
{
    qDebug() << "slotBootloaderMode called - fwUpdateRequested: "<< fwUpdateRequested;
    if (!fwUpdateRequested)
    {
        QMessageBox msgBox;
        msgBox.setText("Your device is in bootloader mode. Hit OK to attempt a firmware update.");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Ok);
        int ret = msgBox.exec();

        if(ret == QMessageBox::Ok)
        {
            slotForceFirmwareUpdate();
        }
    }
}

void MainWindow::slotFwUpdateSuccessCloseDialog(bool success)
{
    qDebug() << "slotFwUpdateSuccessCloseDialog called - success: " << success;

    if (success)
    {
        QuNeo->fwUpdateRequested = false;

        slotUpdateMIDIaux();

        slotQuNeoConnected(true);
        midiDeviceAccess->slotUpdateAllPresets(); // send all presets
        //slotEnableDisableMidiFunctions(true);
        //slotSyncQuNeoDialog();
    }
    else
    {
        QuNeo->slotFirmwareUpdateReset();
        //slotQuNeoConnected(false);
        //slotEnableDisableMidiFunctions(false);
    }

}

void MainWindow::slotForceFirmwareUpdate()
{
    slotFirmwareDetected(QuNeo, false); // act as if we received a firmware mismatch
}

void MainWindow::slotFirmwareDetected(MidiDeviceManager *thisMDM, bool matches)
{
    qDebug() << "slotFirmwareDetected called";
    if (matches)
    {
        qDebug() << "FirmwareMatch: " << thisMDM->PID << "name:" << thisMDM->deviceName;

        //---------------------------------- Sync QuNeo Dialog
        // MIDI Overhaul

        //slotSyncQuNeoDialog();
    }
    else
    {
        qDebug() << "Firmware MisMatch: " << thisMDM->PID << "name:" << thisMDM->deviceName;

        // setup sysex connections to receive globals data
        QuNeo->disconnect(SIGNAL(signalRxSysExBA(QByteArray))); // disconnect to be safe

        // QuNeo only listens to the device ID response
        // EB TODO - detect rogue mode
        //connect(QuNeo, SIGNAL(signalRxSysExBA(QByteArray)), sysExEncDecode, SLOT(slotProcessSysEx(QByteArray)));

        qDebug() << "prepare fwUpdateWindow";
        fwUpdateWindow->slotClearText();
        fwUpdateWindow->slotAppendTextToConsole(deviceBootloaderVersionString());
        fwUpdateWindow->slotAppendTextToConsole(deviceFirmwareVersionString());

        qDebug() << "show";
        fwUpdateWindow->show();
    }
}


void MainWindow::slotQuNeoConnected(bool state)
{
    qDebug() << "slotQuNeoConnected called - state: " << state;
    if(state)
    {
        QLabel* connectedLabel = this->findChild<QLabel *>("QuNeo_Connected_Label");
        connectedLabel->setText("Connected");

        QFrame* connectedFrame = this->findChild<QFrame *>("QuNeo_Connected_Frame");
        connectedFrame->setStyleSheet("border: 1px solid rgb(67,67,67); background:rgb(10,255,0); border-radius:6;");
        qDebug() << "<< set connected: true";
        connected = true;
        midiDeviceAccess->connected = true;

        QuNeo->slotStopPolling("slotQuNeoConnected - true");

        //presetManager->syncFlag = true;
    }
    else
    {
        QLabel* connectedLabel = this->findChild<QLabel *>("QuNeo_Connected_Label");
        connectedLabel->setText("Not Connected");

        QFrame* connectedFrame = this->findChild<QFrame *>("QuNeo_Connected_Frame");
        connectedFrame->setStyleSheet("border: 1px solid rgb(67,67,67); background: rgb(100,100,100); border-radius:6;");
        qDebug() << "<< set connected: false";

        // safety to ensure we don't connect this twice
        if (connected)
        {
            QuNeo->disconnect(SIGNAL(signalRxSysExBA(QByteArray)));
        }
        connected = false;
        midiDeviceAccess->connected = false;

//        presetManager->syncFlag = false; // update presetmanager
    }
}

// connect QuNeo midi input to to midi aux out
void MainWindow::slotUpdateMIDIaux()
{
    qDebug() << "slotUpdateMIDIaux called";

    QuNeo->disconnect(SIGNAL(signalRxMidi_raw(uchar, uchar, uchar, uchar)));

    if (!connected) return; // don't continue if we aren't connected

    // EB TODO - implement midi aux ports
//    if (ui->midi_outputs->currentText() != "None")
//    {
//        // set and open the ports
//        int thisOutPort = kmiPorts->getOutPortNumber(ui->midi_outputs->currentText());
//        midiAuxOut->updatePortOut(thisOutPort);
//        midiAuxOut->slotOpenMidiOut();
//        connect(QuNeo, SIGNAL(signalRxMidi_raw(uchar, uchar, uchar, uchar)), midiAuxOut, SLOT(slotSendMIDI(uchar, uchar, uchar, uchar)));
//    }
//    else
//    {
//        midiAuxOut->slotCloseMidiOut();
//    }
}

QString MainWindow::deviceBootloaderVersionString()
{
    return QString("Device Bootloader Version: %1.%2\n\n")
            .arg(uchar(QuNeo->devicebootloaderVersion.at(0)))
            .arg(uchar(QuNeo->devicebootloaderVersion.at(1)));
}


QString MainWindow::deviceFirmwareVersionString()
{
    return QString("Device Firmware Version: %1.%2.%3")
            .arg(uchar(QuNeo->deviceFirmwareVersion.at(0)))
            .arg(uchar(QuNeo->deviceFirmwareVersion.at(1)))
            .arg(uchar(QuNeo->deviceFirmwareVersion.at(2)));
}

QString MainWindow::applicationFirmwareVersionString()
{
    return QString("Application Firmware Version: %1.%2.%3\n\n")
            .arg(uchar(thisFw.at(0)))
            .arg(uchar(thisFw.at(1)))
            .arg(uchar(thisFw.at(2)));
}

QString MainWindow::applicationVersionString()
{

    return QString("Application Version: %1.%2.%3%4\n\n")
            .arg(uchar(applicationVersion.at(0)))
            .arg(uchar(applicationVersion.at(1)))
            .arg(uchar(applicationVersion.at(2)))
            .arg(betaVersion);

}

void MainWindow::slotUpdateAboutMenuVersions()
{
    // update cph version strings
    copyPasteHandler->applicationFirmwareVersionString = applicationFirmwareVersionString();
    copyPasteHandler->applicationVersionString = applicationVersionString();
    copyPasteHandler->deviceFirmwareVersionString = deviceFirmwareVersionString();
    copyPasteHandler->deviceBootloaderVersionString = deviceBootloaderVersionString();
}


// --------------------------------------------------------------------------------------
// ------ end midi overhaul -------------------------------------------------------------
// --------------------------------------------------------------------------------------



