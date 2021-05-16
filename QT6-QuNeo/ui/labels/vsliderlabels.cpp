#include "vsliderlabels.h"

VSliderLabels::VSliderLabels(QWidget *mainWindowAddress, QVariantMap *vSliderPresetValues, QWidget *parent) :
    QWidget(parent)
{
    presetValues = vSliderPresetValues;
    mainWindow = mainWindowAddress;
}

//displays a parameter's value on a given component's label.
void VSliderLabels::slotDisplayParameter(QString objectName, int componentNum, int preset)
{
    //now get this slider's value for that object
    vSliderDisplayValue = presetValues->value(QString("Preset %1").arg(preset)).toMap().value("ComponentSettings").toMap().value
            ("VSliders").toMap().value(QString("VSlider%1").arg(componentNum)).toMap().value(objectName).toInt();
    vSliderDisplayBank = objectName;    //QString to hold the bank number
    vSliderDisplayBank = vSliderDisplayBank.remove(0,1);    //remove the 'v' prefix
    vSliderDisplayBank = vSliderDisplayBank.remove(2,vSliderDisplayBank.length());  //remove everything after 'B#'
    vSliderDisplayType = objectName;//QString to hold the parameter type (channel, note, pressure) or its abbreviation
    //qDebug() << QString("objectName: %1").arg(vSliderDisplayType);
    //do nothing if sensitivity clicked on.
    if((!objectName.contains("Sensitivity")) && (!objectName.contains("LocalLEDControl"))) {
        if(objectName.contains("out") || objectName.contains("Channel")) {
             vSliderDisplayType = QString("%1").arg(vSliderDisplayType.remove(0, 6));
             if (objectName.contains("Press") && !objectName.contains("Value") && !objectName.contains("Mode")) {
                 vSliderDisplayType = QString("Pcc");
             }
             if (objectName.contains("PressValue")){
                 vSliderDisplayType = QString("Pval");
             }
             if (objectName.contains("PassThru")){
                 vSliderDisplayType = QString("Pt");
             }
             if (objectName.contains("Location")) {
                 vSliderDisplayType = QString("Lo");
             }
             if (objectName.contains("Note") && !objectName.contains("PressMode")) {
                 vSliderDisplayType = QString("No");
             }
             if(objectName.contains("Mode")){
                 if(vSliderDisplayValue){
                 vSliderDisplayType = QString("T");
                 }
                 else{
                 vSliderDisplayType = QString("N");
                 }
             }
             if (objectName.contains("Channel") && !objectName.contains("In")) {
                 vSliderDisplayType = QString("Ch");
                 vSliderDisplayValue = (vSliderDisplayValue + 1);
             }

             if (objectName.contains("Channel") && objectName.contains("In")) {
                 vSliderDisplayType = QString("LED");
                 vSliderDisplayValue = presetValues->value(QString("Preset %1").arg(preset)).toMap().value("ComponentSettings").toMap().value
                         ("VSliders").toMap().value(objectName).toInt();
                 vSliderDisplayValue = (vSliderDisplayValue + 1);
             }
             if (objectName.contains("Velocity")) {
                 vSliderDisplayType = QString("Ve");
             }
        }
        //display parameter name by taking off vB#out
        else {
            //qDebug() << QString("%1").arg(vSliderDisplayType);
            vSliderDisplayType = QString("LED"); //otherwise it contains the string in and is reformatted to say LED.
        }
    }

    //find this slider's display label
    vSliderLabelBank = mainWindow->findChild<QLabel *>(QString("vSlider%1DisplayLabelBank").arg(componentNum));
    vSliderLabelType = mainWindow->findChild<QLabel *>(QString("vSlider%1DisplayLabelType").arg(componentNum));
    vSliderLabelValue = mainWindow->findChild<QLabel *>(QString("vSlider%1DisplayValue").arg(componentNum));

#ifdef Q_OS_MAC

#else
    vSliderLabelBank->setStyleSheet("QLabel {font:10px;}");
    vSliderLabelType->setStyleSheet("QLabel {font:10px;}");
    vSliderLabelValue->setStyleSheet("QLabel {font:10px;}");
#endif

    //if it isn't sensitivity, set the text to be in the format Bank#: ParameterType Value (for sensitivity do nothing)
    if((!objectName.contains("Sensitivity")) && (!objectName.contains("LocalLEDControl"))) {
        if(objectName.contains("Note") && !objectName.contains("PressMode")) {    //if type is Note, convert note number to note name. Then display text.
            vSliderLabelBank->setText(QString("%1").arg(vSliderDisplayBank));
            vSliderLabelType->setText(QString("%1").arg(vSliderDisplayType));
            vSliderLabelValue->setText(QString("%1").arg(noteNumberToName(vSliderDisplayValue)));
        }
        else if(objectName.contains("Channel") && !objectName.contains("In")){
            vSliderDisplayType = QString("Ch");
        }
        else if(objectName.contains("Channel") && objectName.contains("In")){
            vSliderLabelBank->hide();
        }
        if(!objectName.contains("Note")) {
            vSliderLabelBank->setText(QString("%1").arg(vSliderDisplayBank));
            vSliderLabelType->setText(QString("%1").arg(vSliderDisplayType));
            vSliderLabelValue->setText(QString("%1").arg(numberToOffState(vSliderDisplayValue)));
        }
        if(objectName.contains("PressMode")){
            vSliderLabelBank->setText(QString("%1").arg(vSliderDisplayBank));
            vSliderLabelType->setText(QString("%1").arg(vSliderDisplayType));
            vSliderLabelValue->hide();
        }
        if(objectName.contains("Threshold")){
            vSliderLabelBank->hide();
            vSliderLabelValue->setText(QString("%1").arg(presetValues->value(QString("Preset %1").arg(preset)).toMap().value("ComponentSettings")
                                                         .toMap().value("VSliders").toMap().value(objectName).toInt()));
            if(objectName.contains("On")){
                vSliderLabelType->setText("On");
            }else{
                vSliderLabelType->setText("Off");
            }
            vSliderLabelValue->show();
            vSliderLabelType->show();
        }
        else{
            vSliderLabelValue->show();
        }
        if((objectName.contains("PressValue"))) {
            vSliderLabelBank->setText(QString("%1").arg(vSliderDisplayBank));
            vSliderLabelType->setText(QString("%1").arg(vSliderDisplayType));
            if(vSliderDisplayValue < 1){
                vSliderLabelValue->setText("C");
            }
            else{
                vSliderLabelValue->setText(QString("%1").arg(numberToOffState(vSliderDisplayValue)));
            }
        }
        if(objectName.contains("Velocity")) {
            vSliderLabelBank->setText(QString("%1").arg(vSliderDisplayBank));
            vSliderLabelType->setText(QString("%1").arg(vSliderDisplayType));
            if(vSliderDisplayValue < 1){
                vSliderLabelValue->setText("V");
            }
            else{
                vSliderLabelValue->setText(QString("%1").arg(numberToOffState(vSliderDisplayValue)));
            }
        }
        if(!objectName.contains("InChannel") && !objectName.contains("Threshold")){
            vSliderLabelBank->show();
        }
    }
}


//sets -1 to display as "off" instead for all non note parameters (off state for note parameter is dealt with in its own function below)
QString VSliderLabels::numberToOffState(int numberToOff)
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

//to convert midi note number to note name where note 60 = C3.
QString VSliderLabels::noteNumberToName(int noteNumber)
{
    int pitchClass = noteNumber % 12;   //modulo 12 gives remainder 0-11 which tells you scale degree in key of C.
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
        noteLetter = QString("%1%2").arg(noteLetter).arg(octNumber);   //format note letter with octave number
    }
    return QString("%1").arg(noteLetter); //return as a QString
}
