#include "leftrightlabels.h"

LeftrightLabels::LeftrightLabels(QWidget *mainWindowAddress, QVariantMap *leftrightPresetValues, QWidget *parent) :
    QWidget(parent)
{
    presetValues = leftrightPresetValues;
    mainWindow = mainWindowAddress;
}

//displays a parameter's value on a given component's label
void LeftrightLabels::slotDisplayParameter(QString objectName, int componentNum, int preset)
{
    //get this button's value for that object
    //leftrightDisplayValue = presetValues->value(QString("Preset %1").arg(preset)).toMap().value("ComponentSettings").toMap().value
    //        ("LeftRightButtons").toMap().value(QString("LeftRightButton%1").arg(componentNum)).toMap().value(objectName).toInt();

    //find the button's display labels
    leftrightLDisplay = mainWindow->findChild<QLabel *>(QString("leftright%1LDisplay").arg(componentNum));
    leftrightRDisplay = mainWindow->findChild<QLabel *>(QString("leftright%1RDisplay").arg(componentNum));

#ifdef Q_OS_MAC

#else
    leftrightLDisplay->setStyleSheet("QLabel {font:10px; color: white;}");
    leftrightRDisplay->setStyleSheet("QLabel {font:10px; color: white}");
#endif


    //get object's value for this button
    leftrightDisplayValue = presetValues->value(QString("Preset %1").arg(preset)).toMap().value("ComponentSettings").toMap().value
            ("LeftRightButtons").toMap().value(QString("LeftRightButton%1").arg(componentNum)).toMap().value(objectName).toInt();
    leftrightRDisplayValue = presetValues->value(QString("Preset %1").arg(preset)).toMap().value("ComponentSettings").toMap().value
            ("LeftRightButtons").toMap().value(QString("LeftRightButton%1").arg(componentNum)).toMap().value(QString("leftrightR%1").arg(objectName.remove(0,10))).toInt();
    leftrightLDisplayValue = presetValues->value(QString("Preset %1").arg(preset)).toMap().value("ComponentSettings").toMap().value
            ("LeftRightButtons").toMap().value(QString("LeftRightButton%1").arg(componentNum)).toMap().value(QString("leftrightL%1").arg(objectName)).toInt();

    //if type is not Sensitivity or Local LED Control
    if((!objectName.contains("ensitivity")) && (!objectName.contains("LEDControl"))) {
        //if type is note, convert note number to note name. then display text
        if(objectName.contains("Note") && !objectName.contains("Mode")) {
            leftrightRDisplay->setText(QString("%1").arg(noteNumberToName(leftrightRDisplayValue)));
            leftrightLDisplay->setText(QString("%1").arg(noteNumberToName(leftrightLDisplayValue)));
        }
        else if(objectName.contains("Velocity")) {
            if(leftrightRDisplayValue > 0) {
                leftrightRDisplay->setText(QString("%1").arg(leftrightRDisplayValue));
                if(leftrightLDisplayValue > 0) {
                    leftrightLDisplay->setText(QString("%1").arg(leftrightLDisplayValue));
                }
                else {
                    leftrightLDisplay->setText("Vary");
                }
            }
            else {
                leftrightRDisplay->setText("Vary");
                if(leftrightLDisplayValue > 0) {
                    leftrightLDisplay->setText(QString("%1").arg(leftrightLDisplayValue));
                }
                else {
                    leftrightLDisplay->setText("Vary");
                }
            }
        }
        else if(objectName.contains("PressValue")) {
            if(leftrightRDisplayValue > 0) {
                leftrightRDisplay->setText(QString("%1").arg(leftrightRDisplayValue));
                if(leftrightLDisplayValue > 0) {
                    leftrightLDisplay->setText(QString("%1").arg(leftrightLDisplayValue));
                }
                else {
                    leftrightLDisplay->setText("Cont");
                }
            }
            else {
                leftrightRDisplay->setText("Cont");
                if(leftrightLDisplayValue > 0) {
                    leftrightLDisplay->setText(QString("%1").arg(leftrightLDisplayValue));
                }
                else {
                    leftrightLDisplay->setText("Cont");
                }
            }
        }
        //show the Enable Bank Switching parameter
        else if(objectName.contains("Switch")) {
            if(leftrightDisplayValue) {
                leftrightLDisplay->setText("on");
                leftrightRDisplay->setText("on");
            }
            else {
                leftrightLDisplay->setText("off");
                leftrightRDisplay->setText("off");
            }
        }
        else if(objectName.contains("hannel") && !objectName.contains("nChannel")){
            leftrightRDisplay->setText(QString("%1").arg(numberToOffState(leftrightDisplayValue + 1)));
            leftrightLDisplay->setText(QString("Ch:"));
        }
        else if(objectName.contains("hannel") && objectName.contains("nChannel")){
            leftrightDisplayValue = presetValues->value(QString("Preset %1").arg(preset)).toMap().value("ComponentSettings").toMap().value
                    ("LeftRightButtons").toMap().value("leftrightInChannel").toInt();
            leftrightRDisplay->setText(QString("%1").arg(numberToOffState(leftrightDisplayValue + 1)));
            leftrightLDisplay->setText(QString("LED:"));
        }
        else if(objectName.contains("Mode")){
            if(leftrightRDisplayValue){
                leftrightRDisplay->setText("Tog");
            }
            else{
                leftrightRDisplay->setText("Norm");
            }
            if(leftrightLDisplayValue){
                leftrightLDisplay->setText("Tog");
            }
            else{
                leftrightLDisplay->setText("Norm");
            }
        }
        else {
            leftrightRDisplay->setText(QString("%1").arg(numberToOffState(leftrightRDisplayValue)));
            leftrightLDisplay->setText(QString("%1").arg(numberToOffState(leftrightLDisplayValue)));
        }

    }
}

//sets -1 to display as "off" instead for all non note parameters
QString LeftrightLabels::numberToOffState(int numberToOff)
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
QString LeftrightLabels::noteNumberToName(int noteNumber)
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
