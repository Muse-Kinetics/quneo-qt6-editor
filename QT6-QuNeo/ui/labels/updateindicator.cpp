#include "updateindicator.h"

UpdateIndicator::UpdateIndicator(QWidget *widget, QVariantMap * presetMapOrig, QVariantMap * presetMapModified, QWidget *parent) :
    QWidget(parent)
{
    //create qtimer for flashing
    buttonFlasher = new QTimer(this);
    connect(buttonFlasher, SIGNAL(timeout()), this, SLOT(slotFlasher()));

    //find save button address
    mainWindow = widget;
    indicatorButton = mainWindow->findChild<QPushButton*>("saveButton");
    presetMenu = mainWindow->findChild<QComboBox *>("presetMenu");
    updateButton = mainWindow->findChild<QPushButton *>("updateButton");

    redBlack = false; //indicator starts off turning red from black
    alreadyOn = false;


    //set pointers
    presetOrig = presetMapOrig;
    presetCopy = presetMapModified;

    for(int i=0; i<16; i++){
        presetModified[i] = false;
    }

    presetNum = 0;

    slotPresetModified(false);

    // timers have to be triggered by these signals from the main thread
    connect(this, SIGNAL(signalStartButtonFlasher()), this, SLOT(slotStartButtonFlasher()));
    connect(this, SIGNAL(signalStopButtonFlasher()), this, SLOT(slotStopButtonFlasher()));

}

bool UpdateIndicator::slotCompareMaps(int presetNumber){

    presetNum = presetNumber;
    bool presetState = false; //before testing with a new preset, set presetState to off. otherwise it never turns off.


    GlobalParamList = presetCopy->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value
            ("HSliders").toMap().keys();
    for (int nx = 0; nx < GlobalParamList.count(); nx++){
        copyValue = presetCopy->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value
                ("HSliders").toMap().value(QString(GlobalParamList.at(nx))).toInt();
        origValue = presetOrig->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value
                ("HSliders").toMap().value(QString(GlobalParamList.at(nx))).toInt();
        if (copyValue != origValue) {
            presetState = true;
            qDebug() << "mismatch" << GlobalParamList.at(nx) << "copied" << copyValue << "orig" << origValue;
            break;
            }
    }

    //for each of the 4 hSliders, create a list of the parameters in the original map and the copy.
    for (int i = 0; i < 4; i++){
        ParamList = presetCopy->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value
                   ("HSliders").toMap().value(QString("HSlider%1").arg(i)).toMap().keys();
        for (int n = 0; n < ParamList.count(); n++){     //compare the values stored at each key.
        copyValue = presetCopy->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value
                 ("HSliders").toMap().value(QString("HSlider%1").arg(i)).toMap().value(QString(ParamList.at(n))).toInt();
        origValue = presetOrig->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value
                 ("HSliders").toMap().value(QString("HSlider%1").arg(i)).toMap().value(QString(ParamList.at(n))).toInt();
        if (copyValue != origValue){ //if they are not equal, that parameter has been modified in this preset.
            presetState = true;      //so turn the indicator on and break stop testing.
            qDebug() << "mismatch ??" << ParamList.at(n) << "copied" << copyValue << "orig" << origValue;
            break;
        }
    }
        if (presetState)             //before testing the next component's parameters, check if presetState is true.
            break;                  //that means the test already found a parameter that has been modified. so stop testing.
    }

    //in next component test, check presetState before starting a for loop

    GlobalParamList = presetCopy->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value
            ("VSliders").toMap().keys();
    for (int nx = 0; nx < GlobalParamList.count(); nx++){
        copyValue = presetCopy->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value
                ("VSliders").toMap().value(QString(GlobalParamList.at(nx))).toInt();
        origValue = presetOrig->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value
                ("VSliders").toMap().value(QString(GlobalParamList.at(nx))).toInt();
        if (copyValue != origValue) {
            presetState = true;
            qDebug() << "mismatch" << GlobalParamList.at(nx) << "copied" << copyValue << "orig" << origValue;
            break;
            }
    }

    //for each of the 4 vSliders, create a list of the parameters in the original map and the copy.
    for (int j = 0; j < 4; j++){
        if (presetState)             //before testing check if presetState is true.
            break;                  //that means the test already found a parameter that has been modified. so stop testing.
        ParamList = presetCopy->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value
                   ("VSliders").toMap().value(QString("VSlider%1").arg(j)).toMap().keys();
        for (int m = 0; m < ParamList.count(); m++){     //compare the values stored at each key.
        copyValue = presetCopy->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value
                 ("VSliders").toMap().value(QString("VSlider%1").arg(j)).toMap().value(QString(ParamList.at(m))).toInt();
        origValue = presetOrig->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value
                 ("VSliders").toMap().value(QString("VSlider%1").arg(j)).toMap().value(QString(ParamList.at(m))).toInt();
        if (copyValue != origValue){ //if they are not equal, that parameter has been modified in this preset.
            presetState = true;      //so turn the indicator on and break stop testing.
            qDebug() << "mismatch" << ParamList.at(m) << "copied" << copyValue << "orig" << origValue;
            break;
        }
    }
        if (presetState)             //before testing the next component's parameters, check if presetState is true.
            break;                  //that means the test already found a parameter that has been modified. so stop testing.
    }


    GlobalParamList = presetCopy->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value
            ("Rotaries").toMap().keys();
    for (int nx = 0; nx < GlobalParamList.count(); nx++){
        copyValue = presetCopy->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value
                ("Rotaries").toMap().value(QString(GlobalParamList.at(nx))).toInt();
        origValue = presetOrig->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value
                ("Rotaries").toMap().value(QString(GlobalParamList.at(nx))).toInt();
        if (copyValue != origValue) {
            qDebug() << "mismatch" << GlobalParamList.at(nx) << "copied" << copyValue << "orig" << origValue;
            presetState = true;
            break;
            }
    }

    //for each of the 2 Rotaries, create a list of the parameters in the original map and the copy.
    for (int k = 0; k < 2; k++){
        if (presetState)             //before testing check if presetState is true.
            break;                  //that means the test already found a parameter that has been modified. so stop testing.
        ParamList = presetCopy->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value
                   ("Rotaries").toMap().value(QString("Rotary%1").arg(k)).toMap().keys();
        for (int r = 0; r < ParamList.count(); r++){     //compare the values stored at each key.
        copyValue = presetCopy->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value
                 ("Rotaries").toMap().value(QString("Rotary%1").arg(k)).toMap().value(QString(ParamList.at(r))).toInt();
        origValue = presetOrig->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value
                 ("Rotaries").toMap().value(QString("Rotary%1").arg(k)).toMap().value(QString(ParamList.at(r))).toInt();
        if (copyValue != origValue){ //if they are not equal, that parameter has been modified in this preset.
            presetState = true;      //so turn the indicator on and break stop testing.
            qDebug() << "mismatch" << ParamList.at(r) << "copied" << copyValue << "orig" << origValue;
            break;
        }
    }
        if (presetState)             //before testing the next component's parameters, check if presetState is true.
            break;                  //that means the test already found a parameter that has been modified. so stop testing.
    }

    GlobalParamList = presetCopy->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value
            ("LongSliders").toMap().keys();
    for (int nx = 0; nx < GlobalParamList.count(); nx++){
        copyValue = presetCopy->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value
                ("LongSliders").toMap().value(QString(GlobalParamList.at(nx))).toInt();
        origValue = presetOrig->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value
                ("LongSliders").toMap().value(QString(GlobalParamList.at(nx))).toInt();
        if (copyValue != origValue) {
            presetState = true;
             qDebug() << "mismatch" << GlobalParamList.at(nx) << "copied" << copyValue << "orig" << origValue;
            break;
            }
    }

    //for the LongSlider, create a list of the parameters in the original map and the copy.
    for (int l = 0; l < 1; l++){
        if (presetState)             //before testing check if presetState is true.
            break;                  //that means the test already found a parameter that has been modified. so stop testing.
        ParamList = presetCopy->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value
                   ("LongSliders").toMap().value(QString("LongSlider%1").arg(l)).toMap().keys();
        for (int x = 0; x < ParamList.count(); x++){     //compare the values stored at each key.
        copyValue = presetCopy->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value
                 ("LongSliders").toMap().value(QString("LongSlider%1").arg(l)).toMap().value(QString(ParamList.at(x))).toInt();
        origValue = presetOrig->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value
                 ("LongSliders").toMap().value(QString("LongSlider%1").arg(l)).toMap().value(QString(ParamList.at(x))).toInt();
        if (copyValue != origValue){ //if they are not equal, that parameter has been modified in this preset.
            presetState = true;      //so turn the indicator on and break stop testing.
            qDebug() << "mismatch" << ParamList.at(x) << "copied" << copyValue << "orig" << origValue;
            break;
        }
    }
        if (presetState)             //before testing the next component's parameters, check if presetState is true.
            break;                  //that means the test already found a parameter that has been modified. so stop testing.
    }

    GlobalParamList = presetCopy->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value
            ("Pads").toMap().keys();
    for (int nx = 0; nx < GlobalParamList.count(); nx++){
                if(!GlobalParamList.at(nx).contains("Velocity")){
            copyValue = presetCopy->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value
                ("Pads").toMap().value(QString(GlobalParamList.at(nx))).toInt();
            origValue = presetOrig->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value
                ("Pads").toMap().value(QString(GlobalParamList.at(nx))).toInt();
                }
            if(GlobalParamList.at(nx).contains("padVelocityTableID")){
                QString copyString = presetCopy->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value
                        ("Pads").toMap().value(QString(GlobalParamList.at(nx))).toString();
                QString origString = presetOrig->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value
                        ("Pads").toMap().value(QString(GlobalParamList.at(nx))).toString();
                if (copyString != origString){
                    presetState = true;
                    qDebug() << "mismatch" << GlobalParamList.at(nx) << "copied" << copyValue << "orig" << origValue;
                    break;
                }
            }
        if (copyValue != origValue) {
            presetState = true;
            qDebug() << "mismatch" << GlobalParamList.at(nx) << "copied" << copyValue << "orig" << origValue;
            break;
            }
    }

    //for each of the 16 Pads, create a list of the parameters in the original map and the copy.
    for (int p = 0; p < 16; p++){
        if (presetState)             //before testing check if presetState is true.
            break;                  //that means the test already found a parameter that has been modified. so stop testing.
        ParamList = presetCopy->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value
                   ("Pads").toMap().value(QString("Pad%1").arg(p)).toMap().keys();
        for (int P = 0; P < ParamList.count(); P++){     //compare the values stored at each key.
        copyValue = presetCopy->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value
                 ("Pads").toMap().value(QString("Pad%1").arg(p)).toMap().value(QString(ParamList.at(P))).toInt();
        origValue = presetOrig->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value
                 ("Pads").toMap().value(QString("Pad%1").arg(p)).toMap().value(QString(ParamList.at(P))).toInt();
        if (copyValue != origValue){ //if they are not equal, that parameter has been modified in this preset.
            presetState = true;      //so turn the indicator on and break stop testing.
            qDebug() << "mismatch" << ParamList.at(P) << "copied" << copyValue << "orig" << origValue;
            break;
        }
    }
        if (presetState)             //before testing the next component's parameters, check if presetState is true.
            break;                  //that means the test already found a parameter that has been modified. so stop testing.
    }

    GlobalParamList = presetCopy->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value
            ("UpDownButtons").toMap().keys();
    for (int nx = 0; nx < GlobalParamList.count(); nx++){
        copyValue = presetCopy->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value
                ("UpDownButtons").toMap().value(QString(GlobalParamList.at(nx))).toInt();
        origValue = presetOrig->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value
                ("UpDownButtons").toMap().value(QString(GlobalParamList.at(nx))).toInt();
        if (copyValue != origValue) {
            presetState = true;
            qDebug() << "mismatch" << GlobalParamList.at(nx) << "copied" << copyValue << "orig" << origValue;
            break;
            }
    }

    //for each of the 2 updown button sets, create a list of the parameters in the original map and the copy.
    for (int u = 0; u < 2; u++){
        if (presetState)             //before testing check if presetState is true.
            break;                  //that means the test already found a parameter that has been modified. so stop testing.
        ParamList = presetCopy->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value
                   ("UpDownButtons").toMap().value(QString("UpDownButton%1").arg(u)).toMap().keys();
        for (int y = 0; y < ParamList.count(); y++){     //compare the values stored at each key.
        copyValue = presetCopy->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value
                 ("UpDownButtons").toMap().value(QString("UpDownButton%1").arg(u)).toMap().value(QString(ParamList.at(y))).toInt();
        origValue = presetOrig->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value
                 ("UpDownButtons").toMap().value(QString("UpDownButton%1").arg(u)).toMap().value(QString(ParamList.at(y))).toInt();
        if (copyValue != origValue){ //if they are not equal, that parameter has been modified in this preset.
            presetState = true;      //so turn the indicator on and break stop testing.
            qDebug() << "mismatch" << ParamList.at(y) << "copied" << copyValue << "orig" << origValue;
            break;
        }
    }
        if (presetState)             //before testing the next component's parameters, check if presetState is true.
            break;                  //that means the test already found a parameter that has been modified. so stop testing.
    }


    GlobalParamList = presetCopy->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value
            ("LeftRightButtons").toMap().keys();
    for (int nx = 0; nx < GlobalParamList.count(); nx++){
        copyValue = presetCopy->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value
                ("LeftRightButtons").toMap().value(QString(GlobalParamList.at(nx))).toInt();
        origValue = presetOrig->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value
                ("LeftRightButtons").toMap().value(QString(GlobalParamList.at(nx))).toInt();
        if (copyValue != origValue) {
            presetState = true;
            qDebug() << "mismatch" << GlobalParamList.at(nx) << "copied" << copyValue << "orig" << origValue;
            break;
            }
    }


    //for each of the 2 leftright button sets, create a list of the parameters in the original map and the copy.
    for (int z = 0; z < 4; z++){
        if (presetState)             //before testing check if presetState is true.
            break;                  //that means the test already found a parameter that has been modified. so stop testing.
        ParamList = presetCopy->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value
                   ("LeftRightButtons").toMap().value(QString("LeftRightButton%1").arg(z)).toMap().keys();
        for (int Z = 0; Z < ParamList.count(); Z++){     //compare the values stored at each key.
        copyValue = presetCopy->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value
                 ("LeftRightButtons").toMap().value(QString("LeftRightButton%1").arg(z)).toMap().value(QString(ParamList.at(Z))).toInt();
        origValue = presetOrig->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value
                 ("LeftRightButtons").toMap().value(QString("LeftRightButton%1").arg(z)).toMap().value(QString(ParamList.at(Z))).toInt();
        if (copyValue != origValue){ //if they are not equal, that parameter has been modified in this preset.
            presetState = true;      //so turn the indicator on and break stop testing.
            qDebug() << "mismatch" << ParamList.at(Z) << "copied" << copyValue << "orig" << origValue;
            break;
        }
    }
        if (presetState)             //before testing the next component's parameters, check if presetState is true.
            break;                  //that means the test already found a parameter that has been modified. so stop testing.
    }


    GlobalParamList = presetCopy->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value
            ("RhombusButtons").toMap().keys();
    for (int nx = 0; nx < GlobalParamList.count(); nx++){
        copyValue = presetCopy->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value
                ("RhombusButtons").toMap().value(QString(GlobalParamList.at(nx))).toInt();
        origValue = presetOrig->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value
                ("RhombusButtons").toMap().value(QString(GlobalParamList.at(nx))).toInt();
        if (copyValue != origValue) {
            presetState = true;
            qDebug() << "mismatch" << GlobalParamList.at(nx) << "copied" << copyValue << "orig" << origValue;
            break;
            }
    }


    //for the rhombus button, create a list of the parameters in the original map and the copy.
    for (int rh = 0; rh < 1; rh++){
        if (presetState)             //before testing check if presetState is true.
            break;                  //that means the test already found a parameter that has been modified. so stop testing.
        ParamList = presetCopy->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value
                   ("RhombusButtons").toMap().value(QString("RhombusButton%1").arg(rh)).toMap().keys();
        for (int RH = 0; RH < ParamList.count(); RH++){     //compare the values stored at each key.
        copyValue = presetCopy->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value
                 ("RhombusButtons").toMap().value(QString("RhombusButton%1").arg(rh)).toMap().value(QString(ParamList.at(RH))).toInt();
        origValue = presetOrig->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value
                 ("RhombusButtons").toMap().value(QString("RhombusButton%1").arg(rh)).toMap().value(QString(ParamList.at(RH))).toInt();
        if (copyValue != origValue){ //if they are not equal, that parameter has been modified in this preset.
            presetState = true;      //so turn the indicator on and break stop testing.
            qDebug() << "mismatch" << ParamList.at(RH) << "copied" << copyValue << "orig" << origValue;
            break;
        }
    }
        if (presetState)             //before testing the next component's parameters, check if presetState is true.
            break;                  //that means the test already found a parameter that has been modified. so stop testing.
    }

        //for the mode button, create a list of the parameters in the original map and the copy.
        for (int mo = 0; mo < 1; mo++){
            if (presetState)             //before testing check if presetState is true.
                break;                  //that means the test already found a parameter that has been modified. so stop testing.
            ParamList = presetCopy->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value
                       ("ModeButtons").toMap().value(QString("ModeButton%1").arg(mo)).toMap().keys();
            for (int MO = 0; MO < ParamList.count(); MO++){     //compare the values stored at each key.
            copyValue = presetCopy->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value
                     ("ModeButtons").toMap().value(QString("ModeButton%1").arg(mo)).toMap().value(QString(ParamList.at(MO))).toInt();
            origValue = presetOrig->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value
                     ("ModeButtons").toMap().value(QString("ModeButton%1").arg(mo)).toMap().value(QString(ParamList.at(MO))).toInt();
            if (copyValue != origValue){ //if they are not equal, that parameter has been modified in this preset.
                presetState = true;      //so turn the indicator on and break stop testing.
                qDebug() << "mismatch" << ParamList.at(MO) << "copied" << copyValue << "orig" << origValue;
                break;
            }
        }
            if (presetState)             //before testing the next component's parameters, check if presetState is true.
                break;                  //that means the test already found a parameter that has been modified. so stop testing.
        }


        GlobalParamList = presetCopy->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value
                ("TransportButtons").toMap().keys();
        for (int nx = 0; nx < GlobalParamList.count(); nx++){
            copyValue = presetCopy->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value
                    ("TransportButtons").toMap().value(QString(GlobalParamList.at(nx))).toInt();
            origValue = presetOrig->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value
                    ("TransportButtons").toMap().value(QString(GlobalParamList.at(nx))).toInt();
            if (copyValue != origValue) {
                presetState = true;
                qDebug() << "mismatch" << GlobalParamList.at(nx) << "copied" << copyValue << "orig" << origValue;
                break;
                }
        }

    //for each of the 3 transport buttons, create a list of the parameters in the original map and the copy.
    for (int t = 0; t < 3; t++){
        if (presetState)             //before testing check if presetState is true.
            break;                  //that means the test already found a parameter that has been modified. so stop testing.
        ParamList = presetCopy->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value
                   ("TransportButtons").toMap().value(QString("TransportButton%1").arg(t)).toMap().keys();
        for (int T = 0; T < ParamList.count(); T++){     //compare the values stored at each key.
        copyValue = presetCopy->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value
                 ("TransportButtons").toMap().value(QString("TransportButton%1").arg(t)).toMap().value(QString(ParamList.at(T))).toInt();
        origValue = presetOrig->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value
                 ("TransportButtons").toMap().value(QString("TransportButton%1").arg(t)).toMap().value(QString(ParamList.at(T))).toInt();
        if (copyValue != origValue){ //if they are not equal, that parameter has been modified in this preset.
            presetState = true;      //so turn the indicator on and break stop testing.
            qDebug() << "mismatch" << ParamList.at(T) << "copied" << copyValue << "orig" << origValue;
            break;
        }
    }
        if (presetState)             //before testing the next component's parameters, check if presetState is true.
            break;                  //that means the test already found a parameter that has been modified. so stop testing.
    }
    if(!presetState){//if preset doesn't have any other modified parameters, check the name.
    QString presetNameOrig = presetOrig->value(QString("Preset %1").arg(presetNumber)).toMap().value("presetName").toString();
    QString presetNameCopy = presetCopy->value(QString("Preset %1").arg(presetNumber)).toMap().value("presetName").toString();
        if(presetNameOrig != presetNameCopy){
            presetState = true;}
    }
    return presetState;
}


void UpdateIndicator::slotPresetModified(bool state){
    //if indicator is not already on, and state is true, turn on the indicator.

    presetModified[presetNum] = state;

    if(state && !alreadyOn){
        alreadyOn = true;
        emit signalStartButtonFlasher();
        updateButton->setText("Audition Preset");
    }
    else if(!state && alreadyOn){ //if it is already on and state is false, turn off the indicator.
        emit signalStopButtonFlasher();
        alreadyOn = false;
        indicatorButton->setStyleSheet("color: black");
        updateButton->setText("Update Preset");
    }
    //otherwise do nothing (filtering out repeat calls to turn on the timer.)
}

void UpdateIndicator::slotFlasher(){
    if (redBlack){
        indicatorButton->setStyleSheet("color: transparent; background-color: rgb(130, 0, 11);");
        redBlack = false;
    }
    else {
        indicatorButton->setStyleSheet("color: red; background-color: rgb(130, 0, 11);");
        redBlack = true;
    }

}

bool UpdateIndicator::slotCheckForUnsavedPresets(){
    bool unsavedOrNot = false;
    for(int q = 0; q <16; q++){
    if (this->slotCompareMaps(q)){
        unsavedOrNot = true;
        break;}
    }
    return unsavedOrNot;
}

bool UpdateIndicator::slotCheckThisPreset(int preset){
    if(this->slotCompareMaps(preset)){
        slotPresetModified(true);
        return true;
    }
    else {slotPresetModified(false);
    return false;}
}

void UpdateIndicator::slotStartButtonFlasher()
{
    buttonFlasher->start(500);
}

void UpdateIndicator::slotStopButtonFlasher()
{
    buttonFlasher->stop();
}
