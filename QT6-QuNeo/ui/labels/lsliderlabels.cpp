#include "lsliderlabels.h"

LSliderLabels::LSliderLabels(QWidget *mainWindowAddress, QVariantMap *lSliderPresetValues, QWidget *parent) :
    QWidget(parent)
{
    presetValues = lSliderPresetValues;
    mainWindow = mainWindowAddress;
}

void LSliderLabels::slotDisplayParameter(QString objectName, int componentNum, int preset){

    //now get this slider's value for that object
    lSliderDisplayValue = presetValues->value(QString("Preset %1").arg(preset)).toMap().value("ComponentSettings")
            .toMap().value("LongSliders").toMap().value(QString("LongSlider%1").arg(componentNum)).toMap().value(objectName).toInt();

    //displays a parameter's value on a given component's label.
    lSliderDisplayBank = objectName;  //QString to hold the bank number
    lSliderDisplayBank = lSliderDisplayBank.remove(0,1); //remove the "l" prefix
    lSliderDisplayBank = lSliderDisplayBank.remove(2,lSliderDisplayBank.length()); //remove everything after "B#"
    lSliderDisplayType = objectName;  //QString to hold the parameter type (channel, note, pressure) or its abbreviation

    //find this slider's display label
    lSliderLabelBank = mainWindow->findChild<QLabel *>(QString("lSlider%1DisplayLabelBank").arg(componentNum));
    lSliderLabelType = mainWindow->findChild<QLabel *>(QString("lSlider%1DisplayLabelType").arg(componentNum));
    lSliderLabelValue = mainWindow->findChild<QLabel *>(QString("lSlider%1DisplayValue").arg(componentNum));

    if((!lSliderDisplayType.contains("Sensitivity")) && (!lSliderDisplayType.contains("LocalLEDControl"))) {
        //do nothing if sensitivity is clicked on
        if(lSliderDisplayType.contains("out") && !lSliderDisplayType.contains("NotePressMode")) {
            lSliderDisplayType = QString("%1").arg(lSliderDisplayType.remove(0,6)); //display parameter name by taking off lB#out
            if(lSliderDisplayType.contains("PassThru")){
                lSliderDisplayType = QString("Pass Thru Width");
            }
            lSliderLabelValue->show();
        }
        else if(lSliderDisplayType.contains("Channel") && !lSliderDisplayType.contains("In")){
            lSliderDisplayType = QString("Channel"); //otherwise it contains the string in and is reformatted to say LED.
            lSliderDisplayValue = (lSliderDisplayValue + 1);
        }

        else if(lSliderDisplayType.contains("InChannel")){
            lSliderDisplayType = QString("LED Channel:"); //otherwise it contains the string in and is reformatted to say LED.
            lSliderDisplayValue = presetValues->value(QString("Preset %1").arg(preset)).toMap().value("ComponentSettings")
                    .toMap().value("LongSliders").toMap().value(objectName).toInt();
            lSliderDisplayValue = (lSliderDisplayValue + 1);
            lSliderLabelBank->hide();
        }
        else if(objectName.contains("NotePressMode")){
            if (lSliderDisplayValue == 0){
                lSliderDisplayType = QString("Normal");
            }
            if (lSliderDisplayValue == 1){
                lSliderDisplayType = QString("Toggle");
            }
            lSliderLabelValue->hide();
        }
    }


#ifdef Q_OS_MAC

#else
    lSliderLabelBank->setStyleSheet("QLabel {font:10px;}");
    lSliderLabelType->setStyleSheet("QLabel {font:10px;}");
    lSliderLabelValue->setStyleSheet("QLabel {font:10px;}");
#endif


    //if it isn't sensitivity, set the text to be in the format Bank#: ParameterType value (for sensitivity do nothing)
    if((!lSliderDisplayType.contains("Sensitivity")) && (!lSliderDisplayType.contains("LocalLEDControl"))) {
        lSliderLabelValue->show();
        if(objectName.contains("Note")) {
            //if type is note convert note number to note names.  Then display text
            lSliderDisplayNote = noteNumberToName(lSliderDisplayValue);
            lSliderLabelBank->setText(QString("%1").arg(lSliderDisplayBank));
            lSliderLabelType->setText(QString("%1").arg(lSliderDisplayType));
            lSliderLabelValue->setText(QString("%1").arg(lSliderDisplayNote));
        }
        else if(objectName.contains("Velocity")) {
            lSliderLabelBank->setText(QString("%1").arg(lSliderDisplayBank));
            if(lSliderDisplayValue > 0) {
                lSliderLabelValue->setText(QString("%1").arg(lSliderDisplayValue));
                lSliderLabelType->setText(QString("%1").arg("Fix Velocity:"));
                lSliderLabelValue->show();
            }
            else {
                lSliderLabelType->setText("Variable Velocity");
                lSliderLabelValue->hide();
            }
        }
        else if(objectName.contains("PressValue")) {
            lSliderLabelBank->setText(QString("%1").arg(lSliderDisplayBank));
            if(lSliderDisplayValue > 0) {
                lSliderLabelValue->setText(QString("%1").arg(lSliderDisplayValue));
                lSliderLabelType->setText(QString("%1").arg("Fix Pressure:"));
                lSliderLabelValue->show();
            }
            else {
                lSliderLabelType->setText("Continuous Pressure");
                lSliderLabelValue->hide();
            }
        }
        else if(objectName.contains("Threshold")){
            lSliderLabelBank->hide();
            lSliderLabelValue->setText(QString("%1").arg(presetValues->value(QString("Preset %1").arg(preset)).toMap().value("ComponentSettings")
                                                         .toMap().value("LongSliders").toMap().value(objectName).toInt()));
            if(objectName.contains("On")){
                lSliderLabelType->setText("On Threshold");
            }else{
                lSliderLabelType->setText("Off Threshold");
            }
            lSliderLabelValue->show();
            lSliderLabelType->show();
        }
        else {
            lSliderDisplayNonNote = numberToOffState(lSliderDisplayValue);
            lSliderLabelBank->setText(QString("%1").arg(lSliderDisplayBank));
            lSliderLabelType->setText(QString("%1").arg(lSliderDisplayType));
            lSliderLabelValue->setText(QString("%1").arg(lSliderDisplayNonNote));
        }
    }
    if(!objectName.contains("InChannel") && !objectName.contains("VelocityValue") && !objectName.contains("PressValue") && !objectName.contains("NotePressMode")){
        lSliderLabelBank->show();
        lSliderLabelType->show();
        lSliderLabelValue->show();
    }
    if(objectName.contains("NotePressMode")){
        lSliderLabelBank->show();
        lSliderLabelType->show();
        lSliderLabelValue->hide();
    }
}

QString LSliderLabels::numberToOffState(int numberToOff) {
    QString offState;
    if(numberToOff == -1) {
        offState = QString("off");
    }
    else {
        offState = QString("%1").arg(numberToOff);
    }
    return QString("%1").arg(offState);
}

QString LSliderLabels::noteNumberToName(int noteNumber) {
    //to convert midi note number to note name where note 60 = C3.
    int pitchClass = noteNumber % 12;   //modulo 12 gives remainder 0-11 which tells you scale degree in key of C.
    QString noteLetter; //create space for corresponding letter
    if(noteNumber == -1) {
        noteLetter = QString("off");
    }
    else {
        switch(pitchClass) {
            case 0:{
                noteLetter = QString("C");
                break;
            }
            case 1:{
                noteLetter = QString("C#");
                break;
            }
            case 2:{
                noteLetter = QString("D");
                break;
            }
            case 3:{
                noteLetter = QString("D#");
                break;
            }
            case 4:{
                noteLetter = QString("E");
                break;
            }
            case 5:{
                noteLetter = QString("F");
                break;
            }
            case 6:{
                noteLetter = QString("F#");
                break;
            }
            case 7:{
                noteLetter = QString("G");
                break;
            }
            case 8:{
                noteLetter = QString("G#");
                break;
            }
            case 9:{
                noteLetter = QString("A");
                break;
            }
            case 10:{
                noteLetter = QString("A#");
                break;
            }
            case 11:{
                noteLetter = QString("B");
                break;
            }
        }
    }
    int octNumber;
    if(noteNumber<0) {
        noteLetter = QString("%1").arg(noteLetter);
    }
    else {
        octNumber = (noteNumber / 12) - 2; //find octave by using integers and truncating
        noteLetter = QString("%1%2").arg(noteLetter).arg(octNumber);   //format note letter with octave number
    }
    return QString("%1").arg(noteLetter); //return as a QString
}
