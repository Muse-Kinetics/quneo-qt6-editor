#include "padeditpane.h"

PadEditPane::PadEditPane(QVariantMap *variantMap, QVariantMap *variantMapCopy, QWidget *widget, QWidget *parent) :
    QWidget(parent)
{
    qDebug() << "new PadEditPane";
    prevObjectToLabel = QString("None");
    qDebug() << "connect object references";
    mainWindow = widget;
    presetMap = variantMap;
    presetMapCopy = variantMapCopy;
    currentPreset = 0;
    qDebug() << "find ui elements";
    findUiElements();

    qDebug() << "connect signals";
    connect(mainWindow, SIGNAL(signalEvents(QString)), this, SLOT(slotEvents(QString)));

    connect(enableGridMode, SIGNAL(stateChanged(int)), this, SLOT(slotEnableGridMode(int)));
    connect(enableGridMode, SIGNAL(stateChanged(int)), this, SLOT(slotValueChanged(int)));

    qDebug() << "setup padlabels";
    for(int i=0; i<16; i++) {
        padLabel[i] = new PadLabels(mainWindow, presetMapCopy, 0);
    }

    qDebug() << "set stylesheets";
    this->setStyleSheet("QLabel { font: 10px }");

    qDebug() << "init tables";
    //---- Velocity Tables ---- //
    //initialize the list of values for each kind of table and store them in qlists

    int loadVelLight[128] = {0, 1, 3, 4, 6, 7, 9, 10, 12, 13, 15, 16, 17, 19, 20, 22, 23, 24, 25, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 46, 47, 47, 48, 49, 49, 50, 50, 51, 51, 51, 52, 52, 52, 53, 53, 53, 53, 54, 54, 54, 54, 55, 55, 55, 56, 56, 56, 56, 57, 57, 57, 57, 58, 58, 58, 58, 59, 59, 59, 60, 60, 61, 61, 61, 62, 62, 63, 63, 64, 64, 65, 66, 66, 67, 68, 68, 69, 69, 70, 71, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 82, 83, 85, 86, 88, 90, 91, 93, 95, 100, 104, 110, 114, 118, 122, 125, 127, 127};
    int loadVelMedium[128] = {0, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 12, 12, 13, 13, 14, 15, 16, 16, 17, 18, 19, 19, 20, 21, 22, 23, 24, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 40, 41, 42, 43, 44, 45, 47, 48, 49, 50, 52, 53, 54, 55, 57, 58, 60, 61, 62, 64, 65, 67, 68, 70, 71, 73, 74, 76, 77, 79, 80, 82, 84, 85, 87, 89, 90, 92, 94, 96, 97, 99, 101, 103, 104, 106, 108, 110, 112, 114, 116, 118, 120, 122, 124, 125, 127};
    int loadVelDynamic[128] = {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 9, 9, 9, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 20, 21, 22, 24, 26, 28, 30, 32, 34, 36, 38, 41, 44, 47, 49, 51, 54, 56, 59, 62, 65, 68, 70, 72, 74, 76, 78, 80, 82, 84, 85, 87, 88, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 117, 118, 118, 119, 119, 120, 120, 121, 121, 122, 122, 123, 123, 124, 124, 124, 124, 126, 126, 126, 126, 126, 127, 127, 127, 127, 127, 127, 127};
    int loadVelHard[128] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 14, 16, 18, 20, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 22, 22, 22, 22, 23, 23, 23, 23, 24, 24, 25, 25, 25, 26, 26, 26, 27, 28, 28, 29, 29, 30, 31, 31, 32, 32, 33, 33, 34, 34, 35, 36, 37, 38, 39, 40, 41, 42, 44, 45, 48, 52, 58, 63, 73, 78, 85, 92, 98, 104, 107, 111, 116, 120, 124, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127};

    //load all of the above tables plus one for linear into qlists of qvariants.
    for(int a=0; a<128; a++){
        velTableNone.append(a);
    }
    for(int b=0; b<128; b++){
        velTableLight.append(loadVelLight[b]);
    }
    for(int c=0; c<128; c++){
        velTableMedium.append(loadVelMedium[c]);
    }
    for(int d=0; d<128; d++){
        velTableDynamic.append(loadVelDynamic[d]);
    }
    for(int e=0; e<128; e++){
        velTableHard.append(loadVelHard[e]);
    }
    for(int a=0; a<128; a++){
        velTableFull.append(127);
    }

    //store all qlists in a master qmap. The menu selects an entry from this qmap.
    velocityTables.insert(QString("None"), velTableNone);
    velocityTables.insert(QString("Light"), velTableLight);
    velocityTables.insert(QString("Medium"), velTableMedium);
    velocityTables.insert(QString("Dynamic"), velTableDynamic);
    velocityTables.insert(QString("Hard"), velTableHard);
    velocityTables.insert(QString("Full Level"), velTableFull);

    //load a table from a text file
    /*QFile file("variable_table.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        return;}

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        slotLoadVariableTable(line);
    }*/

    //set up a list of all the table names to populate menu.
    tableIDs = velocityTables.keys();
    velTableMenu = mainWindow->findChild<QComboBox *>("padVelocityTableMenu");
    velTableMenu->addItems(tableIDs);
    connect(velTableMenu, SIGNAL(currentTextChanged(QString)), this, SLOT(slotSelectVelocityTable(QString)));

    //add menu icons named the same thing as each velocity table
    for(int n = 0; n < tableIDs.length(); n++){
        int indexOfThisTableID = velTableMenu->findText(tableIDs.at(n));
        QIcon tablePic(QString(":Quneo/images/resources/images/tables/%1.png").arg(tableIDs.at(n)));
        velTableMenu->setItemIcon(indexOfThisTableID, tablePic);
    }

    qDebug() << "connect mainwindow ui";
    xSource = mainWindow->findChild<QSpinBox *>("outDmXCC");
    ySource = mainWindow->findChild<QSpinBox *>("outDmYCC");
    connect(xSource, SIGNAL(valueChanged(int)), this, SLOT(slotChangeXText(int)));
    connect(ySource, SIGNAL(valueChanged(int)), this, SLOT(slotChangeYText(int)));

#ifdef Q_OS_MAC

#else
    QLabel *sensitivityLabel = mainWindow->findChild<QLabel *>("padSensitivityLabel");
    QLabel *sensitivityLabel2 = mainWindow->findChild<QLabel *>("padSensitivityLabel_2");
    QFont f( "Arial", 3, QFont::Normal);
    sensitivityLabel->setFont(f);
    sensitivityLabel2->setFont(f);
    sensitivityDial->move(9, 378);
    sensitivityLabel->move(-10, 414);
    sensitivityLabel2->move(-11, 403);
#endif

}

//this function looks up the table based on the ID selected in the menu and stores it in the json.
void PadEditPane::slotSelectVelocityTable(QString tableID){
    QList<QVariant> selectedTable;
    selectedTable = velocityTables.value(tableID);
    //emit table name and QList of values to store in json as 2 parameters.
    emit signalTableChanged(currentPreset, tableID, selectedTable);
}

void PadEditPane::findUiElements() {
    padTabs = mainWindow->findChild<QTabWidget *>("padTabs");
    padTabs->setCurrentIndex(0); //set master pad tab widget to basic

    //link pad edit pane master label
    paneLabel = mainWindow->findChild<QLabel *>("padName");

    //get list of pad parameters (use Prest 0 and Pad0 as a template) **** below is one line of code in two lines ****
    padParamList = presetMapCopy->
            value("Preset 0").toMap().
            value("ComponentSettings").toMap().
            value("Pads").toMap().
            value(QString("Pad0")).toMap().keys();

    //iterate through list of params and populate map of spin box addresses linked by param name
    for(int i=0; i<padParamList.count();i++)
    {

        if(mainWindow->findChild<QSpinBox *>(QString(padParamList.at(i))))
        {


            if(!QString(padParamList.at(i)).contains("localLEDControl"))
            {

                if(!QString(padParamList.at(i)).contains("enable"))
                {
                    spinBoxMap.insert(QString(padParamList.at(i)),mainWindow->findChild<QSpinBox *>(QString(padParamList.at(i))));

                    //set up all spin boxes to have correct minimum / maximum and "off" text.
                    if(!padParamList.at(i).contains("Channel", Qt::CaseSensitive) &&
                            !padParamList.at(i).contains("Return") &&
                            !padParamList.at(i).contains("padSensitivityPerPadUI") )
                    {

                        spinBoxMap.value(QString(padParamList.at(i)))->setMinimum(-1);
                        spinBoxMap.value(QString(padParamList.at(i)))->setMaximum(127);
                        spinBoxMap.value(QString(padParamList.at(i)))->setSpecialValueText("off");
                        if(padParamList.at(i).contains("VelocityValue", Qt::CaseSensitive))
                        {
                            spinBoxMap.value(QString(padParamList.at(i)))->setMinimum(0);
                            spinBoxMap.value(QString(padParamList.at(i)))->setMaximum(127);
                            spinBoxMap.value(QString(padParamList.at(i)))->setSpecialValueText("var");
                        }
                        if(padParamList.at(i).contains("PressValue", Qt::CaseSensitive))
                        {
                            spinBoxMap.value(QString(padParamList.at(i)))->setMinimum(0);
                            spinBoxMap.value(QString(padParamList.at(i)))->setMaximum(127);
                            spinBoxMap.value(QString(padParamList.at(i)))->setSpecialValueText("con");
                        }

                    } else if(padParamList.at(i).contains("Return"))
                    {
                        spinBoxMap.value(QString(padParamList.at(i)))->setMinimum(-1);
                        spinBoxMap.value(QString(padParamList.at(i)))->setMaximum(127);
                        spinBoxMap.value(QString(padParamList.at(i)))->setSpecialValueText("latch");

                    } else if(padParamList.at(i).contains("Channel", Qt::CaseSensitive))
                    {
                        //no off state, 1-16
                        spinBoxMap.value(QString(padParamList.at(i)))->setMinimum(1);
                        spinBoxMap.value(QString(padParamList.at(i)))->setMaximum(16);

                    } else if(padParamList.at(i).contains("padSensitivityPerPadUI", Qt::CaseSensitive))
                    {
                        perPadSensitivityUI = mainWindow->findChild<QSpinBox *>("padSensitivityPerPadUI");
                        connect(perPadSensitivityUI, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int)));
                        perPadSensitivityUI->setMinimum(-20);
                        perPadSensitivityUI->setMaximum(20);
                    }
                    connect(spinBoxMap.value(QString(padParamList.at(i))), SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int)));

                }
            }

            //find check boxes
        } else if(mainWindow->findChild<QCheckBox *>(QString(padParamList.at(i))))
        {

            if(mainWindow->findChild<QCheckBox *>("enableGrid"))
            {
                enableGridMode = mainWindow->findChild<QCheckBox *>("enableGrid");
            } else
            {
                padUiBlackList.append(padParamList.at(i));
                qDebug() << "Pad GLobal Ui Checkbox ***" << padParamList.at(i) << "Not Found";
            }

            //find menus
        } else if(mainWindow->findChild<QComboBox *>(QString(padParamList.at(i))))
        {

            if(padParamList.at(i).contains("NotePressMode")){
                if(padParamList.at(i).contains("outDmNotePressMode"))
                {
                    modeMenuDm = mainWindow->findChild<QComboBox *>(padParamList.at(i));
                    connect(modeMenuDm, SIGNAL(currentIndexChanged(int)), this, SLOT(slotValueChanged(int)));
                }
                else if(padParamList.at(i).contains("outGmNotePressModeNE"))
                {
                    modeMenuNE = mainWindow->findChild<QComboBox *>(padParamList.at(i));
                    connect(modeMenuNE, SIGNAL(currentIndexChanged(int)), this, SLOT(slotValueChanged(int)));
                }
                else if(padParamList.at(i).contains("outGmNotePressModeNW"))
                {
                    modeMenuNW = mainWindow->findChild<QComboBox *>(padParamList.at(i));
                    connect(modeMenuNW, SIGNAL(currentIndexChanged(int)), this, SLOT(slotValueChanged(int)));
                }
                else if(padParamList.at(i).contains("outGmNotePressModeSE"))
                {
                    modeMenuSE = mainWindow->findChild<QComboBox *>(padParamList.at(i));
                    connect(modeMenuSE, SIGNAL(currentIndexChanged(int)), this, SLOT(slotValueChanged(int)));
                }
                else if(padParamList.at(i).contains("outGmNotePressModeSW"))
                {
                    modeMenuSW = mainWindow->findChild<QComboBox *>(padParamList.at(i));
                    connect(modeMenuSW, SIGNAL(currentIndexChanged(int)), this, SLOT(slotValueChanged(int)));
                } else
                {
                    padUiBlackList.append(padParamList.at(i));
                    qDebug() << "Pad GLobal Ui ComboBox ***" << padParamList.at(i) << "Not Found";
                }
            }

        } else
        {
            padUiBlackList.append(padParamList.at(i));
            qDebug() << "Pad GLobal Ui ***" << padParamList.at(i) << "Not Found";
        }
    }

    //install event filter in all the spinboxes so that
    //copy/paste commands can be intercepted while they are in focus
    for(int i=0; i<padParamList.count();i++)
       {
           if(spinBoxMap.contains(padParamList.at(i)))
           {
               spinBoxMap.value(QString(padParamList.at(i)))->installEventFilter(this);
           }
       }

    //------ GLOBALS -------//
    sensitivityDial = mainWindow->findChild<QDial *>("padSensitivity");
    sensitivityDial->setMinimum(0);
    sensitivityDial->setMaximum(127);
    sensitivityDial->setSingleStep(1);
    connect(sensitivityDial, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int)));

    //find localLEDControl checkbox
    localLEDControl = mainWindow->findChild<QCheckBox *>("localLEDControl");
    connect(localLEDControl, SIGNAL(stateChanged(int)), this, SLOT(slotValueChanged(int)));

    ledDrumInChannel = mainWindow->findChild<QSpinBox *>("padDrumInChannel");
    connect(ledDrumInChannel, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int)));

    ledGridDiscreteInChannel = mainWindow->findChild<QSpinBox *>("padGridDiscreteInChannel");
    connect(ledGridDiscreteInChannel, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int)));

    ledGridDualInChannel = mainWindow->findChild<QSpinBox *>("padGridDualInChannel");
    connect(ledGridDualInChannel, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int)));

    cornerIsolation = mainWindow->findChild<QSpinBox *>("cornerIsolation");
    cornerIsolation->setMinimum(0);
    cornerIsolation->setMaximum(100);
    connect(cornerIsolation, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int)));
    //cornerIsolation->setSpecialValueText("off");

    //if on thresh could get any lower than 3, off thresh would have to be <1 or less than 2 away from the on thresh.
    onThresh = mainWindow->findChild<QSpinBox *>("padOnThreshold");
    onThresh->setMinimum(3);
    connect(onThresh, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int)));
    connect(onThresh, SIGNAL(valueChanged(int)), this, SLOT(slotCalculateOffThreshFromOn(int)));

    offThresh = mainWindow->findChild<QSpinBox *>("padOffThreshold");
    offThresh->setMinimum(1);
    connect(offThresh, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int)));
    connect(offThresh, SIGNAL(valueChanged(int)), this, SLOT(slotCheckOffThreshAgainstOn(int)));

    //padOffset = mainWindow->findChild<QSpinBox *>("padOffset");
    //connect(padOffset, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int)));

    //iterate through bank transpos boxes and connect
    for(int i =0; i<4; i++)
    {
        backTransposeSpinBoxes[i] = mainWindow->findChild<QSpinBox *>(QString("bank%1TransposeInterval").arg(i+1));
        connect(backTransposeSpinBoxes[i], SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int)));
        backTransposeSpinBoxes[i]->installEventFilter(this);
    }

    ledDrumInChannel->installEventFilter(this);
    ledGridDiscreteInChannel->installEventFilter(this);
    ledGridDualInChannel->installEventFilter(this);
    cornerIsolation->installEventFilter(this);
    onThresh->installEventFilter(this);
    offThresh->installEventFilter(this);
    sensitivityDial->installEventFilter(this);
}

void PadEditPane::slotCalculateOffThreshFromOn(int onThreshChanged){
    if(offThresh->value() > (onThresh->value() - 2)){
        offThresh->setValue(onThresh->value() - 2);
    }
    offThresh->setMaximum(onThresh->value() - 2);
}

void PadEditPane::slotCheckOffThreshAgainstOn(int offThreshChanged){
    if(offThreshChanged > (onThresh->value() - 2)){
        offThresh->setValue(onThresh->value() - 2);
    }
    offThresh->setMaximum(onThresh->value() - 2);
}

void PadEditPane::slotChangeXText(int xVal){
    //this function is called whenever the value of the x CC box changes.
    if(xVal == -1){
        xSource->setSpecialValueText("off");
        xSource->setMinimum(-1);
        xSource->setValue(-1);     //at a value of -1 latch text displayed
        xSource->setMinimum(-2);   //set minimum back to -2 so text still says latch but if numbox can still decrement
    }
    else if(xVal == -2){
        xSource->setSpecialValueText("bend");
        xSource->setMinimum(-2);
        xSource->setValue(-2);     //at a value of -2 bend text displayed
    }
    else {
        xSource->setValue(xVal);   //if it's a regular positive number, just display it.
    }
}

void PadEditPane::slotChangeYText(int yVal){
    //this function is the same as the one above but it is for the y CC box.
    if(yVal == -1){
        ySource->setSpecialValueText("off");
        ySource->setMinimum(-1);
        ySource->setValue(-1);
        ySource->setMinimum(-2);
    }
    else if(yVal == -2){
        ySource->setSpecialValueText("bend");
        ySource->setMinimum(-2);
        ySource->setValue(-2);
    }
    else{
        ySource->setValue(yVal);
    }
}

void PadEditPane::slotUpdateXYText()
{
    //this function is called during preset recall, when a pad button is pressed, when the pad tabs change, and when a different pad edit pane param is in focus.
    //it prevents the X and Y CC numboxes from resetting to display -1 instead of latch when the minimum is actually -2 to allow for the user to select bend even though latch is the current value.
    //it updates the display to show correct text when the value hasn't changed and is just being displayed.
    valXCC = presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("Pads").toMap().value(QString("Pad%1").arg(currentPad)).toMap().value("outDmXCC").toInt();
    this->slotChangeXText(valXCC);//look up the value in the preset map and call the above function.
    valYCC = presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("Pads").toMap().value(QString("Pad%1").arg(currentPad)).toMap().value("outDmYCC").toInt();
    this->slotChangeYText(valYCC);
}

void PadEditPane::slotRecallPreset(int preset)
{
    currentPreset = preset;
    displayPadValues();
    slotToLabels(prevObjectToLabel);
    if(!prevObjectToLabel.contains("None")) {
        emit signalToLabels(prevObjectToLabel);
    }
}

void PadEditPane::displayPadValues()
{
    //disconnect param display labels during preset recall.
    disconnect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), this, SLOT(slotFocusParam(QWidget*,QWidget*)));
    disconnect(this, SIGNAL(signalToLabels(QString)), this, SLOT(slotToLabels(QString))); //displays when value changed
    //get clicked pad map
    padMap = presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").
            toMap().value("Pads").toMap().value(QString("Pad%1").arg(currentPad)).toMap();

    //set pad editpane title
    paneLabel->setText(QString("Pad %1").arg(currentPad + 1));

    //clear focus before displaying values
    for(int i=0; i<padParamList.count();i++)
    {
        if(spinBoxMap.contains(padParamList.at(i)))
        {
            spinBoxMap.value(QString(padParamList.at(i)))->clearFocus();
        }
    }

    //iterate through param names and set values in edit pane ui
    this->slotUpdateXYText();
    for(int i=0; i<padParamList.count();i++)
    {

        if(!padUiBlackList.contains(padParamList.at(i)))
        {

            if(!QString(padParamList.at(i)).contains("enable"))
            {
                if(!QString(padParamList.at(i)).contains("localLEDControl") && !QString(padParamList.at(i)).contains("cornerIsolation"))
                {
                    if(QString(padParamList.at(i)) != QString("padSensitivityPerPad"))
                    {
                        if(padParamList.at(i).contains("Channel", Qt::CaseSensitive))
                        {
                            if(spinBoxMap.contains(padParamList.at(i)))
                            {
                                spinBoxMap.value(QString(padParamList.at(i)))->setValue(padMap.value(QString(padParamList.at(i))).toInt() + 1);
                            }
                        } else
                        {
                            if(spinBoxMap.contains(padParamList.at(i)) && !padParamList.at(i).contains("outDmXCC") && !padParamList.at(i).contains("outDmYCC"))
                            {
                                spinBoxMap.value(QString(padParamList.at(i)))->setValue(padMap.value(QString(padParamList.at(i))).toInt());
                            }
                        }

                        if(QString(padParamList.at(i)).contains("in"))
                        {
                            spinBoxMap.value(QString(padParamList.at(i)))->hide();
                            QLabel *ledParamToHide = mainWindow->findChild<QLabel *>(QString("%1Label").arg(padParamList.at(i)));
                            ledParamToHide->hide();
                            ledParamToHide = mainWindow->findChild<QLabel *>(QString("GridModeLEDLabel"));
                            ledParamToHide->hide();
                            ledParamToHide = mainWindow->findChild<QLabel *>(QString("DrumLEDLabel"));
                            ledParamToHide->hide();
                        }
                    }
                }
            }

            if(padParamList.at(i).contains("outDmNotePressMode"))
            {
                modeMenuDm = mainWindow->findChild<QComboBox *>(padParamList.at(i));
                modeMenuDm->setCurrentIndex(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("Pads").toMap().value(QString("Pad%1").arg(currentPad)).toMap().value(padParamList.at(i)).toInt());
            }
            else if(padParamList.at(i).contains("outGmNotePressModeNE"))
            {
                modeMenuNE = mainWindow->findChild<QComboBox *>(padParamList.at(i));
                modeMenuNE->setCurrentIndex(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("Pads").toMap().value(QString("Pad%1").arg(currentPad)).toMap().value(padParamList.at(i)).toInt());
            }
            else if(padParamList.at(i).contains("outGmNotePressModeNW"))
            {
                modeMenuNW = mainWindow->findChild<QComboBox *>(padParamList.at(i));
                modeMenuNW->setCurrentIndex(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("Pads").toMap().value(QString("Pad%1").arg(currentPad)).toMap().value(padParamList.at(i)).toInt());
            }
            else if(padParamList.at(i).contains("outGmNotePressModeSE"))
            {
                modeMenuSE = mainWindow->findChild<QComboBox *>(padParamList.at(i));
                modeMenuSE->setCurrentIndex(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("Pads").toMap().value(QString("Pad%1").arg(currentPad)).toMap().value(padParamList.at(i)).toInt());
            }
            else if(padParamList.at(i).contains("outGmNotePressModeSW"))
            {
                modeMenuSW = mainWindow->findChild<QComboBox *>(padParamList.at(i));
                modeMenuSW->setCurrentIndex(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("Pads").toMap().value(QString("Pad%1").arg(currentPad)).toMap().value(padParamList.at(i)).toInt());
            }
        }
    }


    if(sensitivityDial){
        sensitivityDial->setValue(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("Pads").toMap().value("padSensitivity").toInt());

    }

    if(cornerIsolation){
        cornerIsolation->setValue(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("Pads").toMap().value("cornerIsolation").toInt());
    }

    if(localLEDControl){
        localLEDControl->setChecked(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("Pads").toMap().value("localLEDControl").toBool());
    }

    if(ledDrumInChannel){
        ledDrumInChannel->setValue(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("Pads").toMap().value("padDrumInChannel").toInt() + 1);
    }

    if(ledGridDiscreteInChannel){
        ledGridDiscreteInChannel->setValue(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("Pads").toMap().value("padGridDiscreteInChannel").toInt() + 1);
    }

    if(ledGridDualInChannel){
        ledGridDualInChannel->setValue(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("Pads").toMap().value("padGridDualInChannel").toInt() + 1);
    }


    if(enableGridMode){
        enableGridMode->setChecked(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("Pads").toMap().value(QString("Pad%1").arg(currentPad)).toMap().value("enableGrid").toBool());
    }

    if(!padUiBlackList.contains("padVelocityTableID")){
        VelTableID = presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("Pads").toMap().value(QString("padVelocityTableID")).toString();
    }

    if(velTableMenu){
        velTableMenu->setCurrentIndex(velTableMenu->findText(VelTableID));
    }

    if(onThresh){
        onThresh->setValue(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("Pads").toMap().value("padOnThreshold").toInt());
    }

    if(offThresh){
        offThresh->setValue(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("Pads").toMap().value("padOffThreshold").toInt());
    }

   // if(padOffset){
   //     padOffset->setValue(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("Pads").toMap().value("padOffset").toInt());
   // }

    //iterate through bank transpos boxes and connect
    for(int i =0; i<4; i++)
    {
        if(backTransposeSpinBoxes[i])
        {
            backTransposeSpinBoxes[i]->setValue(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("Pads").toMap().value(QString("bank%1TransposeInterval").arg(i+1)).toInt());
        }

    }


    bool gridMode =presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("Pads").toMap().value(QString("Pad%1").arg(currentPad)).toMap().value("enableGrid").toBool();
    slotEnableGridMode(gridMode);
    padTabs = mainWindow->findChild<QTabWidget *>("padDrumGridTab");
    if(gridMode){
        padTabs->setCurrentIndex(1);
    }
    else{
        padTabs->setCurrentIndex(0);
    }


    connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), this, SLOT(slotFocusParam(QWidget*,QWidget*)));
    connect(this, SIGNAL(signalToLabels(QString)), this, SLOT(slotToLabels(QString))); //displays when value changed
}

void PadEditPane::slotEvents(QString string)
{
    //format string from event into a single instance (pad number)
    if(string.contains("padButton", Qt::CaseSensitive)) {
        currentPad = string.remove(0,9).toInt();//remove "padButton" and convert last character to int
        this->slotUpdateXYText();
        displayPadValues();
    }
}

bool PadEditPane::eventFilter(QObject *object, QEvent *event){

    if(event->type() == QEvent::KeyPress){
       QKeyEvent *keyEvent = (QKeyEvent *)event;

       if(keyEvent->matches(QKeySequence::Copy)){
           emit signalTriggerCopy();
       }
       if(keyEvent->matches(QKeySequence::Paste)){
           emit signalTriggerPaste();
       }
    }
    return 0;
}

void PadEditPane::slotCalculatePadSensitivity(int changedValue)
{

    /* QWidget *sensitivitySender = qobject_cast<QWidget *>(QObject::sender());

    if(sensitivitySender->objectName() == QString("padSensitivity")){
        globalPadSensitivity = (float(changedValue) / 127.00) + .5;
    }

    if(sensitivitySender->objectName() == QString("padSensitivityPerPadUI")){
        thisPadSensitivity = (float(changedValue + 12) / 24.00) + .5;
    }

    calculatedSensitivityPerPad = (globalPadSensitivity * thisPadSensitivity * 100.00);


    for(int i =0; i<16; i++){


        presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap()
                        .value("ComponentSettings").toMap().value("Pads").toMap()
                        .value(QString("Pad%1").arg(currentPad)).toMap()
                        .value("padSensitivityPerPadUI").toInt();


        calculatedSensitivityPerPad = (globalPadSensitivity * thisPadSensitivity * 100.00);
        emit signalValueChanged(currentPreset, "ComponentSettings", "Pad", currentPad, QString("padSensitivityPerPad"), calculatedSensitivityPerPad);
    }
    displayPadValues();*/


}

void PadEditPane::slotValueChanged(int num)
{
    focusCheck = qobject_cast<QWidget *>(QObject::sender());
    if((!focusCheck->objectName().contains(QString("Enable"))) && (!focusCheck->objectName().contains(QString("enable"))))
    {


        if(focusCheck->hasFocus())
        {
            qDebug() << "pad object edited" << focusCheck->objectName();
            //int presetNum, QString settingsType, QString paramType, int typeNum, QVariantMap paramValMap
            if(focusCheck->objectName().contains("VelocityValue") || focusCheck->objectName().contains("PressValue")){
                if (num == 0){
                    num = -1;
                }
            }
            emit signalValueChanged(currentPreset, "ComponentSettings", "Pad", currentPad, focusCheck->objectName(),num);
            emit signalToLabels(focusCheck->objectName()); //whenever a value is changed, this should be emitted, but it shouldn't always have an effect.


        } else if(focusCheck->objectName().contains("localLEDControl"))
        {
            emit signalValueChanged(currentPreset, "ComponentSettings", "Pad", currentPad, focusCheck->objectName(),num);

        } else if(focusCheck->objectName().contains("NotePressMode"))
        {
            emit signalValueChanged(currentPreset, "ComponentSettings", "Pad", currentPad, focusCheck->objectName(),num);
            emit signalToLabels(QString("Note Mode"));
        }

    }
    else if(focusCheck->objectName().contains("cornerIsolation"))
    {
        emit signalValueChanged(currentPreset, "ComponentSettings", "Pad", currentPad, focusCheck->objectName(),num);

    }
    else
    { //these handle velocity enables
        emit signalValueChanged(currentPreset, "ComponentSettings", "Pad", currentPad, focusCheck->objectName(),num);
        emit signalToLabels(focusCheck->objectName());
    }


}

void PadEditPane::slotFocusParam(QWidget* oldObject, QWidget* nowObject) {
    qDebug() << "PadEditPane called";

    if(nowObject != nullptr) { // eb TODO - was nowObject > 0, see others
        qDebug() << "slotFocusParam called: " << nowObject->objectName() << " Parent: " << nowObject->parent()->objectName();

        clicked = nowObject;

        if(clicked->parent()->objectName().contains("pad") && clicked->objectName() != justClicked) {
            qDebug() << "first if";
            this->slotUpdateXYText();
            justClicked = clicked->objectName();
        }

        if(clicked->parent()->objectName().contains("pad")){
            qDebug() << "second if";
                emit signalToLabels(clicked->objectName()); //whenever a value is changed, this should be emitted, but it shouldn't always have an effect.
        }
    }
    qDebug() << "Survived nowObject";
}

void PadEditPane::slotToLabels(QString parameter)
{
    prevObjectToLabel = parameter;
    for(int i = 0; i < 16; i++)
    {  //when a parameter has been changed, display that parameter on all 16 pads.
        padLabel[i]->slotDisplayParameter(parameter, i, currentPreset);
    }
}

void PadEditPane::slotEnableGridMode(int gridEnabled)
{
    if(gridEnabled)
    {
        for (int i=0; i<padParamList.count(); i++)
        {

            if(!padUiBlackList.contains(padParamList.at(i)))
            {

                    if(QString("%1").arg(padParamList.at(i)).contains("Dm"))
                    {
                        if(spinBoxMap.contains(padParamList.at(i)))
                        {
                            spinBoxMap.value(QString(padParamList.at(i)))->setEnabled(false);
                            //padDmVelocity->setEnabled(false);
                        }
                        if(QString("%1").arg(padParamList.at(i)).contains("NotePressMode")){
                            QComboBox *notePressModeMenu = mainWindow->findChild<QComboBox *>(QString("%1").arg(padParamList.at(i)));
                            notePressModeMenu->setEnabled(false);
                        }
                    }
                    if(QString("%1").arg(padParamList.at(i)).contains("Gm"))
                    {
                        if(spinBoxMap.contains(padParamList.at(i))){
                            spinBoxMap.value(QString(padParamList.at(i)))->setEnabled(true);
                        }
                        if(QString("%1").arg(padParamList.at(i)).contains("NotePressMode")){
                            QComboBox *notePressModeMenu = mainWindow->findChild<QComboBox *>(QString("%1").arg(padParamList.at(i)));
                            notePressModeMenu->setEnabled(true);
                        }
                    }                
            }
        }
    } else
    {
        for (int i=0; i<padParamList.count(); i++)
        {

            if(!padUiBlackList.contains(padParamList.at(i)))
            {
                    if(QString("%1").arg(padParamList.at(i)).contains("Dm"))
                    {
                        if(spinBoxMap.contains(padParamList.at(i)))
                        {
                            spinBoxMap.value(QString(padParamList.at(i)))->setEnabled(true);
                            //padDmVelocity->setEnabled(true);
                        }
                        if(QString("%1").arg(padParamList.at(i)).contains("NotePressMode")){
                            QComboBox *notePressModeMenu = mainWindow->findChild<QComboBox *>(QString("%1").arg(padParamList.at(i)));
                            notePressModeMenu->setEnabled(true);
                        }

                    }
                    if(QString("%1").arg(padParamList.at(i)).contains("Gm"))
                    {
                        if(spinBoxMap.contains(padParamList.at(i)))
                        {
                            spinBoxMap.value(QString(padParamList.at(i)))->setEnabled(false);
                        }
                        if(QString("%1").arg(padParamList.at(i)).contains("NotePressMode")){
                            QComboBox *notePressModeMenu = mainWindow->findChild<QComboBox *>(QString("%1").arg(padParamList.at(i)));
                            notePressModeMenu->setEnabled(false);
                        }
                    }

            }

        }

    }
}

//this function is not called anymore. left it in just in case we need to try new tables by loading colls.
void PadEditPane::slotLoadVariableTable(QString table)
{
    //remove "1, " and ; at end.
    table.remove(0, 3);
    table.remove(table.length() - 1, table.length());
    //create a QStringList out of all integers separated by spaces
    newTableValues = table.split(" ");
    //put that QStringList into the QVariantList
    for(int i = 0; i < newTableValues.length(); i++)
    {
        velTableVariableConverter.append(newTableValues.at(i));
    }
    //Test that qvariant list can successfully convert to a list of integers.
    for(int n = 0; n < velTableVariableConverter.length(); n++)
    {
        int x;
        x = velTableVariableConverter.value(n).toInt();
        velTableVariable.append(x);
    }
    //add the QVariantList to the master list of tables.
    velocityTables.insert(QString("Variable"), velTableVariable);
}

//drum styles
void PadEditPane::slotStyleDefault()
{
    int thisPreset = currentPreset;
    for(int p = 0; p < 16; p++)
    {
        emit signalSelectPreset(QString("Preset %1").arg(p+1));
        onThresh->setValue(25);
        emit signalValueChanged(currentPreset, "ComponentSettings", "Pad", currentPad, QString("padOnThreshold"),25);
        offThresh->setValue(5);
        emit signalValueChanged(currentPreset, "ComponentSettings", "Pad", currentPad, QString("padOffThreshold"),5);
        sensitivityDial->setValue(100);
        emit signalValueChanged(currentPreset, "ComponentSettings", "Pad", currentPad, QString("padSensitivity"),100);
        velTableMenu->setCurrentIndex(velTableMenu->findText(QString("Medium")));
    }
    emit signalSelectPreset(QString("Preset %1").arg(thisPreset+1));
}

void PadEditPane::slotStyleMachine()
{
    qDebug()<<QString("machine");
    int thisPreset = currentPreset;
    for(int p = 0; p < 16; p++)
    {
        emit signalSelectPreset(QString("Preset %1").arg(p+1));
        onThresh->setValue(25);
        emit signalValueChanged(currentPreset, "ComponentSettings", "Pad", currentPad, QString("padOnThreshold"),25);
        offThresh->setValue(5);
        emit signalValueChanged(currentPreset, "ComponentSettings", "Pad", currentPad, QString("padOffThreshold"),5);
        sensitivityDial->setValue(100);
        emit signalValueChanged(currentPreset, "ComponentSettings", "Pad", currentPad, QString("padSensitivity"),93);
        velTableMenu->setCurrentIndex(velTableMenu->findText(QString("Medium")));
    }
    emit signalSelectPreset(QString("Preset %1").arg(thisPreset+1));
}

void PadEditPane::slotStyleAkaMP()
{
    qDebug()<<QString("akaMP");
    int thisPreset = currentPreset;
    for(int p = 0; p < 16; p++)
    {
        emit signalSelectPreset(QString("Preset %1").arg(p+1));
        onThresh->setValue(25);
        emit signalValueChanged(currentPreset, "ComponentSettings", "Pad", currentPad, QString("padOnThreshold"),25);
        offThresh->setValue(5);
        emit signalValueChanged(currentPreset, "ComponentSettings", "Pad", currentPad, QString("padOffThreshold"),5);
        sensitivityDial->setValue(100);
        emit signalValueChanged(currentPreset, "ComponentSettings", "Pad", currentPad, QString("padSensitivity"),75);
        velTableMenu->setCurrentIndex(velTableMenu->findText(QString("Light")));
    }
    emit signalSelectPreset(QString("Preset %1").arg(thisPreset+1));
}

void PadEditPane::slotStyleKorPad()
{
    qDebug()<<QString("korePad");
    int thisPreset = currentPreset;
    for(int p = 0; p < 16; p++)
    {
        emit signalSelectPreset(QString("Preset %1").arg(p+1));
        onThresh->setValue(25);
        emit signalValueChanged(currentPreset, "ComponentSettings", "Pad", currentPad, QString("padOnThreshold"),25);
        offThresh->setValue(5);
        emit signalValueChanged(currentPreset, "ComponentSettings", "Pad", currentPad, QString("padOffThreshold"),5);
        sensitivityDial->setValue(100);
        emit signalValueChanged(currentPreset, "ComponentSettings", "Pad", currentPad, QString("padSensitivity"),127);
        velTableMenu->setCurrentIndex(velTableMenu->findText(QString("Hard")));
    }
    emit signalSelectPreset(QString("Preset %1").arg(thisPreset+1));
}

void PadEditPane::slotStyleFullLevel()
{
    qDebug()<<QString("full");
    int thisPreset = currentPreset;
    for(int p = 0; p < 16; p++)
    {
        emit signalSelectPreset(QString("Preset %1").arg(p+1));
        onThresh->setValue(25);
        emit signalValueChanged(currentPreset, "ComponentSettings", "Pad", currentPad, QString("padOnThreshold"),25);
        offThresh->setValue(5);
        emit signalValueChanged(currentPreset, "ComponentSettings", "Pad", currentPad, QString("padOffThreshold"),5);
        sensitivityDial->setValue(100);
        emit signalValueChanged(currentPreset, "ComponentSettings", "Pad", currentPad, QString("padSensitivity"),127);
        velTableMenu->setCurrentIndex(velTableMenu->findText(QString("Full Level")));
    }
    emit signalSelectPreset(QString("Preset %1").arg(thisPreset+1));
}
