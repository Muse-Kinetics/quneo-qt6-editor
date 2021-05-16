#include "rhombuslabels.h"

RhombusLabels::RhombusLabels(QWidget *mainWindowAddress, QVariantMap *rhombusPresetValues, QWidget *parent) :
    QWidget(parent)
{
    presetValues = rhombusPresetValues;
    mainWindow = mainWindowAddress;
}

//displays a parameter's value on a given component's label
void RhombusLabels::slotDisplayParameter(QString objectName, int componentNum, int preset)
{
    //find the button's display labels
    rhombusDisplay = mainWindow->findChild<QLabel *>(QString("rhombus%1Display").arg(componentNum));

#ifdef Q_OS_MAC

#else
rhombusDisplay->setStyleSheet("QLabel {font:10px;}");
#endif



    //get object's value for this button
    rhombusDisplayValue = presetValues->value(QString("Preset %1").arg(preset)).toMap().value("ComponentSettings").toMap().value
            ("RhombusButtons").toMap().value(QString("RhombusButton%1").arg(componentNum)).toMap().value(objectName).toInt();

    //if type is not Sensitivity or Local LED Control
    if((!objectName.contains("Sensitivity")) && (!objectName.contains("LocalLEDControl"))) {

        //if type is note, convert note number to note name. then display text
        if(objectName.contains("Note") && !objectName.contains("Mode")) {
            rhombusDisplay->setText(QString("%1").arg(noteNumberToName(rhombusDisplayValue)));
        }
        else if(objectName.contains("Velocity")) {
            if(rhombusDisplayValue > 0) {
                rhombusDisplay->setText(QString("%1").arg(rhombusDisplayValue));
            }
            else {
                rhombusDisplay->setText("Vary");
            }
        }
        else if(objectName.contains("PressValue")) {
            if(rhombusDisplayValue > 0) {
                rhombusDisplay->setText(QString("%1").arg(rhombusDisplayValue));
            }
            else {
                rhombusDisplay->setText("Cont");
            }
        }
        else if(objectName.contains("Mode")) {
            if(rhombusDisplayValue){
                rhombusDisplay->setText("Tog");
            }
            else{
                rhombusDisplay->setText("Norm");
            }
        }
        //show the enable bank switching parameter
        else if(objectName.contains("Switch")) {
            if(rhombusDisplayValue) {
                rhombusDisplay->setText("on");
            }
            else {
                rhombusDisplay->setText("off");
            }
        }

        //show the bank control parameter
        else if(objectName.contains("Control")) {
            int bankControlEnabled = presetValues->value(QString("Preset %1").arg(preset)).toMap().value("ComponentSettings").toMap().value
                    ("RhombusButtons").toMap().value(QString("RhombusButton0")).toMap().value("rhombusEnableSwitch").toInt();
            if(rhombusDisplayValue == 0 && bankControlEnabled) {
                rhombusDisplay->setText("R");
            }
            else if(rhombusDisplayValue == 1 && bankControlEnabled) {
                rhombusDisplay->setText("V");
            }
            else if(rhombusDisplayValue == 2 && bankControlEnabled) {
                rhombusDisplay->setText("L");
            }
            else if(rhombusDisplayValue == 3 && bankControlEnabled) {
                rhombusDisplay->setText("P");
            }
            else if(!bankControlEnabled){
                rhombusDisplay->setText("off");
            }
        }
        else if(objectName.contains("Channel") && !objectName.contains("In")){
            rhombusDisplay->setText(QString("%1").arg(numberToOffState(rhombusDisplayValue + 1)));
        }
        else if(objectName.contains("Channel") && objectName.contains("In")){
            rhombusDisplayValue = presetValues->value(QString("Preset %1").arg(preset)).toMap().value("ComponentSettings").toMap().value
                    ("RhombusButtons").toMap().value(objectName).toInt();
            rhombusDisplay->setText(QString("%1").arg(numberToOffState(rhombusDisplayValue + 1)));
        }
        else {
            rhombusDisplay->setText(QString("%1").arg(numberToOffState(rhombusDisplayValue)));
        }
    }
}

//sets -1 display as "off instead for all non note parameters
QString RhombusLabels::numberToOffState(int numberToOff)
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

//to convert MIDI note number to note name where note 60 = C3
QString RhombusLabels::noteNumberToName(int noteNumber)
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
