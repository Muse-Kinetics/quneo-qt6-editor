#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QEvent>
#include <QMouseEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
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

    //*****MIDI*****//


    qDebug() << "setup MIDI";
    midiDeviceAccess = new MidiDeviceAccess(&presetHandler->presetMapsCopy,this);
    connect(presetHandler->presetMenu, SIGNAL(currentTextChanged(QString)), midiDeviceAccess, SLOT(slotSetCurrentPreset(QString)));
    connect(midiDeviceAccess, SIGNAL(sigRogueWarning()), this, SLOT(slotRogueWarning()));

    copyPasteHandler = new CopyPasteHandler(presetHandler, midiDeviceAccess,this, dataValidator, 0);
    connect(copyPasteHandler, SIGNAL(sigUpdateFirmware(bool)), this, SLOT(firmwareUpdateDialogMenu(bool)));
    //connect(midiDeviceAccess, SIGNAL(sigSetVersions(QString,QString)), copyPasteHandler, SLOT(slotSetVersions(QString,QString)));

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
    toNextSensor->setShortcut(Qt::CTRL + Qt::Key_Right);
    connect(toNextSensor, SIGNAL(triggered()), this, SLOT(slotGoToNextSensor()));

    toPrevSensor = new QAction(tr("&Edit Prev Sensor"), this);
    copyPasteHandler->editMenu->addAction(toPrevSensor);
    toPrevSensor->setShortcut(Qt::CTRL + Qt::Key_Left);
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

    //Save button
    // EB commented out to avoid crash
    qDebug() << "Connect PresetHandler save button";
    connect(presetHandler->saveButton, SIGNAL(clicked()), presetHandler, SLOT(slotSave()));

    qDebug() << "Firmware update stuff";
    //Firmware Updating Stuff ***********
    firmwareUpdate = this->findChild<QPushButton *>(QString("updateFirmwareButton"));
    firmwareUpdate->hide();
    connect(midiDeviceAccess, SIGNAL(sigFirmwareCurrent(bool)), this, SLOT(firmwareUpdateDialog(bool)));
    centerWidgetOnScreen(this);

    updateComplete.setText("Firmware Sent. \n\nAfter hitting OK please press Update All.");
    updateComplete.move(this->width()/2, this->height()/2);

    fwUpdateDialogAuto.setIcon(QMessageBox::Warning);
    fwUpdateDialogAuto.addButton(QMessageBox::Cancel);
    fwUpdateDialogAuto.addButton(QMessageBox::Ok);
    fwUpdateDialogAuto.setText(QString("The firmware on the selected QuNeo is out of date.\n""Click OK to update your firmware."));
    fwUpdateDialogAuto.move(this->width()/2, this->height()/2);

    fwUpdateDialogManual.setIcon(QMessageBox::Warning);
    fwUpdateDialogManual.addButton(QMessageBox::Cancel);
    fwUpdateDialogManual.addButton(QMessageBox::Ok);
    fwUpdateDialogManual.setText(QString("Click OK to update your firmware."));
    fwUpdateDialogManual.move(this->width()/2, this->height()/2);

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

    connect(midiDeviceAccess, SIGNAL(sigFwBytesLeft(int)), this, SLOT(slotUpdateFwProgressDialog(int)));
    //connect(midiDeviceAccess, SIGNAL(sigShowFWUpdateDialog()), this, SLOT(slotShowFWUpdateDialog()));
    // this is moved to slotShowFWUpdateDialog()
    progress = new QProgressDialog("Updating Firmware...", "Cancel", 0, totalFwBytes, this);
    //progress->setWindowModality(Qt::WindowModal);
    progress->setCancelButton(0);
    //progress->setValue(1000);
    progress->close();

    //************ THIS SHOULD BE DONE LAST, AFTER APP IS "CONSTRUCTED" / LOADED ****************//
    midiDeviceAccess->getSourcesDests(); //populate midi devices

#else
    //windows progress bar...

    progress = new QProgressDialog("Updating Firmware...", "Cancel", 0, 0, this);
    progress->setWindowModality(Qt::WindowModal);
    progress->setCancelButton(0);

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
    else {return 3;}
    //if there are unsaved changes on close, open message box.
}

int MainWindow::firmwareUpdateDialog(bool upToDate)
{
    //return 1; // debug
    qDebug() << "fw update DIALOG";

    rogueQueryDevice = false;

    if(rogueWarning.isVisible())
    {
        rogueWarning.close();
        //midiDeviceAccess->replyTimeout->stop();
    }

    if(!fwUpdateDialogAuto.isVisible())
    {
        if(!upToDate)
        {
            int ret = fwUpdateDialogAuto.exec();
            qDebug() << "FW UPDATE CALLED";

            if(ret == QMessageBox::Ok)
            {

                slotShowFWUpdateDialog();
                midiDeviceAccess->slotUpdateFirmware();
                progressDialog();

                //firmwareUpdateCompleteDialog();
                return 1;
            }
            else if(ret == QMessageBox::Cancel)
            {
                qDebug() << "CAncleelll";
                return 0;
            }
        }
        else
        {
            if(progress->isVisible())
            {
                progress->close();
                firmwareUpdateCompleteDialog();
            }

        }
    }

}

void MainWindow::progressDialog(){

    emit sigFwProgressDialogOpen(true);
    //progress->show(); // debug
}

void MainWindow::firmwareUpdateCompleteDialog(){



    updateComplete.exec();

    emit sigFwProgressDialogOpen(false);

    midiDeviceAccess->slotLoadPreset();

    //*************************//
    // Here we will need to later instantiate preset acquisition
    //and reformatting if versions differ instead of loading preset
    //*************************//

    //qDebug() << "update completed";
}

void MainWindow::slotUpdateFwProgressDialog(int val){

    qDebug() << "called slotUpdateFwProgressDialog";

    if(val != 0){
        qDebug() << "setvalue: " << val;
        progress->setValue(totalFwBytes - val);
    } else {
        progress->setValue(totalFwBytes - val);

        firmwareUpdateCompleteDialog();
    }
}

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

int MainWindow::firmwareUpdateDialogMenu(bool upToDate)
{
    if(!fwUpdateDialogManual.isVisible())
    {
        if(!upToDate)
        {
            int ret = fwUpdateDialogManual.exec();
            //qDebug() << "FW UPDATE CALLED";

            if(ret == QMessageBox::Ok)
            {
                slotShowFWUpdateDialog();
                midiDeviceAccess->slotUpdateFirmware();
                progressDialog();

                //firmwareUpdateCompleteDialog();
                return 1;
            }
            else if(ret == QMessageBox::Cancel)
            {
                return 0;
            }
        }
        else
        {
            if(progress->isVisible())
            {
                progress->close();
                firmwareUpdateCompleteDialog();
            }
        }
    }
}

void MainWindow::slotRogueWarning()
{
    //qDebug() << "------ slotRogueWarning called";
    rogueQueryDevice = true;

    if(!rogueWarning.isVisible())
    {
        if(!ui->deviceMenu->currentText().contains("None"))
        {
            qDebug() << "pre exec";
            //rogueQueryDevice = true;
            int rtrn = rogueWarning.exec();
            qDebug() << "post exec";

            if(rtrn == QMessageBox::Ok && rogueQueryDevice)
            {
                qDebug() << "rogue query";
                midiDeviceAccess->slotCheckFirmwareVersion();
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

void MainWindow::slotShowFWUpdateDialog()
{
    /*
    progress = new QProgressDialog("Updating Firmware...", "Cancel", 0, totalFwBytes, this);
    progress->setWindowModality(Qt::WindowModal);
    progress->setCancelButton(0);
    */
    progress->open();
    progress->show();
}
