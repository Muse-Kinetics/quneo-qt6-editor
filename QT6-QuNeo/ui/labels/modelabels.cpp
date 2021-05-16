#include "modelabels.h"

ModeLabels::ModeLabels(QWidget *mainWindowAddress, QVariantMap *modePresetValues, QWidget *parent) :
    QWidget(parent)
{
    presetValues = modePresetValues;
    mainWindow = mainWindowAddress;
}

//displays a parameter's value on a given component's label
void ModeLabels::slotDisplayParameter(QString objectName, int componentNum, int preset)
{
    //find the button's display labels
    modeDisplay = mainWindow->findChild<QLabel *>(QString("mode%1Display").arg(componentNum));

    //get object's value for this button
    modeDisplayValue = presetValues->value(QString("Preset %1").arg(preset)).toMap().value("ComponentSettings").toMap().value
            ("ModeButtons").toMap().value(QString("ModeButton%1").arg(componentNum)).toMap().value(objectName).toInt();

    //if type is not Sensitivity or Local LED Control
    if((!objectName.contains("Sensitivity")) && (!objectName.contains("LocalLEDControl"))) {

        //if type is note, convert note number to note name, then display.
        if(objectName.contains("Note")) {
            modeDisplay->setText(QString("%1").arg(noteNumberToName(modeDisplayValue)));
        }
        else if(objectName.contains("Velocity")) {
            if(modeDisplayValue) {
                modeDisplay->setText("on");
            }
            else {
                modeDisplay->setText("fixed");
            }
        }

        //show the enable MIDI parameter
        else if(objectName.contains("Switch")) {
            if(modeDisplayValue) {
                modeDisplay->setText("on");
            }
            else {
                modeDisplay->setText("off");
            }
        }

        else {
            modeDisplay->setText(QString("%1").arg(numberToOffState(modeDisplayValue)));
        }
    }
}

//sets -1 to display as "off" instead for all non note parameters
QString ModeLabels::numberToOffState(int numberToOff)
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
QString ModeLabels::noteNumberToName(int noteNumber)
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
