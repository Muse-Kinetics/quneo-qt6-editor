#include "padlabels.h"

PadLabels::PadLabels(QWidget *mainWindowAddress, QVariantMap *padPresetValues, QWidget *parent) :
    QWidget(parent)
{
    presetValues = padPresetValues;
    mainWindow = mainWindowAddress;
}

void PadLabels::slotDisplayParameter(QString objectName, int componentNum, int preset) {
    xVal = presetValues->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value("Pads").toMap().value(QString("Pad%1").arg(componentNumber)).toMap().value("outDmXCC").toInt();
    yVal = presetValues->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value("Pads").toMap().value(QString("Pad%1").arg(componentNumber)).toMap().value("outDmYCC").toInt();

    presetNumber = preset;
    componentNumber = componentNum;
    //check if this pad is in grid mode or not.
    gridMode = presetValues->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value("Pads").toMap().value(QString("Pad%1").arg(componentNumber)).toMap().value("enableGrid").toInt();
    //copy the object name into padGridString
    padGridString = objectName;

    padDisplayWidget = mainWindow->findChild<QWidget *>(QString("pad%1Display").arg(componentNumber));//find widget containing labels.
    padNWDisplay = padDisplayWidget->findChild<QLabel *>(QString("pad%1DisplayGmNW").arg(componentNumber));    //find the 4 corner labels
    padNEDisplay = padDisplayWidget->findChild<QLabel *>(QString("pad%1DisplayGmNE").arg(componentNumber));
    padSWDisplay = padDisplayWidget->findChild<QLabel *>(QString("pad%1DisplayGmSW").arg(componentNumber));
    padSEDisplay = padDisplayWidget->findChild<QLabel *>(QString("pad%1DisplayGmSE").arg(componentNumber));
    padDmDisplay = padDisplayWidget->findChild<QLabel *>(QString("pad%1DisplayDm").arg(componentNumber));      //find drum mode and channel param value display.
    padDisplayLabel = padDisplayWidget->findChild<QLabel *>(QString("pad%1DisplayLabel").arg(componentNumber));//find parameter type label displayed for drum and grid mode in middle.
    padDisplayLow = padDisplayWidget->findChild<QLabel *>(QString("pad%1DisplayDmLow").arg(componentNumber));
    padDmDisplay->move(4, 30);
    padDmDisplay->setFixedWidth(73);
    padNWDisplay->move(2, 4);
    padSWDisplay->move(2, 56);
    padNEDisplay->move(43, 4);
    padSEDisplay->move(43, 56);
    padNWDisplay->setFixedWidth(36);
    padNEDisplay->setFixedWidth(36);
    padSWDisplay->setFixedWidth(36);
    padSEDisplay->setFixedWidth(36);
    padDisplayLow->move(1, 50);
    padDisplayLow->setFixedWidth(79);
#ifdef Q_OS_MAC

#else
padDisplayWidget->setStyleSheet("QLabel {font:10px;}");
#endif





    if(objectName.contains("enableGrid")) {    //when enable switch is pressed, show all the pads' grid / drum mode state.
        //get object's value for this pad
        padDisplayValue = presetValues->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value("Pads").toMap().value(QString("Pad%1").arg(componentNumber)).toMap().value(objectName).toInt();

        if (padDisplayValue > 0) {
            padParamName = QString("Grid");
            padNWDisplay->hide();
            padNEDisplay->hide();
            padSWDisplay->hide();
            padSEDisplay->hide();
            padDmDisplay->hide();
            padDisplayLow->hide();
        }
        else {
            padParamName = QString("Drum");
            padDisplayLabel->move(8, 10);
            padNWDisplay->hide();
            padNEDisplay->hide();
            padSWDisplay->hide();
            padSEDisplay->hide();
            padDmDisplay->hide();
            padDisplayLow->hide();
        }
        padDisplayLabel->setText(padParamName);
        padDisplayLabel->show();
    }
    if(objectName.contains("Threshold") || objectName.contains("Isolation") || objectName.contains("PerPadUI")){
        slotDisplayDrum(objectName);
    }
    if((objectName.contains("Dm") && !gridMode) || (objectName.contains("InChannel") && !gridMode)) {
        slotDisplayDrum(objectName);
    }
    else if((objectName.contains("Gm") && gridMode) || (objectName.contains("InChannel") && gridMode)){
        slotDisplayGrid(objectName);
    }
    else if(objectName.contains("Dm") && gridMode){
        if(objectName.contains("X") || objectName.contains("Y") || objectName.contains("Press")){
            if(!objectName.contains("Return") && !objectName.contains("PressValue")){
            slotDisplayGrid(QString("outGmPressNW"));}
            if(objectName.contains("PressValue")){
                slotDisplayGrid(QString("outGmPressValueNW"));
            }
        }
        if(objectName.contains("out") && (objectName.contains("Note")) && !objectName.contains("NotePressMode")){
            slotDisplayGrid(QString("outGmNoteNW"));}
        if(objectName.contains("NotePressMode")){
            slotDisplayGrid(QString("outGmNotePressModeNW"));
        }
        if(objectName.contains("Velocity")){
            slotDisplayGrid(QString("outGmVelocityValueNW"));}
        if(objectName.contains("Channel") && !objectName.contains("InChannel")){
            slotDisplayGrid(QString("padChannel"));}
        if(objectName.contains("InChannel")){
            slotDisplayGrid(QString("padGridDualInChannel"));}
        if(objectName.contains("Return")){ //return is not relevant to grid mode. display mode instead.
            padParamName = QString("Grid");
            padNWDisplay->hide();
            padNEDisplay->hide();
            padSWDisplay->hide();
            padSEDisplay->hide();
            padDmDisplay->hide();
            padDisplayLow->hide();
            padDisplayLabel->setText(padParamName);
        }
    }
    else if(objectName.contains("Gm") && !gridMode){
        if(objectName.contains("Press") && !objectName.contains("PressValue")){
            slotDisplayDrum(QString("outDmPress"));}
        if(objectName.contains("PressValue")){
            slotDisplayDrum(QString("outDmPressValue"));
        }
        if(objectName.contains("out") && objectName.contains("Note") && !objectName.contains("NotePressMode")){
            slotDisplayDrum(QString("outDmNote"));}
        if(objectName.contains("NotePressMode")){
            slotDisplayDrum(QString("outDmNotePressMode"));
        }
        if(objectName.contains("Velocity")){
            slotDisplayDrum(QString("outDmVelocityValue"));}
        if(objectName.contains("InChannel")){
            slotDisplayDrum(QString("padDrumInChannel"));}
    }
    else if(objectName.contains("Channel") && !objectName.contains("InChannel")){
        slotDisplayChannel();
    }
    else if(objectName.contains("TransposeInterval")){
        slotDisplayTranspose(objectName, componentNum, preset);
    }
}

void PadLabels::slotDisplayDrum(QString objectName) {

        padDisplayLabel->move(8, 10);
    //get object's value for this pad
    padDisplayValue = presetValues->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value("Pads").toMap().value(QString("Pad%1").arg(componentNumber)).toMap().value(objectName).toInt();
    //if the object being modified is in drum mode...
    //find the parameter type name

    if(objectName.contains("InChannel") && !objectName.contains("Grid")) {
            padDisplayLabel->setText(QString("Drum Mode"));
            padDmDisplay->setText(QString("LED Channel"));
            padDisplayLow->setText(QString("%1").arg(presetValues->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value("Pads").toMap().value(objectName).toInt() + 1));
            padDisplayLow->show();
            padDisplayLabel->show();
            padDmDisplay->show();
    }
    else if(objectName.contains("out")) {

        //copy object name to format it
        padParamName = objectName;
        //remove outDm
        padParamName = padParamName.remove(0, 5);
        if (objectName.contains("Velocity")) {
            if(padDisplayValue > 0) {
                padDisplayLabel->setText("Fixed");
                padDmDisplay->setText("Velocity:");
                padDisplayLow->setText(QString("%1").arg(padDisplayValue));
                padDisplayLow->show();
            }
            else {
                padDisplayLabel->setText("Velocity");
                padDmDisplay->setText("Variable");
                padDisplayLow->hide();
            }
            padDisplayLabel->show();
            padDmDisplay->show();
        }
        else if (objectName.contains("PressValue")) {
            if(padDisplayValue > 0) {
                padDisplayLabel->setText("Fixed");
                padDmDisplay->setText("Pressure:");
                padDisplayLow->setText(QString("%1").arg(padDisplayValue));
                padDisplayLow->show();
            }
            else {
                padDisplayLabel->setText("Pressure");
                padDmDisplay->setText("Continuous");
                padDisplayLow->hide();
            }
            padDisplayLabel->show();
            padDmDisplay->show();
        }
        // use NoteNumberToName if it's a note parameter.
        else if ((objectName.contains("Note") && !objectName.contains("Mode"))) {
            padDmDisplay->setText(QString("%1").arg(NoteNumberToName(padDisplayValue)));
            padDmDisplay->show();
            padDisplayLabel->setText("Note");
            padDisplayLabel->show();
            padDisplayLow->hide();
        }
        else if (objectName.contains("Mode")) {
            padDmDisplay->setText(QString("%1").arg(padDisplayValue));
            padDmDisplay->hide();
            if(padDisplayValue == 1){
            padDisplayLabel->setText("Toggle");}
            if(padDisplayValue == 0){
            padDisplayLabel->setText("Normal");}
            padDisplayLabel->show();
            padDisplayLow->hide();
        }
        //display value of parameter
        else if (objectName.contains("Press") || objectName.contains("X") || objectName.contains("Y")){
            if (!objectName.contains("Return")){
                pressVal = presetValues->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value("Pads").toMap().value(QString("Pad%1").arg(componentNumber)).toMap().value("outDmPress").toInt();
                padDisplayLabel->setText(QString("Press: %1").arg(numberToOffState(pressVal)));
                if(xVal < 0){
                    if(xVal == -1){
                        padDmDisplay->setText(QString("X CC: off"));
                    }
                    if(xVal == -2){
                        padDmDisplay->setText(QString("X CC: bend"));
                    }
                }
                else{
                    padDmDisplay->setText(QString("X CC: %1").arg(numberToOffState(xVal)));
                }
                if(yVal < 0){
                    if(yVal == -1){
                        padDisplayLow->setText(QString("Y CC: off"));
                    }
                    if(yVal == -2){
                        padDisplayLow->setText(QString("Y CC: bend"));
                    }
                }
                else{
                    padDisplayLow->setText(QString("Y CC: %1").arg(numberToOffState(yVal)));
                }
                padDmDisplay->show();
                padDisplayLabel->show();
                padDisplayLow->show();
            }
        }
        if (objectName.contains("Return")){
            if (objectName.contains("Y") && yVal != -1){
                padDisplayLabel->setText(QString("Y Return:"));
                padDmDisplay->setText(QString("%1").arg(numberToLatchState(padDisplayValue)));
                padDmDisplay->show();
            }
            if (objectName.contains("Y") && yVal == -1){
                padDisplayLabel->setText(QString("Y is off."));
                padDmDisplay->hide();
            }
            if (objectName.contains("X") && xVal != -1){
                padDisplayLabel->setText(QString("X Return:"));
                padDmDisplay->setText(QString("%1").arg(numberToLatchState(padDisplayValue)));
                padDmDisplay->show();
            }
            if (objectName.contains("X") && xVal == -1){
                padDisplayLabel->setText(QString("X is off."));
                padDmDisplay->hide();
            }
            padDisplayLow->hide();
            padDisplayLabel->show();
        }
    }
    if(objectName.contains("Threshold")){
        if(objectName.contains("On")){
            padDisplayLabel->setText("On");
        }else{
            padDisplayLabel->setText("Off");
        }
        padDmDisplay->setText("Threshold");
        padDisplayValue = presetValues->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value("Pads").toMap().value(objectName).toInt();
        padDisplayLow->setText(QString("%1").arg(padDisplayValue));

        padDisplayLabel->show();
        padDmDisplay->show();
        padDisplayLow->show();
    }
    if(objectName.contains("Isolation")){
        padDisplayLabel->setText("Corner");
        padDmDisplay->setText("Isolation");
        padDisplayValue = presetValues->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value("Pads").toMap().value(objectName).toInt();
        padDisplayLow->setText(QString("%1").arg(padDisplayValue));
        padDisplayLabel->show();
        padDmDisplay->show();
        padDisplayLow->show();
    }
    if(objectName.contains("PerPadUI")){
        padDisplayLabel->setText("Sensitivity");
        padDmDisplay->setText("Adjust");
        padDisplayValue = presetValues->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value("Pads").toMap().value(QString("Pad%1").arg(componentNumber)).toMap().value(objectName).toInt();
        padDisplayLow->setText(QString("%1").arg(padDisplayValue));
        padDisplayLabel->show();
        padDmDisplay->show();
        padDisplayLow->show();
    }
    //hide grid mode parameters
    padNWDisplay->hide();
    padNEDisplay->hide();
    padSWDisplay->hide();
    padSEDisplay->hide();
    if(!objectName.contains("Mode") && !objectName.contains("Return")){
        padDmDisplay->show();
    }
}

void PadLabels::slotDisplayGrid(QString objectName) {
    //qDebug()<<padDisplayWidget->geometry();
    padGridString = objectName;
    //if it's grid mode...
    //if it's an LED parameter...
    if(objectName.contains("In")) {
        padDisplayLabel->move(8, 10);

            padNWDisplay->hide();
            padNEDisplay->hide();
            padSWDisplay->hide();
            padSEDisplay->hide();
            padDisplayLabel->setText(QString("Grid Mode"));
            if(objectName.contains("Dual")){
                padDmDisplay->setText(QString("Single Note"));
                padDisplayLow->setText(QString("LED Ch: %1").arg(presetValues->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value("Pads").toMap().value(QString("padGridDualInChannel")).toInt() + 1));
            }
            else{
                padDmDisplay->setText(QString("Dual Note"));
                padDisplayLow->setText(QString("LED Ch: %1").arg(presetValues->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value("Pads").toMap().value(QString("padGridDiscreteInChannel")).toInt() + 1));
            }
            padDmDisplay->show();
            padDisplayLow->show();
            padDisplayLabel->show();
    }
    else if(objectName.contains("out")) {
        padDisplayLow->hide();

        padDisplayLabel->move(8, 25);

        padDmDisplay->hide();
        //to be displayed in the middle
        padParamName = objectName;
        padParamName = padParamName.remove(0, 5); //remove outGm
        padParamName = padParamName.remove((padParamName.length() - 2), padParamName.length()); //remove NW, SW, SE, NE
        if(!objectName.contains("Channel")) {
            //if it is not the outChannel parameter...
            //hide drum mode parameters.
            padNWDisplay->show();
            padNEDisplay->show();
            padSWDisplay->show();
            padSEDisplay->show();
            padDmDisplay->hide();
            //create the string used to get all 4 values by taking off NW, SW, SE, NE
            padGridString = objectName;
            padGridString = QString("%1").arg(padGridString.remove((padGridString.length() - 2), padGridString.length()));
            padDisplayValueNW = presetValues->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value("Pads").toMap().value(QString("Pad%1").arg(componentNumber)).toMap().value(QString("%1NW").arg(padGridString)).toInt();
            padDisplayValueNE = presetValues->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value("Pads").toMap().value(QString("Pad%1").arg(componentNumber)).toMap().value(QString("%1NE").arg(padGridString)).toInt();
            padDisplayValueSW = presetValues->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value("Pads").toMap().value(QString("Pad%1").arg(componentNumber)).toMap().value(QString("%1SW").arg(padGridString)).toInt();
            padDisplayValueSE = presetValues->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value("Pads").toMap().value(QString("Pad%1").arg(componentNumber)).toMap().value(QString("%1SE").arg(padGridString)).toInt();
//            qDebug() << QString("object: %1 NW: %2 NE: %3 SE: %4 SW: %5").arg(padGridString).arg(padDisplayValueNW).arg(padDisplayValueNE).arg(padDisplayValueSW).arg(padDisplayValueSE);

            if(objectName.contains("Note") && !objectName.contains("Mode")) {
                padNWDisplay->setText(QString("%1").arg(NoteNumberToName(padDisplayValueNW)));
                padNEDisplay->setText(QString("%1").arg(NoteNumberToName(padDisplayValueNE)));
                padSWDisplay->setText(QString("%1").arg(NoteNumberToName(padDisplayValueSW)));
                padSEDisplay->setText(QString("%1").arg(NoteNumberToName(padDisplayValueSE)));
            }
            else if (objectName.contains("Velocity")) {
                if (padDisplayValueNW > 0){
                    padNWDisplay->setText(QString("%1").arg(padDisplayValueNW));
                }
                else {
                    padNWDisplay->setText("Vary");
                }
                if (padDisplayValueNE > 0) {
                    padNEDisplay->setText(QString("%1").arg(padDisplayValueNE));
                }
                else {
                    padNEDisplay->setText("Vary");
                }
                if(padDisplayValueSW > 0) {
                    padSWDisplay->setText(QString("%1").arg(padDisplayValueSW));
                }
                else {
                    padSWDisplay->setText("Vary");
                }
                if (padDisplayValueSE > 0) {
                    padSEDisplay->setText(QString("%1").arg(padDisplayValueSE));
                }
                else {
                    padSEDisplay->setText("Vary");
                }
            }
            else if (objectName.contains("PressValue")) {
                if (padDisplayValueNW > 0){
                    padNWDisplay->setText(QString("%1").arg(padDisplayValueNW));
                }
                else {
                    padNWDisplay->setText("Cont");
                }
                if (padDisplayValueNE > 0) {
                    padNEDisplay->setText(QString("%1").arg(padDisplayValueNE));
                }
                else {
                    padNEDisplay->setText("Cont");
                }
                if(padDisplayValueSW > 0) {
                    padSWDisplay->setText(QString("%1").arg(padDisplayValueSW));
                }
                else {
                    padSWDisplay->setText("Cont");
                }
                if (padDisplayValueSE > 0) {
                    padSEDisplay->setText(QString("%1").arg(padDisplayValueSE));
                }
                else {
                    padSEDisplay->setText("Cont");
                }
            }
            else if (objectName.contains("Mode")) {
                if(padDisplayValueNW){
                    padNWDisplay->setText("Toggle");
                }else{
                    padNWDisplay->setText("Norm");
                }
                if(padDisplayValueNE){
                    padNEDisplay->setText("Toggle");
                }else{
                    padNEDisplay->setText("Norm");
                }
                if(padDisplayValueSW){
                    padSWDisplay->setText("Toggle");
                }else{
                    padSWDisplay->setText("Norm");
                }
                if(padDisplayValueSE){
                    padSEDisplay->setText("Toggle");
                }else{
                    padSEDisplay->setText("Norm");
                }
            }
            else { //pressure
                padNWDisplay->setText(QString("%1").arg(numberToOffState(padDisplayValueNW)));
                padNEDisplay->setText(QString("%1").arg(numberToOffState(padDisplayValueNE)));
                padSWDisplay->setText(QString("%1").arg(numberToOffState(padDisplayValueSW)));
                padSEDisplay->setText(QString("%1").arg(numberToOffState(padDisplayValueSE)));
            }
        }
        else {
            padDmDisplay->setText(QString("%1").arg(padDisplayValue));
            padNWDisplay->hide();
            padNEDisplay->hide();
            padSWDisplay->hide();
            padSEDisplay->hide();
            padDmDisplay->show();
        }
        if(objectName.contains("Velocity")){
            padDisplayLabel->setText("Velocity");
        }else if(objectName.contains("PressValue")){
            padDisplayLabel->setText("Press");
        }else if(objectName.contains("Mode")){
            padDisplayLabel->setText("Mode");
        }else{
        padDisplayLabel->setText(padParamName);}
    }
}
void PadLabels::slotDisplayChannel(void){
    padDisplayLabel->move(8, 10);
    //get object's value for this pad
    padDisplayValue = presetValues->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value("Pads").toMap().value(QString("Pad%1").arg(componentNumber)).toMap().value(QString("padChannel")).toInt();

    padParamName = QString("Channel");
    padDisplayLabel->setText(padParamName);
    padDmDisplay->setText(QString("%1").arg(numberToOffState(padDisplayValue + 1)));
    //hide grid mode parameters
    padNWDisplay->hide();
    padNEDisplay->hide();
    padSWDisplay->hide();
    padSEDisplay->hide();
    padDmDisplay->show();
    padDisplayLow->hide();
}

void PadLabels::slotDisplayTranspose(QString objectName, int componentNum, int preset){
    Q_UNUSED(componentNum);
    Q_UNUSED(preset);
    QString bank = objectName;
    bank = bank.remove(0, 4);
    bank = bank.remove(1, bank.length());
    int interval = presetValues->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value("Pads").toMap().value(objectName).toInt();

    padDisplayLabel->setText(QString("Bank %1").arg(bank));
    padDmDisplay->setText(QString("Interval: %1").arg(interval));

    if(gridMode){
        int noteNW;
        noteNW = presetValues->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value("Pads").toMap().value(QString("Pad%1").arg(componentNumber)).toMap().value(QString("outGmNoteNW")).toInt();
        int noteNE;
        noteNE = presetValues->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value("Pads").toMap().value(QString("Pad%1").arg(componentNumber)).toMap().value(QString("outGmNoteNE")).toInt();
        int noteSW;
        noteSW = presetValues->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value("Pads").toMap().value(QString("Pad%1").arg(componentNumber)).toMap().value(QString("outGmNoteSW")).toInt();
        int noteSE;
        noteSE = presetValues->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value("Pads").toMap().value(QString("Pad%1").arg(componentNumber)).toMap().value(QString("outGmNoteSE")).toInt();
        noteNW = noteNW + interval;
        noteSW = noteSW + interval;
        noteNE = noteNE + interval;
        noteSE = noteSE + interval;
        padNWDisplay->setText(NoteNumberToName(noteNW));
        padNEDisplay->setText(NoteNumberToName(noteNE));
        padSWDisplay->setText(NoteNumberToName(noteSW));
        padSEDisplay->setText(NoteNumberToName(noteSE));
        padDmDisplay->setText(QString("Bank %1: %2").arg(bank).arg(interval));
        padNWDisplay->show();
        padNEDisplay->show();
        padSWDisplay->show();
        padSEDisplay->show();
        padDisplayLabel->hide();
        padDisplayLow->hide();

    }else{
        int note;
        note = presetValues->value(QString("Preset %1").arg(presetNumber)).toMap().value("ComponentSettings").toMap().value("Pads").toMap().value(QString("Pad%1").arg(componentNumber)).toMap().value(QString("outDmNote")).toInt();
        int newNote = note + interval;
        padDisplayLow->setText(QString("%1 -> %2").arg(NoteNumberToName(note)).arg(NoteNumberToName(newNote)));
                padNWDisplay->hide();
                padNEDisplay->hide();
                padSWDisplay->hide();
                padSEDisplay->hide();
                padDisplayLow->show();
                padDisplayLabel->show();

    }
    padDmDisplay->show();


}

//to convert midi note number to note name where note 60 = C3.
QString PadLabels::NoteNumberToName(int noteNumber) {
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

QString PadLabels::numberToOffState(int numberToOff) {
    QString offState;
    if(numberToOff == -1) {
        offState = QString("off");
    }
    else {
        offState = QString("%1").arg(numberToOff);
    }
    return QString("%1").arg(offState);
}

QString PadLabels::numberToLatchState(int numberToOff) {
    QString offState;
    if(numberToOff == -1) {
        offState = QString("latch");
    }
    else {
        offState = QString("%1").arg(numberToOff);
    }
    return QString("%1").arg(offState);
}
