#include "hsliderlabels.h"

HSliderLabels::HSliderLabels(QWidget *mainWindowAddress, QVariantMap *hSliderPresetValues, QWidget *parent) :
    QWidget(parent)
{
    //this->setStyleSheet("QLabel { font: 10px } QLineEdit { font: 12px } QCheckBox {font:9px} QSpinBox {font: 10px} QDoubleSpinBox {font: 10px}");

    presetValues = hSliderPresetValues;
    mainWindow = mainWindowAddress;

}

//displays a parameter's value on a given component's label.
void HSliderLabels::slotDisplayParameter(QString objectName, int componentNum, int preset)
{
    //now get this slider's value for that object
    hSliderDisplayValue = presetValues->value(QString("Preset %1").arg(preset)).toMap().value("ComponentSettings").toMap().value
            ("HSliders").toMap().value(QString("HSlider%1").arg(componentNum)).toMap().value(objectName).toInt();

    hSliderDisplayBank = objectName;    //QString to hold the bank number
    hSliderDisplayBank = hSliderDisplayBank.remove(0,1);    //remove the 'h' prefix
    hSliderDisplayBank = hSliderDisplayBank.remove(2,hSliderDisplayBank.length());  //remove everything after 'B#'
    hSliderDisplayType = objectName;//QString to hold the parameter type (channel, note, pressure) or its abbreviation
    //qDebug() << QString("object: %1, component: %2, preset: %3").arg(objectName).arg(componentNum).arg(preset);
    //do nothing if sensitivity clicked on.
    if((!hSliderDisplayType.contains("Sensitivity")) && (!hSliderDisplayType.contains("LocalLEDControl")) && (!hSliderDisplayType.contains("Threshold"))) {
        if(hSliderDisplayType.contains("out")) {
             hSliderDisplayType = QString("%1").arg(hSliderDisplayType.remove(0, 6));
             if(hSliderDisplayType.contains("PassThru")){
                 hSliderDisplayType = QString("Pass Thru");
             }
             if(objectName.contains("NotePressMode")){
                 if (hSliderDisplayValue == 0){
                     hSliderDisplayType = QString("Normal");
                 }
                 if (hSliderDisplayValue == 1){
                     hSliderDisplayType = QString("Toggle");
                 }
             }
        }
        else if(hSliderDisplayType.contains("Channel") && !hSliderDisplayType.contains("In")){
            hSliderDisplayType = QString("Channel"); //otherwise it contains the string in and is reformatted to say LED.
            hSliderDisplayValue = (hSliderDisplayValue + 1);
        }
    }
    //find this slider's display label
    hSliderLabelBank = mainWindow->findChild<QLabel *>(QString("hSlider%1DisplayLabelBank").arg(componentNum));
    hSliderLabelType = mainWindow->findChild<QLabel *>(QString("hSlider%1DisplayLabelType").arg(componentNum));
    hSliderLabelValue = mainWindow->findChild<QLabel *>(QString("hSlider%1DisplayValue").arg(componentNum));

#ifdef Q_OS_MAC

#else
    hSliderLabelBank->setStyleSheet("QLabel {font:10px}");
    hSliderLabelType->setStyleSheet("QLabel {font:10px}");
    hSliderLabelValue->setStyleSheet("QLabel {font:10px}");
#endif

    //if it isn't sensitivity, set the text to be in the format Bank#: ParameterType Value (for sensitivity do nothing)
    if((!hSliderDisplayType.contains("Sensitivity")) && (!hSliderDisplayType.contains("LocalLEDControl"))) {
        if(objectName.contains("Note") && !objectName.contains("NotePressMode")) {    //if type is Note, convert note number to note name. Then display text.
            hSliderLabelBank->setText(QString("%1").arg(hSliderDisplayBank));
            hSliderLabelType->setText(QString("%1").arg(hSliderDisplayType));
            hSliderLabelValue->setText(QString("%1").arg(noteNumberToName(hSliderDisplayValue)));
        }
        else if(objectName.contains("Threshold")){
            hSliderLabelBank->hide();
            if(objectName.contains("OnThreshold")){
                hSliderDisplayType = QString("On Threshold");
            }
            if(objectName.contains("OffThreshold")){
                hSliderDisplayType = QString("Off Threshold");
            }
            hSliderLabelType->setText(QString("%1").arg(hSliderDisplayType));
            hSliderDisplayValue = presetValues->value(QString("Preset %1").arg(preset)).toMap().value("ComponentSettings").toMap().value
                    ("HSliders").toMap().value(QString(objectName)).toInt();

            hSliderLabelValue->setText(QString("%1").arg(numberToOffState(hSliderDisplayValue)));
            hSliderLabelValue->show();
        }
        else {
            hSliderLabelBank->setText(QString("%1").arg(hSliderDisplayBank));
            hSliderLabelType->setText(QString("%1").arg(hSliderDisplayType));
            hSliderLabelValue->setText(QString("%1").arg(numberToOffState(hSliderDisplayValue)));
        }
        if(objectName.contains("InChannel")){
                    hSliderLabelBank->hide();
                    hSliderLabelType->setText(QString("LED Channel:"));
                    hSliderDisplayValue = presetValues->value(QString("Preset %1").arg(preset)).toMap().value("ComponentSettings").toMap().value
                            ("HSliders").toMap().value(objectName).toInt();
                    hSliderLabelValue->setText(QString("%1").arg(numberToOffState(hSliderDisplayValue + 1)));

        }
    }
    if(!objectName.contains("InChannel") && !objectName.contains("VelocityValue") && !objectName.contains("PressValue") ){
        hSliderLabelBank->show();
        hSliderLabelType->show();
        hSliderLabelValue->show();
    }
    if(objectName.contains("NotePressMode")){
        hSliderLabelValue->hide();
    }
    else{
        hSliderLabelValue->show();
    }
    if(objectName.contains("Velocity")) {
         hSliderLabelBank->setText(QString("%1").arg(hSliderDisplayBank));
         if(hSliderDisplayValue > 0) {
             hSliderLabelType->setText("Fix Velocity:");
             hSliderLabelValue->show();
             hSliderLabelValue->setText(QString("%1").arg(numberToOffState(hSliderDisplayValue)));
         }
         else {
             hSliderLabelType->setText("Variable Vel.");
             hSliderLabelValue->hide();
         }
     }
    if(objectName.contains("PressValue")) {
         hSliderLabelBank->setText(QString("%1").arg(hSliderDisplayBank));
         if(hSliderDisplayValue > 0) {
             hSliderLabelType->setText("Fix Pressure:");
             hSliderLabelValue->show();
             hSliderLabelValue->setText(QString("%1").arg(numberToOffState(hSliderDisplayValue)));
         }
         else {
             hSliderLabelType->setText("Continuous Pr.");
             hSliderLabelValue->hide();
         }
     }
}

//sets -1 to display as "off" instead for all non note parameters (off state for note parameter is dealt with in its own function below)
QString HSliderLabels::numberToOffState(int numberToOff)
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
QString HSliderLabels::noteNumberToName(int noteNumber)
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
