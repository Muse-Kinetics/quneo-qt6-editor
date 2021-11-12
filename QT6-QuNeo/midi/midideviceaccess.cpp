#include "midideviceaccess.h"
#include <QDebug>

#ifdef Q_OS_MAC

// KMI_SysexMessages.h externs
extern unsigned char _quneo_swap_leds[17];
extern unsigned char _quneo_toggleProgramChangeInSysExData[17];
extern unsigned char _quneo_toggleProgramChangeOutSysExData[17];

//sysEx array for querying the board's ID
//char checkFwSysExData[] = {
//    static_cast<char>(0xF0),0x7E,0x7F,0x06,0x01,static_cast<char>(0xF7)
//};

//char swapLedsSysExData[] = {static_cast<char>(0xF0), 0x00, 0x01, 0x5F, 0x7A, 0x1E, 0x00, 0x01, 0x00, 0x02, 0x50, 0x02, 0x44, 0x5D, 0x00, 0x10, static_cast<char>(0xF7)};

//char toggleProgramChangeInSysExData[] = {static_cast<char>(0xF0), 0x00, 0x01, 0x5F, 0x7A, 0x1E, 0x00, 0x01, 0x00, 0x02, 0x50, 0x06, 0x04, 0x59, 0x00, 0x30, static_cast<char>(0xF7)};
//char toggleProgramChangeOutSysExData[] ={static_cast<char>(0xF0), 0x00, 0x01, 0x5F, 0x7A, 0x1E, 0x00, 0x01, 0x00, 0x02, 0x50, 0x05, 0x34, 0x3A, 0x00, 0x30, static_cast<char>(0xF7)};

//sysEx array for entering bootloader mode
//char enterBootloaderData[] = {
//    static_cast<char>(0xF0), 0x00, 0x01, 0x5F, 0x7A, 0x1E, 0x00, 0x01, 0x00, 0x02, 0x11, 0x00, 0x5A, 0x62, 0x00, 0x30,
//    0x10, static_cast<char>(0xF7)};

//var for determining if board has been set to bootloader mode
//bool inBootloader = false;

//determines whether sysex message has been completely sent
//bool firmwareSent = true;

//determines whether device has reconnected or not
//bool deviceReconnected = true;

//
//int fwProgSlowDown; // slows down the fw progress update

//sysex request msgs
//MIDISysexSendRequest *enterBootloaderSysExReq;
//MIDISysexSendRequest *checkFwSysEx;
//MIDISysexSendRequest *downloadFwSysExReq;
//MIDISysexSendRequest *updateAllPresetSysExReq;
//MIDISysexSendRequest *updateSinglePresetSysExReq;
//MIDISysexSendRequest *loadPresetSysExReq;
//MIDISysexSendRequest *swapLEDsSysExReq;
//MIDISysexSendRequest *toggleProgramChangeOutSysExReq;
//MIDISysexSendRequest *toggleProgramChangeInSysExReq;

//----Callback Declarations and Class Pointer----//
//MidiDeviceAccess* callbackPointer; //var for non class callbacks to access class
//void getMIDINotificationVirtual(const MIDINotification *message, void *refCon); //notification of when MIDI state changes (new controllers)
//void sysExComplete(MIDISysexSendRequest*); //called when sysex event has been completely sent
//void incomingMidi(const MIDIPacketList *pktlist, void *readProcRefCon, void *srcConnRefCon); //called upon incoming midi with connected port
//bool sysEx = false; //determines whether or not incomding midi is of sysEx type
//bool deviceReplied = false;



MidiDeviceAccess::MidiDeviceAccess(QVariantMap* presetMapsCopy,QObject *parent) :
    QObject(parent)
{

//    deviceReplied = false;
    replyTimeout = new QTimer(this);
//    connect(replyTimeout, SIGNAL(timeout()), this, SLOT(slotReplyTimeout()));

    //these array vals should be set to the most recent fw and bootloader version
    // versionArray[0] = 2; //bootloader LSB
    //versionArray[1] = 1; //booloader MSB

//    versionArray[0] = 3; //bootloader LSB
//    versionArray[1] = 1; //booloader MSB
//    versionArray[2] = 30; //fw LSB
//    versionArray[3] = 2; //fw MSB 2
//    versionArray[4] = 1; //fw MSB 1

    mainWindow = parent; //assign mainwindow to parent

//    boardVersion = QString("Not Connected");
//    editorVersion = QString("%1.%2.%3").
//            arg(versionArray[4]).
//            arg(versionArray[3]).
//            arg(versionArray[2]);

//    boardVersionBoot = QString("Not Connected");
//    editorVersionBoot = QString("%1.%2").arg(versionArray[1]).arg(versionArray[0]);

    sysExFormat = new SysExFormat(presetMapsCopy, 0); //create new instance of sysExFormat to format presets

    //find device menu, connect it, set initial slot to "None"
//    deviceMenu = mainWindow->findChild<QComboBox *>("deviceMenu");
//    connect(deviceMenu, SIGNAL(currentTextChanged(QString)), this, SLOT(slotSelectDevice(QString)));
//    deviceMenu->setEnabled(false);
//    selectedDevice = NULL;

    //initialize to preset 0, just like everything else, connect preset menu to gather preset num
    currentPreset = 0;
    presetMenu = mainWindow->findChild<QComboBox *>("presetMenu");
    connect(presetMenu, SIGNAL(currentTextChanged(QString)), this, SLOT(slotSetCurrentPreset(QString)));

    //find update button, connect it, used for single preset
    updateDeviceButton = mainWindow->findChild<QPushButton *>("updateButton");
    connect(updateDeviceButton, SIGNAL(clicked()), this, SLOT(slotUpdateSinglePreset()));

    //find update button, connect it, used for update all
    updateAllButton = mainWindow->findChild<QPushButton *>("updateAll");
    connect(updateAllButton, SIGNAL(clicked()), this, SLOT(slotUpdateAllPresets()));

    //pointer to this instance of the class, used in callback functions which are not part of the class
//    callbackPointer = this;

//    //load sysEx firmware file from app package
//    pathSysex = QCoreApplication::applicationDirPath();
//    pathSysex.remove(pathSysex.length() - 5, pathSysex.length());
//    pathSysex.append("Resources/Quneo.syx");
//    //qDebug() << QString("path: %1").arg(pathSysex);
//    //sysExFirmware = new QFile(pathSysex);
//    sysExFirmware = new QFile(":Quneo/sysex/resources/sysex/Quneo.syx");
//    if(sysExFirmware->open(QIODevice::ReadOnly)){
//        qDebug("sysex open");
//    } else {
//        qDebug("couldn't find sysex");
//    }

//    //read all data from file, and store in a QByteArray
//    sysExFirmwareBytes = sysExFirmware->readAll();
//    //qDebug() <<"sysex size:" << sysExFirmwareBytes.size();

//    //create new char array of the above byte array size
//    sysExFirmwareData = new char[sysExFirmwareBytes.size()];

//    //assigne bytes to array, necessary for sending midi according to mac midi services
//    sysExFirmwareData = sysExFirmwareBytes.data();

//    //******* Load Preset SysEx Files ********//
//    for(int i = 0; i<16; i++){
//        loadPreset[i] = new QFile(QString(":Quneo/sysex/resources/sysex/loadPresets/loadPreset%1.syx").arg(i));
//        if(loadPreset[i]->open(QIODevice::ReadOnly)){
//            //qDebug() << "load preset open" << i;
//        } else {
//            //qDebug() << "could not open load preset" << i;
//        }

//        loadPresetBytes[i] = loadPreset[i]->readAll();
//        loadPresetSize[i] = loadPresetBytes[i].size();
//        loadPresetData[i] = new char[loadPresetSize[i]];
//        loadPresetData[i] = loadPresetBytes[i].data();
//    }

    //get sources and dests, and store in vector
    //getSourcesDests();


}

//void MidiDeviceAccess::getSourcesDests()
//{

//    boardVersion = QString("Not Connected");
//    boardVersionBoot = QString("Not Connected");

//    sourceCount = 0;
//    destCount = 0;

//    //clear source/dest vectors
//    quNeoSources.clear();
//    quNeoDests.clear();

//    //Sources
//    //get number of sources
//    sourceCount = MIDIGetNumberOfSources();

//    //iterate through sources
//    for (ItemCount i = 0 ; i < sourceCount ; ++i)
//    {

//        //get endpoint ref for currentsource
//        MIDIEndpointRef source = MIDIGetSource(i);

//        //if endpoint ref is not null, see if name contains QUNEO
//        if (source != NULL)
//        {
//            if(getDisplayName(source).contains("QUNEO"))
//            {

//                //if source contains QUNEO, add to sources vector
//                quNeoSources.push_back(source);
//            }
//        }
//    }

//    //Destinations (same procedure as above)
//    destCount = MIDIGetNumberOfDestinations();

//    for (ItemCount i = 0 ; i < destCount ; ++i)
//    {

//        // Grab a reference to a destination endpoint
//        MIDIEndpointRef dest = MIDIGetDestination(i);

//        if (dest != NULL)
//        {
//            if(getDisplayName(dest).contains("QUNEO"))
//            {
//                quNeoDests.push_back(dest);
//            }
//        }
//    }

    //clear and repopulate device menu
//    deviceMenu->clear();

//    //if there are any dests add them to the menu, here's where we might want to limit number
//    if(quNeoDests.size() == 0)
//    {

//        //if(msgBox.isVisible()){
//        //msgBox.close();
//        //}

//        //if not dests, put none in the menu
//        deviceMenu->insertItem(0, QString("None"));

//        emit sigQuNeoConnected(false);

//    }
//    else if(quNeoDests.size() > 1)
//    {

//        //MIDIRestart();
//        //getSourcesDests();


//        msgBox.setText("You have too many QuNeos plugged in.");
//        msgBox.setInformativeText("Please edit 1 QuNeo at a time.");
//        msgBox.setDefaultButton(QMessageBox::Ok);
//        msgBox.setIcon(QMessageBox::Warning);
//        msgBox.setWindowModality(Qt::NonModal);
//        int ret = msgBox.exec();

//        if(ret == QMessageBox::Ok)
//        {
//            qDebug() << "ok hit";

//            QTimer::singleShot(2000, this, SLOT(getSourcesDests()));
//        }

//    }
//    else
//    {
//        //enumerate quneos in menu
//        for(uint i= 0; i < quNeoDests.size(); i++)
//        {
//            deviceMenu->insertItem(i, QString("QuNeo %1").arg(i+1));
//            emit sigQuNeoConnected(true);
//        }
//    }

    // EB TODO - placeholder for connected method
//    deviceMenu->insertItem(0, QString("QuNeo 1"));

    // emit sigSetVersions(editorVersion, boardVersion);

//}

//QString MidiDeviceAccess::getDisplayName(MIDIObjectRef object) {

//    // Returns the display name of a given MIDIObjectRef as an NSString
//    CFStringRef name = nil; //place holder for name

//    if(noErr != MIDIObjectGetStringProperty(object, kMIDIPropertyDisplayName, &name)){//get the name using midi services function
//        return nil;
//    }

//    return QString(cFStringRefToQString(name)); //return the name
//}

//QString MidiDeviceAccess::cFStringRefToQString(CFStringRef ref){

//    //this function just translates a CFStringRef into a QString
//    CFRange range;
//    range.location = 0;
//    range.length = CFStringGetLength(ref);
//    QString result(range.length, QChar(0));

//    UniChar *chars = new UniChar[range.length];
//    CFStringGetCharacters(ref, range, chars);
//    //[nsstr getCharacters:chars range:range];
//    result = QString::fromUtf16(chars, range.length);
//    delete[] chars;
//    return result;

//}

//void MidiDeviceAccess::connectDevice(){ //makes a connection between a device and our app's i/o ports

//    qDebug() << "_____connectDevice() called_____";

//    //dispose of client (and ports/connections) if it exists
//    //if(appyClientRef){
//    MIDIClientDispose(appyClientRef);
//    //}

//    //create client (CoreMIDI's reference to this app)
//    MIDIClientCreate(CFSTR("appy"), getMIDINotificationVirtual, this, &appyClientRef);

//    //create input and output ports for our client
//    MIDIInputPortCreate(appyClientRef, CFSTR("appyInPort"), incomingMidi, this, &appyInPortRef);
//    MIDIOutputPortCreate(appyClientRef, CFSTR("appyOutPort"), &appyOutPortRef);

//    //if source vector contains any QuNeo sources, then iterate through them and connect
//    if(selectedDevice)
//    {
//        for(uint i=0; i<quNeoSources.size(); i++)
//        {
//            //connect source to our client's input port
//            MIDIPortConnectSource(appyInPortRef, quNeoSources.at(i), &quNeoSources.at(i));
//        }

//        //if not in bootloader mode, send sysex check fw message upon device connection/selection
//        if(!inBootloader){
//            slotCheckFirmwareVersion();
//        }
//    }
//}

//void MidiDeviceAccess::slotSelectDevice(QString index){

//    qDebug() << "_____ slotSelectDevice() called_____" << index;

//    //if selected device contains QuNeo, then iterate through dests and assign index to selected device
//    if(index.contains(QString("QuNeo"))){
//        int i = index.remove(0,6).toInt() - 1;
//        selectedDevice = quNeoDests.at(i);

//    } else {
//        selectedDevice = NULL;
//    }
//    //after device selection connect device (regardless of whether selected device is NULL, this is taken care of in connectDevice())
//    connectDevice();

//}

void MidiDeviceAccess::slotUpdateAllPresets() //used for updating all presets at once
{

    qDebug() << "slotUpdateAllPresets called";

    //if(deviceMenu->currentText() == "QuNeo 1") // EB TODO - change to if(connected)
    if (connected)
    {
        emit sigShowUpdateAllPresetsDialog();

        for(int i= 0; i< 16; i++)
        {

            //encode current preset data
            qDebug() << "encode preset sysex: " << i;
            sysExFormat->slotEncodePreset(i);

            qDebug() << "send preset sysex: " << i;
            // MIDI Overhaul
            emit signalSendSysExBA(sysExFormat->presetSysExByteArray);

            //create char array of the most recently encoded preset using sysExFormat
//            char *presetSysExData = new char(sysExFormat->presetSysExByteArray.size());

            //assign sysExFormat data to char array
//            presetSysExData = sysExFormat->presetSysExByteArray.data();

            //if there is a selected device, send down preset data via sysex protocol
//            if(selectedDevice)// EB redundant
//            {

//                //create new sysex event/request for preset
//                updateAllPresetSysExReq = new MIDISysexSendRequest;

//                //set event/request params
//                updateAllPresetSysExReq->destination = selectedDevice;
//                updateAllPresetSysExReq->data = (const Byte *)presetSysExData;
//                updateAllPresetSysExReq->bytesToSend = sysExFormat->presetSysExByteArray.size();
//                updateAllPresetSysExReq->complete = false;
//                updateAllPresetSysExReq->completionProc = &sysExComplete;
//                updateAllPresetSysExReq->completionRefCon = presetSysExData;

//                //send the syesex data
//                MIDISendSysex(updateAllPresetSysExReq);


//                QElapsedTimer t;

//                t.start();

//                while(updateAllPresetSysExReq->bytesToSend > 0)
//                {
//                    if (t.elapsed() > 100)
//                    {
//                        qDebug() << "bytes to send update all presets" << updateAllPresetSysExReq->bytesToSend << "Time: " << t.elapsed();
//                        t.restart();
//                    }
//                }

                qDebug() << "done with preset" << i;
                emit sigUpdateAllPresetsCount(i);
//            }
        }
        qDebug() << "load preset";
        slotLoadPreset();
        qDebug() << "emit signal 16";
        emit sigUpdateAllPresetsCount(16);
    }

}

void MidiDeviceAccess::slotUpdateSinglePreset()
{
    qDebug() << "slotUpdateSinglePreset called, current preset: " << currentPreset;
    //if(deviceMenu->currentText() == "QuNeo 1")
    if (connected)
    {

        //encode current preset
        qDebug() << "encode preset";
        sysExFormat->slotEncodePreset(currentPreset);

        // MIDI Overhaul - send the preset byte array
        qDebug() << "send preset sysex";
        emit signalSendSysExBA(sysExFormat->presetSysExByteArray);

//        //create new char array using most recently encoded preset
//        char *presetSysExData = new char(sysExFormat->presetSysExByteArray.size());

//        //assign encoded data into char array
//        presetSysExData = sysExFormat->presetSysExByteArray.data();

//        //if there is a selected device
//        if(selectedDevice){

//            //create new sysex event/request
//            updateSinglePresetSysExReq = new MIDISysexSendRequest;

//            //set event/request params
//            updateSinglePresetSysExReq->destination = selectedDevice;
//            updateSinglePresetSysExReq->data = (const Byte *)presetSysExData;
//            updateSinglePresetSysExReq->bytesToSend = sysExFormat->presetSysExByteArray.size();
//            updateSinglePresetSysExReq->complete = false;
//            updateSinglePresetSysExReq->completionProc = &sysExComplete;
//            updateSinglePresetSysExReq->completionRefCon = presetSysExData;

//            //send the syesex data
//            MIDISendSysex(updateSinglePresetSysExReq);
//        }

//        //wait while the sysex gets down...

//        UInt32 bytes = 0;

//        while(updateSinglePresetSysExReq->bytesToSend > 0){

//            if(bytes != updateSinglePresetSysExReq->bytesToSend)
//            {
//                bytes = updateSinglePresetSysExReq->bytesToSend;
//                qDebug() << "bytes left to send single preset" << bytes;
//            }
//        }

        //load the currently selected preset
        qDebug() << "load preset";
        slotLoadPreset();
    }

}

void MidiDeviceAccess::slotLoadPreset(){

    qDebug() << "slotLoadPreset called";

//    char* loadPresetSysExData = new char[loadPresetSize[currentPreset]];
//    loadPresetSysExData = loadPresetData[currentPreset];

    //if there is a selected device EB TODO - change to if(connected)
    //if(selectedDevice)
    if (connected)
    {

        qDebug() << "currentPreset: " << currentPreset;
        sysExFormat->slotLoadPreset(currentPreset);
        emit signalSendSysExBA(sysExFormat->presetSysExByteArray);

//        QByteArray thisSysEx = loadPresetData[currentPreset];
//        emit signalSendSysExBA(thisSysEx);

//        //create new sysex event/request
//        loadPresetSysExReq = new MIDISysexSendRequest;

//        //set event/request params
//        loadPresetSysExReq->destination = selectedDevice;
//        loadPresetSysExReq->data = (const Byte *)loadPresetSysExData;
//        loadPresetSysExReq->bytesToSend = loadPresetSize[currentPreset];
//        loadPresetSysExReq->complete = false;
//        loadPresetSysExReq->completionProc = &sysExComplete;
//        loadPresetSysExReq->completionRefCon = loadPresetSysExData;

//        //send the syesex data
//        MIDISendSysex(loadPresetSysExReq);
    }

//    UInt32 bytes = 0;

//    while(loadPresetSysExReq->bytesToSend > 0){

//        if(bytes != loadPresetSysExReq->bytesToSend)
//        {
//            bytes = loadPresetSysExReq->bytesToSend;
//            qDebug() << "load pesets byes to send" << bytes;
//        }
//    }



}

//void MidiDeviceAccess::slotUpdateFirmware() //this function puts the board into bootloader mode************
//{

//    qDebug() << "slotUpdateFirmware() called";

//    //if selected device...
//    if(selectedDevice){

//        emit(sigShowFWUpdateDialog());

//        //create new sysex event/request
//        enterBootloaderSysExReq = new MIDISysexSendRequest;

//        //currentSysExMessage = enterBootloaderSysExReq;

//        //set event/request params
//        enterBootloaderSysExReq->destination = selectedDevice;
//        enterBootloaderSysExReq->data = (const Byte *)enterBootloaderData;
//        enterBootloaderSysExReq->bytesToSend = 17;
//        enterBootloaderSysExReq->complete = false;
//        enterBootloaderSysExReq->completionProc = &sysExComplete;
//        enterBootloaderSysExReq->completionRefCon = enterBootloaderData;

//        //send the syesex data
//        MIDISendSysex(enterBootloaderSysExReq);
//        qDebug() << "address of msg" << enterBootloaderSysExReq;
//    }

//    QTimer::singleShot(5000, callbackPointer, SLOT(slotDownloadFw()));
//}

//void MidiDeviceAccess::slotDownloadFw(){//this function sends the actual firmware data**********

//    qDebug() << "slowDownloadFw() called";

//    if(selectedDevice){

//        firmwareSent = false;

//        //create new sysex event/request
//        downloadFwSysExReq = new MIDISysexSendRequest;

//        //set event/request params
//        downloadFwSysExReq->destination = selectedDevice;
//        downloadFwSysExReq->data = (const Byte *)sysExFirmwareData;
//        downloadFwSysExReq->bytesToSend = sysExFirmwareBytes.size();
//        downloadFwSysExReq->complete = false;
//        downloadFwSysExReq->completionProc = &sysExComplete;
//        downloadFwSysExReq->completionRefCon = sysExFirmwareData;

//        //send the syesex data
//        MIDISendSysex(downloadFwSysExReq);
//        qDebug() << "address of msg" << downloadFwSysExReq;
//    }
//    if (!downloadFwSysExReq->complete) qDebug() << "Counting Bytes Down";

//    QElapsedTimer t;

//    t.start();


//    while(!downloadFwSysExReq->complete){

//        if (t.elapsed() > 1000)
//        {
//            qDebug() << "bytes left: " << bytesLeft << "time: " << t.elapsed();
//            bytesLeft = downloadFwSysExReq->bytesToSend;
//            emit sigFwBytesLeft(bytesLeft);
//            t.restart();
//        }
//    }

//    //emit sigFwBytesLeft((0)); // this causes the completion window to show up twice


//}

//void MidiDeviceAccess::slotCheckFirmwareVersion(){//this function checks versions with sysEx

//    checkFwSysEx = new MIDISysexSendRequest;

//    //currentSysExMessage = checkFwSysEx;

//    //set event/request params
//    checkFwSysEx->destination = selectedDevice;
//    checkFwSysEx->data = (const Byte *)checkFwSysExData;
//    checkFwSysEx->bytesToSend = 6;
//    checkFwSysEx->complete = false;
//    checkFwSysEx->completionProc = &sysExComplete;
//    checkFwSysEx->completionRefCon = checkFwSysExData;

//    //send the syesex data
//    MIDISendSysex(checkFwSysEx);

//    qDebug() << "------------------------------------------------------- slot check firmware version";
//    deviceReplied = false;
//    replyTimeout->start(2000);

//}

//void MidiDeviceAccess::slotReplyTimeout()
//{
//    qDebug() << "--------------------------------------------------------- no reply";
//    replyTimeout->stop();
//    if(!inBootloader)
//    {
//        emit sigRogueWarning();
//    }
//}

void MidiDeviceAccess::slotSwapLeds(){//this function checks versions with sysEx

//    swapLEDsSysExReq = new MIDISysexSendRequest;

//    //currentSysExMessage = checkFwSysEx;

//    //set event/request params
//    swapLEDsSysExReq->destination = selectedDevice;
//    swapLEDsSysExReq->data = (const Byte *)swapLedsSysExData;
//    swapLEDsSysExReq->bytesToSend = 17;
//    swapLEDsSysExReq->complete = false;
//    swapLEDsSysExReq->completionProc = &sysExComplete;
//    swapLEDsSysExReq->completionRefCon = swapLedsSysExData;

//    //send the syesex data
//    MIDISendSysex(swapLEDsSysExReq);

    emit signalSendSysEx(&_quneo_swap_leds[0], sizeof(_quneo_swap_leds));

}


void MidiDeviceAccess::slotSetCurrentPreset(QString selected){
    if(selected.contains("Preset ")){ //remove prepended preset
        if(selected.contains(" *")){    //remove asterisk if recalling unsaved/modified preset
            selected.remove(" *");
        }
        currentPreset = selected.remove(0,7).toInt() -1; //reduce string to number
        //qDebug() << "Current Preset" << currentPreset;
    }
}

//void MidiDeviceAccess::slotProcessSysExRx(int val)
//{
//    replyTimeout->stop();

//    //if not in bootloader...
//    if(!inBootloader)
//    {
//        //qDebug() << "sysex data" << val;
//        //qDebug("sysEx %x", val);

//        if(val == 247){ //if val is terminating sysex, append list with it
//            sysExMsg.append(247);

//            //if device ID, set fw and bootloader vars
//            if(sysExMsg.size() == 17 && sysExMsg.at(3) == 6 && sysExMsg.at(4) == 2)
//            {

//                deviceReplied = true;

//                qDebug() << "boot LSB" << sysExMsg.at(12);
//                qDebug() << "boot MSB" << sysExMsg.at(13);
//                qDebug() << "fw LSB" << sysExMsg.at(14);
//                qDebug() << "fw MSB" << sysExMsg.at(15);

//                //set version vars from sysex message
//                bootloaderVersionLSB = sysExMsg.at(12);
//                bootloaderVersionMSB = sysExMsg.at(13);
//                fwVersionLSB = sysExMsg.at(14);
//                fwVersionMSB = sysExMsg.at(15);

//                boardVersion = QString("%1.%2.%3").arg(fwVersionMSB/16).arg(fwVersionMSB%16).arg(fwVersionLSB);
//                boardVersionBoot = QString("%1.%2").arg(bootloaderVersionMSB).arg(bootloaderVersionLSB);

//                qDebug() << "fw:" << fwVersionMSB << fwVersionLSB;
//                qDebug() << "bootloader:" << bootloaderVersionMSB << bootloaderVersionLSB;

//                // EB TODO - this is the rogue indicator byte #10
//                //First check if board is in expander mode or is Rogue
//                if(sysExMsg.at(10))
//                {
//                    //If a Rogue or in expander, throw warning dialog
//                    emit sigRogueWarning();
//                }
//                else
//                {
//                    //check version vars against version array
//                    if(//versionArray[0] != bootloaderVersionLSB ||
//                            //versionArray[1] != bootloaderVersionMSB ||
//                            versionArray[2] != fwVersionLSB ||
//                            versionArray[3] != fwVersionMSB%16 ||
//                            versionArray[4] != fwVersionMSB/16)
//                    {
//                        emit sigFirmwareCurrent(false);
//                        qDebug() << "version good" << false;
//                    }
//                    else
//                    {
//                        qDebug() << "version good" << true;
//                        emit sigFirmwareCurrent(true);
//                    }
//                }
//            }
//            sysExMsg.clear();
//        }
//        else
//        {
//            sysExMsg.append(val);
//        }
//    }
//}

void MidiDeviceAccess::slotSendToggleProgramChangeOutput(){

//    toggleProgramChangeOutSysExReq = new MIDISysexSendRequest;

//    //currentSysExMessage = checkFwSysEx;

//    //set event/request params
//    toggleProgramChangeOutSysExReq->destination = selectedDevice;
//    toggleProgramChangeOutSysExReq->data = (const Byte *)toggleProgramChangeOutSysExData;
//    toggleProgramChangeOutSysExReq->bytesToSend = 17;
//    toggleProgramChangeOutSysExReq->complete = false;
//    toggleProgramChangeOutSysExReq->completionProc = &sysExComplete;
//    toggleProgramChangeOutSysExReq->completionRefCon = toggleProgramChangeOutSysExData;

//    //send the syesex data
//    MIDISendSysex(toggleProgramChangeOutSysExReq);

    emit signalSendSysEx(&_quneo_toggleProgramChangeOutSysExData[0], sizeof(_quneo_toggleProgramChangeOutSysExData));

}

void MidiDeviceAccess::slotSendToggleProgramChangeInput(){

//    toggleProgramChangeInSysExReq = new MIDISysexSendRequest;

//    //currentSysExMessage = checkFwSysEx;

//    //set event/request params
//    toggleProgramChangeInSysExReq->destination = selectedDevice;
//    toggleProgramChangeInSysExReq->data = (const Byte *)toggleProgramChangeInSysExData;
//    toggleProgramChangeInSysExReq->bytesToSend = 17;
//    toggleProgramChangeInSysExReq->complete = false;
//    toggleProgramChangeInSysExReq->completionProc = &sysExComplete;
//    toggleProgramChangeInSysExReq->completionRefCon = toggleProgramChangeInSysExData;

//    //send the syesex data
//    MIDISendSysex(toggleProgramChangeInSysExReq);

    emit signalSendSysEx(&_quneo_toggleProgramChangeInSysExData[0], sizeof(_quneo_toggleProgramChangeInSysExData));

}

//-------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------//
//--------------------------------------------Callback Definitions---------------------------------------//
//-------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------//


////these functions are used to communicate with apple services midi, and cannot be part of a class
////see http://developer.apple.com/library/ios/#documentation/CoreMidi/Reference/MIDIServices_Reference/Reference/reference.html
//void getMIDINotificationVirtual(const MIDINotification *message, void *refCon){

//    //get midi notifications-- if the setup changes...
//    if(message->messageID == 1){
//        qDebug("MIDI Setup Changed");
//        callbackPointer->getSourcesDests();
//    }
//}

//void sysExComplete(MIDISysexSendRequest* request)
//{

//    //called after sysex messages are completely sent
//    if(request == enterBootloaderSysExReq)
//    {
//        inBootloader = true;
//        //qDebug("enter bootloader sent");
//    }
//    else if(request == checkFwSysEx)
//    {
//        qDebug("check fw sent");
//    }
//    else if(request == downloadFwSysExReq)
//    {
//        firmwareSent = true;
//        qDebug("fw download sent!");
//        inBootloader = false;
//    }
//}

//void incomingMidi(const MIDIPacketList *pktlist, void *readProcRefCon, void *srcConnRefCon){

//    //iterate through midi packets and process according to type
//    const MIDIPacket *packet = &pktlist->packet[0];

//    //for number packets in packet list
//    for(int i =0; i < pktlist->numPackets; i++){

//        //for length of packet
//        for(int j = 0; j < packet->length; j++){

//            //if packet data is sysEx start...
//            if(packet->data[j] == 240){

//                //start to send vals for sysex processing
//                callbackPointer->slotProcessSysExRx(packet->data[j]);

//                //set sysEx message type to true to process rest of sysex
//                sysEx = true;

//                //if packet is terminating sysex...
//            } else if(packet->data[j] == 247){

//                //send terminating val to sysex processing
//                callbackPointer->slotProcessSysExRx(packet->data[j]);

//                //set sysEx message type to false to cease processing
//                sysEx = false;

//            } else {

//                //if sysEx is true, send all input to slot processing
//                if(sysEx){
//                    callbackPointer->slotProcessSysExRx(packet->data[j]);
//                } else if(packet->data[j] != 247){
//                    //here's where non sysEx midi data gets read
//                    //qDebug() << "midi data" << packet->data[j];
//                }
//            }
//        }
//        //advance packet in midi packet list
//        packet = MIDIPacketNext(packet);
//    }
//}


//-------------------------------------------WINDOWS-------------------------------------//

#else 

MidiDeviceAccess* callbackPointer;
void CALLBACK midiInCallback(HMIDIIN, UINT, DWORD_PTR, DWORD_PTR, DWORD_PTR);
void CALLBACK midiOutCallback(HMIDIOUT, UINT, DWORD_PTR, DWORD_PTR, DWORD_PTR);
void Main_OnDeviceChange(HWND hwn, WPARAM wParam, LPARAM lParam);

//this variable will be assigned 1 if the windows version is XP; 0 if not.
int winVersion;

//sysEx array for querying the board's ID
char checkFwSysExData[] = {
    static_cast<char>(0xF0),0x7E,0x7F,0x06,0x01,static_cast<char>(0xF7)
};

char swapLedsSysExData[] = {static_cast<char>(0xF0), 0x00, 0x01, 0x5F, 0x7A, 0x1E, 0x00, 0x01, 0x00, 0x02, 0x50, 0x02, 0x44, 0x5D, 0x00, 0x10, static_cast<char>(0xF7)};

char toggleProgramChangeInSysExData[] = {static_cast<char>(0xF0), 0x00, 0x01, 0x5F, 0x7A, 0x1E, 0x00, 0x01, 0x00, 0x02, 0x50, 0x06, 0x04, 0x59, 0x00, 0x30, static_cast<char>(0xF7)};
char toggleProgramChangeOutSysExData[] ={static_cast<char>(0xF0), 0x00, 0x01, 0x5F, 0x7A, 0x1E, 0x00, 0x01, 0x00, 0x02, 0x50, 0x05, 0x34, 0x3A, 0x00, 0x30, static_cast<char>(0xF7)};

//sysEx array for entering bootloader mode
char enterBootloaderData[] = {
    static_cast<char>(0xF0), 0x00, 0x01, 0x5F, 0x7A, 0x1E, 0x00, 0x01, 0x00, 0x02, 0x11, 0x00, 0x5A, 0x62, 0x00, 0x30,
    0x10, static_cast<char>(0xF7)};


//var for determining if board has been set to bootloader mode
bool inBootloader = 0;
bool inDeviceCheck = false;
int  updatingAllCount = 0;
bool updatingAll = false;

//Rogue
bool deviceReplied = false;
bool updatingFirmware = false;

MidiDeviceAccess::MidiDeviceAccess(QVariantMap* presetMapsCopy,QObject *parent) : QObject(parent){
    
    /*set the winVersion to 1 for XP, 0 for everything else (because we advise vista and
    Win8 users to try our Win7 versions).  This code here determines which code to use in
    certain places throughout the rest of the windows section of midideviceaccess*/
//    if(QSysInfo::windowsVersion() == QSysInfo::WV_XP){
//        winVersion = 1;
//    } else {
        winVersion = 0;
//    }
    
    //Rogue Warning
    deviceReplied = false;
    replyTimeout = new QTimer(this);
    connect(replyTimeout, SIGNAL(timeout()), this, SLOT(slotReplyTimeout()));

    currentPreset = 0;

    versionArray[0] = 3;
    versionArray[1] = 1;
    versionArray[2] = 30;
    versionArray[3] = 2;
    versionArray[4] = 1;

    isQuNeo = 0x1E;

    sysExFormat = new SysExFormat(presetMapsCopy, 0);
    mainWindow = parent;

    boardVersion = QString("Not Connected");
    editorVersion = QString("%1.%2.%3").
            arg(versionArray[4]).
            arg(versionArray[3]).
            arg(versionArray[2]);

    boardVersionBoot = QString("Not Connected");
    editorVersionBoot = QString("%1.%2").arg(versionArray[1]).arg(versionArray[0]);

    deviceMenu = mainWindow->findChild<QComboBox *>("deviceMenu");
    connect(deviceMenu, SIGNAL(currentIndexChanged(QString)), this, SLOT(slotSelectDevice(QString)));
    deviceMenu->setEnabled(false);

    updateSingleButton = mainWindow->findChild<QPushButton *>("updateButton");
    connect(updateSingleButton, SIGNAL(clicked()), this, SLOT(slotUpdateSinglePreset()));

    updateAllButton = mainWindow->findChild<QPushButton *>("updateAll");
    connect(updateAllButton, SIGNAL(clicked()), this, SLOT(slotUpdateAllPresets()));

    //-------------------Load Firmware SysEx and Enter Bootloader SysEx-------------------//

    //******Firmware*******//
    sysExFirmware = new QFile(":Quneo/sysex/resources/sysex/Quneo.syx");
    if(sysExFirmware->open(QIODevice::ReadOnly)){
        //  qDebug("sysExOpen");
    }
    sysExFirmwareBytes = sysExFirmware->readAll();
    //qDebug() << "sysex size" << sysExFirmwareBytes.size();
    firmwareByteSize = sysExFirmwareBytes.size();
    sysExFirmwareData = new char[sysExFirmwareBytes.size()];
    sysExFirmwareData = sysExFirmwareBytes.data();


    //******* Load Preset SysEx Files ********//
    for(int i = 0; i<16; i++){
        loadPreset[i] = new QFile(QString(":Quneo/sysex/resources/sysex/loadPresets/loadPreset%1.syx").arg(i));
        if(loadPreset[i]->open(QIODevice::ReadOnly)){
            qDebug() << "load preset open" << i;
        } else {
            qDebug() << "could not open load preset" << i;
        }

        loadPresetBytes[i] = loadPreset[i]->readAll();
        loadPresetSize[i] = loadPresetBytes[i].size();
        loadPresetData[i] = new char[loadPresetSize[i]];
        loadPresetData[i] = loadPresetBytes[i].data();
    }

    numInDevices = 0;
    numOutDevices = 0;

    //set up timer to check for new devices (easier than using windows' window message system)
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(slotCheckDevicesPulse()));
    timer->start(10);

    //timer for iterating through all presets (just calls single update)
    updatingAllTimer.setInterval(500);
    connect(&updatingAllTimer, SIGNAL(timeout()), this, SLOT(slotUpdateSinglePreset()));

    //for(int i=0; i <16; i++){
    //sysExFormat->slotEncodePreset(i);
    //}

    connect(this, SIGNAL(signalStopRogueTimer()), this, SLOT(slotStopRogueTimer()));
}

void MidiDeviceAccess::slotCheckDevicesPulse(){

    slotCheckDevices();
}

void MidiDeviceAccess::slotCheckDevices()
{

    // timer->stop();
    //qDebug() << "Check devices";
    //-------------- Get / Process Input Devices --------------//

    //if number of devices on this current check differs from last, get clear and populate quneo dev list
    if(numInDevices != midiInGetNumDevs() || numOutDevices != midiOutGetNumDevs())
    {
        boardVersion = QString("Not Connected");
        boardVersionBoot = QString("Not Connected");

        if(!inBootloader){

            qDebug() << "MIDI CHANGED**********************";

            slotCloseMidiOut();
            slotCloseMidiIn();

            QString str; //string for storing device names

            deviceOutList.clear(); //clear device out list
            deviceInList.clear(); //clear device in list
            quNeoDeviceOutList.clear();
            quNeoDeviceInList.clear();
            deviceBeingChecked.clear();
            deviceMenu->clear(); //clear device menu

            numInDevices = midiInGetNumDevs(); //reset input device count to current count

            //iterate through number of input devices
            for(int i = 0; i < numInDevices; i++){

                str.clear();//clear string (display name)
                midiInGetDevCaps(i, &mics, sizeof(MIDIINCAPS));//get device properties of current device index

                int j = 0; //initialize string index
                while(mics.szPname[j] != '\0'){//iterate through chars in current device name
                    QChar ch = mics.szPname[j]; //get the char
                    str.insert(j, ch); //assigne char to string
                    j++; //advance count
                }

                if(winVersion == 1){
                    //----------------THIS LINE IS FOR XP----------------//
                    if(str.contains("USB Audio Device")){ //if string is a ubs audio device
                        deviceInList.insert(str, i); //insert it into the device list map with stored midi device index
                    }
                } else {
                    //----------------THIS LINE IS FOR 7-----------------//
                    if(str.contains("QUNEO")){ //if string is a ubs audio device
                        deviceInList.insert(str, i); //insert it into the device list map with stored midi device index
                    }
                }
            }

            //-------------- Get / Process Output Devices --------------//
            numOutDevices = midiOutGetNumDevs(); //reset previous outdevice count to current count

            //iterate through output devices
            for(int i = 0; i < numOutDevices; i++){

                str.clear();//clear string
                midiOutGetDevCaps(i, &mocs, sizeof(MIDIINCAPS));//get device properties of current device index

                int j = 0; //initialize string counter
                while(mocs.szPname[j] != '\0'){ //get characters until end of string is reached
                    QChar ch = mocs.szPname[j]; //assign char
                    str.insert(j, ch); //insert char into string
                    j++; //advance index
                }

                if(winVersion == 1){
                    //------------------THIS LINE IS FOR XP------------------//
                    if(str.contains("USB Audio Device")){ //if device is USBAD (xp) add to out list
                        deviceOutList.insert(str, i); //insert string into device out list
                    }
                } else {
                    //------------------THIS LINE IS FOR 7-------------------//
                    if(str.contains("QUNEO")){ //if device is USBAD (xp) add to out list
                        deviceOutList.insert(str, i); //insert string into device out list
                    }
                }
            }


            //-------------- Check for QuNeos --------------//
            if(deviceOutList.isEmpty()){ //if there is nothing inside the outlist...
                deviceMenu->addItem("None"); //populate menu with none
                emit sigQuNeoConnected(false);
            } else {

                quNeoDeviceInList = deviceInList;
                quNeoDeviceOutList = deviceOutList;

                slotPopulateDeviceMenu();
                emit sigQuNeoConnected(true);

            }
        }
    }

    //timer->start();

}

void MidiDeviceAccess::slotOpenMidiIn(int index){

    DWORD   err;

    /* Is it not yet open? */
    if (!inHandle) {
        /* Open MIDI Input and set Windows to call my
        midiInputEvt() callback function. You may prefer
        to have something other than CALLBACK_FUNCTION. Also,
        I open device 0. You may want to give the user a choice */
        if (!(err = midiInOpen(&inHandle, index, (DWORD)midiInCallback, 0, CALLBACK_FUNCTION))) {
            /* Start recording Midi and return if SUCCESS */
            if (!(err = midiInStart(inHandle))){
                //return(0);
            }
            
            /* ============== ERROR ============== */
            
            /* Close MIDI In and zero handle */
            slotCloseMidiIn();
        }

        /* Return the error */
        qDebug() << "OPEN MIDI IN ERR:"<< (err);
    }
}

void MidiDeviceAccess::slotOpenMidiOut(int index){

    DWORD   err;

    /* Is it not yet open? */
    if (!outHandle){
        /* Open MIDI Output. */
        if (!(err = midiOutOpen(&outHandle, index, (DWORD_PTR)midiOutCallback, (DWORD_PTR)this, CALLBACK_FUNCTION))) {
            //return(0);
        }

        /* ============== ERROR ============== */
    }

    /* Return the error */
    qDebug() << "OPEN MIDI OUT ERR:"<< (err);

}

void MidiDeviceAccess::slotCloseMidiIn(){

    DWORD   err;

    /* Is the device open? */
    if ((err = (DWORD)inHandle)){
        /* Unqueue any buffers we added. If you don't
        input System Exclusive, you won't need this */
        midiInReset(inHandle);

        /* Close device */
        if (!(err = midiInClose(inHandle))){
            /* Clear handle so that it's safe to call closeMidiIn() anytime */
            inHandle = 0;
        }
    }

    qDebug() << "CLOSE MIDI IN ERR" << err;
}

void MidiDeviceAccess::slotCloseMidiOut(){
    DWORD   err;

    /* Is the device open? */
    if ((err = (DWORD)outHandle)){
        /* If you have any system exclusive buffers that
        you sent via midiOutLongMsg(), and which are still being output,
        you may need to wait for their MIDIERR_STILLPLAYING flags to be
        cleared before you close the device. Some drivers won't close with
        pending output, and will give an error. */

        /* Close device */
        if (!(err = midiOutClose(outHandle))) {
            /* Clear handle so that it's safe to call closeMidiOut() anytime */
            outHandle = 0;
        }
    }
    qDebug() << "CLOSE MIDI OUT ERR" << err;
}

void MidiDeviceAccess::slotIsDeviceQuneo(QString deviceName){

    //--------- Manage Output Port ---------//
    //upon new selection close current in/out ports (basically a midi reset reset)
    slotCloseMidiIn();
    slotCloseMidiOut();

    //Sleep(500);

    if(deviceOutList.contains(deviceName)){//if device outlist contains string (which is should if sent to this function

        midiOutOpen( &outHandle, //open up output at handle
                     deviceOutList.value(deviceName).toInt(), //device index, get from variant map
                     (DWORD_PTR)midiOutCallback, //set call back function
                     (DWORD_PTR)this, //reference passed to callback
                     CALLBACK_FUNCTION); //specify we're using a callback

        openOutDeviceIndex = deviceOutList.value(deviceName).toInt(); //store currently open output index

        qDebug() << "open out device:" << openOutDeviceIndex;
    }


    //--------- Manage Input Port ---------//
    //close in port currently open at app handle

    if(deviceInList.contains(deviceName)){//if inlist contains string...


        MMRESULT errM =  midiInOpen(&inHandle, //open up input at app in handle
                                    deviceInList.value(deviceName).toInt(), //get device in index stored at string in map
                                    (DWORD_PTR)midiInCallback, //use this midi in callback
                                    (DWORD_PTR)this, //pass this class address too the call back as a reference
                                    CALLBACK_FUNCTION); //specify we're using a callback

        openInDeviceIndex = deviceInList.value(deviceName).toInt(); //store currently open input index
        qDebug() << "open in device:" << openInDeviceIndex;

        //---- Manage SysEx Input Stuff ----//
        MMRESULT err;

        sysExInBuffer = GlobalAlloc(GHND, 500); //allocate sysex input buffer
        if(sysExInBuffer){ //if exists...
            sysExInHdr.lpData = (LPSTR)GlobalLock(sysExInBuffer); //set pointer to our buffer in header struct
            if(sysExInHdr.lpData){ //if above pointer successfully set...
                sysExInHdr.dwBufferLength = 500; //allocate an input of 500 byte length
                sysExInHdr.dwFlags = 0; //no flags

                err = midiInPrepareHeader(inHandle, &sysExInHdr, sizeof(MIDIHDR)); //prepare the header (return MMRESULT err)
                if(err == MMSYSERR_NOERROR){ //if not error...
                    err = midiInAddBuffer(inHandle, &sysExInHdr, sizeof(MIDIHDR)); //add the buffer to our current device
                    if(err == MMSYSERR_NOERROR){ //if no error...
                        err = midiInStart(inHandle); //start midi input***** (there's no midi out start)
                        if(err != MMSYSERR_NOERROR){ //if error in starting...
                            qDebug("couldn't open midi in"); //print error
                        } else {
                            qDebug("device open"); //if successful, print device open
                            slotCheckFirmwareVersion(); // call actual firmware check
                        }
                    }
                }
            }
        }
    }
}

void MidiDeviceAccess::slotCheckFirmwareVersion(){

    sysExOutBuffer = GlobalAlloc(GHND, 6);
    sysExOutHdr.lpData = (LPSTR)GlobalLock(sysExOutBuffer);
    sysExOutHdr.dwBufferLength = 6;
    sysExOutHdr.dwFlags = 0;

    err = midiOutPrepareHeader(outHandle, &sysExOutHdr, sizeof(MIDIHDR));

    if(!err){

        memcpy(sysExOutHdr.lpData, &checkFwSysExData[0], 6);

        err = midiOutLongMsg(outHandle, &sysExOutHdr, sizeof(MIDIHDR));

        if(err){
            char errMsg[120];
            midiOutGetErrorText(err, (LPWSTR)errMsg, 120);
            qDebug()<<"err:" << errMsg;
        }
    }

    qDebug() << "------------------------------------------------------- slot check firmware version";
    deviceReplied = false;

    //If not in the middle of a firmware update, start reply timeout timer
    if(!updatingFirmware)
    {
        replyTimeout->start(2000);
    }


    // while(sysExOutHdr.dwBytesRecorded < sysExOutHdr.dwBufferLength){
    //qDebug() << "offset" << sysExOutHdr.;
    //}

}

void MidiDeviceAccess::slotReplyTimeout()
{
    qDebug() << "--------------------------------------------------------- no reply";
    replyTimeout->stop();

    if(!inBootloader)
    {
        qDebug() << "rogue warning from reply timeout";
        emit sigRogueWarning();
    }
}

void MidiDeviceAccess::slotStopRogueTimer()
{
    qDebug() << "---------- Stop rogue timeout timer";
    replyTimeout->stop();
}

void MidiDeviceAccess::slotSwapLeds()
{
    sysExOutBuffer = GlobalAlloc(GHND, 17);
    sysExOutHdr.lpData = (LPSTR)GlobalLock(sysExOutBuffer);
    sysExOutHdr.dwBufferLength = 17;
    sysExOutHdr.dwFlags = 0;

    err = midiOutPrepareHeader(outHandle, &sysExOutHdr, sizeof(MIDIHDR));

    if(!err){

        memcpy(sysExOutHdr.lpData, &swapLedsSysExData[0], 17);

        err = midiOutLongMsg(outHandle, &sysExOutHdr, sizeof(MIDIHDR));

        if(err){
            char errMsg[120];
            midiOutGetErrorText(err, (LPWSTR)errMsg, 120);
            qDebug()<<"err:" << errMsg;
        }
    }
}

void MidiDeviceAccess::slotSendToggleProgramChangeInput(){

    sysExOutBuffer = GlobalAlloc(GHND, 17);
    sysExOutHdr.lpData = (LPSTR)GlobalLock(sysExOutBuffer);
    sysExOutHdr.dwBufferLength = 17;
    sysExOutHdr.dwFlags = 0;

    err = midiOutPrepareHeader(outHandle, &sysExOutHdr, sizeof(MIDIHDR));

    if(!err){

        memcpy(sysExOutHdr.lpData, &toggleProgramChangeInSysExData[0], 17);

        err = midiOutLongMsg(outHandle, &sysExOutHdr, sizeof(MIDIHDR));

        if(err){
            char errMsg[120];
            midiOutGetErrorText(err, (LPWSTR)errMsg, 120);
            qDebug()<<"err:" << errMsg;
        }
    }
}

void MidiDeviceAccess::slotSendToggleProgramChangeOutput(){

    sysExOutBuffer = GlobalAlloc(GHND, 17);
    sysExOutHdr.lpData = (LPSTR)GlobalLock(sysExOutBuffer);
    sysExOutHdr.dwBufferLength = 17;
    sysExOutHdr.dwFlags = 0;

    err = midiOutPrepareHeader(outHandle, &sysExOutHdr, sizeof(MIDIHDR));

    if(!err){

        memcpy(sysExOutHdr.lpData, &toggleProgramChangeOutSysExData[0], 17);

        err = midiOutLongMsg(outHandle, &sysExOutHdr, sizeof(MIDIHDR));

        if(err){
            char errMsg[120];
            midiOutGetErrorText(err, (LPWSTR)errMsg, 120);
            qDebug()<<"err:" << errMsg;
        }
    }
}

void MidiDeviceAccess::slotPopulateDeviceMenu()
{
    qDebug() << "q count" << quNeoDeviceOutList.size();

    if(quNeoDeviceOutList.size() > 1){
        QMessageBox::StandardButton ret;

        ret = QMessageBox::warning((QWidget*)mainWindow, QString("Application"),
                                   QString("You have too many QuNeos plugged in."));

        if(ret == QMessageBox::Ok)
        {
            numInDevices = -1;
            numOutDevices = -1;
            quNeoDeviceOutList.clear();
            quNeoDeviceInList.clear();
            slotCheckDevices();
        }
    }
    else if(quNeoDeviceOutList.size() != 0)
    {
        inDeviceCheck = false;
        for(QVariantMap::iterator i = quNeoDeviceOutList.begin(); i != quNeoDeviceOutList.end(); i++)
        { //if there is stuff in out list...
            deviceMenu->addItem(i.key()); //put it in the menu (which will also activate it and call slotSelectDevice)
        }

        qDebug() << "CHECK FIRMWARE AFTER MENU POPULATION";
        QTimer::singleShot(1000, this, SLOT(slotCheckFirmwareVersion()));
    }
    else
    {
        emit sigQuNeoConnected(false);
    }


    //timer->start();
}

void MidiDeviceAccess::slotSelectDevice(QString str){

    //--------- Manage Output Port ---------//
    //upon new selection close current in/out ports (basically a midi reset reset)
    slotCloseMidiIn();
    slotCloseMidiOut();

    //Sleep(500);

    if(deviceOutList.contains(str)){//if device outlist contains string (which is should if sent to this function

        midiOutOpen( &outHandle, //open up output at handle
                     deviceOutList.value(str).toInt(), //device index, get from variant map
                     (DWORD_PTR)midiOutCallback, //set call back function
                     (DWORD_PTR)this, //reference passed to callback
                     CALLBACK_FUNCTION); //specify we're using a callback

        openOutDeviceIndex = deviceOutList.value(str).toInt(); //store currently open output index

        qDebug() << "open out device:" << openOutDeviceIndex;

    }

    //--------- Manage Input Port ---------//
    //close in port currently open at app handle

    if(deviceInList.contains(str)){//if inlist contains string...


        MMRESULT errM =  midiInOpen(&inHandle, //open up input at app in handle
                                    deviceInList.value(str).toInt(), //get device in index stored at string in map
                                    (DWORD_PTR)midiInCallback, //use this midi in callback
                                    (DWORD_PTR)this, //pass this class address too the call back as a reference
                                    CALLBACK_FUNCTION); //specify we're using a callback

        openInDeviceIndex = deviceInList.value(str).toInt(); //store currently open input index
        qDebug() << "open in device:" << openInDeviceIndex;

        //---- Manage SysEx Input Stuff ----//
        MMRESULT err;

        sysExInBuffer = GlobalAlloc(GHND, 500); //allocate sysex input buffer
        if(sysExInBuffer){ //if exists...
            sysExInHdr.lpData = (LPSTR)GlobalLock(sysExInBuffer); //set pointer to our buffer in header struct
            if(sysExInHdr.lpData){ //if above pointer successfully set...
                sysExInHdr.dwBufferLength = 500; //allocate an input of 500 byte length
                sysExInHdr.dwFlags = 0; //no flags

                err = midiInPrepareHeader(inHandle, &sysExInHdr, sizeof(MIDIHDR)); //prepare the header (return MMRESULT err)
                if(err == MMSYSERR_NOERROR){ //if not error...
                    err = midiInAddBuffer(inHandle, &sysExInHdr, sizeof(MIDIHDR)); //add the buffer to our current device
                    if(err == MMSYSERR_NOERROR){ //if no error...
                        err = midiInStart(inHandle); //start midi input***** (there's no midi out start)
                        if(err != MMSYSERR_NOERROR){ //if error in starting...
                            qDebug("couldn't open midi in"); //print error
                        } else {
                            qDebug("device open"); //if successful, print device open
                            //slotCheckFirmwareVersion(); // call actual firmware check
                        }
                    }
                }
            }
        }
    }
}

void MidiDeviceAccess::slotUpdateSinglePreset(){
    if(deviceMenu->currentText() != "None"){

        if(!updatingAll){
            sysExFormat->slotEncodePreset(currentPreset);

            char *presetSysExData = new char(sysExFormat->presetSysExByteArray.size());
            presetSysExData = sysExFormat->presetSysExByteArray.data();

            sysExOutBuffer = GlobalAlloc(GHND, sysExFormat->presetSysExByteArray.size());
            sysExOutHdr.lpData = (LPSTR)GlobalLock(sysExOutBuffer);
            sysExOutHdr.dwBufferLength = sysExFormat->presetSysExByteArray.size();
            sysExOutHdr.dwFlags = 0;

            qDebug("UPDATE SINGLE PRESET CALLED");
            err = midiOutPrepareHeader(outHandle, &sysExOutHdr, sizeof(MIDIHDR));

            if(!err){

                memcpy(sysExOutHdr.lpData, &presetSysExData[0], sysExFormat->presetSysExByteArray.size());
                err = midiOutLongMsg(outHandle, &sysExOutHdr, sizeof(MIDIHDR));

                if(err){
                    char errMsg[120];
                    midiOutGetErrorText(err, (LPWSTR)errMsg, 120);
                    qDebug()<<"err:" << errMsg;
                }
            }
            QTimer::singleShot(500, this, SLOT(slotLoadPreset()));
        } else {

            sysExFormat->slotEncodePreset(updatingAllCount);

            char *presetSysExData = new char(sysExFormat->presetSysExByteArray.size());
            presetSysExData = sysExFormat->presetSysExByteArray.data();

            sysExOutBuffer = GlobalAlloc(GHND, sysExFormat->presetSysExByteArray.size());
            sysExOutHdr.lpData = (LPSTR)GlobalLock(sysExOutBuffer);
            sysExOutHdr.dwBufferLength = sysExFormat->presetSysExByteArray.size();
            sysExOutHdr.dwFlags = 0;

            qDebug() << "UPDATE ALL PRESET" << updatingAllCount;
            err = midiOutPrepareHeader(outHandle, &sysExOutHdr, sizeof(MIDIHDR));

            if(!err){

                memcpy(sysExOutHdr.lpData, &presetSysExData[0], sysExFormat->presetSysExByteArray.size());

                err = midiOutLongMsg(outHandle, &sysExOutHdr, sizeof(MIDIHDR));

                if(err){
                    char errMsg[120];
                    midiOutGetErrorText(err, (LPWSTR)errMsg, 120);
                    qDebug()<<"err:" << errMsg;
                }
            }

            if(updatingAllCount < 15){
                emit sigUpdateAllPresetsCount(updatingAllCount+1);
                updatingAllCount++;
            } else {
                updatingAll = false;
                updatingAllTimer.stop();
                updatingAllCount = 0;
                emit sigUpdateAllPresetsCount(16);
                QTimer::singleShot(250, this, SLOT(slotLoadPreset()));
            }
        }
    }
}

void MidiDeviceAccess::slotLoadPreset(){

    qDebug() << "LOAD PRESET CALLED" << "size:" << loadPresetSize[currentPreset];
    qDebug() << "current preset" << currentPreset;

    char* data = new char[loadPresetSize[currentPreset]];
    data = loadPresetData[currentPreset];

    sysExOutBuffer = GlobalAlloc(GHND, loadPresetSize[currentPreset]);
    sysExOutHdr.lpData = (LPSTR)GlobalLock(sysExOutBuffer);
    sysExOutHdr.dwBufferLength = loadPresetSize[currentPreset];
    sysExOutHdr.dwFlags = 0;


    MMRESULT err = midiOutPrepareHeader(outHandle, &sysExOutHdr, sizeof(MIDIHDR));

    if(!err){

        memcpy(sysExOutHdr.lpData, &data[0], loadPresetSize[currentPreset]);

        err = midiOutLongMsg(outHandle, &sysExOutHdr, sizeof(MIDIHDR));

        if(err){
            char errMsg[120];
            midiOutGetErrorText(err, (LPWSTR)errMsg, 120);
            qDebug()<<"err:" << errMsg;
        }
    } else {
        qDebug() << "load preset err" << err;
    }

}

void MidiDeviceAccess::slotUpdateAllPresets(){
    if(deviceMenu->currentText() != "None"){
        updatingAll = true;
        updatingAllTimer.start();
    }
}

void MidiDeviceAccess::slotUpdateFirmware()
{
    //Flag to indicate that we're starting the firmware update process
    updatingFirmware = true;

    qDebug() << "stop timer from slot update firmware";
    replyTimeout->stop();

    qDebug() << "UPDATE FIRMWARE CALLED";

    sysExOutBuffer = GlobalAlloc(GHND, 17);
    sysExOutHdr.lpData = (LPSTR)GlobalLock(sysExOutBuffer);
    sysExOutHdr.dwBufferLength = 17;
    sysExOutHdr.dwFlags = 0;

    err = midiOutPrepareHeader(outHandle, &sysExOutHdr, sizeof(MIDIHDR));

    if(!err){

        memcpy(sysExOutHdr.lpData, &enterBootloaderData[0], 17);

        err = midiOutLongMsg(outHandle, &sysExOutHdr, sizeof(MIDIHDR));

        if(err){
            char errMsg[120];
            midiOutGetErrorText(err, (LPWSTR)errMsg, 120);
            qDebug()<<"err:" << errMsg;
        }
    } else {
        qDebug() << "err:::::bootloader "<< err;
    }
}

void MidiDeviceAccess::slotDownloadFirmware(){

    qDebug() << "DOWNLOAD FIRMWARE CALLED";

    slotStopRogueTimer();

    sysExOutBuffer = GlobalAlloc(GHND, firmwareByteSize);
    sysExOutHdr.lpData = (LPSTR)GlobalLock(sysExOutBuffer);
    sysExOutHdr.dwBufferLength = firmwareByteSize;
    sysExOutHdr.dwFlags = 0;

    err = midiOutPrepareHeader(outHandle, &sysExOutHdr, sizeof(MIDIHDR));

    if(!err){

        memcpy(sysExOutHdr.lpData, &sysExFirmwareData[0], firmwareByteSize);

        err = midiOutLongMsg(outHandle, &sysExOutHdr, sizeof(MIDIHDR));

        if(err){
            char errMsg[120];
            midiOutGetErrorText(err, (LPWSTR)errMsg, 120);
            qDebug()<<"err:" << errMsg;
        }
    } else {
        qDebug() << "err::::: "<< err;
    }
}

void MidiDeviceAccess::slotSetCurrentPreset(QString selected){
    if(selected.contains("Preset ")){ //remove prepended preset
        if(selected.contains(" *")){    //remove asterisk if recalling unsaved/modified preset
            selected.remove(" *");
        }

        currentPreset = selected.remove(0,7).toInt()-1;
        qDebug() << "Current Preset" << currentPreset;
    }
}

void MidiDeviceAccess::slotProcessSysEx(QByteArray ba)
{

    qDebug() << "--------------- Try to stop timer from process sysex";
    emit signalStopRogueTimer();

    QChar ch;
    sysExMsg.clear();

    //qDebug() << "size" << ba.size();
    for(int i = 0; i < ba.size(); i++){
        ch = ba.at(i);
        sysExMsg.append(ch.unicode());
        qDebug("sysEx %x", ch.unicode());
    }

    //If it is a device reply...
    if(sysExMsg.size() == 17 && sysExMsg.at(3) == 6 && sysExMsg.at(4) == 2)
    {
        deviceReplied = true;

        //set version vars from sysex message
        fwVersionLSB = sysExMsg.at(14);
        fwVersionMSB = sysExMsg.at(15);
        bootloaderVersionLSB = sysExMsg.at(12);
        bootloaderVersionMSB = sysExMsg.at(13);
        inBootloader = sysExMsg.at(9);

        boardVersion = QString("%1.%2.%3").arg(fwVersionMSB/16).arg(fwVersionMSB%16).arg(fwVersionLSB);
        boardVersionBoot = QString("%1.%2").arg(bootloaderVersionMSB).arg(bootloaderVersionLSB);

        //First check if board is in expander mode or is Rogue
        if(sysExMsg.at(10))
        {
            //If a Rogue or in expander, throw warning dialog
            //emit signalStopRogueTimer();
            qDebug() << "--------------- Throw rogue warning from process sysex";
            emit sigRogueWarning();
        }

        //If a device reply, and a normally connected board...
        else
        {
            //If not in bootloader....
            if(!inBootloader)
            {
                //If not in device check....
                if(!inDeviceCheck)
                {
                    //We're not updating firmware in this condition, set flag
                    updatingFirmware = false;

                    if(//versionArray[0] != bootloaderVersionLSB || //check version vars against version array
                            //versionArray[1] != bootloaderVersionMSB ||
                            versionArray[2] != fwVersionLSB ||
                            versionArray[3] != fwVersionMSB%16 ||
                            versionArray[4] != fwVersionMSB/16)
                    {
                        emit sigFirmwareCurrent(false);
                        //qDebug() << "version good" << false;
                    }
                    else
                    {
                        qDebug() << "version good" << true;
                        emit sigFirmwareCurrent(true);
                    }
                }

                //If in device check... [this is not currently being used]
                else
                {
                    if(sysExMsg.at(5) == 0x00 && //if device code equals quneo device code//
                            sysExMsg.at(6) == 0x01 &&
                            sysExMsg.at(7) == 0x5F &&
                            sysExMsg.at(8) == 0x1E)
                    {
                        //insert string into quneo device list, otheriwse do nothing
                        quNeoDeviceOutList.insert(deviceBeingChecked, deviceOutList.value(deviceBeingChecked));
                        quNeoDeviceInList.insert(deviceBeingChecked, deviceInList.value(deviceBeingChecked));
                        qDebug() << "add Quneo TO LIST" << deviceBeingChecked;
                    }
                    inDeviceCheck = false;
                }
            }

            //If in bootloader
            else
            {
                //Reset flags
                inDeviceCheck = false;
                inBootloader = false;

                //Download firmware
                slotDownloadFirmware();
            }
        }
    }
}

void CALLBACK midiInCallback(HMIDIIN handle, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam, DWORD_PTR dwParam1){
    /*/qDebug() << MIM_DATA << uMsg;
    //qDebug() << dwParam;
    qDebug()    <<"status"<< ((dwParam) & 0xFF)
                << "data1" << ((dwParam>>8) & 0xFF)
                << "data2" << ((dwParam>>16) & 0xFF); //status byte*/

    MidiDeviceAccess *mda = (MidiDeviceAccess *) dwInstance;

    switch(uMsg){
    case MIM_OPEN:
        qDebug("MMOPEN");
        break;
    case MIM_CLOSE:
        qDebug("MIM_ClOSE");
        break;
    case MIM_DATA:
        qDebug("MIM_DATA");
        break;
    case MIM_LONGDATA:
    {
        //qDebug("MIM_LONGDATA");

        LPMIDIHDR lpMidiHdr = (LPMIDIHDR) dwParam;

        if(lpMidiHdr->dwBytesRecorded){

            //qDebug() << "sysEx len" << lpMidiHdr->dwBytesRecorded;
            //qDebug() << "data sys" << lpMidiHdr->lpData;

            mda->slotProcessSysEx(QByteArray(lpMidiHdr->lpData, lpMidiHdr->dwBytesRecorded));

            midiInAddBuffer(handle, lpMidiHdr, sizeof(MIDIHDR));
        }
    }
        break;
    default:
        qDebug("in callback");
        break;
    }
}

void CALLBACK midiOutCallback(HMIDIOUT handle, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam, DWORD_PTR dwParam1){
    qDebug() <<  "msg type" << uMsg << MOM_DONE << MOM_CLOSE << MOM_OPEN;

    MidiDeviceAccess *mda = (MidiDeviceAccess *) dwInstance;

    if(uMsg == MOM_DONE){
        midiOutUnprepareHeader(mda->outHandle, &mda->sysExOutHdr, sizeof(MIDIHDR));
        GlobalUnlock(mda->sysExOutBuffer);
        GlobalFree(mda->sysExOutBuffer);
    }
}

void Main_OnDeviceChange(HWND hwn, WPARAM wParam, LPARAM lParam){
    qDebug() << "device changed called";
}

#endif //Q_OS_MAC
