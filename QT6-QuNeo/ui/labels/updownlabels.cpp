#include "updownlabels.h"

UpdownLabels::UpdownLabels(QWidget *mainWindowAddress, QVariantMap *updownPresetValues, QWidget *parent) :
    QWidget(parent)
{
    presetValues = updownPresetValues;
    mainWindow = mainWindowAddress;
}

//displays a parameter's value on a given component's label
void UpdownLabels::slotDisplayParameter(QString objectName, int componentNum, int preset)
{
    //find the button's display labels
    updownUDisplay = mainWindow->findChild<QLabel *>(QString("updown%1UDisplay").arg(componentNum));
    updownDDisplay = mainWindow->findChild<QLabel *>(QString("updown%1DDisplay").arg(componentNum));

#ifdef Q_OS_MAC

#else
    updownUDisplay->setStyleSheet("QLabel {font:10px;}");
    updownDDisplay->setStyleSheet("QLabel {font:10px;}");
#endif




    //get object's value for this button
    updownDisplayValue = presetValues->value(QString("Preset %1").arg(preset)).toMap().value("ComponentSettings").toMap().value
            ("UpDownButtons").toMap().value(QString("UpDownButton%1").arg(componentNum)).toMap().value(objectName).toInt();
    updownUDisplayValue = presetValues->value(QString("Preset %1").arg(preset)).toMap().value("ComponentSettings").toMap().value
            ("UpDownButtons").toMap().value(QString("UpDownButton%1").arg(componentNum)).toMap().value(QString("updownU%1").arg(objectName.remove(0,7))).toInt();
    updownDDisplayValue = presetValues->value(QString("Preset %1").arg(preset)).toMap().value("ComponentSettings").toMap().value
            ("UpDownButtons").toMap().value(QString("UpDownButton%1").arg(componentNum)).toMap().value(QString("updownD%1").arg(objectName)).toInt();

    //if type is not Sensitivity or Local LED control
    if((!objectName.contains("ensitivity")) && (!objectName.contains("LEDControl"))) {

        //if type is note, convert note number to note name. then display text
        if(objectName.contains("Note") && !objectName.contains("Mode")) {
            updownUDisplay->setText(QString("%1").arg(noteNumberToName(updownUDisplayValue)));
            updownDDisplay->setText(QString("%1").arg(noteNumberToName(updownDDisplayValue)));
        }
        else if(objectName.contains("Mode")) {
            if(updownDDisplayValue){
            updownDDisplay->setText(QString("Tog"));
            }
            else{
                updownDDisplay->setText(QString("Norm"));
            }
            if(updownUDisplayValue){
            updownUDisplay->setText(QString("Tog"));
            }
            else{
                updownUDisplay->setText(QString("Norm"));
            }
        }
        else if(objectName.contains("Velocity")) {
            if(updownUDisplayValue > 0) {
                updownUDisplay->setText(QString("%1").arg(updownUDisplayValue));
                if(updownDDisplayValue > 0) {
                    updownDDisplay->setText(QString("%1").arg(updownDDisplayValue));
                }
                else {
                    updownDDisplay->setText("Vary");
                }
            }
            else {
                updownUDisplay->setText("Vary");
                if(updownDDisplayValue > 0) {
                    updownDDisplay->setText(QString("%1").arg(updownDDisplayValue));
                }
                else {
                    updownDDisplay->setText("Vary");
                }
            }
        }
        else if(objectName.contains("PressValue")) {
            if(updownUDisplayValue > 0) {
                updownUDisplay->setText(QString("%1").arg(updownUDisplayValue));
                if(updownDDisplayValue > 0) {
                    updownDDisplay->setText(QString("%1").arg(updownDDisplayValue));
                }
                else {
                    updownDDisplay->setText("Cont");
                }
            }
            else {
                updownUDisplay->setText("Cont");
                if(updownDDisplayValue > 0) {
                    updownDDisplay->setText(QString("%1").arg(updownDDisplayValue));
                }
                else {
                    updownDDisplay->setText("Cont");
                }
            }
        }

        //show the Enable Bank Switching Parameter
        else if(objectName.contains("Switch")) {
            if(updownDisplayValue) {
                updownUDisplay->setText("on");
                updownDDisplay->setText("on");
            }
            else {
                updownUDisplay->setText("off");
                updownDDisplay->setText("off");
            }
        }

        else if(objectName.contains("hannel")){
            if(objectName.contains("nChannel")){
                updownDisplayValue = presetValues->value(QString("Preset %1").arg(preset)).toMap().value("ComponentSettings").toMap().value
                        ("UpDownButtons").toMap().value("updownInChannel").toInt();
                updownDDisplay->setText(QString("%1").arg(numberToOffState(updownDisplayValue + 1)));
                updownUDisplay->setText(QString("LED:"));
            }
            else{
            updownDDisplay->setText(QString("%1").arg(numberToOffState(updownDisplayValue + 1)));
            updownUDisplay->setText(QString("Ch:"));
            }
        }
        //show the Bank Control parameter
        else if(objectName.contains("ankControl")) {
            int bankControlEnabled = presetValues->value(QString("Preset %1").arg(preset)).toMap().value("ComponentSettings").toMap().value
                    ("UpDownButtons").toMap().value(QString("UpDownButton%1").arg(componentNum)).toMap().value("updownEnableSwitch").toInt();
            if(updownDisplayValue == 0 && bankControlEnabled) {
                updownUDisplay->setText("R");
                updownDDisplay->setText("R");
            }
            else if(updownDisplayValue == 1 && bankControlEnabled) {
                updownUDisplay->setText("V");
                updownDDisplay->setText("V");
            }
            else if(updownDisplayValue == 2 && bankControlEnabled) {
                updownUDisplay->setText("L");
                updownDDisplay->setText("L");
            }
            else if(updownDisplayValue == 3 && bankControlEnabled) {
                updownUDisplay->setText("P");
                updownDDisplay->setText("P");
            }
            else if(!bankControlEnabled){
                updownUDisplay->setText("off");
                updownDDisplay->setText("off");
            }
        }

        else {
            updownUDisplay->setText(QString("%1").arg(numberToOffState(updownUDisplayValue)));
            updownDDisplay->setText(QString("%1").arg(numberToOffState(updownDDisplayValue)));
        }
    }
}

//sets -1 to display as "off" instead for all non note parameters
QString UpdownLabels::numberToOffState(int numberToOff)
{
    QString offState;
    if(numberToOff == -1) {
        offState = QString("off");
    }
    else {
        offState = QString("%1").arg(numberToOff);
    }
    return QString("%1").arg(offState);
}

//to convert MIDI note number to note name where note 60 = C3.
QString UpdownLabels::noteNumberToName(int noteNumber)
{
    int pitchClass = noteNumber % 12;  //modulo 12 gives remainder 0-11 which tells you scale degree in key of C
    QString noteLetter; //create space for corresponding letter

    //if the note number is -1 display off
    if(noteNumber == -1) {
        noteLetter = QString("off");
    }

    //otherwise make the numbers display as letters
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
        noteLetter = QString("%1%2").arg(noteLetter).arg(octNumber);  //format note letter with octave number
    }
    return QString("%1").arg(noteLetter);  //return as a QString
}
