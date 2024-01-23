#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QEvent>
#include <QMouseEvent>
#include "KMI_FwVersions.h"
#include "inc/KMI_Updates/kmi_updates.h"

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
    applicationVersion[2] = 3;

    betaVersion = ""; // release, do not show beta string

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
    connect(kmiPorts, SIGNAL(signalPortUpdated(QString,uchar,uchar,int)),
            this, SLOT(slotMIDIPortChange(QString,uchar,uchar,int)));

    //qDebug() << "end connect";

    // ******************************
    // create KMI device handlers
    // ******************************

    QuNeo = new MidiDeviceManager(this, PID_QUNEO, "QuNeo", kmiPorts);

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
    MIDIThru = new MidiDeviceManager(this, PID_AUX, "MIDI Thru", kmiPorts);

    // ******************************
    // end KMI_Ports and device handlers
    // ******************************

    connected = false;

    // **********************************
    // Load Session Settings and File locations
    // **********************************

    // get session settings, make sure to pass this to objects that need them (ie preset interfaces looking for json files)
    settings = new QSettings(this); //session settings allow us to enable/disable tooltips

    // default file location
    const QString DEFAULT_DIR_KEY("default_dir");

    // test if this is a directory
    QFileInfo check_file(settings->value(DEFAULT_DIR_KEY, QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)).toString());
    if (!check_file.exists() || !check_file.isDir() || settings->value(DEFAULT_DIR_KEY).toString().contains("Contents/MacOS"))
    {
        QString desktop = QStandardPaths::locate(QStandardPaths::DesktopLocation, QString(), QStandardPaths::LocateDirectory);
        qDebug() << "Desktop: " << desktop;
        settings->setValue(DEFAULT_DIR_KEY, desktop);     // if key doesn't exist, set it to desktop
    }

    qDebug() << "Default file save location: " << settings->value(DEFAULT_DIR_KEY).toString();


    // check for updates
    QString JSONVersionCheckUrl = "https://files.keithmcmillen.com/products/quneo/editor/softwareVersionCheck.json";
    checkUpdates = new KMI_Updates(this, "QuNeo", settings, applicationVersion, JSONVersionCheckUrl);

    // ---- End MIDI Overhaul --------------------------------------------

    //*****set up the ui
    ui->setupUi(this);

    // uncomment to work with layouts
    //this->addUIToLayouts();

    this->setWindowTitle("QuNeo Editor");

    //
    qDebug() << "new DataValidator";
    dataValidator = new DataValidator();

    qDebug() << "DONE";


    // ---- FONTS --------------------------
    qDebug() << "------------ [FONTS SETUP] ---------------------------------------------------";

    QString droidFont = ":/fonts/droid-sans/DroidSansMono.ttf";
    QString futuraFont = ":/fonts/futura/futura-normal.ttf";
    QString futuraBFont = ":/fonts/futura/Futura-Bold.ttf";
    QString corbelFont = ":/fonts/corbel/corbel.ttf";
    QString corbelBFont = ":/fonts/corbel/corbelb.ttf";

    if (QFontDatabase::addApplicationFont(droidFont) == -1) qDebug() << "Could not load font: " << droidFont;
    if (QFontDatabase::addApplicationFont(futuraFont) == -1) qDebug() << "Could not load font: " << futuraFont;
    if (QFontDatabase::addApplicationFont(futuraBFont) == -1) qDebug() << "Could not load font: " << futuraBFont;
    if (QFontDatabase::addApplicationFont(corbelFont) == -1) qDebug() << "Could not load font: " << corbelFont;
    if (QFontDatabase::addApplicationFont(corbelBFont) == -1) qDebug() << "Could not load font: " << corbelBFont;

    // ---- end FONTS -------------------------

    // general stylesheets
#ifdef Q_OS_MAC
    QuNeoStylesFile = new QFile(":/resources/stylesheets/QuNeoStyles.qss");
    dialogStylesFile = new QFile(":/stylesheets/appDialog_QuNexus.qss");
#else
    QuNeoStylesFile = new QFile(":/resources/stylesheets/QuNeoStylesWindows.qss");
    dialogStylesFile = new QFile(":/stylesheets/appDialog_QuNexus_WIN.qss");
#endif

    if (!QuNeoStylesFile->open(QFile::ReadOnly))
    {
        qDebug() << "ERROR: could not open stylesheet: " << QuNeoStylesFile->fileName();
    }
    else
    {
        QuNeoStylesString = QLatin1String(QuNeoStylesFile->readAll());
        QMainWindow::setStyleSheet(QuNeoStylesString);
    }

    if (!dialogStylesFile->open(QFile::ReadOnly))
    {
        qDebug() << "ERROR: could not open stylesheet: " << dialogStylesFile->fileName();
    }
    else
    {
        dialogStylesString = QLatin1String(dialogStylesFile->readAll());
    }

    //StyleSheets for "grey" pushbuttons
    grayStyleFile = new QFile(":/stylesheets/GrayButtonStyleSheet.qss");
    grayStyleFile->open(QFile::ReadOnly);
    grayStyleString = QLatin1String(grayStyleFile->readAll());

    qDebug() << "Delaying connections until main window loads";
    QTimer::singleShot(0, this, &MainWindow::windowHasLoaded);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::windowHasLoaded()
{
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
    connect(ui->midi_thru, SIGNAL(currentIndexChanged(int)), this, SLOT(slotUpdateMIDIaux()));
    connect(QuNeo, SIGNAL(signalConnected(bool)), this, SLOT(slotUpdateMIDIaux()));


    // Firmware update Window
    fwUpdateWindow = new fwUpdate(this, "QuNeo", applicationFirmwareVersionString());
    //fwUpdateWindow->setStyleSheet(generalStylesString);

    // Troubleshooting Window
    troubleshootWindow = new troubleshoot(this, "QuNeo", applicationFirmwareVersionString());
    //troubleshootWindow->setStyleSheet(generalStylesString);
    troubleshootWindow->hide();

    connect(kmiPorts, SIGNAL(signalInputCount(int)), troubleshootWindow, SLOT(slotInputCount(int)));
    connect(kmiPorts, SIGNAL(signalOutputCount(int)), troubleshootWindow, SLOT(slotOutputCount(int)));
    connect(kmiPorts, SIGNAL(signalInputPort(QString, int)), troubleshootWindow, SLOT(slotInputPort(QString, int)));
    connect(kmiPorts, SIGNAL(signalOutputPort(QString, int)), troubleshootWindow, SLOT(slotOutputPort(QString, int)));

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

    // cph also handles the menu so we need to connect the connected label to troubleshooter here
    connect(ui->QuNeo_Connected_Label, SIGNAL(clicked()), this, SLOT(slotOpenTroubleshooting()));
    connect(copyPasteHandler->troubleShooting, SIGNAL(triggered()), this, SLOT(slotOpenTroubleshooting()));

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

    // connect fwUpdate console messages to connection troubleshooter
    connect(QuNeo, SIGNAL(signalFwConsoleMessage(QString)), troubleshootWindow, SLOT(slotAppendToStatusLog(QString)));
    connect(fwUpdateWindow, SIGNAL(signalRequestFwUpdate()), troubleshootWindow, SLOT(slotRequestFwUpdate()));
    connect(QuNeo, SIGNAL(signalFirmwareUpdateComplete(bool)), troubleshootWindow, SLOT(slotFirmwareUpdated(bool)));

    // handle device unexpectedly in bootloader mode
    connect(QuNeo, SIGNAL(signalBootloaderMode(bool)), this, SLOT(slotBootloaderMode(bool)));

    // reset portlist after sending bootloader commands, catch changes to port names
//    connect(QuNeo, SIGNAL(signalBeginBlTimer()), this, SLOT(slotRefreshConnection()));
//    connect(QuNeo, SIGNAL(signalBeginFwTimer()), this, SLOT(slotRefreshConnection()));


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

    centerWidgetOnScreen(this);


    qDebug() << "setup Update All Presets Dialog";
    updateAllPresetsProgressDialog = new QProgressDialog("Updating All Presets...","Cancel", 0,16,this);
    updateAllPresetsProgressDialog->setCancelButton(0);
    updateAllPresetsProgressDialog->close();

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

#ifdef Q_OS_MAC

#else
    //windows edit pane fonts
    //QGroupBox* gb = this->findChild<QGroupBox *>(QString("groupBox"));
    //gb->setStyleSheet("QLabel { font: 10px } QLineEdit { font: 8px } QCheckBox {font:9px} QSpinBox {font: 10px} QDoubleSpinBox {font: 10px} QComboBox {font: 10px}");
    //QLineEdit* qle = this->findChild<QLineEdit *>(QString("presetName"));
    //qle->setStyleSheet("QLineEdit {font:8px}");

#endif

    qDebug() << "setup Rogue warning";
    rogueWarning.setIcon(QMessageBox::Warning);
    rogueWarning.addButton(QMessageBox::Ok);
    rogueWarning.setText("Your QuNeo is either in Expander Mode or you are using a Rogue.");
    rogueWarning.setInformativeText("Please connect your QuNeo with a USB cable, and make sure it is not in \"expander\" mode.");

    // update version string
    QString versionLabelText = QString("%1.%2.%3%4").arg((uchar)applicationVersion[0]).arg((uchar)applicationVersion[1]).arg((uchar)applicationVersion[2]).arg(betaVersion);
    ui->QuNeoVersionLabel->setText(versionLabelText);

    //fixing connected indicator label size on Windows
#ifdef Q_OS_MAC

#else
    QPushButton* connectedLabel = this->findChild<QPushButton *>("QuNeo_Connected_Label");
    connectedLabel->setStyleSheet("font: bold; color: white; background-color:rgba(0,0,0,0); font-size: 9px;");
#endif
}



#define DIALOG_WIDTH 400
#define DIALOG_HEIGHT 125
#define DIALOG_TEXT_PADDING 10
#define DIALOG_W_CENTER (DIALOG_WIDTH / 2)
#define DIALOG_BUTT_W 70
#define DIALOG_BUTT_H 28
#define DIALOG_BUTT_X DIALOG_W_CENTER - (DIALOG_BUTT_W / 2)
#define DIALOG_BUTT_Y DIALOG_HEIGHT - DIALOG_BUTT_H - (DIALOG_TEXT_PADDING * 2)
#define DIALOG_TEXT_W (DIALOG_WIDTH - (DIALOG_TEXT_PADDING * 2))
#define DIALOG_TEXT_H (DIALOG_HEIGHT - DIALOG_BUTT_H - (DIALOG_TEXT_PADDING * 2))

void MainWindow::slotCreateDialog(QString dialogText)
{
    QDialog *msgBox = new QDialog(this);
    msgBox->setModal(true);
    msgBox->setWindowFlags(Qt::FramelessWindowHint);
    msgBox->setStyleSheet(dialogStylesString);


    msgBox->setMinimumSize(DIALOG_WIDTH, DIALOG_HEIGHT);
    msgBox->setFixedSize(DIALOG_WIDTH, DIALOG_HEIGHT);

    int x = this->width();
    int y = this->height();

    int dialogX = ((x / 2) - (DIALOG_WIDTH / 2));
    int dialogY = ((y / 2) - (DIALOG_HEIGHT / 2));

    qDebug() << "parent x: " << x << " y: " << y << " dialogX: " << dialogX << "dialogY: " << dialogY;

    msgBox->move(dialogX, dialogY);

    QLabel* text = new QLabel(dialogText, msgBox, Qt::WindowFlags());
    text->setAlignment(Qt::AlignCenter);
    text->setMinimumSize(DIALOG_TEXT_W, DIALOG_TEXT_H);
    text->setFixedSize(DIALOG_TEXT_W, DIALOG_TEXT_H);
    text->move(DIALOG_TEXT_PADDING, DIALOG_TEXT_PADDING);

    QPushButton* okButton = new QPushButton(msgBox);
    okButton->setStyleSheet(grayStyleString);
    okButton->setText("Ok");
    okButton->setGeometry(QRect(DIALOG_BUTT_X, DIALOG_BUTT_Y, DIALOG_BUTT_W,DIALOG_BUTT_H));
    connect(okButton, SIGNAL(clicked()), msgBox, SLOT(close()));

    msgBox->exec();
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
                //qDebug()<<QString("leftclick no shift: %1").arg(object->objectName());
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

void MainWindow::addUIToLayouts()
{
    // Left section
    QHBoxLayout *transportLayout = new QHBoxLayout();
    transportLayout->addWidget(ui->buttonButton0);
    transportLayout->addWidget(ui->buttonButton1);
    transportLayout->addWidget(ui->buttonButton2);
    transportLayout->addWidget(ui->buttonButton3);
    QGridLayout *hArrowSliderLayout = new QGridLayout();
    hArrowSliderLayout->addWidget(ui->padButton0,0,0,1,1);
    hArrowSliderLayout->addWidget(ui->padButton1,0,1,1,1);
    hArrowSliderLayout->addWidget(ui->padButton2,0,2,1,1);
    hArrowSliderLayout->addWidget(ui->padButton3,0,3,1,1);
    QHBoxLayout *rotaryLayout = new QHBoxLayout();
    QHBoxLayout *vSliderLayout = new QHBoxLayout();

    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftLayout->addLayout(transportLayout);
    leftLayout->addLayout(hArrowSliderLayout);
    leftLayout->addLayout(rotaryLayout);
    leftLayout->addLayout(vSliderLayout);

    // Right section
    QGridLayout *padLayout = new QGridLayout();

    QVBoxLayout *leftVArrowLayout = new QVBoxLayout();
    QVBoxLayout *lSliderLayout = new QVBoxLayout();
    QVBoxLayout *rightVArrowLayout = new QVBoxLayout();
    QHBoxLayout *bottomRightLayout = new QHBoxLayout();
    bottomRightLayout->addLayout(leftVArrowLayout);
    bottomRightLayout->addLayout(lSliderLayout);
    bottomRightLayout->addLayout(rightVArrowLayout);

    QVBoxLayout *rightLayout = new QVBoxLayout();
    rightLayout->addLayout(padLayout);
    rightLayout->addLayout(bottomRightLayout);

    // Add the left and right halves to the main layout
    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);

    // Add finished layout to main QWidget
    ui->QuNeo->setLayout(mainLayout);
}


// --------------------------------------------------------------------------------------
// ------ midi overhaul -----------------------------------------------------------------
// --------------------------------------------------------------------------------------
// kmiPorts monitors MIDI device connect/disconnects, this function handles those changes
void MainWindow::slotMIDIPortChange(QString portName, uchar inOrOut, uchar messageType, int portNum)
{
    //qDebug() << "slotMIDIPortChange - " << kmiPorts->mType[messageType] << kmiPorts->inOut[inOrOut] << " portName:" << portName << " messageType: " << " portNum: " << portNum << "\n";
    QPushButton* connectedLabel = this->findChild<QPushButton *>("QuNeo_Connected_Label");
    QFrame* connectedFrame = this->findChild<QFrame *>("QuNeo_Connected_Frame");

    switch (messageType)
    {
    case PORT_CONNECT:

        // update dropdown
        if (inOrOut == PORT_OUT && !portName.toUpper().contains("QUNEO"))
        {
            // EB TODO - add this dropdown for the aux output
            QComboBox *thisDropDown = ui->midi_thru;

            thisDropDown->addItem(portName); // update dropdown
            //slotFixDropDownWidth(thisDropDown);
        }

        // **** QuNeo connect *****************************************
        //
        if ((portName == QUNEO_IN_P1 || portName == QUNEO_BL_PORT) && inOrOut == PORT_IN)
        {
            QuNeo->slotSetExpectedFW(thisFw);
            troubleshootWindow->slotDetected();
            connectedLabel->setText("Detected");
            connectedFrame->setStyleSheet("border: 1px solid rgb(67,67,67); background:rgb(255,128,0); border-radius:6;");

            if (!QuNeo->slotUpdatePortIn(portNum))
            {
                slotCreateDialog(QString("ERROR: MIDI input port \"%1\"\nis currently being used by another program or process!").arg(portName));
            }
            else
            {
                fwUpdateWindow->slotAppendTextToConsole("\nQuNeo Connected\n");
            }

        }
        else if ((portName == QUNEO_OUT_P1 || portName == QUNEO_BL_PORT) && inOrOut == PORT_OUT)
        {
            if (!QuNeo->slotUpdatePortOut(portNum))
            {
                slotCreateDialog(QString("ERROR: MIDI output port \"%1\"\nis currently being used by another program or process!").arg(portName));
            }
            else
            {
                QuNeo->pollingStatus = true;
            }
        }

        break;
    case PORT_DISCONNECT:

        if (inOrOut == PORT_OUT)
        {
            // update dropdown EB TODO - implement this
            if (ui->midi_thru->currentText() == portName)
            {
                ui->midi_thru->setCurrentIndex(0);
            }

            ui->midi_thru->removeItem(ui->midi_thru->findText(portName));
        }

        // **** QuNeo disconnect ***************************************
        if (inOrOut == PORT_IN && portName == QuNeo->portName_in)
        {
            // close ports and stop polling
            QuNeo->slotCloseMidiIn(SIGNAL_SEND);
            fwUpdateWindow->slotAppendTextToConsole("\nQuNeo Disconnected\n");
            connectedLabel->setText("Not Connected");
            connectedFrame->setStyleSheet("border: 1px solid rgb(67,67,67); background: rgba(100,100,100,0); border-radius:6;");
            midiDeviceAccess->connected = false; // failsafe for allowing editor to talk to old firmware
        }
        else if (inOrOut == PORT_OUT && portName == QuNeo->portName_out)
        {
            QuNeo->slotCloseMidiOut(SIGNAL_SEND);
            QuNeo->pollingStatus = false;
        }
        else if (inOrOut == PORT_OUT && (portName == QUNEO_OUT_P1 || portName == QUNEO_BL_PORT) )
        {
            troubleshootWindow->slotConnected(false);
            connectedLabel->setText("Not Connected");
            connectedFrame->setStyleSheet("QFrame{border: 1px solid rgb(67,67,67); background: rgba(100,100,100,0); border-radius:6;}");
        }

        break;
    case PORT_CHANGED:
        qDebug() << " PORT CHANGED - name: " << portName << portName << " inOrOut: " << kmiPorts->inOut[inOrOut] << " messageType: " << kmiPorts->mType[messageType] << " portNum: " << portNum << "\n";

        // **** QuNeo renumber *****************************************
        if ((portName == QUNEO_IN_P1 || portName == QUNEO_BL_PORT) && inOrOut == PORT_IN)
        {
            QuNeo->slotUpdatePortIn(portNum);
        }
        else if ((portName == QUNEO_OUT_P1 || portName == QUNEO_BL_PORT) && inOrOut == PORT_OUT)
        {
            QuNeo->slotUpdatePortOut(portNum);
        }

        break;
    default:
        break;
    }
}

void MainWindow::slotBootloaderMode(bool fwUpdateRequested)
{
    qDebug() << "slotBootloaderMode called - fwUpdateRequested: "<< fwUpdateRequested;

    QPushButton* connectedLabel = this->findChild<QPushButton *>("QuNeo_Connected_Label");
    QFrame* connectedFrame = this->findChild<QFrame *>("QuNeo_Connected_Frame");

    connectedLabel->setText("BOOTLOADER");
    connectedFrame->setStyleSheet("border: 1px solid rgb(67,67,67); background:rgb(0,0,255); border-radius:6;");

    troubleshootWindow->slotSetDevVersion(deviceFirmwareVersionString(), deviceBootloaderVersionString());
    troubleshootWindow->slotBootloaderMode();

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
    if (!QuNeo->port_out_open) return;

    slotFirmwareDetected(QuNeo, false); // act as if we received a firmware mismatch
    forceFirmwareUpdate = true;
}

void MainWindow::slotFirmwareDetected(MidiDeviceManager *thisMDM, bool matches)
{
    qDebug() << "slotFirmwareDetected called";

    troubleshootWindow->slotSetDevVersion(deviceFirmwareVersionString(), deviceBootloaderVersionString());
    if (forceFirmwareUpdate)
    {
        troubleshootWindow->slotRequestFwUpdate();
        forceFirmwareUpdate = false;
    }

    if (matches)
    {
        qDebug() << "FirmwareMatch: " << thisMDM->PID << "name:" << thisMDM->deviceName;
    }
    else
    {
        qDebug() << "Firmware MisMatch: " << thisMDM->PID << "name:" << thisMDM->deviceName;
        QPushButton* connectedLabel = this->findChild<QPushButton *>("QuNeo_Connected_Label");
        QFrame* connectedFrame = this->findChild<QFrame *>("QuNeo_Connected_Frame");

        midiDeviceAccess->connected = true; // experiment with allowing editor to talk to older firmware

        connectedLabel->setText("OUT OF DATE");
        connectedFrame->setStyleSheet("border: 1px solid rgb(67,67,67); background:rgb(255,0,0); border-radius:6;");

        // setup sysex connections to receive globals data
        QuNeo->disconnect(SIGNAL(signalRxSysExBA(QByteArray))); // disconnect to be safe

        // QuNeo only listens to the device ID response
        // EB TODO - detect rogue mode
        //connect(QuNeo, SIGNAL(signalRxSysExBA(QByteArray)), sysExEncDecode, SLOT(slotProcessSysEx(QByteArray)));

        qDebug() << "prepare fwUpdateWindow";
        fwUpdateWindow->slotClearText();
        fwUpdateWindow->slotUpdateProgressBar(0);
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
        QPushButton* connectedLabel = this->findChild<QPushButton *>("QuNeo_Connected_Label");
        connectedLabel->setText("Connected");

        QFrame* connectedFrame = this->findChild<QFrame *>("QuNeo_Connected_Frame");
        connectedFrame->setStyleSheet("border: 1px solid rgb(67,67,67); background:rgb(10,255,0); border-radius:6;");
        qDebug() << "<< set connected: true";
        connected = true;
        midiDeviceAccess->connected = true;

        troubleshootWindow->slotConnected(true);
    }
    else
    {
        QPushButton* connectedLabel = this->findChild<QPushButton *>("QuNeo_Connected_Label");
        connectedLabel->setText("Not Connected");

        QFrame* connectedFrame = this->findChild<QFrame *>("QuNeo_Connected_Frame");
        connectedFrame->setStyleSheet("border: 1px solid rgb(67,67,67); background: rgba(100,100,100,0); border-radius:6;");
        qDebug() << "<< set connected: false";

        // safety to ensure we don't connect this twice
        if (connected)
        {
            QuNeo->disconnect(SIGNAL(signalRxSysExBA(QByteArray)));
        }
        connected = false;
        midiDeviceAccess->connected = false;

        troubleshootWindow->slotConnected(false);
    }
}

// connect QuNeo midi input to to midi aux out
void MainWindow::slotUpdateMIDIaux()
{
    qDebug() << "slotUpdateMIDIaux called";

    QuNeo->disconnect(SIGNAL(signalRxMidi_raw(uchar, uchar, uchar, uchar)));

    if (!connected) return; // don't continue if we aren't connected

    // EB TODO - implement midi aux ports
    if (ui->midi_thru->currentText() != "None")
    {
        // set and open the ports
        int thisOutPort = kmiPorts->getOutPortNumber(ui->midi_thru->currentText());
        MIDIThru->slotUpdatePortOut(thisOutPort);
        MIDIThru->slotOpenMidiOut();
        connect(QuNeo, SIGNAL(signalRxMidi_raw(uchar, uchar, uchar, uchar)), MIDIThru, SLOT(slotSendMIDI(uchar, uchar, uchar, uchar)));
    }
    else
    {
        MIDIThru->slotCloseMidiOut();
    }
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

void MainWindow::slotOpenTroubleshooting()
{
    troubleshootWindow->show();
    troubleshootWindow->slotScrollTroubleUp();
}

// --------------------------------------------------------------------------------------
// ------ end midi overhaul -------------------------------------------------------------
// --------------------------------------------------------------------------------------



