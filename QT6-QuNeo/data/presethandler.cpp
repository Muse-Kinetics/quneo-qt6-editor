#include "presethandler.h"
#include <QDebug>

PresetHandler::PresetHandler(QWidget *widget,QObject *parent) :
    QObject(parent)
{
    mainWindow = widget;
    saveButton = mainWindow->findChild<QPushButton *>("saveButton");
    revertButton = mainWindow->findChild<QPushButton *>("revertButton");
    presetMenu = mainWindow->findChild<QComboBox *>("presetMenu");
    editPanes = mainWindow->findChild<QStackedWidget *>("EditPanes");
    editPanes->hide();
    alreadyShown = false;
    currentPresetModified = false;
    alreadyOn = false;
    connect(mainWindow, SIGNAL(signalEvents(QString)), this, SLOT(slotEvents(QString)));
    connect(presetMenu, SIGNAL(activated(QString)), this, SLOT(slotRecallPreset(QString)));
    connect(revertButton, SIGNAL(clicked()), this, SLOT(slotRevertPreset()));
    x = 0;

    /***********************************Load and Parse JSON***********************************/

    QString jsonPath = QCoreApplication::applicationDirPath(); //get bundle path

#if defined(Q_OS_MAC) && !defined(QT_DEBUG)
    jsonPath.remove(jsonPath.length() - 5, jsonPath.length());
    jsonPath.append("Resources/presets/QuNeo.json");
#else
    jsonPath = QString("../../../presets/QuNeo.json");
#endif

    //load json into QFile
    jsonFile = new QFile(jsonPath);

    if(jsonFile->open(QIODevice::ReadWrite | QIODevice::Text)){
        //qDebug("jsonFound");
    }

    //load json file into a byte array to be processd by the parser
    jsonByteArray = jsonFile->readAll();

    //parse the json data, convert it to a map and set it equal to the master jsonMap
    jsonMasterMap = parser.parse(jsonByteArray, &ok).toMap();

    //store a map of the 16 preset maps
    presetMaps = jsonMasterMap["QuNeo Presets"].toMap();
    presetMapsCopy = jsonMasterMap["QuNeo Presets"].toMap();

    //instantiate pad edit pane, passing preset map and main window address, link value changed signal/slots
    padEditPane = new PadEditPane(&presetMaps, &presetMapsCopy, mainWindow, 0);
    connect(padEditPane, SIGNAL(signalValueChanged(int,QString,QString,int,QString, int)),
            this, SLOT(slotValueChanged(int,QString,QString,int,QString, int)));//connect info about value to be saved to the preset handler when one changes.
    connect(padEditPane, SIGNAL(signalToLabels(QString)), this, SLOT(slotDisplayAllLabels(QString)));
    connect(padEditPane, SIGNAL(signalTableChanged(int,QString,QList<QVariant>)),
            this, SLOT(slotPadVelocityTableChanged(int,QString,QList<QVariant>)));
    connect(padEditPane, SIGNAL(signalSelectPreset(QString)), this, SLOT(slotRecallPreset(QString)));

    hSliderEditPane = new HSliderEditPane(&presetMaps, &presetMapsCopy, mainWindow, 0);
    connect(hSliderEditPane, SIGNAL(signalValueChanged(int,QString,QString,int,QString,int)),
            this, SLOT(slotValueChanged(int,QString,QString,int,QString,int)));
    connect(hSliderEditPane, SIGNAL(signalToLabels(QString)), this, SLOT(slotDisplayAllLabels(QString)));

    vSliderEditPane = new VSliderEditPane(&presetMaps, &presetMapsCopy, mainWindow, 0);
    connect(vSliderEditPane, SIGNAL(signalValueChanged(int,QString,QString,int,QString,int)),
            this, SLOT(slotValueChanged(int,QString,QString,int,QString,int)));
    connect(vSliderEditPane, SIGNAL(signalToLabels(QString)), this, SLOT(slotDisplayAllLabels(QString)));

    rotaryEditPane = new RotaryEditPane(&presetMaps, &presetMapsCopy, mainWindow, 0);
    connect(rotaryEditPane, SIGNAL(signalValueChanged(int,QString,QString,int,QString,int)),
            this, SLOT(slotValueChanged(int,QString,QString,int,QString,int)));
    connect(rotaryEditPane, SIGNAL(signalToLabels(QString)), this, SLOT(slotDisplayAllLabels(QString)));

    lSliderEditPane = new LSliderEditPane(&presetMaps, &presetMapsCopy, mainWindow, 0);
    connect(lSliderEditPane, SIGNAL(signalValueChanged(int,QString,QString,int,QString,int)),
            this, SLOT(slotValueChanged(int,QString,QString,int,QString,int)));
    connect(lSliderEditPane, SIGNAL(signalToLabels(QString)), this, SLOT(slotDisplayAllLabels(QString)));

    transportEditPane = new TransportEditPane(&presetMaps, &presetMapsCopy, mainWindow, 0);
    connect(transportEditPane, SIGNAL(signalValueChanged(int,QString,QString,int,QString,int)),
            this, SLOT(slotValueChanged(int,QString,QString,int,QString,int)));
    connect(transportEditPane, SIGNAL(signalToLabels(QString)), this, SLOT(slotDisplayAllLabels(QString)));

    // modeEditPane = new ModeEditPane(&presetMaps, &presetMapsCopy, mainWindow, 0);
    // connect(modeEditPane, SIGNAL(signalValueChanged(int,QString,QString,int,QString,int)),
    //this, SLOT(slotValueChanged(int,QString,QString,int,QString,int)));

    leftrightEditPane = new LeftrightEditPane(&presetMaps, &presetMapsCopy, mainWindow, 0);
    connect(leftrightEditPane, SIGNAL(signalValueChanged(int,QString,QString,int,QString,int)),
            this, SLOT(slotValueChanged(int,QString,QString,int,QString,int)));
    connect(leftrightEditPane, SIGNAL(signalToLabels(QString)), this, SLOT(slotDisplayAllLabels(QString)));

    rhombusEditPane = new RhombusEditPane(&presetMaps, &presetMapsCopy, mainWindow, 0);
    connect(rhombusEditPane, SIGNAL(signalValueChanged(int,QString,QString,int,QString,int)),
            this, SLOT(slotValueChanged(int,QString,QString,int,QString,int)));
    connect(rhombusEditPane, SIGNAL(signalToLabels(QString)), this, SLOT(slotDisplayAllLabels(QString)));

    updownEditPane = new UpdownEditPane(&presetMaps, &presetMapsCopy, mainWindow, 0);
    connect(updownEditPane, SIGNAL(signalValueChanged(int,QString,QString,int,QString,int)),
            this, SLOT(slotValueChanged(int,QString,QString,int,QString,int)));
    connect(updownEditPane, SIGNAL(signalToLabels(QString)), this, SLOT(slotDisplayAllLabels(QString)));

    globalParameters = new GlobalParameters(&presetMaps, &presetMapsCopy, mainWindow, 0);
    connect(globalParameters, SIGNAL(signalPresetNameChanged(QString)),
            this,SLOT(slotPresetNameChanged(QString)));

    updateIndicator = new UpdateIndicator(mainWindow, &presetMaps, &presetMapsCopy, 0);
    connect(this, SIGNAL(signalPresetModified(bool)), updateIndicator, SLOT(slotPresetModified(bool)));

    currentPreset = 0;
}

void PresetHandler::slotRecallPreset(QString selected){
    qDebug() << "slotRecallPreset() called: " << selected;
    if(selected.contains("Preset ")){ //remove prepended preset
        /* // EB Removing this asterisk to test if this causes the crash
        if(selected.contains(" *")){    //remove asterisk if recalling unsaved/modified preset
            selected.remove(" *");
        }
        */

        currentPreset = selected.remove(0,7).toInt() - 1;

        qDebug() << "currentPresest:" << currentPreset;

        disconnect(this, SIGNAL(signalPresetModified(bool)), updateIndicator, SLOT(slotPresetModified(bool)));
        slotConnectDisconnectDisplayAllLabels(false);
        padEditPane->slotRecallPreset(currentPreset);
        rotaryEditPane->slotRecallPreset(currentPreset);
        hSliderEditPane->slotRecallPreset(currentPreset);
        vSliderEditPane->slotRecallPreset(currentPreset);
        lSliderEditPane->slotRecallPreset(currentPreset);
        transportEditPane->slotRecallPreset(currentPreset);
        leftrightEditPane->slotRecallPreset(currentPreset);
        //modeEditPane->slotRecallPreset(currentPreset);
        rhombusEditPane->slotRecallPreset(currentPreset);
        updownEditPane->slotRecallPreset(currentPreset);
        globalParameters->slotRecallPreset(currentPreset);
        updateIndicator->slotCheckThisPreset(currentPreset);
        slotConnectDisconnectDisplayAllLabels(true);
        connect(this, SIGNAL(signalPresetModified(bool)), updateIndicator, SLOT(slotPresetModified(bool)));
        slotCheckPresets();
    }
    qDebug() << "recall called";
}

//to revert presets this function inserts the presetMaps values from the current preset into that preset slot of presetMapsCopy
void PresetHandler::slotRevertPreset() {

    presetMapsCopy.insert(QString("Preset %1").arg(currentPreset), presetMaps.value(QString("Preset %1").arg(currentPreset)).toMap());
    slotRecallPreset(QString("Preset %1").arg(currentPreset + 1));
    slotCheckPresets();
    qDebug() << "slotRevertPreset() called";
}

void PresetHandler::slotSave(){

    //presetMaps.insert(QString("Preset %1").arg(currentPreset), presetNumMap);//*
    slotPresetNameValidator();

    presetMaps.insert(QString("Preset %1").arg(currentPreset), presetMapsCopy.value(QString("Preset %1").arg(currentPreset)));





    jsonMasterMap.insert("QuNeo Presets", presetMaps);

    jsonByteArray = serializer.serialize(jsonMasterMap);//serialize the master json map into the byte array
    jsonFile->resize(0);//clear jsonFile (set to 0 byte size)
    jsonFile->write(jsonByteArray);//write json byte array to file

    emit signalPresetModified(false);
    //slotCheckPresets();
    slotRecallPreset(QString("Preset %1").arg(currentPreset + 1));
}

void PresetHandler::slotEvents(QString string){
    if(string.contains("Button")){
        slotConnectDisconnectDisplayAllLabels(false);}
    //qDebug()<<QString("slotEvents string: %1").arg(string);
    if(!alreadyShown){
        if(string.contains("Button")){
            editPanes->show();
            alreadyShown = true;
        }}

}

void PresetHandler::slotPropogateValuesSelectMultiple(int presetNum, QString settingsType, QString paramType, int typeNum, QString paramName, int paramValue){

    //query clickedSensors map to see if each sensor has been highlighted.
    //If it has beeen then insert incoming parameter.
    for(int i = 0; i < sensorList.count(); i++){
        int sensorClicked = clickedSensors.value(QString(sensorList.at(i)));
        //Appending an s in the code in the next section doesn't work for Rotary because of the stupid English language.  This little bit accounts for that
        QString paramTypeName;

        if(paramType == "Rotary"){
            paramTypeName = "Rotarie";
        }
        else {
            paramTypeName = paramType;
        }
        if(paramName.contains("Channel")){
            int channelParam = paramValue;
            channelParam = (paramValue - 1);
        }

        //account for typenum. since I am counting through all sensors in the sensor list, not just pads, I need to
        //subtract x from i to get the correct typenum.
        //for example, for vsliders x=19 because vsliders come up as 19-22 in the sensorList. subtracting 19 gives vslider0-3.
        int x = 0; //for pads which need no offset.

        if(paramType ==  QString("Rotary")){
            x = 35;
        }
        if(paramType ==  QString("VSlider")){
            x = 20;
        }
        if(paramType ==  QString("HSlider")){
            x = 16;
        }
        if(paramType ==  QString("LongSlider")){
            x = 37;
        }
        if(paramType ==  QString("TransportButton")){
            x = 25;
        }
        if(paramType ==  QString("LeftRightButton")){
            x = 28;
        }
        if(paramType ==  QString("UpDownButton")){
            x = 33;
        }
        if(paramType ==  QString("RhombusButton")){
            x = 32;
        }

        int z = i - x;

        if (sensorClicked){
            //parse JSON parameters
            typeNumMap = presetMapsCopy.value(QString("Preset %1").arg(presetNum)).toMap().
                    value(settingsType).toMap().value(QString("%1s").arg(paramTypeName)).toMap().
                    value(QString("%1%2").arg(paramType).arg(z)).toMap(); //pad#
            paramTypeMap = presetMapsCopy.value(QString("Preset %1").arg(presetNum)).toMap().
                    value(settingsType).toMap().value(QString("%1s").arg(paramTypeName)).toMap();//Pads
            settingsTypeMap = presetMapsCopy.value(QString("Preset %1").arg(presetNum)).toMap().
                    value(settingsType).toMap();//ComponentSettings
            presetNumMap = presetMapsCopy.value(QString("Preset %1").arg(presetNum)).toMap();//Preset#

            //insert the new parameter value into the sensor's typeNumMap and then insert that into the tree of maps.
            typeNumMap.insert(paramName, paramValue);
            paramTypeMap.insert(QString("%1%2").arg(paramType).arg(z), typeNumMap);
            settingsTypeMap.insert(QString("%1s").arg(paramTypeName), paramTypeMap);
            presetNumMap.insert(settingsType, settingsTypeMap);
            presetMapsCopy.insert(QString("Preset %1").arg(presetNum), presetNumMap);
        }
    }

    emit signalPresetModified(true);
    slotCheckPresets();
}

void::PresetHandler::slotValueChanged(int presetNum, QString settingsType, QString paramType, int typeNum,
                                      QString paramName, int paramValue){
    //Appending an s in the code in the next section doesn't work for Rotary because of the stupid English language.  This little bit accounts for that
    QString paramTypeName;

    if(paramType == "Rotary")
    {
        paramTypeName = "Rotarie";
    }
    else
    {
        paramTypeName = paramType;
    }
    if(paramName.contains("Channel"))
    {
        paramValue = (paramValue - 1);
    }

    //fully populate each json stratum with all params, keys, values, etc... otherwise, upon saving, what we have not changed gets erased
    typeNumMap = presetMapsCopy.value(QString("Preset %1").arg(presetNum)).toMap().
            value(settingsType).toMap().value(QString("%1s").arg(paramTypeName)).toMap().
            value(QString("%1%2").arg(paramType).arg(typeNum)).toMap(); //pad#
    paramTypeMap = presetMapsCopy.value(QString("Preset %1").arg(presetNum)).toMap().
            value(settingsType).toMap().value(QString("%1s").arg(paramTypeName)).toMap();//Pads
    settingsTypeMap = presetMapsCopy.value(QString("Preset %1").arg(presetNum)).toMap().
            value(settingsType).toMap();//ComponentSettings
    presetNumMap = presetMapsCopy.value(QString("Preset %1").arg(presetNum)).toMap();//Preset#

    //if a global parameter, or sensitivity
    if(paramName.contains("Sensitivity") ||
            paramName.contains("LocalLEDControl") ||
            paramName.contains("localLEDControl") ||
            paramName.contains("cornerIsolation") ||
            paramName.contains("Threshold") ||
            paramName.contains("Offset") ||
            paramName.contains("TransposeInterval") ||
            paramName.contains("InChannel"))
    {

        //if padSensitivityPerPadUI
        if(paramName == "padSensitivityPerPadUI")
        {
            typeNumMap.insert(paramName, paramValue);
            //qDebug() << "PER PAD UI" << paramValue;

            float scaledPadSensitivity = (float(presetMapsCopy.
                                                value(QString("Preset %1").arg(presetNum)).toMap().
                                                value(settingsType).toMap().value(QString("%1s").arg(paramTypeName)).toMap().
                                                value("padSensitivity").toInt()) / 127.00) + .5;

            float scaledPadSensitivityPerPadUI = ((paramValue + 20) / 40.00) + .5;

            int scaledPadSensitivityPerPad = scaledPadSensitivity * scaledPadSensitivityPerPadUI * 100.00;

            typeNumMap.insert(QString("padSensitivityPerPad"), scaledPadSensitivityPerPad);

            paramTypeMap.insert(QString("%1%2").arg(paramType).arg(typeNum), typeNumMap);
            settingsTypeMap.insert(QString("%1s").arg(paramTypeName), paramTypeMap);
            presetNumMap.insert(settingsType, settingsTypeMap);
            presetMapsCopy.insert(QString("Preset %1").arg(presetNum), presetNumMap);

            //call this slot to send values to other sensors if more than one are selected
            slotPropogateValuesSelectMultiple(presetNum, settingsType, paramType, typeNum,
                                              paramName, paramValue);


            //this will never happen...
        } else if(paramName == "padSensitivityPerPad")
        {
            typeNumMap.insert(paramName, paramValue);
            //qDebug() << "PER PAD SENS" << paramValue;

            //if global pad sensitivity
        } else if (paramName == "padSensitivity")
        {
            // paramTypeMap.insert(paramName, paramValue);
            //qDebug() << "GLOBAL SENS" << paramValue;

            float scaledPadSensitivity = (float(paramValue) / 127.00) + .5;

            for(int i = 0; i < 16; i++)
            {

                //parse JSON parameters fully for sensitivity for each pad
                typeNumMap = presetMapsCopy.value(QString("Preset %1").arg(presetNum)).toMap().
                        value(settingsType).toMap().value(QString("%1s").arg(paramTypeName)).toMap().
                        value(QString("%1%2").arg(paramType).arg(i)).toMap(); //pad#

                paramTypeMap = presetMapsCopy.value(QString("Preset %1").arg(presetNum)).toMap().
                        value(settingsType).toMap().value(QString("%1s").arg(paramTypeName)).toMap();//Pads

                settingsTypeMap = presetMapsCopy.value(QString("Preset %1").arg(presetNum)).toMap().
                        value(settingsType).toMap();//ComponentSettings

                presetNumMap = presetMapsCopy.value(QString("Preset %1").arg(presetNum)).toMap();//Preset#

                //do the scaling
                float scaledPadSensitivityPerPadUI = (float(presetMapsCopy.
                                                            value(QString("Preset %1").arg(presetNum)).toMap().
                                                            value(settingsType).toMap().value(QString("%1s").arg(paramTypeName)).toMap().
                                                            value(QString("%1%2").arg(paramType).arg(i)).toMap().
                                                            value(QString("padSensitivityPerPadUI")).toInt() + 20) / 40.00) + .5;



                int scaledPadSensitivityPerPad = scaledPadSensitivity * scaledPadSensitivityPerPadUI * 100.00;
                //qDebug() << i << "padSensitivityPerPad" << scaledPadSensitivityPerPad;

                //qDebug() << i << "padSensitivityPerPadUI" << scaledPadSensitivityPerPadUI << "padSensitivity" << scaledPadSensitivity << "padSensitivityPerPad" << scaledPadSensitivityPerPad;

                //insert into sensitivity-wise parsed map
                typeNumMap.insert(QString("padSensitivityPerPad"), scaledPadSensitivityPerPad);

                paramTypeMap.insert(paramName, paramValue);

                paramTypeMap.insert(QString("%1%2").arg(paramType).arg(i), typeNumMap);
                settingsTypeMap.insert(QString("%1s").arg(paramTypeName), paramTypeMap);
                presetNumMap.insert(settingsType, settingsTypeMap);
                presetMapsCopy.insert(QString("Preset %1").arg(presetNum), presetNumMap);
            }


        //if a global, but not a  PAD sensitivity
        } else
        {
            paramTypeMap.insert(paramName, paramValue);
            paramTypeMap.insert(QString("%1%2").arg(paramType).arg(typeNum), typeNumMap);
            settingsTypeMap.insert(QString("%1s").arg(paramTypeName), paramTypeMap);
            presetNumMap.insert(settingsType, settingsTypeMap);
            presetMapsCopy.insert(QString("Preset %1").arg(presetNum), presetNumMap);
            //qDebug() << "global, but not sensitivity";
        }

        //if NOT a global or PAD sensitivity
    } else

    {

        typeNumMap.insert(paramName, paramValue);
        paramTypeMap.insert(QString("%1%2").arg(paramType).arg(typeNum), typeNumMap);
        settingsTypeMap.insert(QString("%1s").arg(paramTypeName), paramTypeMap);
        presetNumMap.insert(settingsType, settingsTypeMap);
        presetMapsCopy.insert(QString("Preset %1").arg(presetNum), presetNumMap);

        //call this slot to send values to other sensors, if more than one, are selected
        slotPropogateValuesSelectMultiple(presetNum, settingsType, paramType, typeNum,
                                          paramName, paramValue);
    }

    emit signalPresetModified(true);
    slotCheckPresets();
}

void::PresetHandler::slotPresetNameChanged(QString presetName){
    presetNumMap = presetMapsCopy.value(QString("Preset %1").arg(currentPreset)).toMap();//Preset#
    presetNumMap.insert("presetName", presetName);
    presetMapsCopy.insert(QString("Preset %1").arg(currentPreset), presetNumMap);

    emit signalPresetModified(true);
    slotCheckPresets();
}

void::PresetHandler::slotPresetNameValidator(){

    //find current preset name.
    isPresetNameValidatorRunning = true;
    presetNumMap = presetMapsCopy.value(QString("Preset %1").arg(currentPreset)).toMap();//Preset#
    currentPresetName = QString(presetNumMap.value("presetName").toString());

    //go through each of the other 16 presets and find their preset names.
    for(int i = 0; i < 16; i++){
        //if we are not checking the current preset, get its name.
        if(i != currentPreset){
            presetNumMap = presetMapsCopy.value(QString("Preset %1").arg(i)).toMap();//Preset#
            otherPresetName = QString(presetNumMap.value("presetName").toString());

            //if we find a string that matches the current preset, append a number to the current one.
            if(otherPresetName == currentPresetName){
                x++;
                currentPresetName = QString("%1(%2)").arg(currentPresetName).arg(x);
                presetNumMap = presetMapsCopy.value(QString("Preset %1").arg(currentPreset)).toMap();//Preset#
                presetNumMap.insert("presetName", currentPresetName);
                presetMapsCopy.insert(QString("Preset %1").arg(currentPreset), presetNumMap);
                //after changing the name, recheck the new name against all other presets.
                //if another preset has the new name then this if statement is entered again,
                //x is incremented by 1 and appended to the name and it is rechecked.
                //if all 16 presets are checked and this if statement is not true then
                //isPresetNameValidatorRunning remains false (as set below) and x is reset.
                slotPresetNameValidator();
                break;
            }

            if(otherPresetName != currentPresetName){
                isPresetNameValidatorRunning = false;
            }
        }
    }

    if (isPresetNameValidatorRunning == false){//if preset name validator is done, reset x to 0 for next time.
        x = 0;
    }

}

void PresetHandler::slotPadVelocityTableChanged(int presetNum, QString selectedTableID, QList<QVariant> selectedTableValues){
    //this function saves both the table ID and the table array into the json file.
    //print selected table
    /*for(int z = 0; z < selectedTableValues.size(); z++){
        qDebug() << QString("%1").arg(selectedTableValues.at(z).toInt());
    }*/
    //Populate the maps
    paramTypeMap = presetMapsCopy.value(QString("Preset %1").arg(presetNum)).toMap().
            value("ComponentSettings").toMap().value("Pads").toMap();//Pads
    settingsTypeMap = presetMapsCopy.value(QString("Preset %1").arg(presetNum)).toMap().
            value("ComponentSettings").toMap();//ComponentSettings
    presetNumMap = presetMapsCopy.value(QString("Preset %1").arg(presetNum)).toMap();//Preset#

    paramTypeMap.insert("padVelocityTableID", selectedTableID);
    paramTypeMap.insert("padVelocityTable", QVariant(selectedTableValues));

    settingsTypeMap.insert("Pads", paramTypeMap);
    presetNumMap.insert("ComponentSettings", settingsTypeMap);
    presetMapsCopy.insert(QString("Preset %1").arg(presetNum), presetNumMap);

    emit signalPresetModified(true);
    slotCheckPresets();

}

void PresetHandler::slotSaveAllPresets(){
    int presetToReturnTo = currentPreset;
    for(int x = 0; x < 16; x++){
        slotRecallPreset(QString("Preset %1").arg(x+1));
        slotSave();
    }
    slotRecallPreset(QString("Preset %1").arg(presetToReturnTo+1));
}

void PresetHandler::slotCheckPresets(){
    for(int i=0; i<16; i++){

        if(updateIndicator->presetModified[i]){
            presetMenu->setItemText(i, QString("Preset %1 *").arg(i+1));
        } else {
            presetMenu->setItemText(i, QString("Preset %1").arg(i+1));
        }
    }
}

void PresetHandler::slotConnectDisconnectDisplayAllLabels(bool connectDisconnect){
    if(connectDisconnect){
        connect(vSliderEditPane, SIGNAL(signalToLabels(QString)), this, SLOT(slotDisplayAllLabels(QString)));
        connect(vSliderEditPane, SIGNAL(signalValueChanged(int,QString,QString,int,QString,int)),
                this, SLOT(slotValueChanged(int,QString,QString,int,QString,int)));
        connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), vSliderEditPane, SLOT(slotFocusParam(QWidget*,QWidget*)));
        connect(hSliderEditPane, SIGNAL(signalToLabels(QString)), this, SLOT(slotDisplayAllLabels(QString)));
        connect(hSliderEditPane, SIGNAL(signalValueChanged(int,QString,QString,int,QString,int)),
                this, SLOT(slotValueChanged(int,QString,QString,int,QString,int)));
        connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), hSliderEditPane, SLOT(slotFocusParam(QWidget*,QWidget*)));
        connect(lSliderEditPane, SIGNAL(signalToLabels(QString)), this, SLOT(slotDisplayAllLabels(QString)));
        connect(lSliderEditPane, SIGNAL(signalValueChanged(int,QString,QString,int,QString,int)),
                this, SLOT(slotValueChanged(int,QString,QString,int,QString,int)));
        connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), lSliderEditPane, SLOT(slotFocusParam(QWidget*,QWidget*)));
        connect(padEditPane, SIGNAL(signalToLabels(QString)), this, SLOT(slotDisplayAllLabels(QString)));
        connect(padEditPane, SIGNAL(signalValueChanged(int,QString,QString,int,QString,int)),
                this, SLOT(slotValueChanged(int,QString,QString,int,QString,int)));
        connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), padEditPane, SLOT(slotFocusParam(QWidget*,QWidget*)));
        connect(leftrightEditPane, SIGNAL(signalToLabels(QString)), this, SLOT(slotDisplayAllLabels(QString)));
        connect(leftrightEditPane, SIGNAL(signalValueChanged(int,QString,QString,int,QString,int)),
                this, SLOT(slotValueChanged(int,QString,QString,int,QString,int)));
        connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), leftrightEditPane, SLOT(slotFocusParam(QWidget*,QWidget*)));
        connect(updownEditPane, SIGNAL(signalToLabels(QString)), this, SLOT(slotDisplayAllLabels(QString)));
        connect(updownEditPane, SIGNAL(signalValueChanged(int,QString,QString,int,QString,int)),
                this, SLOT(slotValueChanged(int,QString,QString,int,QString,int)));
        connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), updownEditPane, SLOT(slotFocusParam(QWidget*,QWidget*)));
        connect(rhombusEditPane, SIGNAL(signalToLabels(QString)), this, SLOT(slotDisplayAllLabels(QString)));
        connect(rhombusEditPane, SIGNAL(signalValueChanged(int,QString,QString,int,QString,int)),
                this, SLOT(slotValueChanged(int,QString,QString,int,QString,int)));
        connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), rhombusEditPane, SLOT(slotFocusParam(QWidget*,QWidget*)));
        connect(transportEditPane, SIGNAL(signalToLabels(QString)), this, SLOT(slotDisplayAllLabels(QString)));
        connect(transportEditPane, SIGNAL(signalValueChanged(int,QString,QString,int,QString,int)),
                this, SLOT(slotValueChanged(int,QString,QString,int,QString,int)));
        connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), transportEditPane, SLOT(slotFocusParam(QWidget*,QWidget*)));
        connect(rotaryEditPane, SIGNAL(signalToLabels(QString)), this, SLOT(slotDisplayAllLabels(QString)));
        connect(rotaryEditPane, SIGNAL(signalValueChanged(int,QString,QString,int,QString,int)),
                this, SLOT(slotValueChanged(int,QString,QString,int,QString,int)));
        connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), rotaryEditPane, SLOT(slotFocusParam(QWidget*,QWidget*)));
    }
    else{
        disconnect(vSliderEditPane, SIGNAL(signalToLabels(QString)), this, SLOT(slotDisplayAllLabels(QString)));
        disconnect(vSliderEditPane, SIGNAL(signalValueChanged(int,QString,QString,int,QString,int)),
                   this, SLOT(slotValueChanged(int,QString,QString,int,QString,int)));
        disconnect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), vSliderEditPane, SLOT(slotFocusParam(QWidget*,QWidget*)));
        disconnect(hSliderEditPane, SIGNAL(signalToLabels(QString)), this, SLOT(slotDisplayAllLabels(QString)));
        disconnect(hSliderEditPane, SIGNAL(signalValueChanged(int,QString,QString,int,QString,int)),
                   this, SLOT(slotValueChanged(int,QString,QString,int,QString,int)));
        disconnect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), hSliderEditPane, SLOT(slotFocusParam(QWidget*,QWidget*)));
        disconnect(lSliderEditPane, SIGNAL(signalToLabels(QString)), this, SLOT(slotDisplayAllLabels(QString)));
        disconnect(lSliderEditPane, SIGNAL(signalValueChanged(int,QString,QString,int,QString,int)),
                   this, SLOT(slotValueChanged(int,QString,QString,int,QString,int)));
        disconnect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), lSliderEditPane, SLOT(slotFocusParam(QWidget*,QWidget*)));
        disconnect(rotaryEditPane, SIGNAL(signalToLabels(QString)), this, SLOT(slotDisplayAllLabels(QString)));
        disconnect(rotaryEditPane, SIGNAL(signalValueChanged(int,QString,QString,int,QString,int)),
                   this, SLOT(slotValueChanged(int,QString,QString,int,QString,int)));
        disconnect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), rotaryEditPane, SLOT(slotFocusParam(QWidget*,QWidget*)));
        disconnect(padEditPane, SIGNAL(signalToLabels(QString)), this, SLOT(slotDisplayAllLabels(QString)));
        disconnect(padEditPane, SIGNAL(signalValueChanged(int,QString,QString,int,QString,int)),
                   this, SLOT(slotValueChanged(int,QString,QString,int,QString,int)));
        disconnect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), padEditPane, SLOT(slotFocusParam(QWidget*,QWidget*)));
        disconnect(leftrightEditPane, SIGNAL(signalToLabels(QString)), this, SLOT(slotDisplayAllLabels(QString)));
        disconnect(leftrightEditPane, SIGNAL(signalValueChanged(int,QString,QString,int,QString,int)),
                   this, SLOT(slotValueChanged(int,QString,QString,int,QString,int)));
        disconnect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), leftrightEditPane, SLOT(slotFocusParam(QWidget*,QWidget*)));
        disconnect(updownEditPane, SIGNAL(signalToLabels(QString)), this, SLOT(slotDisplayAllLabels(QString)));
        disconnect(updownEditPane, SIGNAL(signalValueChanged(int,QString,QString,int,QString,int)),
                   this, SLOT(slotValueChanged(int,QString,QString,int,QString,int)));
        disconnect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), updownEditPane, SLOT(slotFocusParam(QWidget*,QWidget*)));
        disconnect(rhombusEditPane, SIGNAL(signalToLabels(QString)), this, SLOT(slotDisplayAllLabels(QString)));
        disconnect(rhombusEditPane, SIGNAL(signalValueChanged(int,QString,QString,int,QString,int)),
                   this, SLOT(slotValueChanged(int,QString,QString,int,QString,int)));
        disconnect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), rhombusEditPane, SLOT(slotFocusParam(QWidget*,QWidget*)));
        disconnect(transportEditPane, SIGNAL(signalToLabels(QString)), this, SLOT(slotDisplayAllLabels(QString)));
        disconnect(transportEditPane, SIGNAL(signalValueChanged(int,QString,QString,int,QString,int)),
                   this, SLOT(slotValueChanged(int,QString,QString,int,QString,int)));
        disconnect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), transportEditPane, SLOT(slotFocusParam(QWidget*,QWidget*)));
    }

}
void PresetHandler::slotDisplayAllLabels(QString sourceObject){ //connected in constructor here from each edit pane's signalToLabels - handles cross component labeling
    if (sourceObject.contains("Press")  && !sourceObject.contains("Value")  && !sourceObject.contains("Mode")){
        if(!sourceObject.contains("hB")){
            hSliderEditPane->slotToLabels(QString("hB%1outPress").arg(hSliderEditPane->currentBank));
        }
        if(!sourceObject.contains("vB")){
            vSliderEditPane->slotToLabels(QString("vB%1outPress").arg(vSliderEditPane->currentBank));
        }
        if(!sourceObject.contains("lB")){
            lSliderEditPane->slotToLabels(QString("lB%1outPress").arg(lSliderEditPane->currentBank));
        }
        if(!sourceObject.contains("rB")){
            rotaryEditPane->slotToLabels(QString("rB%1outPress").arg(rotaryEditPane->currentBank));
        }
        if(!sourceObject.contains("Dm") && !sourceObject.contains("Gm")){
            padEditPane->slotToLabels("outDmPress");
        }
        if(!sourceObject.contains("leftright")){
            leftrightEditPane->slotToLabels(QString("leftrightLOutPress"));
        }
        if(!sourceObject.contains("updown")){
            updownEditPane->slotToLabels(QString("updownUOutPress"));
        }
        if(!sourceObject.contains("transport")){
            transportEditPane->slotToLabels(QString("transportOutPress"));
        }
        if(!sourceObject.contains("rhombus")){
            rhombusEditPane->slotToLabels(QString("rhombusOutPress"));
        }
    }
    if(sourceObject.contains("Threshold")){
        if(sourceObject.contains("On")){
            if(!sourceObject.contains("hSlider")){
                hSliderEditPane->slotToLabels(QString("hSliderOnThreshold"));
            }
            if(!sourceObject.contains("vSlider")){
                vSliderEditPane->slotToLabels(QString("vSliderOnThreshold"));
            }
            if(!sourceObject.contains("lSlider")){
                lSliderEditPane->slotToLabels(QString("lSliderOnThreshold"));
            }
            if(!sourceObject.contains("rotary")){
                rotaryEditPane->slotToLabels(QString("rotaryOnThreshold"));
            }
            if(!sourceObject.contains("pad")){
                padEditPane->slotToLabels(QString("padOnThreshold"));
            }
        }else{
            if(!sourceObject.contains("hSlider")){
                hSliderEditPane->slotToLabels(QString("hSliderOffThreshold"));
            }
            if(!sourceObject.contains("vSlider")){
                vSliderEditPane->slotToLabels(QString("vSliderOffThreshold"));
            }
            if(!sourceObject.contains("lSlider")){
                lSliderEditPane->slotToLabels(QString("lSliderOffThreshold"));
            }
            if(!sourceObject.contains("rotary")){
                rotaryEditPane->slotToLabels(QString("rotaryOffThreshold"));
            }
            if(!sourceObject.contains("pad")){
                padEditPane->slotToLabels(QString("padOffThreshold"));
            }

        }
    }
    if (sourceObject.contains("NotePressMode")){
        if(!sourceObject.contains("hB")){
            hSliderEditPane->slotToLabels(QString("hB%1outNotePressMode").arg(hSliderEditPane->currentBank));
        }
        if(!sourceObject.contains("vB")){
            vSliderEditPane->slotToLabels(QString("vB%1outNotePressMode").arg(vSliderEditPane->currentBank));
        }
        if(!sourceObject.contains("lB")){
            lSliderEditPane->slotToLabels(QString("lB%1outNotePressMode").arg(lSliderEditPane->currentBank));
        }
        if(!sourceObject.contains("rB")){
            rotaryEditPane->slotToLabels(QString("rB%1outNotePressMode").arg(rotaryEditPane->currentBank));
        }
        if(!sourceObject.contains("Dm") && !sourceObject.contains("Gm")){
            if(padEditPane->enableGridMode->isChecked()){
                padEditPane->slotToLabels("outGmNotePressModeNW");
            }
            else{
                padEditPane->slotToLabels("outDmNotePressMode");
            }
        }
        if(!sourceObject.contains("leftright")){
            leftrightEditPane->slotToLabels(QString("leftrightLOutNotePressMode"));
        }
        if(!sourceObject.contains("updown")){
            updownEditPane->slotToLabels(QString("updownUOutNotePressMode"));
        }
        if(!sourceObject.contains("transport")){
            transportEditPane->slotToLabels(QString("transportOutNotePressMode"));
        }
        if(!sourceObject.contains("rhombus")){
            rhombusEditPane->slotToLabels(QString("rhombusOutNotePressMode"));
        }
    }
    if (sourceObject.contains("Channel") && !sourceObject.contains("In") && !sourceObject.contains("in")){
        if(!sourceObject.contains("hB")){
            hSliderEditPane->slotToLabels(QString("hB%1Channel").arg(hSliderEditPane->currentBank));
        }
        if(!sourceObject.contains("vB")){
            vSliderEditPane->slotToLabels(QString("vB%1Channel").arg(vSliderEditPane->currentBank));
        }
        if(!sourceObject.contains("lB")){
            lSliderEditPane->slotToLabels(QString("lB%1Channel").arg(lSliderEditPane->currentBank));
        }
        if(!sourceObject.contains("rB")){
            rotaryEditPane->slotToLabels(QString("rB%1Channel").arg(rotaryEditPane->currentBank));
        }
        if(!sourceObject.contains("Dm") && !sourceObject.contains("Gm")){
            padEditPane->slotToLabels("padChannel");
        }
        if(!sourceObject.contains("leftright")){
            leftrightEditPane->slotToLabels(QString("leftrightChannel"));
        }
        if(!sourceObject.contains("updown")){
            updownEditPane->slotToLabels(QString("updownChannel"));
        }
        if(!sourceObject.contains("transport")){
            transportEditPane->slotToLabels(QString("transportChannel"));
        }
        if(!sourceObject.contains("rhombus")){
            rhombusEditPane->slotToLabels(QString("rhombusChannel"));
        }
    }
    if (sourceObject.contains("X") || sourceObject.contains("Y") || sourceObject.contains("Location")){
        if(!sourceObject.contains("in") && !sourceObject.contains("Return")){
            if(!sourceObject.contains("hB")){
                hSliderEditPane->slotToLabels(QString("hB%1outLocation").arg(hSliderEditPane->currentBank));
            }
            if(!sourceObject.contains("vB")){
                vSliderEditPane->slotToLabels(QString("vB%1outLocation").arg(vSliderEditPane->currentBank));
            }
            if(!sourceObject.contains("lB")){
                lSliderEditPane->slotToLabels(QString("lB%1outLocation").arg(lSliderEditPane->currentBank));
            }
            if(!sourceObject.contains("rB")){
                if(presetMapsCopy.value(QString("Preset %1").arg(currentPreset)).toMap().
                        value("ComponentSettings").toMap().value(QString("Rotaries")).toMap().
                        value(QString("Rotary%1").arg(rotaryEditPane->currentBank)).toMap().
                        value(QString("rB%1outDirectionEnable").arg(rotaryEditPane->currentBank)).toBool()){
                    rotaryEditPane->slotToLabels(QString("rB%1outDirection").arg(rotaryEditPane->currentBank));}
                else {
                    rotaryEditPane->slotToLabels(QString("rB%1outLocation").arg(rotaryEditPane->currentBank));}
            }
            if(!sourceObject.contains("Dm") && !sourceObject.contains("Gm")){
                padEditPane->slotToLabels("outDmXCC");
            }
            if(!sourceObject.contains("leftright")){
                leftrightEditPane->slotToLabels(QString("leftrightLOutPress"));
            }
            if(!sourceObject.contains("updown")){
                updownEditPane->slotToLabels(QString("updownUOutPress"));
            }
            if(!sourceObject.contains("transport")){
                transportEditPane->slotToLabels(QString("transportOutPress"));
            }
            if(!sourceObject.contains("rhombus")){
                rhombusEditPane->slotToLabels(QString("rhombusOutPress"));
            }
        }}
    if (sourceObject.contains("Velocity")){
        if(!sourceObject.contains("hB")){
            hSliderEditPane->slotToLabels(QString("hB%1outVelocityValue").arg(hSliderEditPane->currentBank));
        }
        if(!sourceObject.contains("vB")){
            vSliderEditPane->slotToLabels(QString("vB%1outVelocityValue").arg(vSliderEditPane->currentBank));
        }
        if(!sourceObject.contains("lB")){
            lSliderEditPane->slotToLabels(QString("lB%1outVelocityValue").arg(lSliderEditPane->currentBank));
        }
        if(!sourceObject.contains("rB")){
            rotaryEditPane->slotToLabels(QString("rB%1outVelocityValue").arg(rotaryEditPane->currentBank));
        }
        if(!sourceObject.contains("Dm") && !sourceObject.contains("Gm")){
            if(padEditPane->enableGridMode->isChecked()){
                padEditPane->slotToLabels("outGmVelocityValueNW");
            }
            else{
                padEditPane->slotToLabels("outDmVelocityValue");
            }
        }
        if(!sourceObject.contains("leftright")){
            leftrightEditPane->slotToLabels(QString("leftrightLOutVelocityValue"));
        }
        if(!sourceObject.contains("updown")){
            updownEditPane->slotToLabels(QString("updownUOutVelocityValue"));
        }
        if(!sourceObject.contains("transport")){
            transportEditPane->slotToLabels(QString("transportOutVelocityValue"));
        }
        if(!sourceObject.contains("rhombus")){
            rhombusEditPane->slotToLabels(QString("rhombusOutVelocityValue"));
        }
    }
    if (sourceObject.contains("PressValue")){
        if(!sourceObject.contains("hB")){
            hSliderEditPane->slotToLabels(QString("hB%1outPressValue").arg(hSliderEditPane->currentBank));
        }
        if(!sourceObject.contains("vB")){
            vSliderEditPane->slotToLabels(QString("vB%1outPressValue").arg(vSliderEditPane->currentBank));
        }
        if(!sourceObject.contains("lB")){
            lSliderEditPane->slotToLabels(QString("lB%1outPressValue").arg(lSliderEditPane->currentBank));
        }
        if(!sourceObject.contains("rB")){
            rotaryEditPane->slotToLabels(QString("rB%1outPressValue").arg(rotaryEditPane->currentBank));
        }
        if(!sourceObject.contains("Dm") && !sourceObject.contains("Gm")){
            if(padEditPane->enableGridMode->isChecked()){
                padEditPane->slotToLabels("outGmPressValueNW");
            }
            else{
                padEditPane->slotToLabels("outDmPressValue");
            }
        }
        if(!sourceObject.contains("leftright")){
            leftrightEditPane->slotToLabels(QString("leftrightLOutPressValue"));
        }
        if(!sourceObject.contains("updown")){
            updownEditPane->slotToLabels(QString("updownUOutPressValue"));
        }
        if(!sourceObject.contains("transport")){
            transportEditPane->slotToLabels(QString("transportOutPressValue"));
        }
        if(!sourceObject.contains("rhombus")){
            rhombusEditPane->slotToLabels(QString("rhombusOutPressValue"));
        }
    }
    if (sourceObject.contains("PassThru")){
        if(!sourceObject.contains("hB")){
            hSliderEditPane->slotToLabels(QString("hB%1outLocPassThruRange").arg(hSliderEditPane->currentBank));
        }
        if(!sourceObject.contains("vB")){
            vSliderEditPane->slotToLabels(QString("vB%1outLocPassThruRange").arg(vSliderEditPane->currentBank));
        }
        if(!sourceObject.contains("lB")){
            lSliderEditPane->slotToLabels(QString("lB%1outLocPassThruRange").arg(lSliderEditPane->currentBank));
        }
        if(!sourceObject.contains("rB")){
            rotaryEditPane->slotToLabels(QString("rB%1outLocPassThruRange").arg(rotaryEditPane->currentBank));
        }
    }
    if (sourceObject.contains("Note") && ((sourceObject.contains("out")) || sourceObject.contains("Out"))){
        if(!sourceObject.contains("Mode")){
        if(!sourceObject.contains("hB")){
            hSliderEditPane->slotToLabels(QString("hB%1outNote").arg(hSliderEditPane->currentBank));
        }
        if(!sourceObject.contains("vB")){
            vSliderEditPane->slotToLabels(QString("vB%1outNote").arg(vSliderEditPane->currentBank));
        }
        if(!sourceObject.contains("lB")){
            lSliderEditPane->slotToLabels(QString("lB%1outNote").arg(lSliderEditPane->currentBank));
        }
        if(!sourceObject.contains("rB")){
            rotaryEditPane->slotToLabels(QString("rB%1outNote").arg(rotaryEditPane->currentBank));
        }
        if(!sourceObject.contains("Dm") && !sourceObject.contains("Gm")){
            padEditPane->slotToLabels("outDmNote");
        }
        if(!sourceObject.contains("leftright")){
            leftrightEditPane->slotToLabels(QString("leftrightLOutNote"));
        }
        if(!sourceObject.contains("updown")){
            updownEditPane->slotToLabels(QString("updownUOutNote"));
        }
        if(!sourceObject.contains("transport")){
            transportEditPane->slotToLabels(QString("transportOutNote"));
        }
        if(!sourceObject.contains("rhombus")){
            rhombusEditPane->slotToLabels(QString("rhombusOutNote"));
        }
      }
    }

    if (sourceObject.contains("TransposeInterval")){//if you are looking at the pad transpose intervals, show all other notes.
        if(!sourceObject.contains("hB")){
            hSliderEditPane->slotToLabels(QString("hB%1outNote").arg(hSliderEditPane->currentBank));
        }
        if(!sourceObject.contains("vB")){
            vSliderEditPane->slotToLabels(QString("vB%1outNote").arg(vSliderEditPane->currentBank));
        }
        if(!sourceObject.contains("lB")){
            lSliderEditPane->slotToLabels(QString("lB%1outNote").arg(lSliderEditPane->currentBank));
        }
        if(!sourceObject.contains("rB")){
            rotaryEditPane->slotToLabels(QString("rB%1outNote").arg(rotaryEditPane->currentBank));
        }
        if(!sourceObject.contains("leftright")){
            leftrightEditPane->slotToLabels(QString("leftrightLOutNote"));
        }
        if(!sourceObject.contains("updown")){
            updownEditPane->slotToLabels(QString("updownUOutNote"));
        }
        if(!sourceObject.contains("transport")){
            transportEditPane->slotToLabels(QString("transportOutNote"));
        }
        if(!sourceObject.contains("rhombus")){
            rhombusEditPane->slotToLabels(QString("rhombusOutNote"));
        }
      }

    if (sourceObject.contains("in") || sourceObject.contains("InChannel")){
        if(!sourceObject.contains("hSlider")){
            hSliderEditPane->slotToLabels(QString("hSliderInChannel"));
        }
        if(!sourceObject.contains("vSlider")){
            vSliderEditPane->slotToLabels(QString("vSliderInChannel"));
        }
        if(!sourceObject.contains("lSlider")){
            lSliderEditPane->slotToLabels(QString("lSliderInChannel"));
        }
        if(!sourceObject.contains("rotary")){
            rotaryEditPane->slotToLabels(QString("rotaryInChannel"));
        }
        if(!sourceObject.contains("pad")){
            if(padEditPane->enableGridMode->isChecked()){
                padEditPane->slotToLabels("padGridDualInChannel");
            }
            else{
                padEditPane->slotToLabels("padDrumInChannel");
            }
        }
        if(!sourceObject.contains("leftright")){
            leftrightEditPane->slotToLabels(QString("leftrightInChannel"));
        }
        if(!sourceObject.contains("updown")){
            updownEditPane->slotToLabels(QString("updownInChannel"));
        }
        if(!sourceObject.contains("transport")){
            transportEditPane->slotToLabels(QString("transportInChannel"));
        }
        if(!sourceObject.contains("rhombus")){
            rhombusEditPane->slotToLabels(QString("rhombusInChannel"));
        }
    }
    if (sourceObject.contains("Enable") && !sourceObject.contains("direction") && !sourceObject.contains("Grid")){
        if(!sourceObject.contains("leftright")){
            leftrightEditPane->slotToLabels(QString("leftrightEnableSwitch"));
        }
        if(!sourceObject.contains("updown")){
            updownEditPane->slotToLabels(QString("updownEnableSwitch"));
        }
        if(!sourceObject.contains("rhombus")){
            rhombusEditPane->slotToLabels(QString("rhombusEnableSwitch"));
        }
    }
    if (sourceObject.contains("BankControl")){
        if(!sourceObject.contains("updown")){
            updownEditPane->slotToLabels(QString("updownBankControl"));
        }
        if(!sourceObject.contains("rhombus")){
            rhombusEditPane->slotToLabels(QString("rhombusBankControl"));
        }
    }
}

