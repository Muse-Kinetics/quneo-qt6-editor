#include "datavalidator.h"

DataValidator::DataValidator(QWidget *parent) :
    QWidget(parent)
{

    /***********************************Load and Parse JSON***********************************/

    QString jsonPath = QCoreApplication::applicationDirPath(); //get bundle path

    qDebug() << "Data Validator: Load and parse JSON - jsonPath: " << jsonPath;

#if defined(Q_OS_MAC)
#if defined(Q_OS_IOS)
    QString m_dataLocation = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    if (QDir(m_dataLocation).exists()) {
        qDebug() << "App data directory exists. " << m_dataLocation;
    } else {
        if (QDir("").mkpath(m_dataLocation)) {
            qDebug() << "Created app data directory. " << m_dataLocation;
        } else {
            qDebug() << "Failed to create app data directory. " << m_dataLocation;
        }
    }

    jsonPath = m_dataLocation.append("/QuNeo.json");
    qDebug() << "jsonPath: " << jsonPath;
#else
    jsonPath.remove(jsonPath.length() - 5, jsonPath.length());
    jsonPath.append("Resources/presets/QuNeo.json");
#endif
#else
    jsonPath = QString("./Resources/presets/QuNeo.json");
#endif

    //load json into QFile
    jsonFile = new QFile(jsonPath);

    // open file
    if(jsonFile->exists())
    {
        qDebug("jsonFound");
    }
    else
    {
        QFile sourceJson(":/Quneo/preset/resources/QuNeo.json");
        if (!sourceJson.exists())
        {
            qDebug() << "ERROR: QuNeo.json resource file does not exist!";
        }
        else
        {
            qDebug() << "json not found, copying from resources to: " << jsonPath;
            if (QFile::copy(":/Quneo/preset/resources/QuNeo.json", jsonPath))
            {
                qDebug() << "file copied";
            }
            else
            {
                qDebug() << "copy failed!";
            }
        }

        jsonFile = new QFile(jsonPath);
        qDebug() << "file exists? : " << jsonFile->exists();
    }

    QFileInfo jf(jsonPath);

    qDebug() << "writable: " << jf.isWritable();
    
    if (jsonFile->setPermissions(QFileDevice::ReadUser | QFileDevice::WriteUser))
    {
        qDebug() << "Set permissions to readwrite";
    }
    else
    {
        qDebug() << "ERROR setting read/write permissions!";
    }
    
    QFileInfo jf2(jsonPath);

    qDebug() << "writable: " << jf2.isWritable();

    if(jsonFile->open(QIODevice::ReadWrite | QIODevice::Text))
    {
        qDebug("Open Success");
    }
    else
    {
        qDebug() << "Open Failed";
    }

    // error object
    QJsonParseError JsonParseError;
    // convert file to QJsonDocument. this can be read/written to
    QJsonDocument JsonDocument = QJsonDocument::fromJson(jsonFile->readAll(), &JsonParseError);
    // close jsonFile
    jsonFile->close();

    // convert QJsonDocument to QJsonObject. this can be queried and modified in a human-readable way
    QJsonObject RootObject = JsonDocument.object();

    //load json file into a byte array to be processd by the parser
    jsonByteArray = JsonDocument.toJson();

    //parse the json data, convert it to a map and set it equal to the master jsonMap
    jsonMasterMap = RootObject.toVariantMap();

    //store a map of the 16 preset maps
    jsonQuNeoPresetsMap = jsonMasterMap["QuNeo Presets"].toMap();



    //slotLoadJSON();
    slotConstructDefultParamMap();
    slotCheckJSON();
    slotSaveJSON();
}

void DataValidator::slotConstructDefultParamMap(){

    /**************************************************************
      This function populates a default single preset. It also adds
      one parameter, "revisionNumber," to the QuNeoPresetsMap. We
      only need one default preset map, and this extra parameter
      to iterate through all the parameters in the JSON.
      *************************************************************/

    QList<QVariant> mediumTable;
    qDebug() << "slotConstructDefultParamMap called";

    mediumTable << 0 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 2 << 2 << 2 << 2 << 2 << 2 << 2 << 3 << 3 << 3 << 3 << 3 << 4 << 4 << 4 << 4 << 5 << 5 << 5 << 5 << 6 << 6 << 7 << 7 << 8 << 8 << 9 << 9 << 10 << 10 << 11 << 12 << 12 << 13 << 13 << 14 << 15 << 16 << 16 << 17 << 18 << 19 << 19 << 20 << 21 << 22 << 23 << 24 << 24 << 25 << 26 << 27 << 28 << 29 << 30 << 31 << 32 << 33 << 34 << 35 << 36 << 37 << 38 << 40 << 41 << 42 << 43 << 44 << 45 << 47 << 48 << 49 << 50 << 52 << 53 << 54 << 55 << 57 << 58 << 60 << 61 << 62 << 64 << 65 << 67 << 68 << 70 << 71 << 73 << 74 << 76 << 77 << 79 << 80 << 82 << 84 << 85 << 87 << 89 << 90 << 92 << 94 << 96 << 97 << 99 << 101 << 103 << 104 << 106 << 108 << 110 << 112 << 114 << 116 << 118 << 120 << 122 << 124 << 125 << 127;
    //for(int i=0; i<128; i++){
    //    linearTable.append(i);
    //}

    //--------- Populate Maps Individually ---------//

    defaultQuNeoPresetsMap["revisionNumber"] = 576; //this is all for this map, no need to revisit

    defaultSinglePresetMap["revisionNumber"] = 576;
    defaultSinglePresetMap["presetName"] = "Preset";

    //defaultComponentSettingsMap; //Nothing to set yet

    defaultHSlidersMap["hSliderInChannel"] = 0;
    defaultHSlidersMap["hSliderLocalLEDControl"] = 2;
    defaultHSlidersMap["hSliderOnThreshold"] = 10;
    defaultHSlidersMap["hSliderOffThreshold"] = 5;
    defaultHSlidersMap["hSliderSensitivity"] = 66;

    defaultHSlider0Map["hB1Channel"] = 0;
    defaultHSlider0Map["hB1inLocation"] = -1;
    defaultHSlider0Map["hB1outLocPassThruRange"] = 127;
    defaultHSlider0Map["hB1outLocation"] = -1;
    defaultHSlider0Map["hB1outNote"] = -1;
    defaultHSlider0Map["hB1outNotePressMode"] = 0;
    defaultHSlider0Map["hB1outPress"] = -1;
    defaultHSlider0Map["hB1outPressValue"] = -1;
    defaultHSlider0Map["hB1outVelocityValue"] = 127;
    defaultHSlider0Map["hB2Channel"] = 0;
    defaultHSlider0Map["hB2inLocation"] = -1;
    defaultHSlider0Map["hB2outLocPassThruRange"] = 127;
    defaultHSlider0Map["hB2outLocation"] = -1;
    defaultHSlider0Map["hB2outNote"] = -1;
    defaultHSlider0Map["hB2outNotePressMode"] = 0;
    defaultHSlider0Map["hB2outPress"] = -1;
    defaultHSlider0Map["hB2outPressValue"] = -1;
    defaultHSlider0Map["hB2outVelocityValue"] = 127;
    defaultHSlider0Map["hB3Channel"] = 0;
    defaultHSlider0Map["hB3inLocation"] = -1;
    defaultHSlider0Map["hB3outLocPassThruRange"] = 127;
    defaultHSlider0Map["hB3outLocation"] = -1;
    defaultHSlider0Map["hB3outNote"] = -1;
    defaultHSlider0Map["hB3outNotePressMode"] = 0;
    defaultHSlider0Map["hB3outPress"] = -1;
    defaultHSlider0Map["hB3outPressValue"] = -1;
    defaultHSlider0Map["hB3outVelocityValue"] = 127;
    defaultHSlider0Map["hB4Channel"] = 0;
    defaultHSlider0Map["hB4inLocation"] = -1;
    defaultHSlider0Map["hB4outLocPassThruRange"] = 127;
    defaultHSlider0Map["hB4outLocation"] = -1;
    defaultHSlider0Map["hB4outNote"] = -1;
    defaultHSlider0Map["hB4outNotePressMode"] = 0;
    defaultHSlider0Map["hB4outPress"] = -1;
    defaultHSlider0Map["hB4outPressValue"] = -1;
    defaultHSlider0Map["hB4outVelocityValue"] = 127;
    defaultHSlider1Map["hB1Channel"] = 0;
    defaultHSlider1Map["hB1inLocation"] = -1;
    defaultHSlider1Map["hB1outLocPassThruRange"] = 127;
    defaultHSlider1Map["hB1outLocation"] = -1;
    defaultHSlider1Map["hB1outNote"] = -1;
    defaultHSlider1Map["hB1outNotePressMode"] = 0;
    defaultHSlider1Map["hB1outPress"] = -1;
    defaultHSlider1Map["hB1outPressValue"] = -1;
    defaultHSlider1Map["hB1outVelocityValue"] = 127;
    defaultHSlider1Map["hB2Channel"] = 0;
    defaultHSlider1Map["hB2inLocation"] = -1;
    defaultHSlider1Map["hB2outLocPassThruRange"] = 127;
    defaultHSlider1Map["hB2outLocation"] = -1;
    defaultHSlider1Map["hB2outNote"] = -1;
    defaultHSlider1Map["hB2outNotePressMode"] = 0;
    defaultHSlider1Map["hB2outPress"] = -1;
    defaultHSlider1Map["hB2outPressValue"] = -1;
    defaultHSlider1Map["hB2outVelocityValue"] = 127;
    defaultHSlider1Map["hB3Channel"] = 0;
    defaultHSlider1Map["hB3inLocation"] = -1;
    defaultHSlider1Map["hB3outLocPassThruRange"] = 127;
    defaultHSlider1Map["hB3outLocation"] = -1;
    defaultHSlider1Map["hB3outNote"] = -1;
    defaultHSlider1Map["hB3outNotePressMode"] = 0;
    defaultHSlider1Map["hB3outPress"] = -1;
    defaultHSlider1Map["hB3outPressValue"] = -1;
    defaultHSlider1Map["hB3outVelocityValue"] = 127;
    defaultHSlider1Map["hB4Channel"] = 0;
    defaultHSlider1Map["hB4inLocation"] = -1;
    defaultHSlider1Map["hB4outLocPassThruRange"] = 127;
    defaultHSlider1Map["hB4outLocation"] = -1;
    defaultHSlider1Map["hB4outNote"] = -1;
    defaultHSlider1Map["hB4outNotePressMode"] = 0;
    defaultHSlider1Map["hB4outPress"] = -1;
    defaultHSlider1Map["hB4outPressValue"] = -1;
    defaultHSlider1Map["hB4outVelocityValue"] = 127;
    defaultHSlider2Map["hB1Channel"] = 0;
    defaultHSlider2Map["hB1inLocation"] = -1;
    defaultHSlider2Map["hB1outLocPassThruRange"] = 127;
    defaultHSlider2Map["hB1outLocation"] = -1;
    defaultHSlider2Map["hB1outNote"] = -1;
    defaultHSlider2Map["hB1outNotePressMode"] = 0;
    defaultHSlider2Map["hB1outPress"] = -1;
    defaultHSlider2Map["hB1outPressValue"] = -1;
    defaultHSlider2Map["hB1outVelocityValue"] = 127;
    defaultHSlider2Map["hB2Channel"] = 0;
    defaultHSlider2Map["hB2inLocation"] = -1;
    defaultHSlider2Map["hB2outLocPassThruRange"] = 127;
    defaultHSlider2Map["hB2outLocation"] = -1;
    defaultHSlider2Map["hB2outNote"] = -1;
    defaultHSlider2Map["hB2outNotePressMode"] = 0;
    defaultHSlider2Map["hB2outPress"] = -1;
    defaultHSlider2Map["hB2outPressValue"] = -1;
    defaultHSlider2Map["hB2outVelocityValue"] = 127;
    defaultHSlider2Map["hB3Channel"] = 0;
    defaultHSlider2Map["hB3inLocation"] = -1;
    defaultHSlider2Map["hB3outLocPassThruRange"] = 127;
    defaultHSlider2Map["hB3outLocation"] = -1;
    defaultHSlider2Map["hB3outNote"] = -1;
    defaultHSlider2Map["hB3outNotePressMode"] = 0;
    defaultHSlider2Map["hB3outPress"] = -1;
    defaultHSlider2Map["hB3outPressValue"] = -1;
    defaultHSlider2Map["hB3outVelocityValue"] = 127;
    defaultHSlider2Map["hB4Channel"] = 0;
    defaultHSlider2Map["hB4inLocation"] = -1;
    defaultHSlider2Map["hB4outLocPassThruRange"] = 127;
    defaultHSlider2Map["hB4outLocation"] = -1;
    defaultHSlider2Map["hB4outNote"] = -1;
    defaultHSlider2Map["hB4outNotePressMode"] = 0;
    defaultHSlider2Map["hB4outPress"] = -1;
    defaultHSlider2Map["hB4outPressValue"] = -1;
    defaultHSlider2Map["hB4outVelocityValue"] = 127;
    defaultHSlider3Map["hB1Channel"] = 0;
    defaultHSlider3Map["hB1inLocation"] = -1;
    defaultHSlider3Map["hB1outLocPassThruRange"] = 127;
    defaultHSlider3Map["hB1outLocation"] = -1;
    defaultHSlider3Map["hB1outNote"] = -1;
    defaultHSlider3Map["hB1outNotePressMode"] = 0;
    defaultHSlider3Map["hB1outPress"] = -1;
    defaultHSlider3Map["hB1outPressValue"] = -1;
    defaultHSlider3Map["hB1outVelocityValue"] = 127;
    defaultHSlider3Map["hB2Channel"] = 0;
    defaultHSlider3Map["hB2inLocation"] = -1;
    defaultHSlider3Map["hB2outLocPassThruRange"] = 127;
    defaultHSlider3Map["hB2outLocation"] = -1;
    defaultHSlider3Map["hB2outNote"] = -1;
    defaultHSlider3Map["hB2outNotePressMode"] = 0;
    defaultHSlider3Map["hB2outPress"] = -1;
    defaultHSlider3Map["hB2outPressValue"] = -1;
    defaultHSlider3Map["hB2outVelocityValue"] = 127;
    defaultHSlider3Map["hB3Channel"] = 0;
    defaultHSlider3Map["hB3inLocation"] = -1;
    defaultHSlider3Map["hB3outLocPassThruRange"] = 127;
    defaultHSlider3Map["hB3outLocation"] = -1;
    defaultHSlider3Map["hB3outNote"] = -1;
    defaultHSlider3Map["hB3outNotePressMode"] = 0;
    defaultHSlider3Map["hB3outPress"] = -1;
    defaultHSlider3Map["hB3outPressValue"] = -1;
    defaultHSlider3Map["hB3outVelocityValue"] = 127;
    defaultHSlider3Map["hB4Channel"] = 0;
    defaultHSlider3Map["hB4inLocation"] = -1;
    defaultHSlider3Map["hB4outLocPassThruRange"] = 127;
    defaultHSlider3Map["hB4outLocation"] = -1;
    defaultHSlider3Map["hB4outNote"] = -1;
    defaultHSlider3Map["hB4outNotePressMode"] = 0;
    defaultHSlider3Map["hB4outPress"] = -1;
    defaultHSlider3Map["hB4outPressValue"] = -1;
    defaultHSlider3Map["hB4outVelocityValue"] = 127;

    defaultLeftRightButtonsMap["leftrightInChannel"] = 0;
    defaultLeftRightButtonsMap["leftrightLocalLEDControl"] = 2;
    defaultLeftRightButtonsMap["leftrightOnThreshold"] = 10;
    defaultLeftRightButtonsMap["leftrightOffThreshold"] = 5;
    defaultLeftRightButtonsMap["leftrightSensitivity"] = 66;

    defaultLeftRightButton0Map["leftrightChannel"] = 0;
    defaultLeftRightButton0Map["leftrightEnableSwitch"] = 0;
    defaultLeftRightButton0Map["leftrightLInNote"] = -1;
    defaultLeftRightButton0Map["leftrightLOutNote"] = -1;
    defaultLeftRightButton0Map["leftrightLOutNotePressMode"] = 0;
    defaultLeftRightButton0Map["leftrightLOutPress"] = -1;
    defaultLeftRightButton0Map["leftrightLOutPressValue"] = -1;
    defaultLeftRightButton0Map["leftrightLOutVelocityValue"] = 127;
    defaultLeftRightButton0Map["leftrightRInNote"] = -1;
    defaultLeftRightButton0Map["leftrightROutNote"] = -1;
    defaultLeftRightButton0Map["leftrightROutNotePressMode"] = 0;
    defaultLeftRightButton0Map["leftrightROutPress"] = -1;
    defaultLeftRightButton0Map["leftrightROutPressValue"] = -1;
    defaultLeftRightButton0Map["leftrightROutVelocityValue"] = 127;
    defaultLeftRightButton1Map["leftrightChannel"] = 0;
    defaultLeftRightButton1Map["leftrightEnableSwitch"] = 0;
    defaultLeftRightButton1Map["leftrightLInNote"] = -1;
    defaultLeftRightButton1Map["leftrightLOutNote"] = -1;
    defaultLeftRightButton1Map["leftrightLOutNotePressMode"] = 0;
    defaultLeftRightButton1Map["leftrightLOutPress"] = -1;
    defaultLeftRightButton1Map["leftrightLOutPressValue"] = -1;
    defaultLeftRightButton1Map["leftrightLOutVelocityValue"] = 127;
    defaultLeftRightButton1Map["leftrightRInNote"] = -1;
    defaultLeftRightButton1Map["leftrightROutNote"] = -1;
    defaultLeftRightButton1Map["leftrightROutNotePressMode"] = 0;
    defaultLeftRightButton1Map["leftrightROutPress"] = -1;
    defaultLeftRightButton1Map["leftrightROutPressValue"] = -1;
    defaultLeftRightButton1Map["leftrightROutVelocityValue"] = 127;
    defaultLeftRightButton2Map["leftrightChannel"] = 0;
    defaultLeftRightButton2Map["leftrightEnableSwitch"] = 0;
    defaultLeftRightButton2Map["leftrightLInNote"] = -1;
    defaultLeftRightButton2Map["leftrightLOutNote"] = -1;
    defaultLeftRightButton2Map["leftrightLOutNotePressMode"] = 0;
    defaultLeftRightButton2Map["leftrightLOutPress"] = -1;
    defaultLeftRightButton2Map["leftrightLOutPressValue"] = -1;
    defaultLeftRightButton2Map["leftrightLOutVelocityValue"] = 127;
    defaultLeftRightButton2Map["leftrightRInNote"] = -1;
    defaultLeftRightButton2Map["leftrightROutNote"] = -1;
    defaultLeftRightButton2Map["leftrightROutNotePressMode"] = 0;
    defaultLeftRightButton2Map["leftrightROutPress"] = -1;
    defaultLeftRightButton2Map["leftrightROutPressValue"] = -1;
    defaultLeftRightButton2Map["leftrightROutVelocityValue"] = 127;
    defaultLeftRightButton3Map["leftrightChannel"] = 0;
    defaultLeftRightButton3Map["leftrightEnableSwitch"] = 0;
    defaultLeftRightButton3Map["leftrightLInNote"] = -1;
    defaultLeftRightButton3Map["leftrightLOutNote"] = -1;
    defaultLeftRightButton3Map["leftrightLOutNotePressMode"] = 0;
    defaultLeftRightButton3Map["leftrightLOutPress"] = -1;
    defaultLeftRightButton3Map["leftrightLOutPressValue"] = -1;
    defaultLeftRightButton3Map["leftrightLOutVelocityValue"] = 127;
    defaultLeftRightButton3Map["leftrightRInNote"] = -1;
    defaultLeftRightButton3Map["leftrightROutNote"] = -1;
    defaultLeftRightButton3Map["leftrightROutNotePressMode"] = 0;
    defaultLeftRightButton3Map["leftrightROutPress"] = -1;
    defaultLeftRightButton3Map["leftrightROutPressValue"] = -1;
    defaultLeftRightButton3Map["leftrightROutVelocityValue"] = 127;

    defaultLongSlidersMap["lSliderInChannel"] = 0;
    defaultLongSlidersMap["lSliderLocalLEDControl"] = 2;
    defaultLongSlidersMap["lSliderOnThreshold"] = 10;
    defaultLongSlidersMap["lSliderOffThreshold"] = 5;
    defaultLongSlidersMap["lSliderSensitivity"] = 66;

    defaultLongSliders0Map["lB1Channel"] = 0;
    defaultLongSliders0Map["lB1inLocation"] = -1;
    defaultLongSliders0Map["lB1inWidth"] = -1;
    defaultLongSliders0Map["lB1outLocPassThruRange"] = 127;
    defaultLongSliders0Map["lB1outLocation"] = -1;
    defaultLongSliders0Map["lB1outNote"] = -1;
    defaultLongSliders0Map["lB1outNotePressMode"] = 0;
    defaultLongSliders0Map["lB1outPress"] = -1;
    defaultLongSliders0Map["lB1outPressValue"] = -1;
    defaultLongSliders0Map["lB1outVelocityValue"] = 127;
    defaultLongSliders0Map["lB1outWidth"] = -1;
    defaultLongSliders0Map["lB2Channel"] = 0;
    defaultLongSliders0Map["lB2inLocation"] = -1;
    defaultLongSliders0Map["lB2inWidth"] = -1;
    defaultLongSliders0Map["lB2outLocPassThruRange"] = 127;
    defaultLongSliders0Map["lB2outLocation"] = -1;
    defaultLongSliders0Map["lB2outNote"] = -1;
    defaultLongSliders0Map["lB2outNotePressMode"] = 0;
    defaultLongSliders0Map["lB2outPress"] = -1;
    defaultLongSliders0Map["lB2outPressValue"] = -1;
    defaultLongSliders0Map["lB2outVelocityValue"] = 127;
    defaultLongSliders0Map["lB2outWidth"] = -1;
    defaultLongSliders0Map["lB3Channel"] = 0;
    defaultLongSliders0Map["lB3inLocation"] = -1;
    defaultLongSliders0Map["lB3inWidth"] = -1;
    defaultLongSliders0Map["lB3outLocPassThruRange"] = 127;
    defaultLongSliders0Map["lB3outLocation"] = -1;
    defaultLongSliders0Map["lB3outNote"] = -1;
    defaultLongSliders0Map["lB3outNotePressMode"] = 0;
    defaultLongSliders0Map["lB3outPress"] = -1;
    defaultLongSliders0Map["lB3outPressValue"] = -1;
    defaultLongSliders0Map["lB3outVelocityValue"] = 127;
    defaultLongSliders0Map["lB3outWidth"] = -1;
    defaultLongSliders0Map["lB4Channel"] = 0;
    defaultLongSliders0Map["lB4inLocation"] = -1;
    defaultLongSliders0Map["lB4inWidth"] = -1;
    defaultLongSliders0Map["lB4outLocPassThruRange"] = 127;
    defaultLongSliders0Map["lB4outLocation"] = -1;
    defaultLongSliders0Map["lB4outNote"] = -1;
    defaultLongSliders0Map["lB4outNotePressMode"] = 0;
    defaultLongSliders0Map["lB4outPress"] = -1;
    defaultLongSliders0Map["lB4outPressValue"] = -1;
    defaultLongSliders0Map["lB4outVelocityValue"] = 127;
    defaultLongSliders0Map["lB4outWidth"] = -1;

    defaultModeButtonsMap["modeLocalLEDControl"] = 2;
    defaultModeButtonsMap["modeOnThreshold"] = 10;
    defaultModeButtonsMap["modeOffThreshold"] = 5;
    defaultModeButtonsMap["modeSensitivity"] = 66;

    defaultModeButton0Map["modeChannel"] = 0;
    defaultModeButton0Map["modeEnableSwitch"] = 0;
    defaultModeButton0Map["modeInNote"] = -1;
    defaultModeButton0Map["modeOutNote"] = -1;
    defaultModeButton0Map["modeOutPress"] = -1;
    defaultModeButton0Map["modeOutVelocityValue"] = 127;

    defaultPadsMap["padGridDualInChannel"] = 2;
    defaultPadsMap["padGridDiscreteInChannel"] = 1;
    defaultPadsMap["padDrumInChannel"] = 0;
    defaultPadsMap["bank1TransposeInterval"] = 0;
    defaultPadsMap["bank2TransposeInterval"] = 16;
    defaultPadsMap["bank3TransposeInterval"] = 32;
    defaultPadsMap["bank4TransposeInterval"] = 48;
    defaultPadsMap["localLEDControl"] = 2;
    defaultPadsMap["cornerIsolation"] = 80;
    defaultPadsMap["padVelocityTableID"] = "Medium";
    defaultPadsMap["padVelocityTable"] = mediumTable;
    defaultPadsMap["padOffset"] = 0;
    defaultPadsMap["padOnThreshold"] = 25;
    defaultPadsMap["padOffThreshold"] = 5;
    defaultPadsMap["padSensitivity"] = 88;
    defaultPadsMap["padBankChangeMode"] = 1;

    defaultPad0Map["enableGrid"] = 0;
    defaultPad0Map["inDmNoteG"] = -1;
    defaultPad0Map["inDmNoteR"] = -1;
    defaultPad0Map["inGmNoteGNE"] = -1;
    defaultPad0Map["inGmNoteGNW"] = -1;
    defaultPad0Map["inGmNoteGSE"] = -1;
    defaultPad0Map["inGmNoteGSW"] = -1;
    defaultPad0Map["inGmNoteRNE"] = -1;
    defaultPad0Map["inGmNoteRNW"] = -1;
    defaultPad0Map["inGmNoteRSE"] = -1;
    defaultPad0Map["inGmNoteRSW"] = -1;
    defaultPad0Map["outDmNote"] = -1;
    defaultPad0Map["outDmNotePressMode"] = 0;
    defaultPad0Map["outDmPress"] = -1;
    defaultPad0Map["outDmPressValue"] = -1;
    defaultPad0Map["outDmVelocityValue"] = -1;
    defaultPad0Map["outDmXCC"] = -1;
    defaultPad0Map["outDmXReturn"] = 63;
    defaultPad0Map["outDmYReturn"] = 63;
    defaultPad0Map["outDmXYReturn"] = 63;
    defaultPad0Map["outDmYCC"] = -1;
    defaultPad0Map["outGmNoteNE"] = -1;
    defaultPad0Map["outGmNoteNW"] = -1;
    defaultPad0Map["outGmNoteSE"] = -1;
    defaultPad0Map["outGmNoteSW"] = -1;
    defaultPad0Map["outGmNotePressModeNE"] = 0;
    defaultPad0Map["outGmNotePressModeNW"] = 0;
    defaultPad0Map["outGmNotePressModeSE"] = 0;
    defaultPad0Map["outGmNotePressModeSW"] = 0;
    defaultPad0Map["outGmPressNE"] = -1;
    defaultPad0Map["outGmPressNW"] = -1;
    defaultPad0Map["outGmPressSE"] = -1;
    defaultPad0Map["outGmPressSW"] = -1;
    defaultPad0Map["outGmPressValueNE"] = -1;
    defaultPad0Map["outGmPressValueNW"] = -1;
    defaultPad0Map["outGmPressValueSE"] = -1;
    defaultPad0Map["outGmPressValueSW"] = -1;
    defaultPad0Map["outGmVelocityValueNE"] = 127;
    defaultPad0Map["outGmVelocityValueNW"] = 127;
    defaultPad0Map["outGmVelocityValueSE"] = 127;
    defaultPad0Map["outGmVelocityValueSW"] = 127;
    defaultPad0Map["padSensitivityPerPadUI"] = 0;
    defaultPad0Map["padSensitivityPerPad"] = 119;
    defaultPad0Map["padChannel"] = 0;

    defaultPad1Map["enableGrid"] = 0;
    defaultPad1Map["inDmNoteG"] = -1;
    defaultPad1Map["inDmNoteR"] = -1;
    defaultPad1Map["inGmNoteGNE"] = -1;
    defaultPad1Map["inGmNoteGNW"] = -1;
    defaultPad1Map["inGmNoteGSE"] = -1;
    defaultPad1Map["inGmNoteGSW"] = -1;
    defaultPad1Map["inGmNoteRNE"] = -1;
    defaultPad1Map["inGmNoteRNW"] = -1;
    defaultPad1Map["inGmNoteRSE"] = -1;
    defaultPad1Map["inGmNoteRSW"] = -1;
    defaultPad1Map["outDmNote"] = -1;
    defaultPad1Map["outDmNotePressMode"] = 0;
    defaultPad1Map["outDmPress"] = -1;
    defaultPad1Map["outDmPressValue"] = -1;
    defaultPad1Map["outDmVelocityValue"] = -1;
    defaultPad1Map["outDmXCC"] = -1;
    defaultPad1Map["outDmXReturn"] = 63;
    defaultPad1Map["outDmYReturn"] = 63;
    defaultPad1Map["outDmXYReturn"] = 63;
    defaultPad1Map["outDmYCC"] = -1;
    defaultPad1Map["outGmNoteNE"] = -1;
    defaultPad1Map["outGmNoteNW"] = -1;
    defaultPad1Map["outGmNoteSE"] = -1;
    defaultPad1Map["outGmNoteSW"] = -1;
    defaultPad1Map["outGmNotePressModeNE"] = 0;
    defaultPad1Map["outGmNotePressModeNW"] = 0;
    defaultPad1Map["outGmNotePressModeSE"] = 0;
    defaultPad1Map["outGmNotePressModeSW"] = 0;
    defaultPad1Map["outGmPressNE"] = -1;
    defaultPad1Map["outGmPressNW"] = -1;
    defaultPad1Map["outGmPressSE"] = -1;
    defaultPad1Map["outGmPressSW"] = -1;
    defaultPad1Map["outGmPressValueNE"] = -1;
    defaultPad1Map["outGmPressValueNW"] = -1;
    defaultPad1Map["outGmPressValueSE"] = -1;
    defaultPad1Map["outGmPressValueSW"] = -1;
    defaultPad1Map["outGmVelocityValueNE"] = 127;
    defaultPad1Map["outGmVelocityValueNW"] = 127;
    defaultPad1Map["outGmVelocityValueSE"] = 127;
    defaultPad1Map["outGmVelocityValueSW"] = 127;
    defaultPad1Map["padSensitivityPerPadUI"] = 0;
    defaultPad1Map["padSensitivityPerPad"] = 119;
    defaultPad1Map["padChannel"] = 0;

    defaultPad2Map["enableGrid"] = 0;
    defaultPad2Map["inDmNoteG"] = -1;
    defaultPad2Map["inDmNoteR"] = -1;
    defaultPad2Map["inGmNoteGNE"] = -1;
    defaultPad2Map["inGmNoteGNW"] = -1;
    defaultPad2Map["inGmNoteGSE"] = -1;
    defaultPad2Map["inGmNoteGSW"] = -1;
    defaultPad2Map["inGmNoteRNE"] = -1;
    defaultPad2Map["inGmNoteRNW"] = -1;
    defaultPad2Map["inGmNoteRSE"] = -1;
    defaultPad2Map["inGmNoteRSW"] = -1;
    defaultPad2Map["outDmNote"] = -1;
    defaultPad2Map["outDmNotePressMode"] = 0;
    defaultPad2Map["outDmPress"] = -1;
    defaultPad2Map["outDmPressValue"] = -1;
    defaultPad2Map["outDmVelocityValue"] = -1;
    defaultPad2Map["outDmXCC"] = -1;
    defaultPad2Map["outDmXReturn"] = 63;
    defaultPad2Map["outDmYReturn"] = 63;
    defaultPad2Map["outDmXYReturn"] = 63;
    defaultPad2Map["outDmYCC"] = -1;
    defaultPad2Map["outGmNoteNE"] = -1;
    defaultPad2Map["outGmNoteNW"] = -1;
    defaultPad2Map["outGmNoteSE"] = -1;
    defaultPad2Map["outGmNoteSW"] = -1;
    defaultPad2Map["outGmNotePressModeNE"] = 0;
    defaultPad2Map["outGmNotePressModeNW"] = 0;
    defaultPad2Map["outGmNotePressModeSE"] = 0;
    defaultPad2Map["outGmNotePressModeSW"] = 0;
    defaultPad2Map["outGmPressNE"] = -1;
    defaultPad2Map["outGmPressNW"] = -1;
    defaultPad2Map["outGmPressSE"] = -1;
    defaultPad2Map["outGmPressSW"] = -1;
    defaultPad2Map["outGmPressValueNE"] = -1;
    defaultPad2Map["outGmPressValueNW"] = -1;
    defaultPad2Map["outGmPressValueSE"] = -1;
    defaultPad2Map["outGmPressValueSW"] = -1;
    defaultPad2Map["outGmVelocityValueNE"] = 127;
    defaultPad2Map["outGmVelocityValueNW"] = 127;
    defaultPad2Map["outGmVelocityValueSE"] = 127;
    defaultPad2Map["outGmVelocityValueSW"] = 127;
    defaultPad2Map["padSensitivityPerPadUI"] = 0;
    defaultPad2Map["padSensitivityPerPad"] = 119;
    defaultPad2Map["padChannel"] = 0;

    defaultPad3Map["enableGrid"] = 0;
    defaultPad3Map["inDmNoteG"] = -1;
    defaultPad3Map["inDmNoteR"] = -1;
    defaultPad3Map["inGmNoteGNE"] = -1;
    defaultPad3Map["inGmNoteGNW"] = -1;
    defaultPad3Map["inGmNoteGSE"] = -1;
    defaultPad3Map["inGmNoteGSW"] = -1;
    defaultPad3Map["inGmNoteRNE"] = -1;
    defaultPad3Map["inGmNoteRNW"] = -1;
    defaultPad3Map["inGmNoteRSE"] = -1;
    defaultPad3Map["inGmNoteRSW"] = -1;
    defaultPad3Map["outDmNote"] = -1;
    defaultPad3Map["outDmNotePressMode"] = 0;
    defaultPad3Map["outDmPress"] = -1;
    defaultPad3Map["outDmPressValue"] = -1;
    defaultPad3Map["outDmVelocityValue"] = -1;
    defaultPad3Map["outDmXCC"] = -1;
    defaultPad3Map["outDmXReturn"] = 63;
    defaultPad3Map["outDmYReturn"] = 63;
    defaultPad3Map["outDmXYReturn"] = 63;
    defaultPad3Map["outDmYCC"] = -1;
    defaultPad3Map["outGmNoteNE"] = -1;
    defaultPad3Map["outGmNoteNW"] = -1;
    defaultPad3Map["outGmNoteSE"] = -1;
    defaultPad3Map["outGmNoteSW"] = -1;
    defaultPad3Map["outGmNotePressModeNE"] = 0;
    defaultPad3Map["outGmNotePressModeNW"] = 0;
    defaultPad3Map["outGmNotePressModeSE"] = 0;
    defaultPad3Map["outGmNotePressModeSW"] = 0;
    defaultPad3Map["outGmPressNE"] = -1;
    defaultPad3Map["outGmPressNW"] = -1;
    defaultPad3Map["outGmPressSE"] = -1;
    defaultPad3Map["outGmPressSW"] = -1;
    defaultPad3Map["outGmPressValueNE"] = -1;
    defaultPad3Map["outGmPressValueNW"] = -1;
    defaultPad3Map["outGmPressValueSE"] = -1;
    defaultPad3Map["outGmPressValueSW"] = -1;
    defaultPad3Map["outGmVelocityValueNE"] = 127;
    defaultPad3Map["outGmVelocityValueNW"] = 127;
    defaultPad3Map["outGmVelocityValueSE"] = 127;
    defaultPad3Map["outGmVelocityValueSW"] = 127;
    defaultPad3Map["padSensitivityPerPadUI"] = 0;
    defaultPad3Map["padSensitivityPerPad"] = 119;
    defaultPad3Map["padChannel"] = 0;

    defaultPad4Map["enableGrid"] = 0;
    defaultPad4Map["inDmNoteG"] = -1;
    defaultPad4Map["inDmNoteR"] = -1;
    defaultPad4Map["inGmNoteGNE"] = -1;
    defaultPad4Map["inGmNoteGNW"] = -1;
    defaultPad4Map["inGmNoteGSE"] = -1;
    defaultPad4Map["inGmNoteGSW"] = -1;
    defaultPad4Map["inGmNoteRNE"] = -1;
    defaultPad4Map["inGmNoteRNW"] = -1;
    defaultPad4Map["inGmNoteRSE"] = -1;
    defaultPad4Map["inGmNoteRSW"] = -1;
    defaultPad4Map["outDmNote"] = -1;
    defaultPad4Map["outDmNotePressMode"] = 0;
    defaultPad4Map["outDmPress"] = -1;
    defaultPad4Map["outDmPressValue"] = -1;
    defaultPad4Map["outDmVelocityValue"] = -1;
    defaultPad4Map["outDmXCC"] = -1;
    defaultPad4Map["outDmXReturn"] = 63;
    defaultPad4Map["outDmYReturn"] = 63;
    defaultPad4Map["outDmXYReturn"] = 63;
    defaultPad4Map["outDmYCC"] = -1;
    defaultPad4Map["outGmNoteNE"] = -1;
    defaultPad4Map["outGmNoteNW"] = -1;
    defaultPad4Map["outGmNoteSE"] = -1;
    defaultPad4Map["outGmNoteSW"] = -1;
    defaultPad4Map["outGmNotePressModeNE"] = 0;
    defaultPad4Map["outGmNotePressModeNW"] = 0;
    defaultPad4Map["outGmNotePressModeSE"] = 0;
    defaultPad4Map["outGmNotePressModeSW"] = 0;
    defaultPad4Map["outGmPressNE"] = -1;
    defaultPad4Map["outGmPressNW"] = -1;
    defaultPad4Map["outGmPressSE"] = -1;
    defaultPad4Map["outGmPressSW"] = -1;
    defaultPad4Map["outGmPressValueNE"] = -1;
    defaultPad4Map["outGmPressValueNW"] = -1;
    defaultPad4Map["outGmPressValueSE"] = -1;
    defaultPad4Map["outGmPressValueSW"] = -1;
    defaultPad4Map["outGmVelocityValueNE"] = 127;
    defaultPad4Map["outGmVelocityValueNW"] = 127;
    defaultPad4Map["outGmVelocityValueSE"] = 127;
    defaultPad4Map["outGmVelocityValueSW"] = 127;
    defaultPad4Map["padSensitivityPerPadUI"] = 0;
    defaultPad4Map["padSensitivityPerPad"] = 119;
    defaultPad4Map["padChannel"] = 0;

    defaultPad5Map["enableGrid"] = 0;
    defaultPad5Map["inDmNoteG"] = -1;
    defaultPad5Map["inDmNoteR"] = -1;
    defaultPad5Map["inGmNoteGNE"] = -1;
    defaultPad5Map["inGmNoteGNW"] = -1;
    defaultPad5Map["inGmNoteGSE"] = -1;
    defaultPad5Map["inGmNoteGSW"] = -1;
    defaultPad5Map["inGmNoteRNE"] = -1;
    defaultPad5Map["inGmNoteRNW"] = -1;
    defaultPad5Map["inGmNoteRSE"] = -1;
    defaultPad5Map["inGmNoteRSW"] = -1;
    defaultPad5Map["outDmNote"] = -1;
    defaultPad5Map["outDmNotePressMode"] = 0;
    defaultPad5Map["outDmPress"] = -1;
    defaultPad5Map["outDmPressValue"] = -1;
    defaultPad5Map["outDmVelocityValue"] = -1;
    defaultPad5Map["outDmXCC"] = -1;
    defaultPad5Map["outDmXReturn"] = 63;
    defaultPad5Map["outDmYReturn"] = 63;
    defaultPad5Map["outDmXYReturn"] = 63;
    defaultPad5Map["outDmYCC"] = -1;
    defaultPad5Map["outGmNoteNE"] = -1;
    defaultPad5Map["outGmNoteNW"] = -1;
    defaultPad5Map["outGmNoteSE"] = -1;
    defaultPad5Map["outGmNoteSW"] = -1;
    defaultPad5Map["outGmNotePressModeNE"] = 0;
    defaultPad5Map["outGmNotePressModeNW"] = 0;
    defaultPad5Map["outGmNotePressModeSE"] = 0;
    defaultPad5Map["outGmNotePressModeSW"] = 0;
    defaultPad5Map["outGmPressNE"] = -1;
    defaultPad5Map["outGmPressNW"] = -1;
    defaultPad5Map["outGmPressSE"] = -1;
    defaultPad5Map["outGmPressSW"] = -1;
    defaultPad5Map["outGmPressValueNE"] = -1;
    defaultPad5Map["outGmPressValueNW"] = -1;
    defaultPad5Map["outGmPressValueSE"] = -1;
    defaultPad5Map["outGmPressValueSW"] = -1;
    defaultPad5Map["outGmVelocityValueNE"] = 127;
    defaultPad5Map["outGmVelocityValueNW"] = 127;
    defaultPad5Map["outGmVelocityValueSE"] = 127;
    defaultPad5Map["outGmVelocityValueSW"] = 127;
    defaultPad5Map["padSensitivityPerPadUI"] = 0;
    defaultPad5Map["padSensitivityPerPad"] = 119;
    defaultPad5Map["padChannel"] = 0;

    defaultPad6Map["enableGrid"] = 0;
    defaultPad6Map["inDmNoteG"] = -1;
    defaultPad6Map["inDmNoteR"] = -1;
    defaultPad6Map["inGmNoteGNE"] = -1;
    defaultPad6Map["inGmNoteGNW"] = -1;
    defaultPad6Map["inGmNoteGSE"] = -1;
    defaultPad6Map["inGmNoteGSW"] = -1;
    defaultPad6Map["inGmNoteRNE"] = -1;
    defaultPad6Map["inGmNoteRNW"] = -1;
    defaultPad6Map["inGmNoteRSE"] = -1;
    defaultPad6Map["inGmNoteRSW"] = -1;
    defaultPad6Map["outDmNote"] = -1;
    defaultPad6Map["outDmNotePressMode"] = 0;
    defaultPad6Map["outDmPress"] = -1;
    defaultPad6Map["outDmPressValue"] = -1;
    defaultPad6Map["outDmVelocityValue"] = -1;
    defaultPad6Map["outDmXCC"] = -1;
    defaultPad6Map["outDmXReturn"] = 63;
    defaultPad6Map["outDmYReturn"] = 63;
    defaultPad6Map["outDmXYReturn"] = 63;
    defaultPad6Map["outDmYCC"] = -1;
    defaultPad6Map["outGmNoteNE"] = -1;
    defaultPad6Map["outGmNoteNW"] = -1;
    defaultPad6Map["outGmNoteSE"] = -1;
    defaultPad6Map["outGmNoteSW"] = -1;
    defaultPad6Map["outGmNotePressModeNE"] = 0;
    defaultPad6Map["outGmNotePressModeNW"] = 0;
    defaultPad6Map["outGmNotePressModeSE"] = 0;
    defaultPad6Map["outGmNotePressModeSW"] = 0;
    defaultPad6Map["outGmPressNE"] = -1;
    defaultPad6Map["outGmPressNW"] = -1;
    defaultPad6Map["outGmPressSE"] = -1;
    defaultPad6Map["outGmPressSW"] = -1;
    defaultPad6Map["outGmPressValueNE"] = -1;
    defaultPad6Map["outGmPressValueNW"] = -1;
    defaultPad6Map["outGmPressValueSE"] = -1;
    defaultPad6Map["outGmPressValueSW"] = -1;
    defaultPad6Map["outGmVelocityValueNE"] = 127;
    defaultPad6Map["outGmVelocityValueNW"] = 127;
    defaultPad6Map["outGmVelocityValueSE"] = 127;
    defaultPad6Map["outGmVelocityValueSW"] = 127;
    defaultPad6Map["padSensitivityPerPadUI"] = 0;
    defaultPad6Map["padSensitivityPerPad"] = 119;
    defaultPad6Map["padChannel"] = 0;

    defaultPad7Map["enableGrid"] = 0;
    defaultPad7Map["inDmNoteG"] = -1;
    defaultPad7Map["inDmNoteR"] = -1;
    defaultPad7Map["inGmNoteGNE"] = -1;
    defaultPad7Map["inGmNoteGNW"] = -1;
    defaultPad7Map["inGmNoteGSE"] = -1;
    defaultPad7Map["inGmNoteGSW"] = -1;
    defaultPad7Map["inGmNoteRNE"] = -1;
    defaultPad7Map["inGmNoteRNW"] = -1;
    defaultPad7Map["inGmNoteRSE"] = -1;
    defaultPad7Map["inGmNoteRSW"] = -1;
    defaultPad7Map["outDmNote"] = -1;
    defaultPad7Map["outDmNotePressMode"] = 0;
    defaultPad7Map["outDmPress"] = -1;
    defaultPad7Map["outDmPressValue"] = -1;
    defaultPad7Map["outDmVelocityValue"] = -1;
    defaultPad7Map["outDmXCC"] = -1;
    defaultPad7Map["outDmXReturn"] = 63;
    defaultPad7Map["outDmYReturn"] = 63;
    defaultPad7Map["outDmXYReturn"] = 63;
    defaultPad7Map["outDmYCC"] = -1;
    defaultPad7Map["outGmNoteNE"] = -1;
    defaultPad7Map["outGmNoteNW"] = -1;
    defaultPad7Map["outGmNoteSE"] = -1;
    defaultPad7Map["outGmNoteSW"] = -1;
    defaultPad7Map["outGmNotePressModeNE"] = 0;
    defaultPad7Map["outGmNotePressModeNW"] = 0;
    defaultPad7Map["outGmNotePressModeSE"] = 0;
    defaultPad7Map["outGmNotePressModeSW"] = 0;
    defaultPad7Map["outGmPressNE"] = -1;
    defaultPad7Map["outGmPressNW"] = -1;
    defaultPad7Map["outGmPressSE"] = -1;
    defaultPad7Map["outGmPressSW"] = -1;
    defaultPad7Map["outGmPressValueNE"] = -1;
    defaultPad7Map["outGmPressValueNW"] = -1;
    defaultPad7Map["outGmPressValueSE"] = -1;
    defaultPad7Map["outGmPressValueSW"] = -1;
    defaultPad7Map["outGmVelocityValueNE"] = 127;
    defaultPad7Map["outGmVelocityValueNW"] = 127;
    defaultPad7Map["outGmVelocityValueSE"] = 127;
    defaultPad7Map["outGmVelocityValueSW"] = 127;
    defaultPad7Map["padSensitivityPerPadUI"] = 0;
    defaultPad7Map["padSensitivityPerPad"] = 119;
    defaultPad7Map["padChannel"] = 0;

    defaultPad8Map["enableGrid"] = 0;
    defaultPad8Map["inDmNoteG"] = -1;
    defaultPad8Map["inDmNoteR"] = -1;
    defaultPad8Map["inGmNoteGNE"] = -1;
    defaultPad8Map["inGmNoteGNW"] = -1;
    defaultPad8Map["inGmNoteGSE"] = -1;
    defaultPad8Map["inGmNoteGSW"] = -1;
    defaultPad8Map["inGmNoteRNE"] = -1;
    defaultPad8Map["inGmNoteRNW"] = -1;
    defaultPad8Map["inGmNoteRSE"] = -1;
    defaultPad8Map["inGmNoteRSW"] = -1;
    defaultPad8Map["outDmNote"] = -1;
    defaultPad8Map["outDmNotePressMode"] = 0;
    defaultPad8Map["outDmPress"] = -1;
    defaultPad8Map["outDmPressValue"] = -1;
    defaultPad8Map["outDmVelocityValue"] = -1;
    defaultPad8Map["outDmXCC"] = -1;
    defaultPad8Map["outDmXReturn"] = 63;
    defaultPad8Map["outDmYReturn"] = 63;
    defaultPad8Map["outDmXYReturn"] = 63;
    defaultPad8Map["outDmYCC"] = -1;
    defaultPad8Map["outGmNoteNE"] = -1;
    defaultPad8Map["outGmNoteNW"] = -1;
    defaultPad8Map["outGmNoteSE"] = -1;
    defaultPad8Map["outGmNoteSW"] = -1;
    defaultPad8Map["outGmNotePressModeNE"] = 0;
    defaultPad8Map["outGmNotePressModeNW"] = 0;
    defaultPad8Map["outGmNotePressModeSE"] = 0;
    defaultPad8Map["outGmNotePressModeSW"] = 0;
    defaultPad8Map["outGmPressNE"] = -1;
    defaultPad8Map["outGmPressNW"] = -1;
    defaultPad8Map["outGmPressSE"] = -1;
    defaultPad8Map["outGmPressSW"] = -1;
    defaultPad8Map["outGmPressValueNE"] = -1;
    defaultPad8Map["outGmPressValueNW"] = -1;
    defaultPad8Map["outGmPressValueSE"] = -1;
    defaultPad8Map["outGmPressValueSW"] = -1;
    defaultPad8Map["outGmVelocityValueNE"] = 127;
    defaultPad8Map["outGmVelocityValueNW"] = 127;
    defaultPad8Map["outGmVelocityValueSE"] = 127;
    defaultPad8Map["outGmVelocityValueSW"] = 127;
    defaultPad8Map["padSensitivityPerPadUI"] = 0;
    defaultPad8Map["padSensitivityPerPad"] = 119;
    defaultPad8Map["padChannel"] = 0;

    defaultPad9Map["enableGrid"] = 0;
    defaultPad9Map["inDmNoteG"] = -1;
    defaultPad9Map["inDmNoteR"] = -1;
    defaultPad9Map["inGmNoteGNE"] = -1;
    defaultPad9Map["inGmNoteGNW"] = -1;
    defaultPad9Map["inGmNoteGSE"] = -1;
    defaultPad9Map["inGmNoteGSW"] = -1;
    defaultPad9Map["inGmNoteRNE"] = -1;
    defaultPad9Map["inGmNoteRNW"] = -1;
    defaultPad9Map["inGmNoteRSE"] = -1;
    defaultPad9Map["inGmNoteRSW"] = -1;
    defaultPad9Map["outDmNote"] = -1;
    defaultPad9Map["outDmNotePressMode"] = 0;
    defaultPad9Map["outDmPress"] = -1;
    defaultPad9Map["outDmPressValue"] = -1;
    defaultPad9Map["outDmVelocityValue"] = -1;
    defaultPad9Map["outDmXCC"] = -1;
    defaultPad9Map["outDmXReturn"] = 63;
    defaultPad9Map["outDmYReturn"] = 63;
    defaultPad9Map["outDmXYReturn"] = 63;
    defaultPad9Map["outDmYCC"] = -1;
    defaultPad9Map["outGmNoteNE"] = -1;
    defaultPad9Map["outGmNoteNW"] = -1;
    defaultPad9Map["outGmNoteSE"] = -1;
    defaultPad9Map["outGmNoteSW"] = -1;
    defaultPad9Map["outGmNotePressModeNE"] = 0;
    defaultPad9Map["outGmNotePressModeNW"] = 0;
    defaultPad9Map["outGmNotePressModeSE"] = 0;
    defaultPad9Map["outGmNotePressModeSW"] = 0;
    defaultPad9Map["outGmPressNE"] = -1;
    defaultPad9Map["outGmPressNW"] = -1;
    defaultPad9Map["outGmPressSE"] = -1;
    defaultPad9Map["outGmPressSW"] = -1;
    defaultPad9Map["outGmPressValueNE"] = -1;
    defaultPad9Map["outGmPressValueNW"] = -1;
    defaultPad9Map["outGmPressValueSE"] = -1;
    defaultPad9Map["outGmPressValueSW"] = -1;
    defaultPad9Map["outGmVelocityValueNE"] = 127;
    defaultPad9Map["outGmVelocityValueNW"] = 127;
    defaultPad9Map["outGmVelocityValueSE"] = 127;
    defaultPad9Map["outGmVelocityValueSW"] = 127;
    defaultPad9Map["padSensitivityPerPadUI"] = 0;
    defaultPad9Map["padSensitivityPerPad"] = 119;
    defaultPad9Map["padChannel"] = 0;

    defaultPad10Map["enableGrid"] = 0;
    defaultPad10Map["inDmNoteG"] = -1;
    defaultPad10Map["inDmNoteR"] = -1;
    defaultPad10Map["inGmNoteGNE"] = -1;
    defaultPad10Map["inGmNoteGNW"] = -1;
    defaultPad10Map["inGmNoteGSE"] = -1;
    defaultPad10Map["inGmNoteGSW"] = -1;
    defaultPad10Map["inGmNoteRNE"] = -1;
    defaultPad10Map["inGmNoteRNW"] = -1;
    defaultPad10Map["inGmNoteRSE"] = -1;
    defaultPad10Map["inGmNoteRSW"] = -1;
    defaultPad10Map["outDmNote"] = -1;
    defaultPad10Map["outDmNotePressMode"] = 0;
    defaultPad10Map["outDmPress"] = -1;
    defaultPad10Map["outDmPressValue"] = -1;
    defaultPad10Map["outDmVelocityValue"] = -1;
    defaultPad10Map["outDmXCC"] = -1;
    defaultPad10Map["outDmXReturn"] = 63;
    defaultPad10Map["outDmYReturn"] = 63;
    defaultPad10Map["outDmXYReturn"] = 63;
    defaultPad10Map["outDmYCC"] = -1;
    defaultPad10Map["outGmNoteNE"] = -1;
    defaultPad10Map["outGmNoteNW"] = -1;
    defaultPad10Map["outGmNoteSE"] = -1;
    defaultPad10Map["outGmNoteSW"] = -1;
    defaultPad10Map["outGmNotePressModeNE"] = 0;
    defaultPad10Map["outGmNotePressModeNW"] = 0;
    defaultPad10Map["outGmNotePressModeSE"] = 0;
    defaultPad10Map["outGmNotePressModeSW"] = 0;
    defaultPad10Map["outGmPressNE"] = -1;
    defaultPad10Map["outGmPressNW"] = -1;
    defaultPad10Map["outGmPressSE"] = -1;
    defaultPad10Map["outGmPressSW"] = -1;
    defaultPad10Map["outGmPressValueNE"] = -1;
    defaultPad10Map["outGmPressValueNW"] = -1;
    defaultPad10Map["outGmPressValueSE"] = -1;
    defaultPad10Map["outGmPressValueSW"] = -1;
    defaultPad10Map["outGmVelocityValueNE"] = 127;
    defaultPad10Map["outGmVelocityValueNW"] = 127;
    defaultPad10Map["outGmVelocityValueSE"] = 127;
    defaultPad10Map["outGmVelocityValueSW"] = 127;
    defaultPad10Map["padSensitivityPerPadUI"] = 0;
    defaultPad10Map["padSensitivityPerPad"] = 119;
    defaultPad10Map["padChannel"] = 0;

    defaultPad11Map["enableGrid"] = 0;
    defaultPad11Map["inDmNoteG"] = -1;
    defaultPad11Map["inDmNoteR"] = -1;
    defaultPad11Map["inGmNoteGNE"] = -1;
    defaultPad11Map["inGmNoteGNW"] = -1;
    defaultPad11Map["inGmNoteGSE"] = -1;
    defaultPad11Map["inGmNoteGSW"] = -1;
    defaultPad11Map["inGmNoteRNE"] = -1;
    defaultPad11Map["inGmNoteRNW"] = -1;
    defaultPad11Map["inGmNoteRSE"] = -1;
    defaultPad11Map["inGmNoteRSW"] = -1;
    defaultPad11Map["outDmNote"] = -1;
    defaultPad11Map["outDmNotePressMode"] = 0;
    defaultPad11Map["outDmPress"] = -1;
    defaultPad11Map["outDmPressValue"] = -1;
    defaultPad11Map["outDmVelocityValue"] = -1;
    defaultPad11Map["outDmXCC"] = -1;
    defaultPad11Map["outDmXReturn"] = 63;
    defaultPad11Map["outDmYReturn"] = 63;
    defaultPad11Map["outDmXYReturn"] = 63;
    defaultPad11Map["outDmYCC"] = -1;
    defaultPad11Map["outGmNoteNE"] = -1;
    defaultPad11Map["outGmNoteNW"] = -1;
    defaultPad11Map["outGmNoteSE"] = -1;
    defaultPad11Map["outGmNoteSW"] = -1;
    defaultPad11Map["outGmNotePressModeNE"] = 0;
    defaultPad11Map["outGmNotePressModeNW"] = 0;
    defaultPad11Map["outGmNotePressModeSE"] = 0;
    defaultPad11Map["outGmNotePressModeSW"] = 0;
    defaultPad11Map["outGmPressNE"] = -1;
    defaultPad11Map["outGmPressNW"] = -1;
    defaultPad11Map["outGmPressSE"] = -1;
    defaultPad11Map["outGmPressSW"] = -1;
    defaultPad11Map["outGmPressValueNE"] = -1;
    defaultPad11Map["outGmPressValueNW"] = -1;
    defaultPad11Map["outGmPressValueSE"] = -1;
    defaultPad11Map["outGmPressValueSW"] = -1;
    defaultPad11Map["outGmVelocityValueNE"] = 127;
    defaultPad11Map["outGmVelocityValueNW"] = 127;
    defaultPad11Map["outGmVelocityValueSE"] = 127;
    defaultPad11Map["outGmVelocityValueSW"] = 127;
    defaultPad11Map["padSensitivityPerPadUI"] = 0;
    defaultPad11Map["padSensitivityPerPad"] = 119;
    defaultPad11Map["padChannel"] = 0;

    defaultPad12Map["enableGrid"] = 0;
    defaultPad12Map["inDmNoteG"] = -1;
    defaultPad12Map["inDmNoteR"] = -1;
    defaultPad12Map["inGmNoteGNE"] = -1;
    defaultPad12Map["inGmNoteGNW"] = -1;
    defaultPad12Map["inGmNoteGSE"] = -1;
    defaultPad12Map["inGmNoteGSW"] = -1;
    defaultPad12Map["inGmNoteRNE"] = -1;
    defaultPad12Map["inGmNoteRNW"] = -1;
    defaultPad12Map["inGmNoteRSE"] = -1;
    defaultPad12Map["inGmNoteRSW"] = -1;
    defaultPad12Map["outDmNote"] = -1;
    defaultPad12Map["outDmNotePressMode"] = 0;
    defaultPad12Map["outDmPress"] = -1;
    defaultPad12Map["outDmPressValue"] = -1;
    defaultPad12Map["outDmVelocityValue"] = -1;
    defaultPad12Map["outDmXCC"] = -1;
    defaultPad12Map["outDmXReturn"] = 63;
    defaultPad12Map["outDmYReturn"] = 63;
    defaultPad12Map["outDmXYReturn"] = 63;
    defaultPad12Map["outDmYCC"] = -1;
    defaultPad12Map["outGmNoteNE"] = -1;
    defaultPad12Map["outGmNoteNW"] = -1;
    defaultPad12Map["outGmNoteSE"] = -1;
    defaultPad12Map["outGmNoteSW"] = -1;
    defaultPad12Map["outGmNotePressModeNE"] = 0;
    defaultPad12Map["outGmNotePressModeNW"] = 0;
    defaultPad12Map["outGmNotePressModeSE"] = 0;
    defaultPad12Map["outGmNotePressModeSW"] = 0;
    defaultPad12Map["outGmPressNE"] = -1;
    defaultPad12Map["outGmPressNW"] = -1;
    defaultPad12Map["outGmPressSE"] = -1;
    defaultPad12Map["outGmPressSW"] = -1;
    defaultPad12Map["outGmPressValueNE"] = -1;
    defaultPad12Map["outGmPressValueNW"] = -1;
    defaultPad12Map["outGmPressValueSE"] = -1;
    defaultPad12Map["outGmPressValueSW"] = -1;
    defaultPad12Map["outGmVelocityValueNE"] = 127;
    defaultPad12Map["outGmVelocityValueNW"] = 127;
    defaultPad12Map["outGmVelocityValueSE"] = 127;
    defaultPad12Map["outGmVelocityValueSW"] = 127;
    defaultPad12Map["padSensitivityPerPadUI"] = 0;
    defaultPad12Map["padSensitivityPerPad"] = 119;
    defaultPad12Map["padChannel"] = 0;

    defaultPad13Map["enableGrid"] = 0;
    defaultPad13Map["inDmNoteG"] = -1;
    defaultPad13Map["inDmNoteR"] = -1;
    defaultPad13Map["inGmNoteGNE"] = -1;
    defaultPad13Map["inGmNoteGNW"] = -1;
    defaultPad13Map["inGmNoteGSE"] = -1;
    defaultPad13Map["inGmNoteGSW"] = -1;
    defaultPad13Map["inGmNoteRNE"] = -1;
    defaultPad13Map["inGmNoteRNW"] = -1;
    defaultPad13Map["inGmNoteRSE"] = -1;
    defaultPad13Map["inGmNoteRSW"] = -1;
    defaultPad13Map["outDmNote"] = -1;
    defaultPad13Map["outDmNotePressMode"] = 0;
    defaultPad13Map["outDmPress"] = -1;
    defaultPad13Map["outDmPressValue"] = -1;
    defaultPad13Map["outDmVelocityValue"] = -1;
    defaultPad13Map["outDmXCC"] = -1;
    defaultPad13Map["outDmXReturn"] = 63;
    defaultPad13Map["outDmYReturn"] = 63;
    defaultPad13Map["outDmXYReturn"] = 63;
    defaultPad13Map["outDmYCC"] = -1;
    defaultPad13Map["outGmNoteNE"] = -1;
    defaultPad13Map["outGmNoteNW"] = -1;
    defaultPad13Map["outGmNoteSE"] = -1;
    defaultPad13Map["outGmNoteSW"] = -1;
    defaultPad13Map["outGmNotePressModeNE"] = 0;
    defaultPad13Map["outGmNotePressModeNW"] = 0;
    defaultPad13Map["outGmNotePressModeSE"] = 0;
    defaultPad13Map["outGmNotePressModeSW"] = 0;
    defaultPad13Map["outGmPressNE"] = -1;
    defaultPad13Map["outGmPressNW"] = -1;
    defaultPad13Map["outGmPressSE"] = -1;
    defaultPad13Map["outGmPressSW"] = -1;
    defaultPad13Map["outGmPressValueNE"] = -1;
    defaultPad13Map["outGmPressValueNW"] = -1;
    defaultPad13Map["outGmPressValueSE"] = -1;
    defaultPad13Map["outGmPressValueSW"] = -1;
    defaultPad13Map["outGmVelocityValueNE"] = 127;
    defaultPad13Map["outGmVelocityValueNW"] = 127;
    defaultPad13Map["outGmVelocityValueSE"] = 127;
    defaultPad13Map["outGmVelocityValueSW"] = 127;
    defaultPad13Map["padSensitivityPerPadUI"] = 0;
    defaultPad13Map["padSensitivityPerPad"] = 119;
    defaultPad13Map["padChannel"] = 0;

    defaultPad14Map["enableGrid"] = 0;
    defaultPad14Map["inDmNoteG"] = -1;
    defaultPad14Map["inDmNoteR"] = -1;
    defaultPad14Map["inGmNoteGNE"] = -1;
    defaultPad14Map["inGmNoteGNW"] = -1;
    defaultPad14Map["inGmNoteGSE"] = -1;
    defaultPad14Map["inGmNoteGSW"] = -1;
    defaultPad14Map["inGmNoteRNE"] = -1;
    defaultPad14Map["inGmNoteRNW"] = -1;
    defaultPad14Map["inGmNoteRSE"] = -1;
    defaultPad14Map["inGmNoteRSW"] = -1;
    defaultPad14Map["outDmNote"] = -1;
    defaultPad14Map["outDmNotePressMode"] = 0;
    defaultPad14Map["outDmPress"] = -1;
    defaultPad14Map["outDmPressValue"] = -1;
    defaultPad14Map["outDmVelocityValue"] = -1;
    defaultPad14Map["outDmXCC"] = -1;
    defaultPad14Map["outDmXReturn"] = 63;
    defaultPad14Map["outDmYReturn"] = 63;
    defaultPad14Map["outDmXYReturn"] = 63;
    defaultPad14Map["outDmYCC"] = -1;
    defaultPad14Map["outGmNoteNE"] = -1;
    defaultPad14Map["outGmNoteNW"] = -1;
    defaultPad14Map["outGmNoteSE"] = -1;
    defaultPad14Map["outGmNoteSW"] = -1;
    defaultPad14Map["outGmNotePressModeNE"] = 0;
    defaultPad14Map["outGmNotePressModeNW"] = 0;
    defaultPad14Map["outGmNotePressModeSE"] = 0;
    defaultPad14Map["outGmNotePressModeSW"] = 0;
    defaultPad14Map["outGmPressNE"] = -1;
    defaultPad14Map["outGmPressNW"] = -1;
    defaultPad14Map["outGmPressSE"] = -1;
    defaultPad14Map["outGmPressSW"] = -1;
    defaultPad14Map["outGmPressValueNE"] = -1;
    defaultPad14Map["outGmPressValueNW"] = -1;
    defaultPad14Map["outGmPressValueSE"] = -1;
    defaultPad14Map["outGmPressValueSW"] = -1;
    defaultPad14Map["outGmVelocityValueNE"] = 127;
    defaultPad14Map["outGmVelocityValueNW"] = 127;
    defaultPad14Map["outGmVelocityValueSE"] = 127;
    defaultPad14Map["outGmVelocityValueSW"] = 127;
    defaultPad14Map["padSensitivityPerPadUI"] = 0;
    defaultPad14Map["padSensitivityPerPad"] = 119;
    defaultPad14Map["padChannel"] = 0;

    defaultPad15Map["enableGrid"] = 0;
    defaultPad15Map["inDmNoteG"] = -1;
    defaultPad15Map["inDmNoteR"] = -1;
    defaultPad15Map["inGmNoteGNE"] = -1;
    defaultPad15Map["inGmNoteGNW"] = -1;
    defaultPad15Map["inGmNoteGSE"] = -1;
    defaultPad15Map["inGmNoteGSW"] = -1;
    defaultPad15Map["inGmNoteRNE"] = -1;
    defaultPad15Map["inGmNoteRNW"] = -1;
    defaultPad15Map["inGmNoteRSE"] = -1;
    defaultPad15Map["inGmNoteRSW"] = -1;
    defaultPad15Map["outDmNote"] = -1;
    defaultPad15Map["outDmNotePressMode"] = 0;
    defaultPad15Map["outDmPress"] = -1;
    defaultPad15Map["outDmPressValue"] = -1;
    defaultPad15Map["outDmVelocityValue"] = -1;
    defaultPad15Map["outDmXCC"] = -1;
    defaultPad15Map["outDmXReturn"] = 63;
    defaultPad15Map["outDmYReturn"] = 63;
    defaultPad15Map["outDmXYReturn"] = 63;
    defaultPad15Map["outDmYCC"] = -1;
    defaultPad15Map["outGmNoteNE"] = -1;
    defaultPad15Map["outGmNoteNW"] = -1;
    defaultPad15Map["outGmNoteSE"] = -1;
    defaultPad15Map["outGmNoteSW"] = -1;
    defaultPad15Map["outGmNotePressModeNE"] = 0;
    defaultPad15Map["outGmNotePressModeNW"] = 0;
    defaultPad15Map["outGmNotePressModeSE"] = 0;
    defaultPad15Map["outGmNotePressModeSW"] = 0;
    defaultPad15Map["outGmPressNE"] = -1;
    defaultPad15Map["outGmPressNW"] = -1;
    defaultPad15Map["outGmPressSE"] = -1;
    defaultPad15Map["outGmPressSW"] = -1;
    defaultPad15Map["outGmPressValueNE"] = -1;
    defaultPad15Map["outGmPressValueNW"] = -1;
    defaultPad15Map["outGmPressValueSE"] = -1;
    defaultPad15Map["outGmPressValueSW"] = -1;
    defaultPad15Map["outGmVelocityValueNE"] = 127;
    defaultPad15Map["outGmVelocityValueNW"] = 127;
    defaultPad15Map["outGmVelocityValueSE"] = 127;
    defaultPad15Map["outGmVelocityValueSW"] = 127;
    defaultPad15Map["padSensitivityPerPadUI"] = 0;
    defaultPad15Map["padSensitivityPerPad"] = 119;
    defaultPad15Map["padChannel"] = 0;

    defaultRhombusButtonsMap["rhombusInChannel"] = 0;
    defaultRhombusButtonsMap["rhombusLocalLEDControl"] = 2;
    defaultRhombusButtonsMap["rhombusOnThreshold"] = 10;
    defaultRhombusButtonsMap["rhombusOffThreshold"] = 5;
    defaultRhombusButtonsMap["rhombusSensitivity"] = 66;

    defaultRhombusButton0Map["rhombusBankControl"] = 0;
    defaultRhombusButton0Map["rhombusChannel"] = 0;
    defaultRhombusButton0Map["rhombusEnableSwitch"] = 0;
    defaultRhombusButton0Map["rhombusInNoteG"] = -1;
    defaultRhombusButton0Map["rhombusInNoteR"] = -1;
    defaultRhombusButton0Map["rhombusOutNote"] = -1;
    defaultRhombusButton0Map["rhombusOutNotePressMode"] = 0;
    defaultRhombusButton0Map["rhombusOutPress"] = -1;
    defaultRhombusButton0Map["rhombusOutPressValue"] = -1;
    defaultRhombusButton0Map["rhombusOutVelocityValue"] = 127;


    defaultRotariesMap["rotaryInChannel"] = 0;
    defaultRotariesMap["rotaryLocalLEDControl"] = 2;
    defaultRotariesMap["rotaryOnThreshold"] = 10;
    defaultRotariesMap["rotaryOffThreshold"] = 5;
    defaultRotariesMap["rotarySensitivity"] = 66;

    defaultRotary0Map["rB1Channel"] = 0;
    defaultRotary0Map["rB1DirStorage"] = -1;
    defaultRotary0Map["rB1LocStorage"] = -1;
    defaultRotary0Map["rB1inLocation"] = -1;
    defaultRotary0Map["rB1outDirection"] = -1;
    defaultRotary0Map["rB1outDirectionEnable"] = 2;
    defaultRotary0Map["rB1outLocPassThruRange"] = 127;
    defaultRotary0Map["rB1outLocation"] = -1;
    defaultRotary0Map["rB1outNote"] = -1;
    defaultRotary0Map["rB1outNotePressMode"] = 0;
    defaultRotary0Map["rB1outPress"] = -1;
    defaultRotary0Map["rB1outPressValue"] = -1;
    defaultRotary0Map["rB1outSpeed"] = 5;
    defaultRotary0Map["rB1outVelocityValue"] = 127;
    defaultRotary0Map["rB2Channel"] = 0;
    defaultRotary0Map["rB2DirStorage"] = -1;
    defaultRotary0Map["rB2LocStorage"] = -1;
    defaultRotary0Map["rB2inLocation"] = -1;
    defaultRotary0Map["rB2outDirection"] = -1;
    defaultRotary0Map["rB2outDirectionEnable"] = 0;
    defaultRotary0Map["rB2outLocPassThruRange"] = 127;
    defaultRotary0Map["rB2outLocation"] = -1;
    defaultRotary0Map["rB2outNote"] = -1;
    defaultRotary0Map["rB2outNotePressMode"] = 0;
    defaultRotary0Map["rB2outPress"] = -1;
    defaultRotary0Map["rB2outPressValue"] = -1;
    defaultRotary0Map["rB2outSpeed"] = 5;
    defaultRotary0Map["rB2outVelocityValue"] = 127;
    defaultRotary0Map["rB3Channel"] = 0;
    defaultRotary0Map["rB3DirStorage"] = -1;
    defaultRotary0Map["rB3LocStorage"] = -1;
    defaultRotary0Map["rB3inLocation"] = -1;
    defaultRotary0Map["rB3outDirection"] = -1;
    defaultRotary0Map["rB3outDirectionEnable"] = 0;
    defaultRotary0Map["rB3outLocPassThruRange"] = 127;
    defaultRotary0Map["rB3outLocation"] = -1;
    defaultRotary0Map["rB3outNote"] = -1;
    defaultRotary0Map["rB3outNotePressMode"] = 0;
    defaultRotary0Map["rB3outPress"] = -1;
    defaultRotary0Map["rB3outPressValue"] = -1;
    defaultRotary0Map["rB3outSpeed"] = 5;
    defaultRotary0Map["rB3outVelocityValue"] = 127;
    defaultRotary0Map["rB4Channel"] = 0;
    defaultRotary0Map["rB4DirStorage"] = -1;
    defaultRotary0Map["rB4LocStorage"] = -1;
    defaultRotary0Map["rB4inLocation"] = -1;
    defaultRotary0Map["rB4outDirection"] = -1;
    defaultRotary0Map["rB4outDirectionEnable"] = 0;
    defaultRotary0Map["rB4outLocPassThruRange"] = 127;
    defaultRotary0Map["rB4outLocation"] = -1;
    defaultRotary0Map["rB4outNote"] = -1;
    defaultRotary0Map["rB4outNotePressMode"] = 0;
    defaultRotary0Map["rB4outPress"] = -1;
    defaultRotary0Map["rB4outPressValue"] = -1;
    defaultRotary0Map["rB4outSpeed"] = 5;
    defaultRotary0Map["rB4outVelocityValue"] = 127;

    defaultRotary1Map["rB1Channel"] = 0;
    defaultRotary1Map["rB1DirStorage"] = -1;
    defaultRotary1Map["rB1LocStorage"] = -1;
    defaultRotary1Map["rB1inLocation"] = -1;
    defaultRotary1Map["rB1outDirection"] = -1;
    defaultRotary1Map["rB1outDirectionEnable"] = 2;
    defaultRotary1Map["rB1outLocPassThruRange"] = 127;
    defaultRotary1Map["rB1outLocation"] = -1;
    defaultRotary1Map["rB1outNote"] = -1;
    defaultRotary1Map["rB1outNotePressMode"] = 0;
    defaultRotary1Map["rB1outPress"] = -1;
    defaultRotary1Map["rB1outPressValue"] = -1;
    defaultRotary1Map["rB1outSpeed"] = 5;
    defaultRotary1Map["rB1outVelocityValue"] = 127;
    defaultRotary1Map["rB2Channel"] = 0;
    defaultRotary1Map["rB2DirStorage"] = -1;
    defaultRotary1Map["rB2LocStorage"] = -1;
    defaultRotary1Map["rB2inLocation"] = -1;
    defaultRotary1Map["rB2outDirection"] = -1;
    defaultRotary1Map["rB2outDirectionEnable"] = 0;
    defaultRotary1Map["rB2outLocPassThruRange"] = 127;
    defaultRotary1Map["rB2outLocation"] = -1;
    defaultRotary1Map["rB2outNote"] = -1;
    defaultRotary1Map["rB2outNotePressMode"] = 0;
    defaultRotary1Map["rB2outPress"] = -1;
    defaultRotary1Map["rB2outPressValue"] = -1;
    defaultRotary1Map["rB2outSpeed"] = 5;
    defaultRotary1Map["rB2outVelocityValue"] = 127;
    defaultRotary1Map["rB3Channel"] = 0;
    defaultRotary1Map["rB3DirStorage"] = -1;
    defaultRotary1Map["rB3LocStorage"] = -1;
    defaultRotary1Map["rB3inLocation"] = -1;
    defaultRotary1Map["rB3outDirection"] = -1;
    defaultRotary1Map["rB3outDirectionEnable"] = 0;
    defaultRotary1Map["rB3outLocPassThruRange"] = 127;
    defaultRotary1Map["rB3outLocation"] = -1;
    defaultRotary1Map["rB3outNote"] = -1;
    defaultRotary1Map["rB3outNotePressMode"] = 0;
    defaultRotary1Map["rB3outPress"] = -1;
    defaultRotary1Map["rB3outPressValue"] = -1;
    defaultRotary1Map["rB3outSpeed"] = 5;
    defaultRotary1Map["rB3outVelocityValue"] = 127;
    defaultRotary1Map["rB4Channel"] = 0;
    defaultRotary1Map["rB4DirStorage"] = -1;
    defaultRotary1Map["rB4LocStorage"] = -1;
    defaultRotary1Map["rB4inLocation"] = -1;
    defaultRotary1Map["rB4outDirection"] = -1;
    defaultRotary1Map["rB4outDirectionEnable"] = 0;
    defaultRotary1Map["rB4outLocPassThruRange"] = 127;
    defaultRotary1Map["rB4outLocation"] = -1;
    defaultRotary1Map["rB4outNote"] = -1;
    defaultRotary1Map["rB4outNotePressMode"] = 0;
    defaultRotary1Map["rB4outPress"] = -1;
    defaultRotary1Map["rB4outPressValue"] = -1;
    defaultRotary1Map["rB4outSpeed"] = 5;
    defaultRotary1Map["rB4outVelocityValue"] = 127;

    defaultTransportButtonsMap["transportInChannel"] = 0;
    defaultTransportButtonsMap["transportLocalLEDControl"] = 2;
    defaultTransportButtonsMap["transportOnThreshold"] = 10;
    defaultTransportButtonsMap["transportOffThreshold"] = 5;
    defaultTransportButtonsMap["transportSensitivity"] = 66;

    defaultTransportButton0Map["transportChannel"] = 0;
    defaultTransportButton0Map["transportInNote"] = -1;
    defaultTransportButton0Map["transportOutNote"] = -1;
    defaultTransportButton0Map["transportOutNotePressMode"] = 0;
    defaultTransportButton0Map["transportOutPress"] = -1;
    defaultTransportButton0Map["transportOutPressValue"] = -1;
    defaultTransportButton0Map["transportOutVelocityValue"] = 127;

    defaultTransportButton1Map["transportChannel"] = 0;
    defaultTransportButton1Map["transportInNote"] = -1;
    defaultTransportButton1Map["transportOutNote"] = -1;
    defaultTransportButton1Map["transportOutNotePressMode"] = 0;
    defaultTransportButton1Map["transportOutPress"] = -1;
    defaultTransportButton1Map["transportOutPressValue"] = -1;
    defaultTransportButton1Map["transportOutVelocityValue"] = 127;

    defaultTransportButton2Map["transportChannel"] = 0;
    defaultTransportButton2Map["transportInNote"] = -1;
    defaultTransportButton2Map["transportOutNote"] = -1;
    defaultTransportButton2Map["transportOutNotePressMode"] = 0;
    defaultTransportButton2Map["transportOutPress"] = -1;
    defaultTransportButton2Map["transportOutPressValue"] = -1;
    defaultTransportButton2Map["transportOutVelocityValue"] = 127;

    defaultUpDownButtonsMap["updownInChannel"] = 0;
    defaultUpDownButtonsMap["updownLocalLEDControl"] = 2;
    defaultUpDownButtonsMap["updownOnThreshold"] = 10;
    defaultUpDownButtonsMap["updownOffThreshold"] = 5;
    defaultUpDownButtonsMap["updownSensitivity"] = 66;

    defaultUpDownButton0Map["updownBankControl"] = 1;
    defaultUpDownButton0Map["updownChannel"] = 0;
    defaultUpDownButton0Map["updownDInNote"] = -1;
    defaultUpDownButton0Map["updownDOutNote"] = -1;
    defaultUpDownButton0Map["updownDOutNotePressMode"] = 0;
    defaultUpDownButton0Map["updownDOutPress"] = -1;
    defaultUpDownButton0Map["updownDOutPressValue"] = -1;
    defaultUpDownButton0Map["updownDOutVelocityValue"] = 127;
    defaultUpDownButton0Map["updownEnableSwitch"] = 0;
    defaultUpDownButton0Map["updownUInNote"] = -1;
    defaultUpDownButton0Map["updownUOutNote"] = -1;
    defaultUpDownButton0Map["updownUOutNotePressMode"] = 0;
    defaultUpDownButton0Map["updownUOutPress"] = -1;
    defaultUpDownButton0Map["updownUOutPressValue"] = -1;
    defaultUpDownButton0Map["updownUOutVelocityValue"] = 127;

    defaultUpDownButton1Map["updownBankControl"] = 1;
    defaultUpDownButton1Map["updownChannel"] = 0;
    defaultUpDownButton1Map["updownDInNote"] = -1;
    defaultUpDownButton1Map["updownDOutNote"] = -1;
    defaultUpDownButton1Map["updownDOutNotePressMode"] = 0;
    defaultUpDownButton1Map["updownDOutPress"] = -1;
    defaultUpDownButton1Map["updownDOutPressValue"] = -1;
    defaultUpDownButton1Map["updownDOutVelocityValue"] = 127;
    defaultUpDownButton1Map["updownEnableSwitch"] = 0;
    defaultUpDownButton1Map["updownUInNote"] = -1;
    defaultUpDownButton1Map["updownUOutNote"] = -1;
    defaultUpDownButton1Map["updownUOutNotePressMode"] = 0;
    defaultUpDownButton1Map["updownUOutPress"] = -1;
    defaultUpDownButton1Map["updownUOutPressValue"] = -1;
    defaultUpDownButton1Map["updownUOutVelocityValue"] = 127;

    defaultVSlidersMap["vSliderInChannel"] = 0;
    defaultVSlidersMap["vSliderLocalLEDControl"] = 2;
    defaultVSlidersMap["vSliderOnThreshold"] = 10;
    defaultVSlidersMap["vSliderOffThreshold"] = 5;
    defaultVSlidersMap["vSliderSensitivity"] = 66;

    defaultVSlider0Map["vB1Channel"] = 0;
    defaultVSlider0Map["vB1inLocation"] = -1;
    defaultVSlider0Map["vB1outLocPassThruRange"] = 127;
    defaultVSlider0Map["vB1outLocation"] = -1;
    defaultVSlider0Map["vB1outNote"] = -1;
    defaultVSlider0Map["vB1outNotePressMode"] = 0;
    defaultVSlider0Map["vB1outPress"] = -1;
    defaultVSlider0Map["vB1outPressValue"] = -1;
    defaultVSlider0Map["vB1outVelocityValue"] = 127;
    defaultVSlider0Map["vB2Channel"] = 0;
    defaultVSlider0Map["vB2inLocation"] = -1;
    defaultVSlider0Map["vB2outLocPassThruRange"] = 127;
    defaultVSlider0Map["vB2outLocation"] = -1;
    defaultVSlider0Map["vB2outNote"] = -1;
    defaultVSlider0Map["vB2outNotePressMode"] = 0;
    defaultVSlider0Map["vB2outPress"] = -1;
    defaultVSlider0Map["vB2outPressValue"] = -1;
    defaultVSlider0Map["vB2outVelocityValue"] = 127;
    defaultVSlider0Map["vB3Channel"] = 0;
    defaultVSlider0Map["vB3inLocation"] = -1;
    defaultVSlider0Map["vB3outLocPassThruRange"] = 127;
    defaultVSlider0Map["vB3outLocation"] = -1;
    defaultVSlider0Map["vB3outNote"] = -1;
    defaultVSlider0Map["vB3outNotePressMode"] = 0;
    defaultVSlider0Map["vB3outPress"] = -1;
    defaultVSlider0Map["vB3outPressValue"] = -1;
    defaultVSlider0Map["vB3outVelocityValue"] = 127;
    defaultVSlider0Map["vB4Channel"] = 0;
    defaultVSlider0Map["vB4inLocation"] = -1;
    defaultVSlider0Map["vB4outLocPassThruRange"] = 127;
    defaultVSlider0Map["vB4outLocation"] = -1;
    defaultVSlider0Map["vB4outNote"] = -1;
    defaultVSlider0Map["vB4outNotePressMode"] = 0;
    defaultVSlider0Map["vB4outPress"] = -1;
    defaultVSlider0Map["vB4outPressValue"] = -1;
    defaultVSlider0Map["vB4outVelocityValue"] = 127;

    defaultVSlider1Map["vB1Channel"] = 0;
    defaultVSlider1Map["vB1inLocation"] = -1;
    defaultVSlider1Map["vB1outLocPassThruRange"] = 127;
    defaultVSlider1Map["vB1outLocation"] = -1;
    defaultVSlider1Map["vB1outNote"] = -1;
    defaultVSlider1Map["vB1outNotePressMode"] = 0;
    defaultVSlider1Map["vB1outPress"] = -1;
    defaultVSlider1Map["vB1outPressValue"] = -1;
    defaultVSlider1Map["vB1outVelocityValue"] = 127;
    defaultVSlider1Map["vB2Channel"] = 0;
    defaultVSlider1Map["vB2inLocation"] = -1;
    defaultVSlider1Map["vB2outLocPassThruRange"] = 127;
    defaultVSlider1Map["vB2outLocation"] = -1;
    defaultVSlider1Map["vB2outNote"] = -1;
    defaultVSlider1Map["vB2outNotePressMode"] = 0;
    defaultVSlider1Map["vB2outPress"] = -1;
    defaultVSlider1Map["vB2outPressValue"] = -1;
    defaultVSlider1Map["vB2outVelocityValue"] = 127;
    defaultVSlider1Map["vB3Channel"] = 0;
    defaultVSlider1Map["vB3inLocation"] = -1;
    defaultVSlider1Map["vB3outLocPassThruRange"] = 127;
    defaultVSlider1Map["vB3outLocation"] = -1;
    defaultVSlider1Map["vB3outNote"] = -1;
    defaultVSlider1Map["vB3outNotePressMode"] = 0;
    defaultVSlider1Map["vB3outPress"] = -1;
    defaultVSlider1Map["vB3outPressValue"] = -1;
    defaultVSlider1Map["vB3outVelocityValue"] = 127;
    defaultVSlider1Map["vB4Channel"] = 0;
    defaultVSlider1Map["vB4inLocation"] = -1;
    defaultVSlider1Map["vB4outLocPassThruRange"] = 127;
    defaultVSlider1Map["vB4outLocation"] = -1;
    defaultVSlider1Map["vB4outNote"] = -1;
    defaultVSlider1Map["vB4outNotePressMode"] = 0;
    defaultVSlider1Map["vB4outPress"] = -1;
    defaultVSlider1Map["vB4outPressValue"] = -1;
    defaultVSlider1Map["vB4outVelocityValue"] = 127;

    defaultVSlider2Map["vB1Channel"] = 0;
    defaultVSlider2Map["vB1inLocation"] = -1;
    defaultVSlider2Map["vB1outLocPassThruRange"] = 127;
    defaultVSlider2Map["vB1outLocation"] = -1;
    defaultVSlider2Map["vB1outNote"] = -1;
    defaultVSlider2Map["vB1outNotePressMode"] = 0;
    defaultVSlider2Map["vB1outPress"] = -1;
    defaultVSlider2Map["vB1outPressValue"] = -1;
    defaultVSlider2Map["vB1outVelocityValue"] = 127;
    defaultVSlider2Map["vB2Channel"] = 0;
    defaultVSlider2Map["vB2inLocation"] = -1;
    defaultVSlider2Map["vB2outLocPassThruRange"] = 127;
    defaultVSlider2Map["vB2outLocation"] = -1;
    defaultVSlider2Map["vB2outNote"] = -1;
    defaultVSlider2Map["vB2outNotePressMode"] = 0;
    defaultVSlider2Map["vB2outPress"] = -1;
    defaultVSlider2Map["vB2outPressValue"] = -1;
    defaultVSlider2Map["vB2outVelocityValue"] = 127;
    defaultVSlider2Map["vB3Channel"] = 0;
    defaultVSlider2Map["vB3inLocation"] = -1;
    defaultVSlider2Map["vB3outLocPassThruRange"] = 127;
    defaultVSlider2Map["vB3outLocation"] = -1;
    defaultVSlider2Map["vB3outNote"] = -1;
    defaultVSlider2Map["vB3outNotePressMode"] = 0;
    defaultVSlider2Map["vB3outPress"] = -1;
    defaultVSlider2Map["vB3outPressValue"] = -1;
    defaultVSlider2Map["vB3outVelocityValue"] = 127;
    defaultVSlider2Map["vB4Channel"] = 0;
    defaultVSlider2Map["vB4inLocation"] = -1;
    defaultVSlider2Map["vB4outLocPassThruRange"] = 127;
    defaultVSlider2Map["vB4outLocation"] = -1;
    defaultVSlider2Map["vB4outNote"] = -1;
    defaultVSlider2Map["vB4outNotePressMode"] = 0;
    defaultVSlider2Map["vB4outPress"] = -1;
    defaultVSlider2Map["vB4outPressValue"] = -1;
    defaultVSlider2Map["vB4outVelocityValue"] = 127;

    defaultVSlider3Map["vB1Channel"] = 0;
    defaultVSlider3Map["vB1inLocation"] = -1;
    defaultVSlider3Map["vB1outLocPassThruRange"] = 127;
    defaultVSlider3Map["vB1outLocation"] = -1;
    defaultVSlider3Map["vB1outNote"] = -1;
    defaultVSlider3Map["vB1outNotePressMode"] = 0;
    defaultVSlider3Map["vB1outPress"] = -1;
    defaultVSlider3Map["vB1outPressValue"] = -1;
    defaultVSlider3Map["vB1outVelocityValue"] = 127;
    defaultVSlider3Map["vB2Channel"] = 0;
    defaultVSlider3Map["vB2inLocation"] = -1;
    defaultVSlider3Map["vB2outLocPassThruRange"] = 127;
    defaultVSlider3Map["vB2outLocation"] = -1;
    defaultVSlider3Map["vB2outNote"] = -1;
    defaultVSlider3Map["vB2outNotePressMode"] = 0;
    defaultVSlider3Map["vB2outPress"] = -1;
    defaultVSlider3Map["vB2outPressValue"] = -1;
    defaultVSlider3Map["vB2outVelocityValue"] = 127;
    defaultVSlider3Map["vB3Channel"] = 0;
    defaultVSlider3Map["vB3inLocation"] = -1;
    defaultVSlider3Map["vB3outLocPassThruRange"] = 127;
    defaultVSlider3Map["vB3outLocation"] = -1;
    defaultVSlider3Map["vB3outNote"] = -1;
    defaultVSlider3Map["vB3outNotePressMode"] = 0;
    defaultVSlider3Map["vB3outPress"] = -1;
    defaultVSlider3Map["vB3outPressValue"] = -1;
    defaultVSlider3Map["vB3outVelocityValue"] = 127;
    defaultVSlider3Map["vB4Channel"] = 0;
    defaultVSlider3Map["vB4inLocation"] = -1;
    defaultVSlider3Map["vB4outLocPassThruRange"] = 127;
    defaultVSlider3Map["vB4outLocation"] = -1;
    defaultVSlider3Map["vB4outNote"] = -1;
    defaultVSlider3Map["vB4outNotePressMode"] = 0;
    defaultVSlider3Map["vB4outPress"] = -1;
    defaultVSlider3Map["vB4outPressValue"] = -1;
    defaultVSlider3Map["vB4outVelocityValue"] = 127;


    //---------------- Imbed Maps -----------------//


    //***insert individual sensors into sensor-type-maps
    //HSliders
    defaultHSlidersMap.insert("HSlider0", defaultHSlider0Map);
    defaultHSlidersMap.insert("HSlider1", defaultHSlider1Map);
    defaultHSlidersMap.insert("HSlider2", defaultHSlider2Map);
    defaultHSlidersMap.insert("HSlider3", defaultHSlider3Map);

    //LeftRightButtons
    defaultLeftRightButtonsMap.insert("LeftRightButton0", defaultLeftRightButton0Map);
    defaultLeftRightButtonsMap.insert("LeftRightButton1", defaultLeftRightButton1Map);
    defaultLeftRightButtonsMap.insert("LeftRightButton2", defaultLeftRightButton2Map);
    defaultLeftRightButtonsMap.insert("LeftRightButton3", defaultLeftRightButton3Map);

    //LongSliders
    defaultLongSlidersMap.insert("LongSlider0", defaultLongSliders0Map);

    //ModeButtons
    defaultModeButtonsMap.insert("ModeButton0", defaultModeButton0Map);

    //Pads
    defaultPadsMap.insert("Pad0", defaultPad0Map);
    defaultPadsMap.insert("Pad1", defaultPad1Map);
    defaultPadsMap.insert("Pad2", defaultPad2Map);
    defaultPadsMap.insert("Pad3", defaultPad3Map);
    defaultPadsMap.insert("Pad4", defaultPad4Map);
    defaultPadsMap.insert("Pad5", defaultPad5Map);
    defaultPadsMap.insert("Pad6", defaultPad6Map);
    defaultPadsMap.insert("Pad7", defaultPad7Map);
    defaultPadsMap.insert("Pad8", defaultPad8Map);
    defaultPadsMap.insert("Pad9", defaultPad9Map);
    defaultPadsMap.insert("Pad10", defaultPad10Map);
    defaultPadsMap.insert("Pad11", defaultPad11Map);
    defaultPadsMap.insert("Pad12", defaultPad12Map);
    defaultPadsMap.insert("Pad13", defaultPad13Map);
    defaultPadsMap.insert("Pad14", defaultPad14Map);
    defaultPadsMap.insert("Pad15", defaultPad15Map);

    //RhombusButtons
    defaultRhombusButtonsMap.insert("RhombusButton0", defaultRhombusButton0Map);

    //Rotaries
    defaultRotariesMap.insert("Rotary0", defaultRotary0Map);
    defaultRotariesMap.insert("Rotary1", defaultRotary1Map);

    //TranportButtons
    defaultTransportButtonsMap.insert("TransportButton0", defaultTransportButton0Map);
    defaultTransportButtonsMap.insert("TransportButton1", defaultTransportButton1Map);
    defaultTransportButtonsMap.insert("TransportButton2", defaultTransportButton2Map);

    //UpDownButtons
    defaultUpDownButtonsMap.insert("UpDownButton0", defaultUpDownButton0Map);
    defaultUpDownButtonsMap.insert("UpDownButton1", defaultUpDownButton1Map);

    //VSliders
    defaultVSlidersMap.insert("VSlider0", defaultVSlider0Map);
    defaultVSlidersMap.insert("VSlider1", defaultVSlider1Map);
    defaultVSlidersMap.insert("VSlider2", defaultVSlider2Map);
    defaultVSlidersMap.insert("VSlider3", defaultVSlider3Map);

    //***insert sensor-type-maps into component map
    defaultComponentSettingsMap.insert("HSliders", defaultHSlidersMap);
    defaultComponentSettingsMap.insert("LeftRightButtons", defaultLeftRightButtonsMap);
    defaultComponentSettingsMap.insert("LongSliders", defaultLongSlidersMap);
    defaultComponentSettingsMap.insert("ModeButtons", defaultModeButtonsMap);
    defaultComponentSettingsMap.insert("Pads", defaultPadsMap);
    defaultComponentSettingsMap.insert("RhombusButtons", defaultRhombusButtonsMap);
    defaultComponentSettingsMap.insert("Rotaries", defaultRotariesMap);
    defaultComponentSettingsMap.insert("TransportButtons", defaultTransportButtonsMap);
    defaultComponentSettingsMap.insert("UpDownButtons", defaultUpDownButtonsMap);
    defaultComponentSettingsMap.insert("VSliders", defaultVSlidersMap);

    //***insert component map into single preset map
    defaultSinglePresetMap.insert("ComponentSettings", defaultComponentSettingsMap);
}

void DataValidator::slotLoadJSON(){

    qDebug() << "slotLoadJSON called";
    //load json into QFile - moved to main class
    //jsonFile = new QFile("../../../presets/QuNeo.json");

//    if(jsonFile->open(QIODevice::ReadWrite | QIODevice::Text)){
//        qDebug("jsonFound");
//    } else {
//        qDebug() << "json not found";
//    }

    // error object
    QJsonParseError JsonParseError;
    // convert file to QJsonDocument. this can be read/written to
    QJsonDocument JsonDocument = QJsonDocument::fromJson(jsonFile->readAll(), &JsonParseError);
    // close jsonFile
    jsonFile->close();
    // convert QJsonDocument to QJsonObject. this can be queried and modified in a human-readable way
    QJsonObject RootObject = JsonDocument.object();

    //load json file into a byte array to be processd by the parser
//    jsonByteArray = jsonFile->readAll();
    jsonByteArray = JsonDocument.toJson();

    //parse the json data, convert it to a map and set it equal to the master jsonMap
//    jsonMasterMap = parser.parse(jsonByteArray, &ok).toMap();
    jsonMasterMap = RootObject.toVariantMap();

    //store a map of the 16 preset maps
    jsonQuNeoPresetsMap = jsonMasterMap["QuNeo Presets"].toMap();

    //qDebug() << jsonQuNeoPresetsMap.keys();


}

void DataValidator::slotParseJSON(){

    qDebug() << "slotCheckJSON called";

}

void DataValidator::slotCheckJSON(){

    qDebug() << "slotCheckJSON called";

    /***************************************************************************/
    /***************************** QuNeo Globals *******************************/
    /***************************************************************************/
    //check prests map-- this is basically for the version parameter
    mapIterator = defaultQuNeoPresetsMap.constBegin();

    while(mapIterator != defaultQuNeoPresetsMap.constEnd()){

        if(jsonQuNeoPresetsMap.contains(mapIterator.key())){
            //qDebug() << mapIterator.key() << "FOUND IN JSON";
        } else {
            qDebug() << mapIterator.key() << "**** NOT FOUND in JSON ****";
        }
        mapIterator++;
    }

    //iterate through all presets and check against default single preset map
    for(int presetNum = 0; presetNum<16; presetNum++){

        //qDebug() << "***************************************************";
        qDebug() << "Load preset" << presetNum;
        //qDebug() << "***************************************************";

        /***************************************************************************/
        /***************************** Preset Globals ******************************/
        /***************************************************************************/
        //check revision num, presetName (preset level)
        mapIterator = defaultSinglePresetMap.constBegin();

        while(mapIterator != defaultSinglePresetMap.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(jsonQuNeoPresetsMap.
                    value(QString("Preset %1").arg(presetNum)).toMap().contains(mapIterator.key())){
            } else {

                insertionMap[0] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[0]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in JSON. Inserted. ****";

            }
            mapIterator++;
        }


        /***************************************************************************/
        /******************************* HSliders **********************************/
        /***************************************************************************/
        //HSliders Globals///////////////////////////////////////////////////////////
        mapIterator = defaultHSlidersMap.constBegin();

        while(mapIterator != defaultHSlidersMap.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(jsonQuNeoPresetsMap.
                    value(QString("Preset %1").arg(presetNum)).toMap().
                    value("ComponentSettings").toMap().
                    value("HSliders").toMap().contains(mapIterator.key())){

            } else {

                insertionMap[0] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("HSliders").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap();
                insertionMap[1].insert("HSliders", insertionMap[0]);

                insertionMap[2] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap();
                insertionMap[2].insert("ComponentSettings", insertionMap[1]);

                jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[2]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in JSON. Inserted. ****";
            }
            mapIterator++;
        }

        //HSlider0///////////////////////////////////////////////////////////
        mapIterator = defaultHSlider0Map.constBegin();

        while(mapIterator != defaultHSlider0Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(jsonQuNeoPresetsMap.
                    value(QString("Preset %1").arg(presetNum)).toMap().
                    value("ComponentSettings").toMap().
                    value("HSliders").toMap().
                    value("HSlider0").toMap().contains(mapIterator.key())){

            } else {

                insertionMap[0] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("HSliders").toMap().value("HSlider0").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("HSliders").toMap();
                insertionMap[1].insert("HSlider0", insertionMap[0]);

                insertionMap[2] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap();
                insertionMap[2].insert("HSliders", insertionMap[1]);

                insertionMap[3] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap();
                insertionMap[3].insert("ComponentSettings", insertionMap[2]);

                jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in JSON. Inserted. ****";
            }
            mapIterator++;
        }

        //HSlider1///////////////////////////////////////////////////////////
        mapIterator = defaultHSlider1Map.constBegin();

        while(mapIterator != defaultHSlider1Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(jsonQuNeoPresetsMap.
                    value(QString("Preset %1").arg(presetNum)).toMap().
                    value("ComponentSettings").toMap().
                    value("HSliders").toMap().
                    value("HSlider1").toMap().contains(mapIterator.key())){
                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {
                insertionMap[0] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("HSliders").toMap().value("HSlider1").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("HSliders").toMap();
                insertionMap[1].insert("HSlider1", insertionMap[0]);

                insertionMap[2] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap();
                insertionMap[2].insert("HSliders", insertionMap[1]);

                insertionMap[3] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap();
                insertionMap[3].insert("ComponentSettings", insertionMap[2]);

                jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in JSON. Inserted. ****";
            }
            mapIterator++;
        }

        //HSlider2///////////////////////////////////////////////////////////
        mapIterator = defaultHSlider2Map.constBegin();

        while(mapIterator != defaultHSlider2Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(jsonQuNeoPresetsMap.
                    value(QString("Preset %1").arg(presetNum)).toMap().
                    value("ComponentSettings").toMap().
                    value("HSliders").toMap().
                    value("HSlider2").toMap().contains(mapIterator.key())){
                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {
                insertionMap[0] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("HSliders").toMap().value("HSlider2").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("HSliders").toMap();
                insertionMap[1].insert("HSlider2", insertionMap[0]);

                insertionMap[2] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap();
                insertionMap[2].insert("HSliders", insertionMap[1]);

                insertionMap[3] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap();
                insertionMap[3].insert("ComponentSettings", insertionMap[2]);

                jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in JSON. Inserted. ****";
            }
            mapIterator++;
        }

        //HSlider3///////////////////////////////////////////////////////////
        mapIterator = defaultHSlider3Map.constBegin();

        while(mapIterator != defaultHSlider3Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(jsonQuNeoPresetsMap.
                    value(QString("Preset %1").arg(presetNum)).toMap().
                    value("ComponentSettings").toMap().
                    value("HSliders").toMap().
                    value("HSlider3").toMap().contains(mapIterator.key())){
                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {
                insertionMap[0] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("HSliders").toMap().value("HSlider3").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("HSliders").toMap();
                insertionMap[1].insert("HSlider3", insertionMap[0]);

                insertionMap[2] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap();
                insertionMap[2].insert("HSliders", insertionMap[1]);

                insertionMap[3] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap();
                insertionMap[3].insert("ComponentSettings", insertionMap[2]);

                jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in JSON. Inserted. ****";
            }
            mapIterator++;
        }


        /***************************************************************************/
        /**************************** LeftRightButtons *****************************/
        /***************************************************************************/
        //LeftRightButtons Globals///////////////////////////////////////////////////
        mapIterator = defaultLeftRightButtonsMap.constBegin();

        while(mapIterator != defaultLeftRightButtonsMap.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(jsonQuNeoPresetsMap.
                    value(QString("Preset %1").arg(presetNum)).toMap().
                    value("ComponentSettings").toMap().
                    value("LeftRightButtons").toMap().contains(mapIterator.key())){
                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {
                insertionMap[0] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("LeftRightButtons").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap();
                insertionMap[1].insert("LeftRightButtons", insertionMap[0]);

                insertionMap[2] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap();
                insertionMap[2].insert("ComponentSettings", insertionMap[1]);

                jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[2]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in JSON. Inserted. ****";
            }
            mapIterator++;
        }

        //LeftRightButton0///////////////////////////////////////////////////
        mapIterator = defaultLeftRightButton0Map.constBegin();

        while(mapIterator != defaultLeftRightButton0Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(jsonQuNeoPresetsMap.
                    value(QString("Preset %1").arg(presetNum)).toMap().
                    value("ComponentSettings").toMap().
                    value("LeftRightButtons").toMap().
                    value("LeftRightButton0").toMap().contains(mapIterator.key())){

                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {
                insertionMap[0] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("LeftRightButtons").toMap().value("LeftRightButton0").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("LeftRightButtons").toMap();
                insertionMap[1].insert("LeftRightButton0", insertionMap[0]);

                insertionMap[2] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap();
                insertionMap[2].insert("LeftRightButtons", insertionMap[1]);

                insertionMap[3] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap();
                insertionMap[3].insert("ComponentSettings", insertionMap[2]);

                jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in JSON. Inserted. ****";
            }
            mapIterator++;
        }

        //LeftRightButton1///////////////////////////////////////////////////
        mapIterator = defaultLeftRightButton1Map.constBegin();

        while(mapIterator != defaultLeftRightButton1Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(jsonQuNeoPresetsMap.
                    value(QString("Preset %1").arg(presetNum)).toMap().
                    value("ComponentSettings").toMap().
                    value("LeftRightButtons").toMap().
                    value("LeftRightButton1").toMap().contains(mapIterator.key())){

                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {
                insertionMap[0] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("LeftRightButtons").toMap().value("LeftRightButton1").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("LeftRightButtons").toMap();
                insertionMap[1].insert("LeftRightButton1", insertionMap[0]);

                insertionMap[2] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap();
                insertionMap[2].insert("LeftRightButtons", insertionMap[1]);

                insertionMap[3] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap();
                insertionMap[3].insert("ComponentSettings", insertionMap[2]);

                jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in JSON. Inserted. ****";
            }
            mapIterator++;
        }

        //LeftRightButton2///////////////////////////////////////////////////
        mapIterator = defaultLeftRightButton2Map.constBegin();

        while(mapIterator != defaultLeftRightButton2Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(jsonQuNeoPresetsMap.
                    value(QString("Preset %1").arg(presetNum)).toMap().
                    value("ComponentSettings").toMap().
                    value("LeftRightButtons").toMap().
                    value("LeftRightButton2").toMap().contains(mapIterator.key())){

                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {
                insertionMap[0] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("LeftRightButtons").toMap().value("LeftRightButton2").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("LeftRightButtons").toMap();
                insertionMap[1].insert("LeftRightButton2", insertionMap[0]);

                insertionMap[2] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap();
                insertionMap[2].insert("LeftRightButtons", insertionMap[1]);

                insertionMap[3] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap();
                insertionMap[3].insert("ComponentSettings", insertionMap[2]);

                jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in JSON. Inserted. ****";
            }
            mapIterator++;
        }

        //LeftRightButton3///////////////////////////////////////////////////
        mapIterator = defaultLeftRightButton3Map.constBegin();

        while(mapIterator != defaultLeftRightButton3Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(jsonQuNeoPresetsMap.
                    value(QString("Preset %1").arg(presetNum)).toMap().
                    value("ComponentSettings").toMap().
                    value("LeftRightButtons").toMap().
                    value("LeftRightButton3").toMap().contains(mapIterator.key())){

                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {
                insertionMap[0] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("LeftRightButtons").toMap().value("LeftRightButton3").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("LeftRightButtons").toMap();
                insertionMap[1].insert("LeftRightButton3", insertionMap[0]);

                insertionMap[2] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap();
                insertionMap[2].insert("LeftRightButtons", insertionMap[1]);

                insertionMap[3] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap();
                insertionMap[3].insert("ComponentSettings", insertionMap[2]);

                jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);
            }
            mapIterator++;
        }


        /***************************************************************************/
        /******************************* LongSliders *******************************/
        /***************************************************************************/
        //LongSliders Globals////////////////////////////////////////////////////////
        mapIterator = defaultLongSlidersMap.constBegin();

        while(mapIterator != defaultLongSlidersMap.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(jsonQuNeoPresetsMap.
                    value(QString("Preset %1").arg(presetNum)).toMap().
                    value("ComponentSettings").toMap().
                    value("LongSliders").toMap().contains(mapIterator.key())){

            } else {
                insertionMap[0] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("LongSliders").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap();
                insertionMap[1].insert("LongSliders", insertionMap[0]);

                insertionMap[2] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap();
                insertionMap[2].insert("ComponentSettings", insertionMap[1]);

                jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[2]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in JSON. Inserted. ****";
            }
            mapIterator++;
        }

        //LongSlider0////////////////////////////////////////////////////////
        mapIterator = defaultLongSliders0Map.constBegin();

        while(mapIterator != defaultLongSliders0Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(jsonQuNeoPresetsMap.
                    value(QString("Preset %1").arg(presetNum)).toMap().
                    value("ComponentSettings").toMap().
                    value("LongSliders").toMap().
                    value("LongSlider0").toMap().contains(mapIterator.key())){

                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {
                insertionMap[0] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("LongSliders").toMap().value("LongSlider0").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("LongSliders").toMap();
                insertionMap[1].insert("LongSlider0", insertionMap[0]);

                insertionMap[2] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap();
                insertionMap[2].insert("LongSliders", insertionMap[1]);

                insertionMap[3] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap();
                insertionMap[3].insert("ComponentSettings", insertionMap[2]);

                jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);
            }
            mapIterator++;
        }


        /***************************************************************************/
        /******************************* ModeButtons *******************************/
        /***************************************************************************/
        //ModeButtons Globals////////////////////////////////////////////////////////
        mapIterator = defaultModeButtonsMap.constBegin();

        while(mapIterator != defaultModeButtonsMap.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(jsonQuNeoPresetsMap.
                    value(QString("Preset %1").arg(presetNum)).toMap().
                    value("ComponentSettings").toMap().
                    value("ModeButtons").toMap().contains(mapIterator.key())){
                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {
                insertionMap[0] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("ModeButtons").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap();
                insertionMap[1].insert("ModeButtons", insertionMap[0]);

                insertionMap[2] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap();
                insertionMap[2].insert("ComponentSettings", insertionMap[1]);

                jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[2]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in JSON. Inserted. ****";
            }
            mapIterator++;
        }

        //ModeButton0////////////////////////////////////////////////////////
        mapIterator = defaultModeButton0Map.constBegin();

        while(mapIterator != defaultModeButton0Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(jsonQuNeoPresetsMap.
                    value(QString("Preset %1").arg(presetNum)).toMap().
                    value("ComponentSettings").toMap().
                    value("ModeButtons").toMap().
                    value("ModeButton0").toMap().contains(mapIterator.key())){

                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {
                insertionMap[0] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("ModeButtons").toMap().value("ModeButton0").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("ModeButtons").toMap();
                insertionMap[1].insert("ModeButton0", insertionMap[0]);

                insertionMap[2] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap();
                insertionMap[2].insert("ModeButtons", insertionMap[1]);

                insertionMap[3] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap();
                insertionMap[3].insert("ComponentSettings", insertionMap[2]);

                jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);
            }
            mapIterator++;
        }


        /***************************************************************************/
        /*********************************** Pads **********************************/
        /***************************************************************************/
        //Pads Globals///////////////////////////////////////////////////////////////
        mapIterator = defaultPadsMap.constBegin();

        while(mapIterator != defaultPadsMap.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(jsonQuNeoPresetsMap.
                    value(QString("Preset %1").arg(presetNum)).toMap().
                    value("ComponentSettings").toMap().
                    value("Pads").toMap().contains(mapIterator.key())){
            } else {
                insertionMap[0] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("Pads").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap();
                insertionMap[1].insert("Pads", insertionMap[0]);

                insertionMap[2] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap();
                insertionMap[2].insert("ComponentSettings", insertionMap[1]);

                jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[2]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in JSON. Inserted. ****";
            }
            mapIterator++;
        }

        //Pad0///////////////////////////////////////////////////////////////
        mapIterator = defaultPad0Map.constBegin();

        while(mapIterator != defaultPad0Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(jsonQuNeoPresetsMap.
                    value(QString("Preset %1").arg(presetNum)).toMap().
                    value("ComponentSettings").toMap().
                    value("Pads").toMap().
                    value("Pad0").toMap().contains(mapIterator.key())){

                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {
                qDebug() << mapIterator.key() << "**** NOT FOUND in JSON ****";
            }
            mapIterator++;
        }

        //Pad1///////////////////////////////////////////////////////////////
        mapIterator = defaultPad1Map.constBegin();

        while(mapIterator != defaultPad1Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(jsonQuNeoPresetsMap.
                    value(QString("Preset %1").arg(presetNum)).toMap().
                    value("ComponentSettings").toMap().
                    value("Pads").toMap().
                    value("Pad1").toMap().contains(mapIterator.key())){

            } else {
                insertionMap[0] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("Pads").toMap().value("Pad1").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("Pads").toMap();
                insertionMap[1].insert("Pad1", insertionMap[0]);

                insertionMap[2] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap();
                insertionMap[2].insert("Pads", insertionMap[1]);

                insertionMap[3] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap();
                insertionMap[3].insert("ComponentSettings", insertionMap[2]);

                jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);
            }
            mapIterator++;
        }

        //Pad2///////////////////////////////////////////////////////////////
        mapIterator = defaultPad2Map.constBegin();

        while(mapIterator != defaultPad2Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(jsonQuNeoPresetsMap.
                    value(QString("Preset %1").arg(presetNum)).toMap().
                    value("ComponentSettings").toMap().
                    value("Pads").toMap().
                    value("Pad2").toMap().contains(mapIterator.key())){

            } else {
                insertionMap[0] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("Pads").toMap().value("Pad2").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("Pads").toMap();
                insertionMap[1].insert("Pad2", insertionMap[0]);

                insertionMap[2] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap();
                insertionMap[2].insert("Pads", insertionMap[1]);

                insertionMap[3] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap();
                insertionMap[3].insert("ComponentSettings", insertionMap[2]);

                jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);
            }
            mapIterator++;
        }

        //Pad3///////////////////////////////////////////////////////////////
        mapIterator = defaultPad3Map.constBegin();

        while(mapIterator != defaultPad3Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(jsonQuNeoPresetsMap.
                    value(QString("Preset %1").arg(presetNum)).toMap().
                    value("ComponentSettings").toMap().
                    value("Pads").toMap().
                    value("Pad3").toMap().contains(mapIterator.key())){

                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {
                insertionMap[0] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("Pads").toMap().value("Pad3").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("Pads").toMap();
                insertionMap[1].insert("Pad3", insertionMap[0]);

                insertionMap[2] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap();
                insertionMap[2].insert("Pads", insertionMap[1]);

                insertionMap[3] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap();
                insertionMap[3].insert("ComponentSettings", insertionMap[2]);

                jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);
            }
            mapIterator++;
        }

        //Pad4///////////////////////////////////////////////////////////////
        mapIterator = defaultPad4Map.constBegin();

        while(mapIterator != defaultPad4Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(jsonQuNeoPresetsMap.
                    value(QString("Preset %1").arg(presetNum)).toMap().
                    value("ComponentSettings").toMap().
                    value("Pads").toMap().
                    value("Pad4").toMap().contains(mapIterator.key())){

                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {
                insertionMap[0] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("Pads").toMap().value("Pad4").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("Pads").toMap();
                insertionMap[1].insert("Pad4", insertionMap[0]);

                insertionMap[2] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap();
                insertionMap[2].insert("Pads", insertionMap[1]);

                insertionMap[3] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap();
                insertionMap[3].insert("ComponentSettings", insertionMap[2]);

                jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);
            }
            mapIterator++;
        }

        //Pad5///////////////////////////////////////////////////////////////
        mapIterator = defaultPad5Map.constBegin();

        while(mapIterator != defaultPad5Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(jsonQuNeoPresetsMap.
                    value(QString("Preset %1").arg(presetNum)).toMap().
                    value("ComponentSettings").toMap().
                    value("Pads").toMap().
                    value("Pad5").toMap().contains(mapIterator.key())){

            } else {
                insertionMap[0] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("Pads").toMap().value("Pad5").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("Pads").toMap();
                insertionMap[1].insert("Pad5", insertionMap[0]);

                insertionMap[2] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap();
                insertionMap[2].insert("Pads", insertionMap[1]);

                insertionMap[3] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap();
                insertionMap[3].insert("ComponentSettings", insertionMap[2]);

                jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);
            }
            mapIterator++;
        }

        //Pad6///////////////////////////////////////////////////////////////
        mapIterator = defaultPad6Map.constBegin();

        while(mapIterator != defaultPad6Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(jsonQuNeoPresetsMap.
                    value(QString("Preset %1").arg(presetNum)).toMap().
                    value("ComponentSettings").toMap().
                    value("Pads").toMap().
                    value("Pad6").toMap().contains(mapIterator.key())){

            } else {
                insertionMap[0] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("Pads").toMap().value("Pad6").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("Pads").toMap();
                insertionMap[1].insert("Pad6", insertionMap[0]);

                insertionMap[2] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap();
                insertionMap[2].insert("Pads", insertionMap[1]);

                insertionMap[3] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap();
                insertionMap[3].insert("ComponentSettings", insertionMap[2]);

                jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);
            }
            mapIterator++;
        }

        //Pad7///////////////////////////////////////////////////////////////
        mapIterator = defaultPad7Map.constBegin();

        while(mapIterator != defaultPad7Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(jsonQuNeoPresetsMap.
                    value(QString("Preset %1").arg(presetNum)).toMap().
                    value("ComponentSettings").toMap().
                    value("Pads").toMap().
                    value("Pad7").toMap().contains(mapIterator.key())){

            } else {
                insertionMap[0] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("Pads").toMap().value("Pad7").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("Pads").toMap();
                insertionMap[1].insert("Pad7", insertionMap[0]);

                insertionMap[2] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap();
                insertionMap[2].insert("Pads", insertionMap[1]);

                insertionMap[3] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap();
                insertionMap[3].insert("ComponentSettings", insertionMap[2]);

                jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);
            }
            mapIterator++;
        }

        //Pad8///////////////////////////////////////////////////////////////
        mapIterator = defaultPad8Map.constBegin();

        while(mapIterator != defaultPad8Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(jsonQuNeoPresetsMap.
                    value(QString("Preset %1").arg(presetNum)).toMap().
                    value("ComponentSettings").toMap().
                    value("Pads").toMap().
                    value("Pad8").toMap().contains(mapIterator.key())){

            } else {
                insertionMap[0] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("Pads").toMap().value("Pad8").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("Pads").toMap();
                insertionMap[1].insert("Pad8", insertionMap[0]);

                insertionMap[2] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap();
                insertionMap[2].insert("Pads", insertionMap[1]);

                insertionMap[3] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap();
                insertionMap[3].insert("ComponentSettings", insertionMap[2]);

                jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);
            }
            mapIterator++;
        }

        //Pad9///////////////////////////////////////////////////////////////
        mapIterator = defaultPad9Map.constBegin();

        while(mapIterator != defaultPad9Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(jsonQuNeoPresetsMap.
                    value(QString("Preset %1").arg(presetNum)).toMap().
                    value("ComponentSettings").toMap().
                    value("Pads").toMap().
                    value("Pad9").toMap().contains(mapIterator.key())){

            } else {
                insertionMap[0] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("Pads").toMap().value("Pad9").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("Pads").toMap();
                insertionMap[1].insert("Pad9", insertionMap[0]);

                insertionMap[2] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap();
                insertionMap[2].insert("Pads", insertionMap[1]);

                insertionMap[3] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap();
                insertionMap[3].insert("ComponentSettings", insertionMap[2]);

                jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);
            }
            mapIterator++;
        }

        //Pad10///////////////////////////////////////////////////////////////
        mapIterator = defaultPad10Map.constBegin();

        while(mapIterator != defaultPad10Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(jsonQuNeoPresetsMap.
                    value(QString("Preset %1").arg(presetNum)).toMap().
                    value("ComponentSettings").toMap().
                    value("Pads").toMap().
                    value("Pad10").toMap().contains(mapIterator.key())){

                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {
                insertionMap[0] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("Pads").toMap().value("Pad10").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("Pads").toMap();
                insertionMap[1].insert("Pad10", insertionMap[0]);

                insertionMap[2] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap();
                insertionMap[2].insert("Pads", insertionMap[1]);

                insertionMap[3] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap();
                insertionMap[3].insert("ComponentSettings", insertionMap[2]);

                jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);
            }
            mapIterator++;
        }

        //Pad11///////////////////////////////////////////////////////////////
        mapIterator = defaultPad11Map.constBegin();

        while(mapIterator != defaultPad11Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(jsonQuNeoPresetsMap.
                    value(QString("Preset %1").arg(presetNum)).toMap().
                    value("ComponentSettings").toMap().
                    value("Pads").toMap().
                    value("Pad11").toMap().contains(mapIterator.key())){

            } else {
                insertionMap[0] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("Pads").toMap().value("Pad11").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("Pads").toMap();
                insertionMap[1].insert("Pad11", insertionMap[0]);

                insertionMap[2] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap();
                insertionMap[2].insert("Pads", insertionMap[1]);

                insertionMap[3] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap();
                insertionMap[3].insert("ComponentSettings", insertionMap[2]);

                jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);
            }
            mapIterator++;
        }

        //Pad12///////////////////////////////////////////////////////////////
        mapIterator = defaultPad12Map.constBegin();

        while(mapIterator != defaultPad12Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(jsonQuNeoPresetsMap.
                    value(QString("Preset %1").arg(presetNum)).toMap().
                    value("ComponentSettings").toMap().
                    value("Pads").toMap().
                    value("Pad12").toMap().contains(mapIterator.key())){

            } else {
                insertionMap[0] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("Pads").toMap().value("Pad12").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("Pads").toMap();
                insertionMap[1].insert("Pad12", insertionMap[0]);

                insertionMap[2] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap();
                insertionMap[2].insert("Pads", insertionMap[1]);

                insertionMap[3] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap();
                insertionMap[3].insert("ComponentSettings", insertionMap[2]);

                jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);
            }
            mapIterator++;
        }

        //Pad13///////////////////////////////////////////////////////////////
        mapIterator = defaultPad13Map.constBegin();

        while(mapIterator != defaultPad13Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(jsonQuNeoPresetsMap.
                    value(QString("Preset %1").arg(presetNum)).toMap().
                    value("ComponentSettings").toMap().
                    value("Pads").toMap().
                    value("Pad13").toMap().contains(mapIterator.key())){

                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {
                insertionMap[0] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("Pads").toMap().value("Pad13").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("Pads").toMap();
                insertionMap[1].insert("Pad13", insertionMap[0]);

                insertionMap[2] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap();
                insertionMap[2].insert("Pads", insertionMap[1]);

                insertionMap[3] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap();
                insertionMap[3].insert("ComponentSettings", insertionMap[2]);

                jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);
            }
            mapIterator++;
        }

        //Pad14///////////////////////////////////////////////////////////////
        mapIterator = defaultPad14Map.constBegin();

        while(mapIterator != defaultPad14Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(jsonQuNeoPresetsMap.
                    value(QString("Preset %1").arg(presetNum)).toMap().
                    value("ComponentSettings").toMap().
                    value("Pads").toMap().
                    value("Pad14").toMap().contains(mapIterator.key())){

            } else {
                insertionMap[0] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("Pads").toMap().value("Pad14").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("Pads").toMap();
                insertionMap[1].insert("Pad14", insertionMap[0]);

                insertionMap[2] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap();
                insertionMap[2].insert("Pads", insertionMap[1]);

                insertionMap[3] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap();
                insertionMap[3].insert("ComponentSettings", insertionMap[2]);

                jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);
            }
            mapIterator++;
        }

        //Pad15///////////////////////////////////////////////////////////////
        mapIterator = defaultPad15Map.constBegin();

        while(mapIterator != defaultPad15Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(jsonQuNeoPresetsMap.
                    value(QString("Preset %1").arg(presetNum)).toMap().
                    value("ComponentSettings").toMap().
                    value("Pads").toMap().
                    value("Pad15").toMap().contains(mapIterator.key())){

            } else {
                insertionMap[0] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("Pads").toMap().value("Pad15").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("Pads").toMap();
                insertionMap[1].insert("Pad15", insertionMap[0]);

                insertionMap[2] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap();
                insertionMap[2].insert("Pads", insertionMap[1]);

                insertionMap[3] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap();
                insertionMap[3].insert("ComponentSettings", insertionMap[2]);

                jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);
            }
            mapIterator++;
        }


        /***************************************************************************/
        /********************************* Rhombus *********************************/
        /***************************************************************************/
        //RhombusButtons Globals/////////////////////////////////////////////////////
        mapIterator = defaultRhombusButtonsMap.constBegin();

        while(mapIterator != defaultRhombusButtonsMap.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(jsonQuNeoPresetsMap.
                    value(QString("Preset %1").arg(presetNum)).toMap().
                    value("ComponentSettings").toMap().
                    value("RhombusButtons").toMap().contains(mapIterator.key())){
                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {
                insertionMap[0] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("RhombusButtons").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap();
                insertionMap[1].insert("RhombusButtons", insertionMap[0]);

                insertionMap[2] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap();
                insertionMap[2].insert("ComponentSettings", insertionMap[1]);

                jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[2]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in JSON. Inserted. ****";
            }
            mapIterator++;
        }

        //RhombusButton0/////////////////////////////////////////////////////
        mapIterator = defaultRhombusButton0Map.constBegin();

        while(mapIterator != defaultRhombusButton0Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(jsonQuNeoPresetsMap.
                    value(QString("Preset %1").arg(presetNum)).toMap().
                    value("ComponentSettings").toMap().
                    value("RhombusButtons").toMap().
                    value("RhombusButton0").toMap().contains(mapIterator.key())){

                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {

                insertionMap[0] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("RhombusButtons").toMap().value("RhombusButton0").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("RhombusButtons").toMap();
                insertionMap[1].insert("RhombusButton0", insertionMap[0]);

                insertionMap[2] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap();
                insertionMap[2].insert("RhombusButtons", insertionMap[1]);

                insertionMap[3] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap();
                insertionMap[3].insert("ComponentSettings", insertionMap[2]);

                jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);
            }
            mapIterator++;
        }


        /***************************************************************************/
        /********************************* Rhombus *********************************/
        /***************************************************************************/
        //Rotaries Globals///////////////////////////////////////////////////////////
        mapIterator = defaultRotariesMap.constBegin();

        while(mapIterator != defaultRotariesMap.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(jsonQuNeoPresetsMap.
                    value(QString("Preset %1").arg(presetNum)).toMap().
                    value("ComponentSettings").toMap().
                    value("Rotaries").toMap().contains(mapIterator.key())){
            } else {
                insertionMap[0] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("Rotaries").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap();
                insertionMap[1].insert("Rotaries", insertionMap[0]);

                insertionMap[2] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap();
                insertionMap[2].insert("ComponentSettings", insertionMap[1]);

                jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[2]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in JSON. Inserted. ****";
            }
            mapIterator++;
        }

        //Rotary0///////////////////////////////////////////////////////////
        mapIterator = defaultRotary0Map.constBegin();

        while(mapIterator != defaultRotary0Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(jsonQuNeoPresetsMap.
                    value(QString("Preset %1").arg(presetNum)).toMap().
                    value("ComponentSettings").toMap().
                    value("Rotaries").toMap().
                    value("Rotary0").toMap().contains(mapIterator.key())){

                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {
                insertionMap[0] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("Rotaries").toMap().value("Rotary0").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("Rotaries").toMap();
                insertionMap[1].insert("Rotary0", insertionMap[0]);

                insertionMap[2] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap();
                insertionMap[2].insert("Rotaries", insertionMap[1]);

                insertionMap[3] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap();
                insertionMap[3].insert("ComponentSettings", insertionMap[2]);

                jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);
            }
            mapIterator++;
        }

        //Rotary1///////////////////////////////////////////////////////////
        mapIterator = defaultRotary1Map.constBegin();

        while(mapIterator != defaultRotary1Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(jsonQuNeoPresetsMap.
                    value(QString("Preset %1").arg(presetNum)).toMap().
                    value("ComponentSettings").toMap().
                    value("Rotaries").toMap().
                    value("Rotary1").toMap().contains(mapIterator.key())){

                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {
                insertionMap[0] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("Rotaries").toMap().value("Rotary1").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("Rotaries").toMap();
                insertionMap[1].insert("Rotary1", insertionMap[0]);

                insertionMap[2] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap();
                insertionMap[2].insert("Rotaries", insertionMap[1]);

                insertionMap[3] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap();
                insertionMap[3].insert("ComponentSettings", insertionMap[2]);

                jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);
            }
            mapIterator++;
        }


        /***************************************************************************/
        /**************************** Transport Buttons ****************************/
        /***************************************************************************/
        //Transport Globals//////////////////////////////////////////////////////////
        mapIterator = defaultTransportButtonsMap.constBegin();

        while(mapIterator != defaultTransportButtonsMap.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(jsonQuNeoPresetsMap.
                    value(QString("Preset %1").arg(presetNum)).toMap().
                    value("ComponentSettings").toMap().
                    value("TransportButtons").toMap().contains(mapIterator.key())){
            } else {
                insertionMap[0] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("TransportButtons").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap();
                insertionMap[1].insert("TransportButtons", insertionMap[0]);

                insertionMap[2] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap();
                insertionMap[2].insert("ComponentSettings", insertionMap[1]);

                jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[2]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in JSON. Inserted. ****";
            }
            mapIterator++;
        }

        //TransportButton0///////////////////////////////////////////////////////////
        mapIterator = defaultTransportButton0Map.constBegin();

        while(mapIterator != defaultTransportButton0Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(jsonQuNeoPresetsMap.
                    value(QString("Preset %1").arg(presetNum)).toMap().
                    value("ComponentSettings").toMap().
                    value("TransportButtons").toMap().
                    value("TransportButton0").toMap().contains(mapIterator.key())){

                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {
                insertionMap[0] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("TransportButtons").toMap().value("TransportButton0").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("TransportButtons").toMap();
                insertionMap[1].insert("TransportButton0", insertionMap[0]);

                insertionMap[2] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap();
                insertionMap[2].insert("TransportButtons", insertionMap[1]);

                insertionMap[3] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap();
                insertionMap[3].insert("ComponentSettings", insertionMap[2]);

                jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);
            }
            mapIterator++;
        }

        //TransportButton1///////////////////////////////////////////////////////////
        mapIterator = defaultTransportButton1Map.constBegin();

        while(mapIterator != defaultTransportButton1Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(jsonQuNeoPresetsMap.
                    value(QString("Preset %1").arg(presetNum)).toMap().
                    value("ComponentSettings").toMap().
                    value("TransportButtons").toMap().
                    value("TransportButton1").toMap().contains(mapIterator.key())){

                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {
                insertionMap[0] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("TransportButtons").toMap().value("TransportButton1").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("TransportButtons").toMap();
                insertionMap[1].insert("TransportButton1", insertionMap[0]);

                insertionMap[2] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap();
                insertionMap[2].insert("TransportButtons", insertionMap[1]);

                insertionMap[3] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap();
                insertionMap[3].insert("ComponentSettings", insertionMap[2]);

                jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);
            }
            mapIterator++;
        }

        //TransportButton2///////////////////////////////////////////////////////////
        mapIterator = defaultTransportButton2Map.constBegin();

        while(mapIterator != defaultTransportButton2Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(jsonQuNeoPresetsMap.
                    value(QString("Preset %1").arg(presetNum)).toMap().
                    value("ComponentSettings").toMap().
                    value("TransportButtons").toMap().
                    value("TransportButton2").toMap().contains(mapIterator.key())){

                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {
                insertionMap[0] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("TransportButtons").toMap().value("TransportButton2").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("TransportButtons").toMap();
                insertionMap[1].insert("TransportButton2", insertionMap[0]);

                insertionMap[2] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap();
                insertionMap[2].insert("TransportButtons", insertionMap[1]);

                insertionMap[3] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap();
                insertionMap[3].insert("ComponentSettings", insertionMap[2]);

                jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);
            }
            mapIterator++;
        }


        /***************************************************************************/
        /**************************** UpDown Buttons *******************************/
        /***************************************************************************/
        //UpDownButtons Globals//////////////////////////////////////////////////////
        mapIterator = defaultUpDownButtonsMap.constBegin();

        while(mapIterator != defaultUpDownButtonsMap.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(jsonQuNeoPresetsMap.
                    value(QString("Preset %1").arg(presetNum)).toMap().
                    value("ComponentSettings").toMap().
                    value("UpDownButtons").toMap().contains(mapIterator.key())){
                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {
                insertionMap[0] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("UpDownButtons").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap();
                insertionMap[1].insert("UpDownButtons", insertionMap[0]);

                insertionMap[2] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap();
                insertionMap[2].insert("ComponentSettings", insertionMap[1]);

                jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[2]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in JSON. Inserted. ****";
            }
            mapIterator++;
        }

        //UpDownButton0//////////////////////////////////////////////////////
        mapIterator = defaultUpDownButton0Map.constBegin();

        while(mapIterator != defaultUpDownButton0Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(jsonQuNeoPresetsMap.
                    value(QString("Preset %1").arg(presetNum)).toMap().
                    value("ComponentSettings").toMap().
                    value("UpDownButtons").toMap().
                    value("UpDownButton0").toMap().contains(mapIterator.key())){

                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {
                insertionMap[0] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("UpDownButtons").toMap().value("UpDownButton0").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("UpDownButtons").toMap();
                insertionMap[1].insert("UpDownButton0", insertionMap[0]);

                insertionMap[2] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap();
                insertionMap[2].insert("UpDownButtons", insertionMap[1]);

                insertionMap[3] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap();
                insertionMap[3].insert("ComponentSettings", insertionMap[2]);

                jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);
            }

            mapIterator++;
        }

        //UpDownButton1//////////////////////////////////////////////////////
        mapIterator = defaultUpDownButton1Map.constBegin();

        while(mapIterator != defaultUpDownButton1Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(jsonQuNeoPresetsMap.
                    value(QString("Preset %1").arg(presetNum)).toMap().
                    value("ComponentSettings").toMap().
                    value("UpDownButtons").toMap().
                    value("UpDownButton1").toMap().contains(mapIterator.key())){

                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {
                insertionMap[0] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("UpDownButtons").toMap().value("UpDownButton1").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("UpDownButtons").toMap();
                insertionMap[1].insert("UpDownButton1", insertionMap[0]);

                insertionMap[2] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap();
                insertionMap[2].insert("UpDownButtons", insertionMap[1]);

                insertionMap[3] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap();
                insertionMap[3].insert("ComponentSettings", insertionMap[2]);

                jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);
            }

            mapIterator++;
        }


        /***************************************************************************/
        /******************************** VSliders *********************************/
        /***************************************************************************/
        //VSliders Globals//////////////////////////////////////////////////////
        mapIterator = defaultVSlidersMap.constBegin();

        while(mapIterator != defaultVSlidersMap.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(jsonQuNeoPresetsMap.
                    value(QString("Preset %1").arg(presetNum)).toMap().
                    value("ComponentSettings").toMap().
                    value("VSliders").toMap().contains(mapIterator.key())){

                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {
                insertionMap[0] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("VSliders").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap();
                insertionMap[1].insert("VSliders", insertionMap[0]);

                insertionMap[2] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap();
                insertionMap[2].insert("ComponentSettings", insertionMap[1]);

                jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[2]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in JSON. Inserted. ****";
            }

            mapIterator++;
        }

        //VSlider0//////////////////////////////////////////////////////
        mapIterator = defaultVSlider0Map.constBegin();

        while(mapIterator != defaultVSlider0Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(jsonQuNeoPresetsMap.
                    value(QString("Preset %1").arg(presetNum)).toMap().
                    value("ComponentSettings").toMap().
                    value("VSliders").toMap().
                    value("VSlider0").toMap().contains(mapIterator.key())){

            } else {
                insertionMap[0] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("VSliders").toMap().value("VSlider0").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("VSliders").toMap();
                insertionMap[1].insert("VSlider0", insertionMap[0]);

                insertionMap[2] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap();
                insertionMap[2].insert("VSliders", insertionMap[1]);

                insertionMap[3] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap();
                insertionMap[3].insert("ComponentSettings", insertionMap[2]);

                jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);
            }

            mapIterator++;
        }

        //VSlider1//////////////////////////////////////////////////////
        mapIterator = defaultVSlider1Map.constBegin();

        while(mapIterator != defaultVSlider1Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(jsonQuNeoPresetsMap.
                    value(QString("Preset %1").arg(presetNum)).toMap().
                    value("ComponentSettings").toMap().
                    value("VSliders").toMap().
                    value("VSlider1").toMap().contains(mapIterator.key())){
                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {
                insertionMap[0] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("VSliders").toMap().value("VSlider1").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("VSliders").toMap();
                insertionMap[1].insert("VSlider1", insertionMap[0]);

                insertionMap[2] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap();
                insertionMap[2].insert("VSliders", insertionMap[1]);

                insertionMap[3] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap();
                insertionMap[3].insert("ComponentSettings", insertionMap[2]);

                jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);
            }

            mapIterator++;
        }

        //VSlider2//////////////////////////////////////////////////////
        mapIterator = defaultVSlider2Map.constBegin();

        while(mapIterator != defaultVSlider2Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(jsonQuNeoPresetsMap.
                    value(QString("Preset %1").arg(presetNum)).toMap().
                    value("ComponentSettings").toMap().
                    value("VSliders").toMap().
                    value("VSlider2").toMap().contains(mapIterator.key())){
                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {
                insertionMap[0] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("VSliders").toMap().value("VSlider2").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("VSliders").toMap();
                insertionMap[1].insert("VSlider2", insertionMap[0]);

                insertionMap[2] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap();
                insertionMap[2].insert("VSliders", insertionMap[1]);

                insertionMap[3] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap();
                insertionMap[3].insert("ComponentSettings", insertionMap[2]);

                jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);
            }

            mapIterator++;
        }

        //VSlider3//////////////////////////////////////////////////////
        mapIterator = defaultVSlider3Map.constBegin();

        while(mapIterator != defaultVSlider3Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(jsonQuNeoPresetsMap.
                    value(QString("Preset %1").arg(presetNum)).toMap().
                    value("ComponentSettings").toMap().
                    value("VSliders").toMap().
                    value("VSlider3").toMap().contains(mapIterator.key())){
                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {
                insertionMap[0] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("VSliders").toMap().value("VSlider3").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap().value("VSliders").toMap();
                insertionMap[1].insert("VSlider3", insertionMap[0]);

                insertionMap[2] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap().value("ComponentSettings").toMap();
                insertionMap[2].insert("VSliders", insertionMap[1]);

                insertionMap[3] = jsonQuNeoPresetsMap.value(QString("Preset %1").arg(presetNum)).toMap();
                insertionMap[3].insert("ComponentSettings", insertionMap[2]);

                jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);
            }

            mapIterator++;
        }

    }

}

void DataValidator::slotSaveJSON(){

    qDebug() << "slotSaveJSON called";
    jsonMasterMap.insert(QString("QuNeo Presets"), jsonQuNeoPresetsMap); //insert updated map into json map

//    jsonByteArray = serializer.serialize(jsonMasterMap); //serialize the master json map into the byte array
    QJsonDocument jsonPresets = QJsonDocument::fromVariant(jsonMasterMap);
    jsonByteArray = jsonPresets.toJson();

    //qDebug() << "byte array" << jsonByteArray;

    if(jsonFile->open(QIODevice::ReadWrite | QIODevice::Text)){
        qDebug("jsonFound");
    } else {
        qDebug() << "json not found";
    }

    jsonFile->resize(0); //clear jsonFile (set to 0 byte size)
    jsonFile->write(jsonByteArray); //write json byte array to file
    jsonFile->close(); //close file and finalize write

}

QVariantMap DataValidator::slotValidatePreset(QVariantMap presetMap){

    qDebug() << "slotValidatePreset called";
    for(int presetNum = 0; presetNum<1; presetNum++)
    {

        qDebug() << "***************************************************";
        qDebug() << "Imported Preset:" << presetNum;
        qDebug() << "***************************************************";

        /***************************************************************************/
        /***************************** Preset Globals ******************************/
        /***************************************************************************/
        qDebug() << "Preset Globals";
        //check revision num, presetName (preset level)
        mapIterator = defaultSinglePresetMap.constBegin();

        while(mapIterator != defaultSinglePresetMap.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(presetMap.contains(mapIterator.key())){

            } else {

                presetMap.insert(mapIterator.key(), mapIterator.value());

                qDebug() << mapIterator.key() << "**** NOT FOUND in PRESET. Inserted. ****";

            }
            mapIterator++;
        }


        //--------------------
        tempDeletionMap = presetMap;
        mapIterator = tempDeletionMap.constBegin();



        while(mapIterator != tempDeletionMap.constEnd()){

            for(int i = 0; i<5; i++){
                deletionMap[i].clear();
            }

            if(!defaultSinglePresetMap.contains(mapIterator.key())){

                presetMap.remove(mapIterator.key());

                qDebug() << mapIterator.key() << "**** NOT FOUND in Defulat Map. Deleted. ****";

            }
            mapIterator++;
        }




        /***************************************************************************/
        /******************************* HSliders **********************************/
        /***************************************************************************/
        qDebug() << "HSlider Globals";

        //HSliders Globals///////////////////////////////////////////////////////////
        mapIterator = defaultHSlidersMap.constBegin();

        while(mapIterator != defaultHSlidersMap.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(presetMap.
                    value("ComponentSettings").toMap().
                    value("HSliders").toMap().contains(mapIterator.key())){

            } else {

                insertionMap[0] = presetMap.value("ComponentSettings").toMap().value("HSliders").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = presetMap.value("ComponentSettings").toMap();
                insertionMap[1].insert("HSliders", insertionMap[0]);

                //insertionMap[2] = presetMap;
                presetMap.insert("ComponentSettings", insertionMap[1]);

                //presetMap.insert(QString("Preset %1").arg(presetNum),insertionMap[2]);

                qDebug() << mapIterator.key() << "h slider globals" << "**** NOT FOUND in PRESET. Inserted. ****";
            }
            mapIterator++;
        }


        //----------------------
        tempDeletionMap = presetMap.value("ComponentSettings").toMap().value("HSliders").toMap();
        mapIterator = tempDeletionMap.constBegin();



        while(mapIterator != tempDeletionMap.constEnd()){

            for(int i = 0; i<5; i++){
                deletionMap[i].clear();
            }

            if(!defaultHSlidersMap.contains(mapIterator.key())){

                deletionMap[0] = presetMap.value("ComponentSettings").toMap().value("HSliders").toMap();
                deletionMap[0].remove(mapIterator.key());

                deletionMap[1] = presetMap.value("ComponentSettings").toMap();
                deletionMap[1].insert("HSliders", deletionMap[0]);

                //insertionMap[2] = presetMap;
                presetMap.insert("ComponentSettings", deletionMap[1]);

                //presetMap.insert(QString("Preset %1").arg(presetNum),insertionMap[2]);

                qDebug() << mapIterator.key() << "h slider globals" << "**** NOT FOUND in Defulat Map. Deleted. ****";

            }
            mapIterator++;
        }

        //HSlider0///////////////////////////////////////////////////////////
        qDebug() << "HSlider 0 ";

        mapIterator = defaultHSlider0Map.constBegin();

        while(mapIterator != defaultHSlider0Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(presetMap.
                    value("ComponentSettings").toMap().
                    value("HSliders").toMap().
                    value("HSlider0").toMap().contains(mapIterator.key())){

            } else {

                insertionMap[0] = presetMap.value("ComponentSettings").toMap().value("HSliders").toMap().value("HSlider0").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = presetMap.value("ComponentSettings").toMap().value("HSliders").toMap();
                insertionMap[1].insert("HSlider0", insertionMap[0]);

                insertionMap[2] = presetMap.value("ComponentSettings").toMap();
                insertionMap[2].insert("HSliders", insertionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", insertionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in PRESET. Inserted. ****";
            }
            mapIterator++;
        }

        //----------------------
        tempDeletionMap = presetMap.value("ComponentSettings").toMap().value("HSliders").toMap().value("HSlider0").toMap();

        mapIterator = tempDeletionMap.constEnd();

        //qDebug() << "map iterator key" << mapIterator.key();

        while(mapIterator != tempDeletionMap.constEnd()){

            for(int i = 0; i<5; i++){
                deletionMap[i].clear();
            }

        qDebug() << "map iterator key second check" << mapIterator.key();

            if(!defaultHSlider0Map.contains(mapIterator.key())){

                deletionMap[0] = presetMap.value("ComponentSettings").toMap().value("HSliders").toMap().value("HSlider0").toMap();
                deletionMap[0].remove(mapIterator.key());

                deletionMap[1] = presetMap.value("ComponentSettings").toMap().value("HSliders").toMap();
                deletionMap[1].insert("HSlider0", deletionMap[0]);

                deletionMap[2] = presetMap.value("ComponentSettings").toMap();
                deletionMap[2].insert("HSliders", deletionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", deletionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in Defulat Map. Deleted. ****";
            }
            mapIterator++;
        }


        //HSlider1///////////////////////////////////////////////////////////
        qDebug() << "HSlider 1 ";

        mapIterator = defaultHSlider1Map.constBegin();

        while(mapIterator != defaultHSlider1Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(presetMap.
                    value("ComponentSettings").toMap().
                    value("HSliders").toMap().
                    value("HSlider1").toMap().contains(mapIterator.key())){
                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {
                insertionMap[0] = presetMap.value("ComponentSettings").toMap().value("HSliders").toMap().value("HSlider1").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = presetMap.value("ComponentSettings").toMap().value("HSliders").toMap();
                insertionMap[1].insert("HSlider1", insertionMap[0]);

                insertionMap[2] = presetMap.value("ComponentSettings").toMap();
                insertionMap[2].insert("HSliders", insertionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", insertionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in PRESET. Inserted. ****";
            }
            mapIterator++;
        }

        //----------------------
        tempDeletionMap = presetMap.value("ComponentSettings").toMap().value("HSliders").toMap().value("HSlider1").toMap();
        mapIterator = tempDeletionMap.constBegin();

        while(mapIterator != tempDeletionMap.constEnd()){

            for(int i = 0; i<5; i++){
                deletionMap[i].clear();
            }

            if(!defaultHSlider1Map.contains(mapIterator.key())){

                deletionMap[0] = presetMap.value("ComponentSettings").toMap().value("HSliders").toMap().value("HSlider1").toMap();
                deletionMap[0].remove(mapIterator.key());

                deletionMap[1] = presetMap.value("ComponentSettings").toMap().value("HSliders").toMap();
                deletionMap[1].insert("HSlider1", deletionMap[0]);

                deletionMap[2] = presetMap.value("ComponentSettings").toMap();
                deletionMap[2].insert("HSliders", deletionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", deletionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in Defulat Map. Deleted. ****";
            }
            mapIterator++;
        }

        //HSlider2///////////////////////////////////////////////////////////
        qDebug() << "HSlider 2 ";

        mapIterator = defaultHSlider2Map.constBegin();

        while(mapIterator != defaultHSlider2Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(presetMap.
                    value("ComponentSettings").toMap().
                    value("HSliders").toMap().
                    value("HSlider2").toMap().contains(mapIterator.key())){
                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {
                insertionMap[0] = presetMap.value("ComponentSettings").toMap().value("HSliders").toMap().value("HSlider2").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = presetMap.value("ComponentSettings").toMap().value("HSliders").toMap();
                insertionMap[1].insert("HSlider2", insertionMap[0]);

                insertionMap[2] = presetMap.value("ComponentSettings").toMap();
                insertionMap[2].insert("HSliders", insertionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", insertionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in PRESET. Inserted. ****";
            }
            mapIterator++;
        }

        //----------------------
        tempDeletionMap = presetMap.value("ComponentSettings").toMap().value("HSliders").toMap().value("HSlider2").toMap();
        mapIterator = tempDeletionMap.constBegin();

        while(mapIterator != tempDeletionMap.constEnd()){

            for(int i = 0; i<5; i++){
                deletionMap[i].clear();
            }

            if(!defaultHSlider2Map.contains(mapIterator.key())){

                deletionMap[0] = presetMap.value("ComponentSettings").toMap().value("HSliders").toMap().value("HSlider2").toMap();
                deletionMap[0].remove(mapIterator.key());

                deletionMap[1] = presetMap.value("ComponentSettings").toMap().value("HSliders").toMap();
                deletionMap[1].insert("HSlider2", deletionMap[0]);

                deletionMap[2] = presetMap.value("ComponentSettings").toMap();
                deletionMap[2].insert("HSliders", deletionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", deletionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in Defulat Map. Deleted. ****";
            }
            mapIterator++;
        }

        //HSlider3///////////////////////////////////////////////////////////
        qDebug() << "HSlider 3 ";

        mapIterator = defaultHSlider3Map.constBegin();

        while(mapIterator != defaultHSlider3Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(presetMap.
                    value("ComponentSettings").toMap().
                    value("HSliders").toMap().
                    value("HSlider3").toMap().contains(mapIterator.key())){
                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {
                insertionMap[0] = presetMap.value("ComponentSettings").toMap().value("HSliders").toMap().value("HSlider3").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = presetMap.value("ComponentSettings").toMap().value("HSliders").toMap();
                insertionMap[1].insert("HSlider3", insertionMap[0]);

                insertionMap[2] = presetMap.value("ComponentSettings").toMap();
                insertionMap[2].insert("HSliders", insertionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", insertionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in PRESET. Inserted. ****";
            }
            mapIterator++;
        }

        //----------------------
        tempDeletionMap = presetMap.value("ComponentSettings").toMap().value("HSliders").toMap().value("HSlider3").toMap();
        mapIterator = tempDeletionMap.constBegin();

        while(mapIterator != tempDeletionMap.constEnd()){

            for(int i = 0; i<5; i++){
                deletionMap[i].clear();
            }

            if(!defaultHSlider3Map.contains(mapIterator.key())){

                deletionMap[0] = presetMap.value("ComponentSettings").toMap().value("HSliders").toMap().value("HSlider3").toMap();
                deletionMap[0].remove(mapIterator.key());

                deletionMap[1] = presetMap.value("ComponentSettings").toMap().value("HSliders").toMap();
                deletionMap[1].insert("HSlider3", deletionMap[0]);

                deletionMap[2] = presetMap.value("ComponentSettings").toMap();
                deletionMap[2].insert("HSliders", deletionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", deletionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in Defulat Map. Deleted. ****";
            }
            mapIterator++;
        }


        /***************************************************************************/
        /**************************** LeftRightButtons *****************************/
        /***************************************************************************/
        qDebug() << "LeftRight Buttons Globals";

        //LeftRightButtons Globals///////////////////////////////////////////////////
        mapIterator = defaultLeftRightButtonsMap.constBegin();

        while(mapIterator != defaultLeftRightButtonsMap.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(presetMap.
                    value("ComponentSettings").toMap().
                    value("LeftRightButtons").toMap().contains(mapIterator.key())){
                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {
                insertionMap[0] = presetMap.value("ComponentSettings").toMap().value("LeftRightButtons").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = presetMap.value("ComponentSettings").toMap();
                insertionMap[1].insert("LeftRightButtons", insertionMap[0]);

                //insertionMap[2] = presetMap;
                presetMap.insert("ComponentSettings", insertionMap[1]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[2]);

                qDebug() << mapIterator.key() << "left right "<< "**** NOT FOUND in PRESET. Inserted. ****";
            }
            mapIterator++;
        }

        //----------------------
        tempDeletionMap = presetMap.value("ComponentSettings").toMap().value("LeftRightButtons").toMap();
        mapIterator = tempDeletionMap.constBegin();

        while(mapIterator != tempDeletionMap.constEnd()){

            for(int i = 0; i<5; i++){
                deletionMap[i].clear();
            }

            if(!defaultLeftRightButtonsMap.contains(mapIterator.key())){

                deletionMap[0] = presetMap.value("ComponentSettings").toMap().value("LeftRightButtons").toMap();
                deletionMap[0].remove(mapIterator.key());

                deletionMap[1] = presetMap.value("ComponentSettings").toMap();
                deletionMap[1].insert("LeftRightButtons", deletionMap[0]);

                //insertionMap[2] = presetMap;
                presetMap.insert("ComponentSettings", deletionMap[1]);

                //presetMap.insert(QString("Preset %1").arg(presetNum),insertionMap[2]);

                qDebug() << mapIterator.key() << "left right" << "**** NOT FOUND in Defulat Map. Deleted. ****";

            }
            mapIterator++;
        }

        //LeftRightButton0///////////////////////////////////////////////////
        qDebug() << "LeftRight Button 0";
        mapIterator = defaultLeftRightButton0Map.constBegin();

        while(mapIterator != defaultLeftRightButton0Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(presetMap.
                    value("ComponentSettings").toMap().
                    value("LeftRightButtons").toMap().
                    value("LeftRightButton0").toMap().contains(mapIterator.key())){

                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {
                insertionMap[0] = presetMap.value("ComponentSettings").toMap().value("LeftRightButtons").toMap().value("LeftRightButton0").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = presetMap.value("ComponentSettings").toMap().value("LeftRightButtons").toMap();
                insertionMap[1].insert("LeftRightButton0", insertionMap[0]);

                insertionMap[2] = presetMap.value("ComponentSettings").toMap();
                insertionMap[2].insert("LeftRightButtons", insertionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", insertionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in PRESET. Inserted. ****";
            }
            mapIterator++;
        }

        //----------------------
        tempDeletionMap = presetMap.value("ComponentSettings").toMap().value("LeftRightButtons").toMap().value("LeftRightButton0").toMap();
        mapIterator = tempDeletionMap.constBegin();

        while(mapIterator != tempDeletionMap.constEnd()){

            for(int i = 0; i<5; i++){
                deletionMap[i].clear();
            }

            if(!defaultLeftRightButton0Map.contains(mapIterator.key())){

                deletionMap[0] = presetMap.value("ComponentSettings").toMap().value("LeftRightButtons").toMap().value("LeftRightButton0").toMap();
                deletionMap[0].remove(mapIterator.key());

                deletionMap[1] = presetMap.value("ComponentSettings").toMap().value("LeftRightButtons").toMap();
                deletionMap[1].insert("LeftRightButton0", deletionMap[0]);

                deletionMap[2] = presetMap.value("ComponentSettings").toMap();
                deletionMap[2].insert("LeftRightButtons", deletionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", deletionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in Defulat Map. Deleted. ****";
            }
            mapIterator++;
        }


        //LeftRightButton1///////////////////////////////////////////////////
        qDebug() << "LeftRight Button 1";
        mapIterator = defaultLeftRightButton1Map.constBegin();

        while(mapIterator != defaultLeftRightButton1Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(presetMap.
                    value("ComponentSettings").toMap().
                    value("LeftRightButtons").toMap().
                    value("LeftRightButton1").toMap().contains(mapIterator.key())){

                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {
                insertionMap[0] = presetMap.value("ComponentSettings").toMap().value("LeftRightButtons").toMap().value("LeftRightButton1").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = presetMap.value("ComponentSettings").toMap().value("LeftRightButtons").toMap();
                insertionMap[1].insert("LeftRightButton1", insertionMap[0]);

                insertionMap[2] = presetMap.value("ComponentSettings").toMap();
                insertionMap[2].insert("LeftRightButtons", insertionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", insertionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in PRESET. Inserted. ****";
            }
            mapIterator++;
        }

        //----------------------
        tempDeletionMap = presetMap.value("ComponentSettings").toMap().value("LeftRightButtons").toMap().value("LeftRightButton1").toMap();
        mapIterator = tempDeletionMap.constBegin();

        while(mapIterator != tempDeletionMap.constEnd()){

            for(int i = 0; i<5; i++){
                deletionMap[i].clear();
            }

            if(!defaultLeftRightButton1Map.contains(mapIterator.key())){

                deletionMap[0] = presetMap.value("ComponentSettings").toMap().value("LeftRightButtons").toMap().value("LeftRightButton1").toMap();
                deletionMap[0].remove(mapIterator.key());

                deletionMap[1] = presetMap.value("ComponentSettings").toMap().value("LeftRightButtons").toMap();
                deletionMap[1].insert("LeftRightButton1", deletionMap[0]);

                deletionMap[2] = presetMap.value("ComponentSettings").toMap();
                deletionMap[2].insert("LeftRightButtons", deletionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", deletionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in Defulat Map. Deleted. ****";
            }
            mapIterator++;
        }

        //LeftRightButton2///////////////////////////////////////////////////
        qDebug() << "LeftRight Button 2";
        mapIterator = defaultLeftRightButton2Map.constBegin();

        while(mapIterator != defaultLeftRightButton2Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(presetMap.
                    value("ComponentSettings").toMap().
                    value("LeftRightButtons").toMap().
                    value("LeftRightButton2").toMap().contains(mapIterator.key())){

                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {
                insertionMap[0] = presetMap.value("ComponentSettings").toMap().value("LeftRightButtons").toMap().value("LeftRightButton2").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = presetMap.value("ComponentSettings").toMap().value("LeftRightButtons").toMap();
                insertionMap[1].insert("LeftRightButton2", insertionMap[0]);

                insertionMap[2] = presetMap.value("ComponentSettings").toMap();
                insertionMap[2].insert("LeftRightButtons", insertionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", insertionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in PRESET. Inserted. ****";
            }
            mapIterator++;
        }

        //----------------------
        tempDeletionMap = presetMap.value("ComponentSettings").toMap().value("LeftRightButtons").toMap().value("LeftRightButton2").toMap();
        mapIterator = tempDeletionMap.constBegin();

        while(mapIterator != tempDeletionMap.constEnd()){

            for(int i = 0; i<5; i++){
                deletionMap[i].clear();
            }

            if(!defaultLeftRightButton2Map.contains(mapIterator.key())){

                deletionMap[0] = presetMap.value("ComponentSettings").toMap().value("LeftRightButtons").toMap().value("LeftRightButton2").toMap();
                deletionMap[0].remove(mapIterator.key());

                deletionMap[1] = presetMap.value("ComponentSettings").toMap().value("LeftRightButtons").toMap();
                deletionMap[1].insert("LeftRightButton2", deletionMap[0]);

                deletionMap[2] = presetMap.value("ComponentSettings").toMap();
                deletionMap[2].insert("LeftRightButtons", deletionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", deletionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in Defulat Map. Deleted. ****";
            }
            mapIterator++;
        }

        //LeftRightButton3///////////////////////////////////////////////////
        qDebug() << "LeftRight Button 3";
        mapIterator = defaultLeftRightButton3Map.constBegin();

        while(mapIterator != defaultLeftRightButton3Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(presetMap.
                    value("ComponentSettings").toMap().
                    value("LeftRightButtons").toMap().
                    value("LeftRightButton3").toMap().contains(mapIterator.key())){

                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {
                insertionMap[0] = presetMap.value("ComponentSettings").toMap().value("LeftRightButtons").toMap().value("LeftRightButton3").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = presetMap.value("ComponentSettings").toMap().value("LeftRightButtons").toMap();
                insertionMap[1].insert("LeftRightButton3", insertionMap[0]);

                insertionMap[2] = presetMap.value("ComponentSettings").toMap();
                insertionMap[2].insert("LeftRightButtons", insertionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", insertionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in PRESET. Inserted. ****";
            }
            mapIterator++;
        }

        //----------------------
        tempDeletionMap = presetMap.value("ComponentSettings").toMap().value("LeftRightButtons").toMap().value("LeftRightButton3").toMap();
        mapIterator = tempDeletionMap.constBegin();

        while(mapIterator != tempDeletionMap.constEnd()){

            for(int i = 0; i<5; i++){
                deletionMap[i].clear();
            }

            if(!defaultLeftRightButton3Map.contains(mapIterator.key())){

                deletionMap[0] = presetMap.value("ComponentSettings").toMap().value("LeftRightButtons").toMap().value("LeftRightButton3").toMap();
                deletionMap[0].remove(mapIterator.key());

                deletionMap[1] = presetMap.value("ComponentSettings").toMap().value("LeftRightButtons").toMap();
                deletionMap[1].insert("LeftRightButton3", deletionMap[0]);

                deletionMap[2] = presetMap.value("ComponentSettings").toMap();
                deletionMap[2].insert("LeftRightButtons", deletionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", deletionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in Defulat Map. Deleted. ****";
            }
            mapIterator++;
        }


        /***************************************************************************/
        /******************************* LongSliders *******************************/
        /***************************************************************************/
        //LongSliders Globals////////////////////////////////////////////////////////
        qDebug() << "Long Slider Globals";

        mapIterator = defaultLongSlidersMap.constBegin();

        while(mapIterator != defaultLongSlidersMap.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(presetMap.
                    value("ComponentSettings").toMap().
                    value("LongSliders").toMap().contains(mapIterator.key())){

            } else {
                insertionMap[0] = presetMap.value("ComponentSettings").toMap().value("LongSliders").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = presetMap.value("ComponentSettings").toMap();
                insertionMap[1].insert("LongSliders", insertionMap[0]);

                //insertionMap[2] = presetMap;
                presetMap.insert("ComponentSettings", insertionMap[1]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[2]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in PRESET. Inserted. ****";
            }
            mapIterator++;
        }

        //----------------------
        tempDeletionMap = presetMap.value("ComponentSettings").toMap().value("LongSliders").toMap();
        mapIterator = tempDeletionMap.constBegin();

        qDebug() << "temp keys" << tempDeletionMap.keys();
        qDebug() << "def keys" << defaultLongSlidersMap.keys();

        while(mapIterator != tempDeletionMap.constEnd()){

            for(int i = 0; i<5; i++){
                deletionMap[i].clear();
            }

            if(!defaultLongSlidersMap.contains(mapIterator.key())){

                deletionMap[0] = presetMap.value("ComponentSettings").toMap().value("LongSliders").toMap();
                deletionMap[0].remove(mapIterator.key());

                deletionMap[1] = presetMap.value("ComponentSettings").toMap();
                deletionMap[1].insert("LongSliders", deletionMap[0]);

                //insertionMap[2] = presetMap;
                presetMap.insert("ComponentSettings", deletionMap[1]);

                //presetMap.insert(QString("Preset %1").arg(presetNum),insertionMap[2]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in Defulat Map. Deleted. ****";

            }
            mapIterator++;
        }

        //LongSlider0////////////////////////////////////////////////////////
        qDebug() << "Long Slider 0";
        mapIterator = defaultLongSliders0Map.constBegin();

        while(mapIterator != defaultLongSliders0Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(presetMap.
                    value("ComponentSettings").toMap().
                    value("LongSliders").toMap().
                    value("LongSlider0").toMap().contains(mapIterator.key())){

                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {
                insertionMap[0] = presetMap.value("ComponentSettings").toMap().value("LongSliders").toMap().value("LongSlider0").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = presetMap.value("ComponentSettings").toMap().value("LongSliders").toMap();
                insertionMap[1].insert("LongSlider0", insertionMap[0]);

                insertionMap[2] = presetMap.value("ComponentSettings").toMap();
                insertionMap[2].insert("LongSliders", insertionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", insertionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in PRESET. Inserted. ****";
            }
            mapIterator++;
        }

        //----------------------
        tempDeletionMap = presetMap.value("ComponentSettings").toMap().value("LongSliders").toMap().value("LongSlider0").toMap();
        mapIterator = tempDeletionMap.constBegin();

        while(mapIterator != tempDeletionMap.constEnd()){

            for(int i = 0; i<5; i++){
                deletionMap[i].clear();
            }

            if(!defaultLongSliders0Map.contains(mapIterator.key())){

                deletionMap[0] = presetMap.value("ComponentSettings").toMap().value("LongSliders").toMap().value("LongSlider0").toMap();
                deletionMap[0].remove(mapIterator.key());

                deletionMap[1] = presetMap.value("ComponentSettings").toMap().value("LongSliders").toMap();
                deletionMap[1].insert("LongSlider0", deletionMap[0]);

                deletionMap[2] = presetMap.value("ComponentSettings").toMap();
                deletionMap[2].insert("LongSliders", deletionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", deletionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in Defulat Map. Deleted. ****";
            }
            mapIterator++;
        }


        /***************************************************************************/
        /******************************* ModeButtons *******************************/
        /***************************************************************************/
        qDebug() << "Mode Buttons Globals";

        //ModeButtons Globals////////////////////////////////////////////////////////
        mapIterator = defaultModeButtonsMap.constBegin();

        while(mapIterator != defaultModeButtonsMap.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(presetMap.
                    value("ComponentSettings").toMap().
                    value("ModeButtons").toMap().contains(mapIterator.key())){
                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {
                insertionMap[0] = presetMap.value("ComponentSettings").toMap().value("ModeButtons").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = presetMap.value("ComponentSettings").toMap();
                insertionMap[1].insert("ModeButtons", insertionMap[0]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", insertionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[2]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in PRESET. Inserted. ****";
            }
            mapIterator++;
        }

        //----------------------
        tempDeletionMap = presetMap.value("ComponentSettings").toMap().value("ModeButtons").toMap();
        mapIterator = tempDeletionMap.constBegin();

        while(mapIterator != tempDeletionMap.constEnd()){

            for(int i = 0; i<5; i++){
                deletionMap[i].clear();
            }

            if(!defaultModeButtonsMap.contains(mapIterator.key())){

                deletionMap[0] = presetMap.value("ComponentSettings").toMap().value("ModeButtons").toMap();
                deletionMap[0].remove(mapIterator.key());

                deletionMap[1] = presetMap.value("ComponentSettings").toMap();
                deletionMap[1].insert("ModeButtons", deletionMap[0]);

                //insertionMap[2] = presetMap;
                presetMap.insert("ComponentSettings", deletionMap[1]);

                //presetMap.insert(QString("Preset %1").arg(presetNum),insertionMap[2]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in Defulat Map. Deleted. ****";

            }
            mapIterator++;
        }

        //ModeButton0////////////////////////////////////////////////////////
        qDebug() << "Mode Buttons 0";
        mapIterator = defaultModeButton0Map.constBegin();

        while(mapIterator != defaultModeButton0Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(presetMap.
                    value("ComponentSettings").toMap().
                    value("ModeButtons").toMap().
                    value("ModeButton0").toMap().contains(mapIterator.key())){

                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {
                insertionMap[0] = presetMap.value("ComponentSettings").toMap().value("ModeButtons").toMap().value("ModeButton0").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = presetMap.value("ComponentSettings").toMap().value("ModeButtons").toMap();
                insertionMap[1].insert("ModeButton0", insertionMap[0]);

                insertionMap[2] = presetMap.value("ComponentSettings").toMap();
                insertionMap[2].insert("ModeButtons", insertionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", insertionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in PRESET. Inserted. ****";
            }
            mapIterator++;
        }

        //----------------------
        tempDeletionMap = presetMap.value("ComponentSettings").toMap().value("LongSliders").toMap().value("ModeButton0").toMap();
        mapIterator = tempDeletionMap.constBegin();

        while(mapIterator != tempDeletionMap.constEnd()){

            for(int i = 0; i<5; i++){
                deletionMap[i].clear();
            }

            if(!defaultModeButton0Map.contains(mapIterator.key())){

                deletionMap[0] = presetMap.value("ComponentSettings").toMap().value("ModeButtons").toMap().value("ModeButton0").toMap();
                deletionMap[0].remove(mapIterator.key());

                deletionMap[1] = presetMap.value("ComponentSettings").toMap().value("ModeButtons").toMap();
                deletionMap[1].insert("ModeButton0", deletionMap[0]);

                deletionMap[2] = presetMap.value("ComponentSettings").toMap();
                deletionMap[2].insert("ModeButtons", deletionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", deletionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in Defulat Map. Deleted. ****";
            }
            mapIterator++;
        }


        /***************************************************************************/
        /*********************************** Pads **********************************/
        /***************************************************************************/
        //Pads Globals///////////////////////////////////////////////////////////////

        qDebug() << "Pad Globals";

        mapIterator = defaultPadsMap.constBegin();

        while(mapIterator != defaultPadsMap.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(presetMap.
                    value("ComponentSettings").toMap().
                    value("Pads").toMap().contains(mapIterator.key())){
            } else {
                insertionMap[0] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = presetMap.value("ComponentSettings").toMap();
                insertionMap[1].insert("Pads", insertionMap[0]);

                //insertionMap[2] = presetMap;
                presetMap.insert("ComponentSettings", insertionMap[1]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[2]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in PRESET. Inserted. ****";
            }
            mapIterator++;
        }

        //----------------------
        tempDeletionMap = presetMap.value("ComponentSettings").toMap().value("Pads").toMap();
        mapIterator = tempDeletionMap.constBegin();

        while(mapIterator != tempDeletionMap.constEnd()){

            for(int i = 0; i<5; i++){
                deletionMap[i].clear();
            }

            if(!defaultPadsMap.contains(mapIterator.key())){

                deletionMap[0] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap();
                deletionMap[0].remove(mapIterator.key());

                deletionMap[1] = presetMap.value("ComponentSettings").toMap();
                deletionMap[1].insert("Pads", deletionMap[0]);

                //insertionMap[2] = presetMap;
                presetMap.insert("ComponentSettings", deletionMap[1]);

                //presetMap.insert(QString("Preset %1").arg(presetNum),insertionMap[2]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in Defulat Map. Deleted. ****";

            }
            mapIterator++;
        }

        //Pad0///////////////////////////////////////////////////////////////

        qDebug() << "Pad 0";

        mapIterator = defaultPad0Map.constBegin();

        while(mapIterator != defaultPad0Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(presetMap.
                    value("ComponentSettings").toMap().
                    value("Pads").toMap().
                    value("Pad0").toMap().contains(mapIterator.key())){

                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {
                insertionMap[0] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap().value("Pad0").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap();
                insertionMap[1].insert("Pad0", insertionMap[0]);

                insertionMap[2] = presetMap.value("ComponentSettings").toMap();
                insertionMap[2].insert("Pads", insertionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", insertionMap[2]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in PRESET. Inserted ****";
            }
            mapIterator++;
        }

        //----------------------
        tempDeletionMap = presetMap.value("ComponentSettings").toMap().value("Pads").toMap().value("Pad0").toMap();
        mapIterator = tempDeletionMap.constBegin();

        while(mapIterator != tempDeletionMap.constEnd()){

            for(int i = 0; i<5; i++){
                deletionMap[i].clear();
            }

            if(!defaultPad0Map.contains(mapIterator.key())){

                deletionMap[0] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap().value("Pad0").toMap();
                deletionMap[0].remove(mapIterator.key());

                deletionMap[1] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap();
                deletionMap[1].insert("Pad0", deletionMap[0]);

                deletionMap[2] = presetMap.value("ComponentSettings").toMap();
                deletionMap[2].insert("Pads", deletionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", deletionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in Defulat Map. Deleted. ****";
            }
            mapIterator++;
        }

        //Pad1///////////////////////////////////////////////////////////////

        qDebug() << "Pad 1";
        mapIterator = defaultPad1Map.constBegin();

        while(mapIterator != defaultPad1Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(presetMap.
                    value("ComponentSettings").toMap().
                    value("Pads").toMap().
                    value("Pad1").toMap().contains(mapIterator.key())){

            } else {
                insertionMap[0] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap().value("Pad1").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap();
                insertionMap[1].insert("Pad1", insertionMap[0]);

                insertionMap[2] = presetMap.value("ComponentSettings").toMap();
                insertionMap[2].insert("Pads", insertionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", insertionMap[2]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in PRESET. Inserted ****";
            }
            mapIterator++;
        }

        //----------------------
        tempDeletionMap = presetMap.value("ComponentSettings").toMap().value("Pads").toMap().value("Pad1").toMap();
        mapIterator = tempDeletionMap.constBegin();

        while(mapIterator != tempDeletionMap.constEnd()){

            for(int i = 0; i<5; i++){
                deletionMap[i].clear();
            }

            if(!defaultPad1Map.contains(mapIterator.key())){

                deletionMap[0] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap().value("Pad1").toMap();
                deletionMap[0].remove(mapIterator.key());

                deletionMap[1] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap();
                deletionMap[1].insert("Pad1", deletionMap[0]);

                deletionMap[2] = presetMap.value("ComponentSettings").toMap();
                deletionMap[2].insert("Pads", deletionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", deletionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in Defulat Map. Deleted. ****";
            }
            mapIterator++;
        }

        //Pad2///////////////////////////////////////////////////////////////
        qDebug() << "Pad 2";
        mapIterator = defaultPad2Map.constBegin();

        while(mapIterator != defaultPad2Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(presetMap.
                    value("ComponentSettings").toMap().
                    value("Pads").toMap().
                    value("Pad2").toMap().contains(mapIterator.key())){

            } else {
                insertionMap[0] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap().value("Pad2").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap();
                insertionMap[1].insert("Pad2", insertionMap[0]);

                insertionMap[2] = presetMap.value("ComponentSettings").toMap();
                insertionMap[2].insert("Pads", insertionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", insertionMap[2]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in PRESET. Inserted ****";
            }
            mapIterator++;
        }

        //----------------------
        tempDeletionMap = presetMap.value("ComponentSettings").toMap().value("Pads").toMap().value("Pad2").toMap();
        mapIterator = tempDeletionMap.constBegin();

        while(mapIterator != tempDeletionMap.constEnd()){

            for(int i = 0; i<5; i++){
                deletionMap[i].clear();
            }

            if(!defaultPad2Map.contains(mapIterator.key())){

                deletionMap[0] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap().value("Pad2").toMap();
                deletionMap[0].remove(mapIterator.key());

                deletionMap[1] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap();
                deletionMap[1].insert("Pad2", deletionMap[0]);

                deletionMap[2] = presetMap.value("ComponentSettings").toMap();
                deletionMap[2].insert("Pads", deletionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", deletionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in Defulat Map. Deleted. ****";
            }
            mapIterator++;
        }

        //Pad3///////////////////////////////////////////////////////////////
        qDebug() << "Pad 3";
        mapIterator = defaultPad3Map.constBegin();

        while(mapIterator != defaultPad3Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(presetMap.
                    value("ComponentSettings").toMap().
                    value("Pads").toMap().
                    value("Pad3").toMap().contains(mapIterator.key())){

                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {
                insertionMap[0] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap().value("Pad3").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap();
                insertionMap[1].insert("Pad3", insertionMap[0]);

                insertionMap[2] = presetMap.value("ComponentSettings").toMap();
                insertionMap[2].insert("Pads", insertionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", insertionMap[2]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in PRESET. Inserted ****";
            }
            mapIterator++;
        }

        //----------------------
        tempDeletionMap = presetMap.value("ComponentSettings").toMap().value("Pads").toMap().value("Pad3").toMap();
        mapIterator = tempDeletionMap.constBegin();

        while(mapIterator != tempDeletionMap.constEnd()){

            for(int i = 0; i<5; i++){
                deletionMap[i].clear();
            }

            if(!defaultPad3Map.contains(mapIterator.key())){

                deletionMap[0] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap().value("Pad3").toMap();
                deletionMap[0].remove(mapIterator.key());

                deletionMap[1] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap();
                deletionMap[1].insert("Pad3", deletionMap[0]);

                deletionMap[2] = presetMap.value("ComponentSettings").toMap();
                deletionMap[2].insert("Pads", deletionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", deletionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in Defulat Map. Deleted. ****";
            }
            mapIterator++;
        }

        //Pad4///////////////////////////////////////////////////////////////
        qDebug() << "Pad 4";
        mapIterator = defaultPad4Map.constBegin();

        while(mapIterator != defaultPad4Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(presetMap.
                    value("ComponentSettings").toMap().
                    value("Pads").toMap().
                    value("Pad4").toMap().contains(mapIterator.key())){

                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {
                insertionMap[0] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap().value("Pad4").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap();
                insertionMap[1].insert("Pad4", insertionMap[0]);

                insertionMap[2] = presetMap.value("ComponentSettings").toMap();
                insertionMap[2].insert("Pads", insertionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", insertionMap[2]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in PRESET. Inserted ****";
            }
            mapIterator++;
        }

        //----------------------
        tempDeletionMap = presetMap.value("ComponentSettings").toMap().value("Pads").toMap().value("Pad4").toMap();
        mapIterator = tempDeletionMap.constBegin();

        while(mapIterator != tempDeletionMap.constEnd()){

            for(int i = 0; i<5; i++){
                deletionMap[i].clear();
            }

            if(!defaultPad4Map.contains(mapIterator.key())){

                deletionMap[0] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap().value("Pad4").toMap();
                deletionMap[0].remove(mapIterator.key());

                deletionMap[1] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap();
                deletionMap[1].insert("Pad4", deletionMap[0]);

                deletionMap[2] = presetMap.value("ComponentSettings").toMap();
                deletionMap[2].insert("Pads", deletionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", deletionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in Defulat Map. Deleted. ****";
            }
            mapIterator++;
        }

        //Pad5///////////////////////////////////////////////////////////////
        qDebug() << "Pad 5";
        mapIterator = defaultPad5Map.constBegin();

        while(mapIterator != defaultPad5Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(presetMap.
                    value("ComponentSettings").toMap().
                    value("Pads").toMap().
                    value("Pad5").toMap().contains(mapIterator.key())){

            } else {
                insertionMap[0] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap().value("Pad5").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap();
                insertionMap[1].insert("Pad5", insertionMap[0]);

                insertionMap[2] = presetMap.value("ComponentSettings").toMap();
                insertionMap[2].insert("Pads", insertionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", insertionMap[2]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in PRESET. Inserted ****";
            }
            mapIterator++;
        }

        //----------------------
        tempDeletionMap = presetMap.value("ComponentSettings").toMap().value("Pads").toMap().value("Pad5").toMap();
        mapIterator = tempDeletionMap.constBegin();

        while(mapIterator != tempDeletionMap.constEnd()){

            for(int i = 0; i<5; i++){
                deletionMap[i].clear();
            }

            if(!defaultPad5Map.contains(mapIterator.key())){

                deletionMap[0] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap().value("Pad5").toMap();
                deletionMap[0].remove(mapIterator.key());

                deletionMap[1] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap();
                deletionMap[1].insert("Pad5", deletionMap[0]);

                deletionMap[2] = presetMap.value("ComponentSettings").toMap();
                deletionMap[2].insert("Pads", deletionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", deletionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in Defulat Map. Deleted. ****";
            }
            mapIterator++;
        }

        //Pad6///////////////////////////////////////////////////////////////
        qDebug() << "Pad 6";
        mapIterator = defaultPad6Map.constBegin();

        while(mapIterator != defaultPad6Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(presetMap.
                    value("ComponentSettings").toMap().
                    value("Pads").toMap().
                    value("Pad6").toMap().contains(mapIterator.key())){

            } else {
                insertionMap[0] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap().value("Pad6").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap();
                insertionMap[1].insert("Pad6", insertionMap[0]);

                insertionMap[2] = presetMap.value("ComponentSettings").toMap();
                insertionMap[2].insert("Pads", insertionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", insertionMap[2]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in PRESET. Inserted ****";
            }
            mapIterator++;
        }

        //----------------------
        tempDeletionMap = presetMap.value("ComponentSettings").toMap().value("Pads").toMap().value("Pad6").toMap();
        mapIterator = tempDeletionMap.constBegin();

        while(mapIterator != tempDeletionMap.constEnd()){

            for(int i = 0; i<5; i++){
                deletionMap[i].clear();
            }

            if(!defaultPad6Map.contains(mapIterator.key())){

                deletionMap[0] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap().value("Pad6").toMap();
                deletionMap[0].remove(mapIterator.key());

                deletionMap[1] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap();
                deletionMap[1].insert("Pad6", deletionMap[0]);

                deletionMap[2] = presetMap.value("ComponentSettings").toMap();
                deletionMap[2].insert("Pads", deletionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", deletionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in Defulat Map. Deleted. ****";
            }
            mapIterator++;
        }

        //Pad7///////////////////////////////////////////////////////////////
        qDebug() << "Pad 7";
        mapIterator = defaultPad7Map.constBegin();

        while(mapIterator != defaultPad7Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(presetMap.
                    value("ComponentSettings").toMap().
                    value("Pads").toMap().
                    value("Pad7").toMap().contains(mapIterator.key())){

            } else {
                insertionMap[0] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap().value("Pad7").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap();
                insertionMap[1].insert("Pad7", insertionMap[0]);

                insertionMap[2] = presetMap.value("ComponentSettings").toMap();
                insertionMap[2].insert("Pads", insertionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", insertionMap[2]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in PRESET. Inserted ****";
            }
            mapIterator++;
        }

        //----------------------
        tempDeletionMap = presetMap.value("ComponentSettings").toMap().value("Pads").toMap().value("Pad7").toMap();
        mapIterator = tempDeletionMap.constBegin();

        while(mapIterator != tempDeletionMap.constEnd()){

            for(int i = 0; i<5; i++){
                deletionMap[i].clear();
            }

            if(!defaultPad7Map.contains(mapIterator.key())){

                deletionMap[0] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap().value("Pad7").toMap();
                deletionMap[0].remove(mapIterator.key());

                deletionMap[1] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap();
                deletionMap[1].insert("Pad7", deletionMap[0]);

                deletionMap[2] = presetMap.value("ComponentSettings").toMap();
                deletionMap[2].insert("Pads", deletionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", deletionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in Defulat Map. Deleted. ****";
            }
            mapIterator++;
        }

        //Pad8///////////////////////////////////////////////////////////////
        qDebug() << "Pad80";
        mapIterator = defaultPad8Map.constBegin();

        while(mapIterator != defaultPad8Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(presetMap.
                    value("ComponentSettings").toMap().
                    value("Pads").toMap().
                    value("Pad8").toMap().contains(mapIterator.key())){

            } else {
                insertionMap[0] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap().value("Pad8").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap();
                insertionMap[1].insert("Pad8", insertionMap[0]);

                insertionMap[2] = presetMap.value("ComponentSettings").toMap();
                insertionMap[2].insert("Pads", insertionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", insertionMap[2]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in PRESET. Inserted ****";
            }
            mapIterator++;
        }

        //----------------------
        tempDeletionMap = presetMap.value("ComponentSettings").toMap().value("Pads").toMap().value("Pad8").toMap();
        mapIterator = tempDeletionMap.constBegin();

        while(mapIterator != tempDeletionMap.constEnd()){

            for(int i = 0; i<5; i++){
                deletionMap[i].clear();
            }

            if(!defaultPad8Map.contains(mapIterator.key())){

                deletionMap[0] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap().value("Pad8").toMap();
                deletionMap[0].remove(mapIterator.key());

                deletionMap[1] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap();
                deletionMap[1].insert("Pad8", deletionMap[0]);

                deletionMap[2] = presetMap.value("ComponentSettings").toMap();
                deletionMap[2].insert("Pads", deletionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", deletionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in Defulat Map. Deleted. ****";
            }
            mapIterator++;
        }

        //Pad9///////////////////////////////////////////////////////////////
        qDebug() << "Pad 9";
        mapIterator = defaultPad9Map.constBegin();

        while(mapIterator != defaultPad9Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(presetMap.
                    value("ComponentSettings").toMap().
                    value("Pads").toMap().
                    value("Pad9").toMap().contains(mapIterator.key())){

            } else {
                insertionMap[0] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap().value("Pad9").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap();
                insertionMap[1].insert("Pad9", insertionMap[0]);

                insertionMap[2] = presetMap.value("ComponentSettings").toMap();
                insertionMap[2].insert("Pads", insertionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", insertionMap[2]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in PRESET. Inserted ****";
            }
            mapIterator++;
        }

        //----------------------
        tempDeletionMap = presetMap.value("ComponentSettings").toMap().value("Pads").toMap().value("Pad9").toMap();
        mapIterator = tempDeletionMap.constBegin();

        while(mapIterator != tempDeletionMap.constEnd()){

            for(int i = 0; i<5; i++){
                deletionMap[i].clear();
            }

            if(!defaultPad9Map.contains(mapIterator.key())){

                deletionMap[0] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap().value("Pad9").toMap();
                deletionMap[0].remove(mapIterator.key());

                deletionMap[1] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap();
                deletionMap[1].insert("Pad9", deletionMap[0]);

                deletionMap[2] = presetMap.value("ComponentSettings").toMap();
                deletionMap[2].insert("Pads", deletionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", deletionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in Defulat Map. Deleted. ****";
            }
            mapIterator++;
        }

        //Pad10///////////////////////////////////////////////////////////////
        qDebug() << "Pad 10";
        mapIterator = defaultPad10Map.constBegin();

        while(mapIterator != defaultPad10Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(presetMap.
                    value("ComponentSettings").toMap().
                    value("Pads").toMap().
                    value("Pad10").toMap().contains(mapIterator.key())){

                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {
                insertionMap[0] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap().value("Pad10").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap();
                insertionMap[1].insert("Pad10", insertionMap[0]);

                insertionMap[2] = presetMap.value("ComponentSettings").toMap();
                insertionMap[2].insert("Pads", insertionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", insertionMap[2]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in PRESET. Inserted ****";
            }
            mapIterator++;
        }

        //----------------------
        tempDeletionMap = presetMap.value("ComponentSettings").toMap().value("Pads").toMap().value("Pad10").toMap();
        mapIterator = tempDeletionMap.constBegin();

        while(mapIterator != tempDeletionMap.constEnd()){

            for(int i = 0; i<5; i++){
                deletionMap[i].clear();
            }

            if(!defaultPad10Map.contains(mapIterator.key())){

                deletionMap[0] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap().value("Pad10").toMap();
                deletionMap[0].remove(mapIterator.key());

                deletionMap[1] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap();
                deletionMap[1].insert("Pad10", deletionMap[0]);

                deletionMap[2] = presetMap.value("ComponentSettings").toMap();
                deletionMap[2].insert("Pads", deletionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", deletionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in Defulat Map. Deleted. ****";
            }
            mapIterator++;
        }

        //Pad11///////////////////////////////////////////////////////////////
        qDebug() << "Pad 11";
        mapIterator = defaultPad11Map.constBegin();

        while(mapIterator != defaultPad11Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(presetMap.
                    value("ComponentSettings").toMap().
                    value("Pads").toMap().
                    value("Pad11").toMap().contains(mapIterator.key())){

            } else {
                insertionMap[0] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap().value("Pad11").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap();
                insertionMap[1].insert("Pad11", insertionMap[0]);

                insertionMap[2] = presetMap.value("ComponentSettings").toMap();
                insertionMap[2].insert("Pads", insertionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", insertionMap[2]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in PRESET. Inserted ****";
            }
            mapIterator++;
        }

        //----------------------
        tempDeletionMap = presetMap.value("ComponentSettings").toMap().value("Pads").toMap().value("Pad11").toMap();
        mapIterator = tempDeletionMap.constBegin();

        while(mapIterator != tempDeletionMap.constEnd()){

            for(int i = 0; i<5; i++){
                deletionMap[i].clear();
            }

            if(!defaultPad11Map.contains(mapIterator.key())){

                deletionMap[0] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap().value("Pad11").toMap();
                deletionMap[0].remove(mapIterator.key());

                deletionMap[1] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap();
                deletionMap[1].insert("Pad11", deletionMap[0]);

                deletionMap[2] = presetMap.value("ComponentSettings").toMap();
                deletionMap[2].insert("Pads", deletionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", deletionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in Defulat Map. Deleted. ****";
            }
            mapIterator++;
        }

        //Pad12///////////////////////////////////////////////////////////////
        qDebug() << "Pad 12";
        mapIterator = defaultPad12Map.constBegin();

        while(mapIterator != defaultPad12Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(presetMap.
                    value("ComponentSettings").toMap().
                    value("Pads").toMap().
                    value("Pad12").toMap().contains(mapIterator.key())){

            } else {
                insertionMap[0] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap().value("Pad12").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap();
                insertionMap[1].insert("Pad12", insertionMap[0]);

                insertionMap[2] = presetMap.value("ComponentSettings").toMap();
                insertionMap[2].insert("Pads", insertionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", insertionMap[2]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in PRESET. Inserted ****";
            }
            mapIterator++;
        }

        //----------------------
        tempDeletionMap = presetMap.value("ComponentSettings").toMap().value("Pads").toMap().value("Pad12").toMap();
        mapIterator = tempDeletionMap.constBegin();

        while(mapIterator != tempDeletionMap.constEnd()){

            for(int i = 0; i<5; i++){
                deletionMap[i].clear();
            }

            if(!defaultPad12Map.contains(mapIterator.key())){

                deletionMap[0] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap().value("Pad12").toMap();
                deletionMap[0].remove(mapIterator.key());

                deletionMap[1] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap();
                deletionMap[1].insert("Pad12", deletionMap[0]);

                deletionMap[2] = presetMap.value("ComponentSettings").toMap();
                deletionMap[2].insert("Pads", deletionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", deletionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in Defulat Map. Deleted. ****";
            }
            mapIterator++;
        }

        //Pad13///////////////////////////////////////////////////////////////
        qDebug() << "Pad 13";
        mapIterator = defaultPad13Map.constBegin();

        while(mapIterator != defaultPad13Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(presetMap.
                    value("ComponentSettings").toMap().
                    value("Pads").toMap().
                    value("Pad13").toMap().contains(mapIterator.key())){

                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {
                insertionMap[0] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap().value("Pad13").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap();
                insertionMap[1].insert("Pad13", insertionMap[0]);

                insertionMap[2] = presetMap.value("ComponentSettings").toMap();
                insertionMap[2].insert("Pads", insertionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", insertionMap[2]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in PRESET. Inserted ****";
            }
            mapIterator++;
        }

        //----------------------
        tempDeletionMap = presetMap.value("ComponentSettings").toMap().value("Pads").toMap().value("Pad13").toMap();
        mapIterator = tempDeletionMap.constBegin();

        while(mapIterator != tempDeletionMap.constEnd()){

            for(int i = 0; i<5; i++){
                deletionMap[i].clear();
            }

            if(!defaultPad13Map.contains(mapIterator.key())){

                deletionMap[0] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap().value("Pad13").toMap();
                deletionMap[0].remove(mapIterator.key());

                deletionMap[1] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap();
                deletionMap[1].insert("Pad13", deletionMap[0]);

                deletionMap[2] = presetMap.value("ComponentSettings").toMap();
                deletionMap[2].insert("Pads", deletionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", deletionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in Defulat Map. Deleted. ****";
            }
            mapIterator++;
        }

        //Pad14///////////////////////////////////////////////////////////////
        qDebug() << "Pad 14";
        mapIterator = defaultPad14Map.constBegin();

        while(mapIterator != defaultPad14Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(presetMap.
                    value("ComponentSettings").toMap().
                    value("Pads").toMap().
                    value("Pad14").toMap().contains(mapIterator.key())){

            } else {
                insertionMap[0] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap().value("Pad14").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap();
                insertionMap[1].insert("Pad14", insertionMap[0]);

                insertionMap[2] = presetMap.value("ComponentSettings").toMap();
                insertionMap[2].insert("Pads", insertionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", insertionMap[2]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in PRESET. Inserted ****";
            }
            mapIterator++;
        }

        //----------------------
        tempDeletionMap = presetMap.value("ComponentSettings").toMap().value("Pads").toMap().value("Pad14").toMap();
        mapIterator = tempDeletionMap.constBegin();

        while(mapIterator != tempDeletionMap.constEnd()){

            for(int i = 0; i<5; i++){
                deletionMap[i].clear();
            }

            if(!defaultPad14Map.contains(mapIterator.key())){

                deletionMap[0] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap().value("Pad14").toMap();
                deletionMap[0].remove(mapIterator.key());

                deletionMap[1] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap();
                deletionMap[1].insert("Pad14", deletionMap[0]);

                deletionMap[2] = presetMap.value("ComponentSettings").toMap();
                deletionMap[2].insert("Pads", deletionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", deletionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in Defulat Map. Deleted. ****";
            }
            mapIterator++;
        }

        //Pad15///////////////////////////////////////////////////////////////
        qDebug() << "Pad 15";
        mapIterator = defaultPad15Map.constBegin();

        while(mapIterator != defaultPad15Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(presetMap.
                    value("ComponentSettings").toMap().
                    value("Pads").toMap().
                    value("Pad15").toMap().contains(mapIterator.key())){

            } else {
                insertionMap[0] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap().value("Pad15").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap();
                insertionMap[1].insert("Pad15", insertionMap[0]);

                insertionMap[2] = presetMap.value("ComponentSettings").toMap();
                insertionMap[2].insert("Pads", insertionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", insertionMap[2]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in PRESET. Inserted ****";
            }
            mapIterator++;
        }

        //----------------------
        tempDeletionMap = presetMap.value("ComponentSettings").toMap().value("Pads").toMap().value("Pad15").toMap();
        mapIterator = tempDeletionMap.constBegin();

        while(mapIterator != tempDeletionMap.constEnd()){

            for(int i = 0; i<5; i++){
                deletionMap[i].clear();
            }

            if(!defaultPad15Map.contains(mapIterator.key())){

                deletionMap[0] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap().value("Pad15").toMap();
                deletionMap[0].remove(mapIterator.key());

                deletionMap[1] = presetMap.value("ComponentSettings").toMap().value("Pads").toMap();
                deletionMap[1].insert("Pad15", deletionMap[0]);

                deletionMap[2] = presetMap.value("ComponentSettings").toMap();
                deletionMap[2].insert("Pads", deletionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", deletionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in Defulat Map. Deleted. ****";
            }
            mapIterator++;
        }


        /***************************************************************************/
        /********************************* Rhombus *********************************/
        /***************************************************************************/
        //RhombusButtons Globals/////////////////////////////////////////////////////
        qDebug() << "Rhombus Global";

        mapIterator = defaultRhombusButtonsMap.constBegin();

        while(mapIterator != defaultRhombusButtonsMap.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(presetMap.
                    value("ComponentSettings").toMap().
                    value("RhombusButtons").toMap().contains(mapIterator.key())){
                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {
                insertionMap[0] = presetMap.value("ComponentSettings").toMap().value("RhombusButtons").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = presetMap.value("ComponentSettings").toMap();
                insertionMap[1].insert("RhombusButtons", insertionMap[0]);

                //insertionMap[2] = presetMap;
                presetMap.insert("ComponentSettings", insertionMap[1]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[2]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in PRESET. Inserted. ****";
            }
            mapIterator++;
        }

        //----------------------
        tempDeletionMap = presetMap.value("ComponentSettings").toMap().value("RhombusButtons").toMap();
        mapIterator = tempDeletionMap.constBegin();

        while(mapIterator != tempDeletionMap.constEnd()){

            for(int i = 0; i<5; i++){
                deletionMap[i].clear();
            }

            if(!defaultRhombusButtonsMap.contains(mapIterator.key())){

                deletionMap[0] = presetMap.value("ComponentSettings").toMap().value("RhombusButtons").toMap();
                deletionMap[0].remove(mapIterator.key());

                deletionMap[1] = presetMap.value("ComponentSettings").toMap();
                deletionMap[1].insert("RhombusButtons", deletionMap[0]);

                //insertionMap[2] = presetMap;
                presetMap.insert("ComponentSettings", deletionMap[1]);

                //presetMap.insert(QString("Preset %1").arg(presetNum),insertionMap[2]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in Defulat Map. Deleted. ****";

            }
            mapIterator++;
        }

        //RhombusButton0/////////////////////////////////////////////////////
        qDebug() << "Rhombus 0";
        mapIterator = defaultRhombusButton0Map.constBegin();

        while(mapIterator != defaultRhombusButton0Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(presetMap.
                    value("ComponentSettings").toMap().
                    value("RhombusButtons").toMap().
                    value("RhombusButton0").toMap().contains(mapIterator.key())){

                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {
                insertionMap[0] = presetMap.value("ComponentSettings").toMap().value("RhombusButtons").toMap().value("RhombusButton0").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = presetMap.value("ComponentSettings").toMap().value("RhombusButtons").toMap();
                insertionMap[1].insert("RhombusButton0", insertionMap[0]);

                insertionMap[2] = presetMap.value("ComponentSettings").toMap();
                insertionMap[2].insert("RhombusButtons", insertionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", insertionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in PRESET. Inserted. ****";
            }
            mapIterator++;
        }

        //----------------------
        tempDeletionMap = presetMap.value("ComponentSettings").toMap().value("RhombusButtons").toMap().value("RhombusButton0").toMap();
        mapIterator = tempDeletionMap.constBegin();

        while(mapIterator != tempDeletionMap.constEnd()){

            for(int i = 0; i<5; i++){
                deletionMap[i].clear();
            }

            if(!defaultRhombusButton0Map.contains(mapIterator.key())){

                deletionMap[0] = presetMap.value("ComponentSettings").toMap().value("RhombusButtons").toMap().value("RhombusButton0").toMap();
                deletionMap[0].remove(mapIterator.key());

                deletionMap[1] = presetMap.value("ComponentSettings").toMap().value("RhombusButtons").toMap();
                deletionMap[1].insert("RhombusButton0", deletionMap[0]);

                deletionMap[2] = presetMap.value("ComponentSettings").toMap();
                deletionMap[2].insert("RhombusButtons", deletionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", deletionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in Defulat Map. Deleted. ****";
            }
            mapIterator++;
        }


        /***************************************************************************/
        /********************************* Rotaries *********************************/
        /***************************************************************************/
        //Rotaries Globals///////////////////////////////////////////////////////////
        qDebug() << "Rotaries Global";
        mapIterator = defaultRotariesMap.constBegin();

        while(mapIterator != defaultRotariesMap.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(presetMap.
                    value("ComponentSettings").toMap().
                    value("Rotaries").toMap().contains(mapIterator.key())){
            } else {
                insertionMap[0] = presetMap.value("ComponentSettings").toMap().value("Rotaries").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = presetMap.value("ComponentSettings").toMap();
                insertionMap[1].insert("Rotaries", insertionMap[0]);

                //insertionMap[2] = presetMap;
                presetMap.insert("ComponentSettings", insertionMap[1]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[2]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in PRESET. Inserted. ****";
            }
            mapIterator++;
        }

        //----------------------
        tempDeletionMap = presetMap.value("ComponentSettings").toMap().value("Rotaries").toMap();
        mapIterator = tempDeletionMap.constBegin();

        while(mapIterator != tempDeletionMap.constEnd()){

            for(int i = 0; i<5; i++){
                deletionMap[i].clear();
            }

            if(!defaultRotariesMap.contains(mapIterator.key())){

                deletionMap[0] = presetMap.value("ComponentSettings").toMap().value("Rotaries").toMap();
                deletionMap[0].remove(mapIterator.key());

                deletionMap[1] = presetMap.value("ComponentSettings").toMap();
                deletionMap[1].insert("Rotaries", deletionMap[0]);

                //insertionMap[2] = presetMap;
                presetMap.insert("ComponentSettings", deletionMap[1]);

                //presetMap.insert(QString("Preset %1").arg(presetNum),insertionMap[2]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in Defulat Map. Deleted. ****";

            }
            mapIterator++;
        }

        //Rotary0///////////////////////////////////////////////////////////
        qDebug() << "Rotary 0";
        mapIterator = defaultRotary0Map.constBegin();

        while(mapIterator != defaultRotary0Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(presetMap.
                    value("ComponentSettings").toMap().
                    value("Rotaries").toMap().
                    value("Rotary0").toMap().contains(mapIterator.key())){

                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {
                insertionMap[0] = presetMap.value("ComponentSettings").toMap().value("Rotaries").toMap().value("Rotary0").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = presetMap.value("ComponentSettings").toMap().value("Rotaries").toMap();
                insertionMap[1].insert("Rotary0", insertionMap[0]);

                insertionMap[2] = presetMap.value("ComponentSettings").toMap();
                insertionMap[2].insert("Rotaries", insertionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", insertionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in PRESET. Inserted. ****";
            }
            mapIterator++;
        }

        //----------------------
        tempDeletionMap = presetMap.value("ComponentSettings").toMap().value("Rotaries").toMap().value("Rotary0").toMap();
        mapIterator = tempDeletionMap.constBegin();

        while(mapIterator != tempDeletionMap.constEnd()){

            for(int i = 0; i<5; i++){
                deletionMap[i].clear();
            }

            if(!defaultRotary0Map.contains(mapIterator.key())){

                deletionMap[0] = presetMap.value("ComponentSettings").toMap().value("Rotaries").toMap().value("Rotary0").toMap();
                deletionMap[0].remove(mapIterator.key());

                deletionMap[1] = presetMap.value("ComponentSettings").toMap().value("Rotaries").toMap();
                deletionMap[1].insert("Rotary0", deletionMap[0]);

                deletionMap[2] = presetMap.value("ComponentSettings").toMap();
                deletionMap[2].insert("Rotaries", deletionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", deletionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in Defulat Map. Deleted. ****";
            }
            mapIterator++;
        }

        //Rotary1///////////////////////////////////////////////////////////
        qDebug() << "Rotary 1";
        mapIterator = defaultRotary1Map.constBegin();

        while(mapIterator != defaultRotary1Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(presetMap.
                    value("ComponentSettings").toMap().
                    value("Rotaries").toMap().
                    value("Rotary1").toMap().contains(mapIterator.key())){

                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {
                insertionMap[0] = presetMap.value("ComponentSettings").toMap().value("Rotaries").toMap().value("Rotary1").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = presetMap.value("ComponentSettings").toMap().value("Rotaries").toMap();
                insertionMap[1].insert("Rotary1", insertionMap[0]);

                insertionMap[2] = presetMap.value("ComponentSettings").toMap();
                insertionMap[2].insert("Rotaries", insertionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", insertionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in PRESET. Inserted. ****";
            }
            mapIterator++;
        }

        //----------------------
        tempDeletionMap = presetMap.value("ComponentSettings").toMap().value("Rotaries").toMap().value("Rotary1").toMap();
        mapIterator = tempDeletionMap.constBegin();

        while(mapIterator != tempDeletionMap.constEnd()){

            for(int i = 0; i<5; i++){
                deletionMap[i].clear();
            }

            if(!defaultRotary1Map.contains(mapIterator.key())){

                deletionMap[0] = presetMap.value("ComponentSettings").toMap().value("Rotaries").toMap().value("Rotary1").toMap();
                deletionMap[0].remove(mapIterator.key());

                deletionMap[1] = presetMap.value("ComponentSettings").toMap().value("Rotaries").toMap();
                deletionMap[1].insert("Rotary1", deletionMap[0]);

                deletionMap[2] = presetMap.value("ComponentSettings").toMap();
                deletionMap[2].insert("Rotaries", deletionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", deletionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in Defulat Map. Deleted. ****";
            }
            mapIterator++;
        }


        /***************************************************************************/
        /**************************** Transport Buttons ****************************/
        /***************************************************************************/
        //Transport Globals//////////////////////////////////////////////////////////
        qDebug() << "Transport Globals";
        mapIterator = defaultTransportButtonsMap.constBegin();

        while(mapIterator != defaultTransportButtonsMap.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(presetMap.
                    value("ComponentSettings").toMap().
                    value("TransportButtons").toMap().contains(mapIterator.key())){
            } else {
                insertionMap[0] = presetMap.value("ComponentSettings").toMap().value("TransportButtons").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = presetMap.value("ComponentSettings").toMap();
                insertionMap[1].insert("TransportButtons", insertionMap[0]);

                //insertionMap[2] = presetMap;
                presetMap.insert("ComponentSettings", insertionMap[1]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[2]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in PRESET. Inserted. ****";
            }
            mapIterator++;
        }

        //----------------------
        tempDeletionMap = presetMap.value("ComponentSettings").toMap().value("TransportButtons").toMap();
        mapIterator = tempDeletionMap.constBegin();

        while(mapIterator != tempDeletionMap.constEnd()){

            for(int i = 0; i<5; i++){
                deletionMap[i].clear();
            }

            if(!defaultTransportButtonsMap.contains(mapIterator.key())){

                deletionMap[0] = presetMap.value("ComponentSettings").toMap().value("TransportButtons").toMap();
                deletionMap[0].remove(mapIterator.key());

                deletionMap[1] = presetMap.value("ComponentSettings").toMap();
                deletionMap[1].insert("TransportButtons", deletionMap[0]);

                //insertionMap[2] = presetMap;
                presetMap.insert("ComponentSettings", deletionMap[1]);

                //presetMap.insert(QString("Preset %1").arg(presetNum),insertionMap[2]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in Defulat Map. Deleted. ****";

            }
            mapIterator++;
        }

        //TransportButton0///////////////////////////////////////////////////////////
        qDebug() << "Transport 0";
        mapIterator = defaultTransportButton0Map.constBegin();

        while(mapIterator != defaultTransportButton0Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(presetMap.
                    value("ComponentSettings").toMap().
                    value("TransportButtons").toMap().
                    value("TransportButton0").toMap().contains(mapIterator.key())){

                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {
                insertionMap[0] = presetMap.value("ComponentSettings").toMap().value("TransportButtons").toMap().value("TransportButton0").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = presetMap.value("ComponentSettings").toMap().value("TransportButtons").toMap();
                insertionMap[1].insert("TransportButton0", insertionMap[0]);

                insertionMap[2] = presetMap.value("ComponentSettings").toMap();
                insertionMap[2].insert("TransportButtons", insertionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", insertionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in PRESET. Inserted. ****";
            }
            mapIterator++;
        }

        //----------------------
        tempDeletionMap = presetMap.value("ComponentSettings").toMap().value("TransportButtons").toMap().value("TransportButton0").toMap();
        mapIterator = tempDeletionMap.constBegin();

        while(mapIterator != tempDeletionMap.constEnd()){

            for(int i = 0; i<5; i++){
                deletionMap[i].clear();
            }

            if(!defaultTransportButton0Map.contains(mapIterator.key())){

                deletionMap[0] = presetMap.value("ComponentSettings").toMap().value("TransportButtons").toMap().value("TransportButton0").toMap();
                deletionMap[0].remove(mapIterator.key());

                deletionMap[1] = presetMap.value("ComponentSettings").toMap().value("TransportButtons").toMap();
                deletionMap[1].insert("TransportButton0", deletionMap[0]);

                deletionMap[2] = presetMap.value("ComponentSettings").toMap();
                deletionMap[2].insert("TransportButtons", deletionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", deletionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in Defulat Map. Deleted. ****";
            }
            mapIterator++;
        }

        //TransportButton1///////////////////////////////////////////////////////////
        qDebug() << "Transport 1";
        mapIterator = defaultTransportButton1Map.constBegin();

        while(mapIterator != defaultTransportButton1Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(presetMap.
                    value("ComponentSettings").toMap().
                    value("TransportButtons").toMap().
                    value("TransportButton1").toMap().contains(mapIterator.key())){

                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {
                insertionMap[0] = presetMap.value("ComponentSettings").toMap().value("TransportButtons").toMap().value("TransportButton1").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = presetMap.value("ComponentSettings").toMap().value("TransportButtons").toMap();
                insertionMap[1].insert("TransportButton1", insertionMap[0]);

                insertionMap[2] = presetMap.value("ComponentSettings").toMap();
                insertionMap[2].insert("TransportButtons", insertionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", insertionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in PRESET. Inserted. ****";
            }
            mapIterator++;
        }

        //----------------------
        tempDeletionMap = presetMap.value("ComponentSettings").toMap().value("TransportButtons").toMap().value("TransportButton1").toMap();
        mapIterator = tempDeletionMap.constBegin();

        while(mapIterator != tempDeletionMap.constEnd()){

            for(int i = 0; i<5; i++){
                deletionMap[i].clear();
            }

            if(!defaultTransportButton1Map.contains(mapIterator.key())){

                deletionMap[0] = presetMap.value("ComponentSettings").toMap().value("TransportButtons").toMap().value("TransportButton1").toMap();
                deletionMap[0].remove(mapIterator.key());

                deletionMap[1] = presetMap.value("ComponentSettings").toMap().value("TransportButtons").toMap();
                deletionMap[1].insert("TransportButton1", deletionMap[0]);

                deletionMap[2] = presetMap.value("ComponentSettings").toMap();
                deletionMap[2].insert("TransportButtons", deletionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", deletionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in Defulat Map. Deleted. ****";
            }
            mapIterator++;
        }

        //TransportButton2///////////////////////////////////////////////////////////
        qDebug() << "Transport 2";
        mapIterator = defaultTransportButton2Map.constBegin();

        while(mapIterator != defaultTransportButton2Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(presetMap.
                    value("ComponentSettings").toMap().
                    value("TransportButtons").toMap().
                    value("TransportButton2").toMap().contains(mapIterator.key())){

                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {
                insertionMap[0] = presetMap.value("ComponentSettings").toMap().value("TransportButtons").toMap().value("TransportButton2").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = presetMap.value("ComponentSettings").toMap().value("TransportButtons").toMap();
                insertionMap[1].insert("TransportButton2", insertionMap[0]);

                insertionMap[2] = presetMap.value("ComponentSettings").toMap();
                insertionMap[2].insert("TransportButtons", insertionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", insertionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in PRESET. Inserted. ****";
            }
            mapIterator++;
        }

        //----------------------
        tempDeletionMap = presetMap.value("ComponentSettings").toMap().value("TransportButtons").toMap().value("TransportButton2").toMap();
        mapIterator = tempDeletionMap.constBegin();

        while(mapIterator != tempDeletionMap.constEnd()){

            for(int i = 0; i<5; i++){
                deletionMap[i].clear();
            }

            if(!defaultTransportButton2Map.contains(mapIterator.key())){

                deletionMap[0] = presetMap.value("ComponentSettings").toMap().value("TransportButtons").toMap().value("TransportButton2").toMap();
                deletionMap[0].remove(mapIterator.key());

                deletionMap[1] = presetMap.value("ComponentSettings").toMap().value("TransportButtons").toMap();
                deletionMap[1].insert("TransportButton2", deletionMap[0]);

                deletionMap[2] = presetMap.value("ComponentSettings").toMap();
                deletionMap[2].insert("TransportButtons", deletionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", deletionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in Defulat Map. Deleted. ****";
            }
            mapIterator++;
        }


        /***************************************************************************/
        /**************************** UpDown Buttons *******************************/
        /***************************************************************************/
        //UpDownButtons Globals//////////////////////////////////////////////////////
        qDebug() << "UpDpown Globals";
        mapIterator = defaultUpDownButtonsMap.constBegin();

        while(mapIterator != defaultUpDownButtonsMap.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(presetMap.
                    value("ComponentSettings").toMap().
                    value("UpDownButtons").toMap().contains(mapIterator.key())){
                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {
                insertionMap[0] = presetMap.value("ComponentSettings").toMap().value("UpDownButtons").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = presetMap.value("ComponentSettings").toMap();
                insertionMap[1].insert("UpDownButtons", insertionMap[0]);

                //insertionMap[2] = presetMap;
                presetMap.insert("ComponentSettings", insertionMap[1]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[2]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in PRESET. Inserted. ****";
            }
            mapIterator++;
        }

        //----------------------
        tempDeletionMap = presetMap.value("ComponentSettings").toMap().value("UpDownButtons").toMap();
        mapIterator = tempDeletionMap.constBegin();

        while(mapIterator != tempDeletionMap.constEnd()){

            for(int i = 0; i<5; i++){
                deletionMap[i].clear();
            }

            if(!defaultUpDownButtonsMap.contains(mapIterator.key())){

                deletionMap[0] = presetMap.value("ComponentSettings").toMap().value("UpDownButtons").toMap();
                deletionMap[0].remove(mapIterator.key());

                deletionMap[1] = presetMap.value("ComponentSettings").toMap();
                deletionMap[1].insert("UpDownButtons", deletionMap[0]);

                //insertionMap[2] = presetMap;
                presetMap.insert("ComponentSettings", deletionMap[1]);

                //presetMap.insert(QString("Preset %1").arg(presetNum),insertionMap[2]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in Defulat Map. Deleted. ****";

            }
            mapIterator++;
        }

        //UpDownButton0//////////////////////////////////////////////////////
        qDebug() << "UpDpown 0";
        mapIterator = defaultUpDownButton0Map.constBegin();

        while(mapIterator != defaultUpDownButton0Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(presetMap.
                    value("ComponentSettings").toMap().
                    value("UpDownButtons").toMap().
                    value("UpDownButton0").toMap().contains(mapIterator.key())){

                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {
                insertionMap[0] = presetMap.value("ComponentSettings").toMap().value("UpDownButtons").toMap().value("UpDownButton0").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = presetMap.value("ComponentSettings").toMap().value("UpDownButtons").toMap();
                insertionMap[1].insert("UpDownButton0", insertionMap[0]);

                insertionMap[2] = presetMap.value("ComponentSettings").toMap();
                insertionMap[2].insert("UpDownButtons", insertionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", insertionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in PRESET. Inserted. ****";
            }

            mapIterator++;
        }

        //----------------------
        tempDeletionMap = presetMap.value("ComponentSettings").toMap().value("UpDownButtons").toMap().value("UpDownButton0").toMap();
        mapIterator = tempDeletionMap.constBegin();

        while(mapIterator != tempDeletionMap.constEnd()){

            for(int i = 0; i<5; i++){
                deletionMap[i].clear();
            }

            if(!defaultUpDownButton0Map.contains(mapIterator.key())){

                deletionMap[0] = presetMap.value("ComponentSettings").toMap().value("UpDownButtons").toMap().value("UpDownButton0").toMap();
                deletionMap[0].remove(mapIterator.key());

                deletionMap[1] = presetMap.value("ComponentSettings").toMap().value("UpDownButtons").toMap();
                deletionMap[1].insert("UpDownButton0", deletionMap[0]);

                deletionMap[2] = presetMap.value("ComponentSettings").toMap();
                deletionMap[2].insert("UpDownButtons", deletionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", deletionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in Defulat Map. Deleted. ****";
            }
            mapIterator++;
        }

        //UpDownButton1//////////////////////////////////////////////////////
        qDebug() << "UpDpown 1";
        mapIterator = defaultUpDownButton1Map.constBegin();

        while(mapIterator != defaultUpDownButton1Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(presetMap.
                    value("ComponentSettings").toMap().
                    value("UpDownButtons").toMap().
                    value("UpDownButton1").toMap().contains(mapIterator.key())){

                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {
                insertionMap[0] = presetMap.value("ComponentSettings").toMap().value("UpDownButtons").toMap().value("UpDownButton1").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = presetMap.value("ComponentSettings").toMap().value("UpDownButtons").toMap();
                insertionMap[1].insert("UpDownButton1", insertionMap[0]);

                insertionMap[2] = presetMap.value("ComponentSettings").toMap();
                insertionMap[2].insert("UpDownButtons", insertionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", insertionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in PRESET. Inserted. ****";
            }

            mapIterator++;
        }

        //----------------------
        tempDeletionMap = presetMap.value("ComponentSettings").toMap().value("UpDownButtons").toMap().value("UpDownButton1").toMap();
        mapIterator = tempDeletionMap.constBegin();

        while(mapIterator != tempDeletionMap.constEnd()){

            for(int i = 0; i<5; i++){
                deletionMap[i].clear();
            }

            if(!defaultUpDownButton1Map.contains(mapIterator.key())){

                deletionMap[0] = presetMap.value("ComponentSettings").toMap().value("UpDownButtons").toMap().value("UpDownButton1").toMap();
                deletionMap[0].remove(mapIterator.key());

                deletionMap[1] = presetMap.value("ComponentSettings").toMap().value("UpDownButtons").toMap();
                deletionMap[1].insert("UpDownButton1", deletionMap[0]);

                deletionMap[2] = presetMap.value("ComponentSettings").toMap();
                deletionMap[2].insert("UpDownButtons", deletionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", deletionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in Defulat Map. Deleted. ****";
            }
            mapIterator++;
        }


        /***************************************************************************/
        /******************************** VSliders *********************************/
        /***************************************************************************/
        //VSliders Globals//////////////////////////////////////////////////////
        qDebug() << "VSlider Globals";
        mapIterator = defaultVSlidersMap.constBegin();

        while(mapIterator != defaultVSlidersMap.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(presetMap.
                    value("ComponentSettings").toMap().
                    value("VSliders").toMap().contains(mapIterator.key())){

                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {
                insertionMap[0] = presetMap.value("ComponentSettings").toMap().value("VSliders").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = presetMap.value("ComponentSettings").toMap();
                insertionMap[1].insert("VSliders", insertionMap[0]);

                //insertionMap[2] = presetMap;
                presetMap.insert("ComponentSettings", insertionMap[1]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[2]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in PRESET. Inserted. ****";
            }

            mapIterator++;
        }

        //----------------------
        tempDeletionMap = presetMap.value("ComponentSettings").toMap().value("VSliders").toMap();
        mapIterator = tempDeletionMap.constBegin();

        while(mapIterator != tempDeletionMap.constEnd()){

            for(int i = 0; i<5; i++){
                deletionMap[i].clear();
            }

            if(!defaultVSlidersMap.contains(mapIterator.key())){

                deletionMap[0] = presetMap.value("ComponentSettings").toMap().value("VSliders").toMap();
                deletionMap[0].remove(mapIterator.key());

                deletionMap[1] = presetMap.value("ComponentSettings").toMap();
                deletionMap[1].insert("VSliders", deletionMap[0]);

                //insertionMap[2] = presetMap;
                presetMap.insert("ComponentSettings", deletionMap[1]);

                //presetMap.insert(QString("Preset %1").arg(presetNum),insertionMap[2]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in Defulat Map. Deleted. ****";

            }
            mapIterator++;
        }

        //VSlider0//////////////////////////////////////////////////////
        qDebug() << "VSlider 0";
        mapIterator = defaultVSlider0Map.constBegin();

        while(mapIterator != defaultVSlider0Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(presetMap.
                    value("ComponentSettings").toMap().
                    value("VSliders").toMap().
                    value("VSlider0").toMap().contains(mapIterator.key())){

            } else {
                insertionMap[0] = presetMap.value("ComponentSettings").toMap().value("VSliders").toMap().value("VSlider0").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = presetMap.value("ComponentSettings").toMap().value("VSliders").toMap();
                insertionMap[1].insert("VSlider0", insertionMap[0]);

                insertionMap[2] = presetMap.value("ComponentSettings").toMap();
                insertionMap[2].insert("VSliders", insertionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", insertionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in PRESET. Inserted. ****";
            }

            mapIterator++;
        }

        //----------------------
        tempDeletionMap = presetMap.value("ComponentSettings").toMap().value("VSliders").toMap().value("VSlider0").toMap();
        mapIterator = tempDeletionMap.constBegin();

        while(mapIterator != tempDeletionMap.constEnd()){

            for(int i = 0; i<5; i++){
                deletionMap[i].clear();
            }

            if(!defaultVSlider0Map.contains(mapIterator.key())){

                deletionMap[0] = presetMap.value("ComponentSettings").toMap().value("VSliders").toMap().value("VSlider0").toMap();
                deletionMap[0].remove(mapIterator.key());

                deletionMap[1] = presetMap.value("ComponentSettings").toMap().value("VSliders").toMap();
                deletionMap[1].insert("VSlider0", deletionMap[0]);

                deletionMap[2] = presetMap.value("ComponentSettings").toMap();
                deletionMap[2].insert("VSliders", deletionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", deletionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in Defulat Map. Deleted. ****";
            }
            mapIterator++;
        }

        //VSlider1//////////////////////////////////////////////////////
        qDebug() << "VSlider 1";
        mapIterator = defaultVSlider1Map.constBegin();

        while(mapIterator != defaultVSlider1Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(presetMap.
                    value("ComponentSettings").toMap().
                    value("VSliders").toMap().
                    value("VSlider1").toMap().contains(mapIterator.key())){
                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {
                insertionMap[0] = presetMap.value("ComponentSettings").toMap().value("VSliders").toMap().value("VSlider1").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = presetMap.value("ComponentSettings").toMap().value("VSliders").toMap();
                insertionMap[1].insert("VSlider1", insertionMap[0]);

                insertionMap[2] = presetMap.value("ComponentSettings").toMap();
                insertionMap[2].insert("VSliders", insertionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", insertionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in PRESET. Inserted. ****";
            }

            mapIterator++;
        }

        //----------------------
        tempDeletionMap = presetMap.value("ComponentSettings").toMap().value("VSliders").toMap().value("VSlider1").toMap();
        mapIterator = tempDeletionMap.constBegin();

        while(mapIterator != tempDeletionMap.constEnd()){

            for(int i = 0; i<5; i++){
                deletionMap[i].clear();
            }

            if(!defaultVSlider1Map.contains(mapIterator.key())){

                deletionMap[0] = presetMap.value("ComponentSettings").toMap().value("VSliders").toMap().value("VSlider1").toMap();
                deletionMap[0].remove(mapIterator.key());

                deletionMap[1] = presetMap.value("ComponentSettings").toMap().value("VSliders").toMap();
                deletionMap[1].insert("VSlider1", deletionMap[0]);

                deletionMap[2] = presetMap.value("ComponentSettings").toMap();
                deletionMap[2].insert("VSliders", deletionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", deletionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in Defulat Map. Deleted. ****";
            }
            mapIterator++;
        }

        //VSlider2//////////////////////////////////////////////////////
        qDebug() << "VSlider 2";
        mapIterator = defaultVSlider2Map.constBegin();

        while(mapIterator != defaultVSlider2Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(presetMap.
                    value("ComponentSettings").toMap().
                    value("VSliders").toMap().
                    value("VSlider2").toMap().contains(mapIterator.key())){
                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {
                insertionMap[0] = presetMap.value("ComponentSettings").toMap().value("VSliders").toMap().value("VSlider2").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = presetMap.value("ComponentSettings").toMap().value("VSliders").toMap();
                insertionMap[1].insert("VSlider2", insertionMap[0]);

                insertionMap[2] = presetMap.value("ComponentSettings").toMap();
                insertionMap[2].insert("VSliders", insertionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", insertionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in PRESET. Inserted. ****";
            }

            mapIterator++;
        }

        //----------------------
        tempDeletionMap = presetMap.value("ComponentSettings").toMap().value("VSliders").toMap().value("VSlider2").toMap();
        mapIterator = tempDeletionMap.constBegin();

        while(mapIterator != tempDeletionMap.constEnd()){

            for(int i = 0; i<5; i++){
                deletionMap[i].clear();
            }

            if(!defaultVSlider2Map.contains(mapIterator.key())){

                deletionMap[0] = presetMap.value("ComponentSettings").toMap().value("VSliders").toMap().value("VSlider2").toMap();
                deletionMap[0].remove(mapIterator.key());

                deletionMap[1] = presetMap.value("ComponentSettings").toMap().value("VSliders").toMap();
                deletionMap[1].insert("VSlider2", deletionMap[0]);

                deletionMap[2] = presetMap.value("ComponentSettings").toMap();
                deletionMap[2].insert("VSliders", deletionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", deletionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in Defulat Map. Deleted. ****";
            }
            mapIterator++;
        }

        //VSlider3//////////////////////////////////////////////////////
        qDebug() << "VSlider 3";
        mapIterator = defaultVSlider3Map.constBegin();

        while(mapIterator != defaultVSlider3Map.constEnd()){

            for(int i = 0; i<5; i++){
                insertionMap[i].clear();
            }

            if(presetMap.
                    value("ComponentSettings").toMap().
                    value("VSliders").toMap().
                    value("VSlider3").toMap().contains(mapIterator.key())){
                //qDebug() << mapIterator.key() << "FOUND IN JSON";
            } else {
                insertionMap[0] = presetMap.value("ComponentSettings").toMap().value("VSliders").toMap().value("VSlider3").toMap();
                insertionMap[0].insert(mapIterator.key(), mapIterator.value());

                insertionMap[1] = presetMap.value("ComponentSettings").toMap().value("VSliders").toMap();
                insertionMap[1].insert("VSlider3", insertionMap[0]);

                insertionMap[2] = presetMap.value("ComponentSettings").toMap();
                insertionMap[2].insert("VSliders", insertionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", insertionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in PRESET. Inserted. ****";
            }

            mapIterator++;
        }

        //----------------------
        tempDeletionMap = presetMap.value("ComponentSettings").toMap().value("VSliders").toMap().value("VSlider3").toMap();
        mapIterator = tempDeletionMap.constBegin();

        while(mapIterator != tempDeletionMap.constEnd()){

            for(int i = 0; i<5; i++){
                deletionMap[i].clear();
            }

            if(!defaultVSlider3Map.contains(mapIterator.key())){

                deletionMap[0] = presetMap.value("ComponentSettings").toMap().value("VSliders").toMap().value("VSlider3").toMap();
                deletionMap[0].remove(mapIterator.key());

                deletionMap[1] = presetMap.value("ComponentSettings").toMap().value("VSliders").toMap();
                deletionMap[1].insert("VSlider3", deletionMap[0]);

                deletionMap[2] = presetMap.value("ComponentSettings").toMap();
                deletionMap[2].insert("VSliders", deletionMap[1]);

                //insertionMap[3] = presetMap;
                presetMap.insert("ComponentSettings", deletionMap[2]);

                //jsonQuNeoPresetsMap.insert(QString("Preset %1").arg(presetNum),insertionMap[3]);

                qDebug() << mapIterator.key() << "**** NOT FOUND in Defulat Map. Deleted. ****";
            }
            mapIterator++;
        }

    }


    return presetMap;
}
