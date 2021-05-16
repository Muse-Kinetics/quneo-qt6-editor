#include "rotarylabels.h"

RotaryLabels::RotaryLabels(QWidget *mainWindowAddress, QVariantMap *rotaryPresetValues, QWidget *parent) :
    QWidget(parent)
{
    presetValues = rotaryPresetValues;
    mainWindow = mainWindowAddress;
}

//displays a parameter's value on a given component's label.
void RotaryLabels::slotDisplayParameter(QString objectName, int componentNum, int preset) {
    rotaryDisplayBank = objectName; //QString to hold the bank number
    rotaryDisplayBank = rotaryDisplayBank.remove(0,1); //remove the "r" prefix
    rotaryDisplayBank = rotaryDisplayBank.remove(2,rotaryDisplayBank.length()); //remove everything after "B#"
    rotaryDisplayType = objectName;  //QString to hold the parameter type (channel, note, pressure) or its abbreviation

    //now get the rotary's value for that object
    rotaryDisplayValue = presetValues->value(QString("Preset %1").arg(preset)).toMap().value
            ("ComponentSettings").toMap().value("Rotaries").toMap().value
            (QString("Rotary%1").arg(componentNum)).toMap().value(objectName).toInt();

    directionEnabled = presetValues->value(QString("Preset %1").arg(preset)).toMap().value
            ("ComponentSettings").toMap().value("Rotaries").toMap().value
            (QString("Rotary%1").arg(componentNum)).toMap().value(QString("r%1outDirectionEnable").arg(rotaryDisplayBank)).toBool();

    if((!rotaryDisplayType.contains("Sensitivity")) && (!rotaryDisplayType.contains("LocalLEDControl"))) {
        //do nothing if sensitivity or Local LED Control is clicked
        if(rotaryDisplayType.contains("out")) {
            rotaryDisplayType = QString("%1").arg(rotaryDisplayType.remove(0,6)); //display parameter name by taking off rB#out
            if (rotaryDisplayType.contains("DirectionEnable")) {
                if(rotaryDisplayValue) {
                    rotaryDisplayType = QString("Direction");
                    directionEnabled = true;
                }
                else {
                    rotaryDisplayType = QString("Location");
                    directionEnabled = false;
                }
            }
        }
        else if(rotaryDisplayType.contains("Channel")){
            rotaryDisplayType = QString("Channel");
            rotaryDisplayValue = (rotaryDisplayValue + 1);
          }
    }

    //find this rotary's display label
    rotaryLabelBank = mainWindow->findChild<QLabel *>(QString("rotary%1DisplayLabelBank").arg(componentNum));
    rotaryLabelType = mainWindow->findChild<QLabel *>(QString("rotary%1DisplayLabelType").arg(componentNum));
    rotaryLabelValue = mainWindow->findChild<QLabel *>(QString("rotary%1DisplayValue").arg(componentNum));

#ifdef Q_OS_MAC

#else
    rotaryLabelBank->setStyleSheet("QLabel {font:10px;}");
    rotaryLabelType->setStyleSheet("QLabel {font:10px;}");
    rotaryLabelValue->setStyleSheet("QLabel {font:10px;}");
#endif

    //if it isn't sensitivity or LocalLEDControl, set the text to be in the format Bank#:ParameterType value (for sensitivity do nothing)
    if((!rotaryDisplayType.contains("Sensitivity")) && (!rotaryDisplayType.contains("LocalLEDControl"))) {
        rotaryLabelValue->show();
        //if the type is note, convert note number to note name.  then display text.
        if(objectName.contains("Note")) {
            if(objectName.contains("NotePressMode")){
                       if (rotaryDisplayValue == 0){
                           rotaryDisplayType = QString("Normal");
                       }
                       if (rotaryDisplayValue == 1){
                           rotaryDisplayType = QString("Toggle");
                       }
                       rotaryLabelValue->hide();
                   }
            rotaryDisplayNote = noteNumberToName(rotaryDisplayValue);
            rotaryLabelBank->setText(QString("%1").arg(rotaryDisplayBank));
            rotaryLabelType->setText(QString("%1").arg(rotaryDisplayType));
            rotaryLabelValue->setText(QString("%1").arg(rotaryDisplayNote));
        }
        else if(objectName.contains("Velocity")) {
            rotaryLabelBank->setText(QString("%1").arg(rotaryDisplayBank));
            if(rotaryDisplayValue > 0) {
                rotaryLabelValue->setText(QString("%1").arg(rotaryDisplayValue));
                rotaryLabelType->setText(QString("%1").arg("Fix Velocity:"));
                rotaryLabelValue->show();
            }
            else {
                rotaryLabelType->setText("Variable Vel.");
                rotaryLabelValue->hide();
            }
        }
        else if(objectName.contains("PressValue")) {
            rotaryLabelBank->setText(QString("%1").arg(rotaryDisplayBank));
            if(rotaryDisplayValue > 0) {
                rotaryLabelValue->setText(QString("%1").arg(rotaryDisplayValue));
                rotaryLabelType->setText(QString("%1").arg("Fix Pressure:"));
                rotaryLabelValue->show();
            }
            else {
                rotaryLabelType->setText("Continuous Pr.");
                rotaryLabelValue->hide();
            }
        }
        else if(objectName.contains("Speed")) {
            rotaryDisplaySpeed = numberSpeedFloat(rotaryDisplayValue);
            rotaryLabelBank->setText(QString("%1").arg(rotaryDisplayBank));
            rotaryLabelType->setText(QString("%1").arg(rotaryDisplayType));
            rotaryLabelValue->setText(QString("%1").arg(rotaryDisplaySpeed));
        }
        else if(objectName.contains("DirectionEnable")) {
            rotaryLabelBank->setText(QString("%1").arg(rotaryDisplayBank));
            rotaryLabelType->setText(QString("%1").arg(rotaryDisplayType));
            rotaryLabelValue->setText("enabled"); //shows either "location enabled" or "direction enabled"
        }
        else if(objectName.contains("Channel") && !objectName.contains("In")){
            rotaryLabelBank->setText(QString("%1").arg(rotaryDisplayBank));
            rotaryLabelType->setText(QString("%1").arg(rotaryDisplayType));
            rotaryDisplayNonNote = numberToOffState(rotaryDisplayValue);
            rotaryLabelValue->setText(QString("%1").arg(rotaryDisplayNonNote));
        }
        else if(objectName.contains("Direction") && !objectName.contains("Enable")){
            if(directionEnabled){
                rotaryLabelBank->setText(QString("%1").arg(rotaryDisplayBank));
                rotaryLabelType->setText(QString("%1").arg(rotaryDisplayType));
                rotaryDisplayNonNote = numberToOffState(rotaryDisplayValue);
                rotaryLabelValue->setText(QString("%1").arg(rotaryDisplayNonNote));
            }
            else {
                rotaryLabelBank->setText(QString("%1").arg(rotaryDisplayBank));
                rotaryLabelType->setText(QString("Direction"));
                rotaryLabelValue->setText(QString("off"));
            }
        }
        else if(objectName.contains("Location") && !objectName.contains("inLocation")){
            if(directionEnabled){
                rotaryLabelBank->setText(QString("%1").arg(rotaryDisplayBank));
                rotaryLabelType->setText(QString("Location"));
                rotaryLabelValue->setText(QString("off"));
            }
            else {
                rotaryLabelBank->setText(QString("%1").arg(rotaryDisplayBank));
                rotaryLabelType->setText(QString("%1").arg(rotaryDisplayType));
                rotaryDisplayNonNote = numberToOffState(rotaryDisplayValue);
                rotaryLabelValue->setText(QString("%1").arg(rotaryDisplayNonNote));
            }
        }
        else if(objectName.contains("PassThru")){
            rotaryLabelBank->setText(QString("%1").arg(rotaryDisplayBank));
            rotaryDisplayType = QString("Pass Thru Width");
            rotaryLabelType->setText(QString("%1").arg(rotaryDisplayType));
            rotaryDisplayNonNote = numberToOffState(rotaryDisplayValue);
            rotaryLabelValue->setText(QString("%1").arg(rotaryDisplayNonNote));
        }
        else if (!objectName.contains("NotePressMode")){
            rotaryDisplayNonNote = numberToOffState(rotaryDisplayValue);
            rotaryLabelBank->setText(QString("%1").arg(rotaryDisplayBank));
            rotaryLabelType->setText(QString("%1").arg(rotaryDisplayType));
            rotaryLabelValue->setText(QString("%1").arg(rotaryDisplayNonNote));
        }
        if (objectName.contains("InChannel")){
            rotaryLabelBank->hide();
            rotaryLabelType->setText("LED Channel");
            rotaryDisplayValue = presetValues->value(QString("Preset %1").arg(preset)).toMap().value
                    ("ComponentSettings").toMap().value("Rotaries").toMap().value(objectName).toInt();
            rotaryLabelValue->setText(QString("%1").arg(rotaryDisplayValue + 1));
        }
        if(objectName.contains("Threshold")){
            rotaryLabelBank->hide();
            rotaryLabelValue->setText(QString("%1").arg(presetValues->value(QString("Preset %1").arg(preset)).toMap().value("ComponentSettings")
                                                         .toMap().value("Rotaries").toMap().value(objectName).toInt()));
            if(objectName.contains("On")){
                rotaryLabelType->setText("On Threshold");
            }else{
                rotaryLabelType->setText("Off Threshold");
            }
            rotaryLabelValue->show();
            rotaryLabelType->show();
        }
        else{
            if(!objectName.contains("In")){
            rotaryLabelBank->show();
            }
        }
    }
}

QString RotaryLabels::numberSpeedFloat(int numberSpeed) {
    double floatSpeed = double(numberSpeed);
    QString speedString;

    if(floatSpeed < 0) {
        speedString = QString("off");
    }
    else {
        speedString = QString("%1").arg(floatSpeed);
    }
    return QString("%1").arg(speedString);
}

QString RotaryLabels::numberToOffState(int numberToOff) {
    QString offState;
    if(numberToOff == -1) {
        offState = QString("off");
    }
    else {
        offState = QString("%1").arg(numberToOff);
    }
    return QString("%1").arg(offState);
}

//to convert midi note number to note name where note 60 = C3.
QString RotaryLabels::noteNumberToName(int noteNumber) {
    int pitchClass = noteNumber % 12; //modulo 12 gives remainder 0-11 which tells you to scale degree in key of C.
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
        octNumber = (noteNumber / 12) -2; //find octave by using integers and truncating
        noteLetter = QString("%1%2").arg(noteLetter).arg(octNumber);  //formag note letter with octave number
    }
    return QString("%1").arg(noteLetter); //return as a QString
}
