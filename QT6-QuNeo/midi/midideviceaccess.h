#ifndef MIDIDEVICEACCESS_H
#define MIDIDEVICEACCESS_H

//#include <QtGui>
#include <QtWidgets> // was QT Gui
#include <QApplication>
#include <QObject>
#include <vector>
#include <sysexformat.h>
#include <QTimer>

#ifdef Q_OS_MAC
#include <CoreMIDI/CoreMIDI.h>
#include <CoreServices/CoreServices.h>
#include <CoreFoundation/CoreFoundation.h>
#include <AudioUnit/AudioUnit.h>
//#include "copypastehandler.h"

using std::vector;

class MidiDeviceAccess : public QObject
{
    Q_OBJECT
public:
    explicit MidiDeviceAccess(QVariantMap*, QObject *parent = 0);

    //---------SysEx Formatting and File Loading-----------//
    SysExFormat *sysExFormat; //instance of sysex formatter
    QString pathSysex; //path of sysex firmware file in build
    QFile *sysExFirmware; //sysex firmware file
    QByteArray sysExFirmwareBytes; //sysex firmware as byte array
    int bytesLeft;
    char *sysExFirmwareData; //char array address for sysex firmware
    QList<int> sysExMsg;    //used to process sysEx messages from start to finish in slotProcessSysExRx(int);

    //---------Bootloader and Firmware Versioning vars--------//
    int fwVersionLSB; //stores LSB of FW version when query returns in slotProcessSysExRx(int);
    int fwVersionMSB; //stores MSB of FW version when query returns in slotProcessSysExRx(int);
    int bootloaderVersionLSB; //stores LSB of bootloader version when query returns in slotProcessSysExRx(int);
    int bootloaderVersionMSB; //stores MSB of bootloader version when query returns in slotProcessSysExRx(int);
    int versionArray[5]; //array containing the most recent verison vars above. above vars are compared with these arrya elements
    bool firmwareUpToDate; //bool describing whether or not the bootloader and fw version are current

    //--------Ui Objects and vars associated with MIDI Functionality-------//
    QObject *mainWindow; //pointer to main window
    QComboBox* deviceMenu; //pointer to device menu
    QPushButton* updateDeviceButton; //name is confusing-- actually updates a single preset
    QPushButton* updateAllButton;   //updates all presets, currently disabled
    QComboBox *presetMenu; //used for gathering current preset for updating
    int currentPreset; //preset to update

    //--------Mac Midi Services Vars--------//
    MIDIClientRef appyClientRef; //reference to our app
    MIDIPortRef appyInPortRef;  //reference to our app's input port
    MIDIPortRef appyOutPortRef; //reference to our app's output port
    vector<MIDIEndpointRef> quNeoDests; //vector of endpoint destination references (our devices' input ports)
    vector<MIDIEndpointRef> quNeoSources; //vector of endpoint source references (our devices' output ports)
    MIDIEndpointRef selectedDevice; //our currently selected device reference
    MIDIPacketList midiPacketList; //our list of midi packets for input
    MIDIPacket* midiPacket;        //pointer to a single midi packet, used as the "current packet" when iterating throuhg packet list

    ItemCount sourceCount;
    ItemCount destCount;

    //-----Internal helper functions, not necessary for SIGS/SLOTS---------//

    void connectDevice(); //connects a single device
    QString getDisplayName(MIDIObjectRef object); //gets "name" of QuNeo device
    QString cFStringRefToQString(CFStringRef); //converts CFString to QString for easier use w/in Qt


    //------- Load Preset Files ------///
    QFile* loadPreset[16];
    QByteArray loadPresetBytes[16];
    char *loadPresetData[16];
    int loadPresetSize[16];


    QMessageBox msgBox; //for too many quneos message

    QString editorVersion;
    QString boardVersion;
    QString editorVersionBoot;
    QString boardVersionBoot;

    QTimer* replyTimeout;


signals:
    void clearDeviceMenu(); //clears the device menu
    void populateDeviceMenu(QStringList); //adds devices to device menu
    void sigFirmwareCurrent(bool); //emits whether or not fw is current after checking
    void sigFwBytesLeft(int);
    void sigUpdateAllPresetsCount(int);
    void sigSetVersions(QString, QString);
    void sigQuNeoConnected(bool);
    void sigRogueWarning();

public slots:
    void slotSetCurrentPreset(QString); //sets preset var internally
    void slotSelectDevice(QString); //activates the selected device
    void slotUpdateAllPresets(); //will update all presets
    void slotUpdateSinglePreset(); //updates current preset
    void slotUpdateFirmware(); //puts board into bootloader mode, waits 5 seconds, then updates fw
    void slotCheckFirmwareVersion(); //checks fw and bootloader versions
    void slotProcessSysExRx(int); //processes sysEx messages
    void slotDownloadFw(); //called 5s after board enters bootloader
    void slotLoadPreset(); //activates preset on the board after an update
    void slotSwapLeds(); //sends sysex message to swap leds

    void getSourcesDests();//gathers references to QuNeo endpoints (device ports)

    void slotSendToggleProgramChangeOutput();
    void slotSendToggleProgramChangeInput();

    void slotReplyTimeout();



};

#else
#include <QTimer>
#include <Windows.h>
#include <MMSystem.h>
#include <Dbt.h>

class MidiDeviceAccess : public QObject
{
    Q_OBJECT
public:
    explicit MidiDeviceAccess(QVariantMap*,QObject *parent = 0);

    QTimer* replyTimeout;

    SysExFormat *sysExFormat;
    int currentPreset;
    int fwVersionLSB; //stores LSB of FW version when query returns in slotProcessSysExRx(int);
    int fwVersionMSB; //stores MSB of FW version when query returns in slotProcessSysExRx(int);
    int bootloaderVersionLSB; //stores LSB of bootloader version when query returns in slotProcessSysExRx(int);
    int bootloaderVersionMSB;
    int versionArray[5];
    int isQuNeo;
    QList<int> sysExMsg;    //used to process sysEx messages from start to finish in slotProcessSysExRx(int);

    QTimer *timer;
    QTimer updatingAllTimer;

    QObject* mainWindow;

    QComboBox* deviceMenu;
    QPushButton* updateSingleButton;
    QPushButton* updateAllButton;
    int presetByteSize;

    QVariantMap deviceOutList;//stores USB Audio Devices (including QuNeos)
    int openOutDeviceIndex;
    QVariantMap deviceInList;
    int openInDeviceIndex;

    QVariantMap quNeoDeviceOutList; //stores QuNeo Devices, after USB Audio Device Checking
    QVariantMap quNeoDeviceInList;
    QString deviceBeingChecked;

    QFile *sysExFirmware;
    QByteArray sysExFirmwareBytes;
    char *sysExFirmwareData;
    int firmwareByteSize;

    QFile* loadPreset[16];
    QByteArray loadPresetBytes[16];
    char *loadPresetData[16];
    int loadPresetSize[16];

    QMessageBox deviceNumWarning;

    MIDIOUTCAPS mocs;
    HMIDIOUT outHandle;
    HANDLE sysExOutBuffer;
    MIDIHDR sysExOutHdr;
    UINT err;
    int numOutDevices;

    MIDIINCAPS mics;
    HMIDIIN inHandle;
    HANDLE sysExInBuffer;
    MIDIHDR sysExInHdr;
    int numInDevices;

    HANDLE hBuffer;

    QString boardVersion;
    QString editorVersion;
    QString editorVersionBoot;
    QString boardVersionBoot;

    //char sysEx[] = {0xF0, 0x7F, 0x7F, 0x04, 0x04, 0x01, 0x7F, 0x7F, 0xF7};

signals:
    void sigFirmwareCurrent(bool);
    void sigFwBytesLeft(int);
    void sigUpdateAllPresetsCount(int);
    //void sigSetVersions(QString,QString);
    void sigQuNeoConnected(bool);
    void sigRogueWarning();

    void signalStopRogueTimer();

public slots:
    void slotCheckDevices(); //checks for USB Audio devices if midi setup has changed
    void slotCheckDevicesPulse(); //executes timer for checking the midi setup
    void slotSetCurrentPreset(QString); //gets current preset number
    void slotSelectDevice(QString);//not needed anymore with device number limit
    void slotUpdateAllPresets();
    void slotUpdateSinglePreset();
    void slotLoadPreset(); //loads updated preste onto board
    void slotUpdateFirmware(); //puts board into bootloader
    void slotDownloadFirmware(); // downloads firmare
    void slotCheckFirmwareVersion(); //sends device query
    void slotProcessSysEx(QByteArray); //processes sysex, if this becomes too large, make new class
    void slotIsDeviceQuneo(QString); //sends device id query, in device check mode
    void slotPopulateDeviceMenu(); //checks the number of quneos connected, and populates menu
    void slotSwapLeds(); //sends sysex message to swap leds

    //these open and close midi ports/devices
    void slotCloseMidiIn();
    void slotCloseMidiOut();
    void slotOpenMidiIn(int);
    void slotOpenMidiOut(int);

    void slotSendToggleProgramChangeOutput();
    void slotSendToggleProgramChangeInput();

    void slotReplyTimeout();
    void slotStopRogueTimer();

};

#endif //Q_OS_MAC
#endif // MIDIDEVICEACCESS_H
