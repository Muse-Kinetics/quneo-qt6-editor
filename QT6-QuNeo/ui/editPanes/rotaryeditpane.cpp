#include "rotaryeditpane.h"

RotaryEditPane::RotaryEditPane(QVariantMap *variantMap, QVariantMap *variantMapCopy, QWidget *widget, QWidget *parent) :
    QWidget(parent)
{
    this->setStyleSheet("QLabel { font: 10px } QLineEdit { font: 12px } QCheckBox {font:9px} QSpinBox {font: 10px} QDoubleSpinBox {font: 10px}");

    currentBank = 1;
    prevObjectToLabel = QString("None");
    mainWindow = widget;
    presetMap = variantMap;
    presetMapCopy = variantMapCopy;
    currentPreset = 0;
    findUiElements();
    connect(mainWindow, SIGNAL(signalEvents(QString)), this, SLOT(slotEvents(QString)));

    sensitivityDial->setMinimum(70);
    sensitivityDial->setMaximum(150);
    sensitivityDial->setSingleStep(1);

    for(int i = 0; i < 2; i++){
        rotaryLabel[i] = new RotaryLabels(mainWindow, presetMapCopy, 0);
    }
#ifdef Q_OS_MAC

#else
    QLabel *sensitivityLabel = mainWindow->findChild<QLabel *>("rotarySensitivityLabel");
    QFont f( "Arial", 3, QFont::Normal);
    sensitivityLabel->setFont(f);
#endif

}

void RotaryEditPane::findUiElements(){
    QTabWidget *rotaryTabs = mainWindow->findChild<QTabWidget *>("rotaryTabs");
    rotaryTabs->setCurrentIndex(0);

    //link rotary edit pane master label
    paneLabel = mainWindow->findChild<QLabel *>("rotaryName");

    //get list of rotary parameters (use Prest 0 and Rotary0 as a template) **** below is one line of code in two lines ****
    rotaryParamList = presetMapCopy->value("Preset 0").toMap().value("ComponentSettings").
            toMap().value("Rotaries").toMap().value(QString("Rotary0")).toMap().keys();

    //iterate through list of params and populate map of spin box addresses linked by param name
    for(int i=0; i<rotaryParamList.count();i++) {

        if(mainWindow->findChild<QSpinBox *>(QString(rotaryParamList.at(i)))){

            if (!rotaryParamList.at(i).contains("Enable", Qt::CaseSensitive) &&
                    !rotaryParamList.at(i).contains("Sensitivity", Qt::CaseSensitive) &&
                    !rotaryParamList.at(i).contains("LocalLEDControl", Qt::CaseSensitive) &&
                    !rotaryParamList.at(i).contains("DirStorage", Qt::CaseSensitive) &&
                    !rotaryParamList.at(i).contains("LocStorage", Qt::CaseSensitive)) {

                spinBoxMap.insert(QString(rotaryParamList.at(i)), mainWindow->findChild<QSpinBox *>(QString(rotaryParamList.at(i))));
                connect(spinBoxMap.value(QString(rotaryParamList.at(i))), SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int)));

                uiObject = mainWindow->findChild<QSpinBox *>(QString(rotaryParamList.at(i)));


                if(rotaryParamList.at(i).contains("PassThru", Qt::CaseSensitive)){
                    uiObject->setMinimum(1);
                    uiObject->setMaximum(127);
                }
                if(!uiObject->objectName().contains("Channel") && !uiObject->objectName().contains("PassThru")) {
                    uiObject->setSpecialValueText("off");
                }
            }

        } else  if(mainWindow->findChild<QCheckBox *>(QString(rotaryParamList.at(i)))){

            if(mainWindow->findChild<QCheckBox *>("rB1outDirectionEnable")){
                rB1DirectionEnable = mainWindow->findChild<QCheckBox *>("rB1outDirectionEnable");
                connect(rB1DirectionEnable, SIGNAL(stateChanged(int)), this, SLOT(slotValueChanged(int)));
                connect(rB1DirectionEnable, SIGNAL(stateChanged(int)), this, SLOT(slotDirectionOrLocation(int)));
            }
            if(mainWindow->findChild<QCheckBox *>("rB2outDirectionEnable")){
                rB2DirectionEnable = mainWindow->findChild<QCheckBox *>("rB2outDirectionEnable");
                connect(rB2DirectionEnable, SIGNAL(stateChanged(int)), this, SLOT(slotValueChanged(int)));
                connect(rB2DirectionEnable, SIGNAL(stateChanged(int)), this, SLOT(slotDirectionOrLocation(int)));
            }
            if(mainWindow->findChild<QCheckBox *>("rB3outDirectionEnable")){
                rB3DirectionEnable = mainWindow->findChild<QCheckBox *>("rB3outDirectionEnable");
                connect(rB3DirectionEnable, SIGNAL(stateChanged(int)), this, SLOT(slotValueChanged(int)));
                connect(rB3DirectionEnable, SIGNAL(stateChanged(int)), this, SLOT(slotDirectionOrLocation(int)));
            }
            if(mainWindow->findChild<QCheckBox *>("rB4outDirectionEnable")){
                rB4DirectionEnable = mainWindow->findChild<QCheckBox *>("rB4outDirectionEnable");
                connect(rB4DirectionEnable, SIGNAL(stateChanged(int)), this, SLOT(slotValueChanged(int)));
                connect(rB4DirectionEnable, SIGNAL(stateChanged(int)), this, SLOT(slotDirectionOrLocation(int)));
            }

        } else  if(mainWindow->findChild<QComboBox *>(QString(rotaryParamList.at(i)))){



            if(rotaryParamList.at(i).contains("outNotePressMode"))
            {
                if(rotaryParamList.at(i).contains("rB1outNotePressMode"))
                {
                    modeMenuB1 = mainWindow->findChild<QComboBox *>(rotaryParamList.at(i));
                    connect(modeMenuB1, SIGNAL(currentIndexChanged(int)), this, SLOT(slotValueChanged(int)));
                }
                if(rotaryParamList.at(i).contains("rB2outNotePressMode"))
                {
                    modeMenuB2 = mainWindow->findChild<QComboBox *>(rotaryParamList.at(i));
                    connect(modeMenuB2, SIGNAL(currentIndexChanged(int)), this, SLOT(slotValueChanged(int)));
                }
                if(rotaryParamList.at(i).contains("rB3outNotePressMode"))
                {
                    modeMenuB3 = mainWindow->findChild<QComboBox *>(rotaryParamList.at(i));
                    connect(modeMenuB3, SIGNAL(currentIndexChanged(int)), this, SLOT(slotValueChanged(int)));
                }
                if(rotaryParamList.at(i).contains("rB4outNotePressMode"))
                {
                    modeMenuB4 = mainWindow->findChild<QComboBox *>(rotaryParamList.at(i));
                    connect(modeMenuB4, SIGNAL(currentIndexChanged(int)), this, SLOT(slotValueChanged(int)));
                }
            }
        }
        else {
            rotaryUiBlacklist.append(rotaryParamList.at(i));
            //qDebug() << "Rotary Ui  ***" << rotaryParamList.at(i) << "Not Found";
        }
    }

    //install event filter in all the spinboxes so that
    //copy/paste commands can be intercepted while they are in focus
    for(int i=0; i<rotaryParamList.count();i++)
       {
           if(spinBoxMap.contains(rotaryParamList.at(i)))
           {
               spinBoxMap.value(QString(rotaryParamList.at(i)))->installEventFilter(this);
           }
       }

    if(mainWindow->findChild<QDial *>("rotarySensitivity")){
        sensitivityDial = mainWindow->findChild<QDial *>("rotarySensitivity");
        connect(sensitivityDial, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int)));
    } else {
        qDebug() << "Rotary Ui ***" << "rotarySensitivity" << "Not Found";
    }

    if(mainWindow->findChild<QCheckBox *>("rotaryLocalLEDControl")){
        localLEDControl = mainWindow->findChild<QCheckBox *>("rotaryLocalLEDControl");
        connect(localLEDControl, SIGNAL(stateChanged(int)), this, SLOT(slotValueChanged(int)));
    } else {
        qDebug() << "Rotary Ui ***" << "rotaryLocalLEDControl" << "Not Found";
    }

    if(mainWindow->findChild<QSpinBox *>("rotaryInChannel")){
        ledInputChannel = mainWindow->findChild<QSpinBox *>("rotaryInChannel");
        connect(ledInputChannel, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int)));
    } else {
        qDebug() << "Rotary Ui ***" << "rotaryInChannel" << "Not Found";
    }

    if(mainWindow->findChild<QSpinBox *>("rotaryOnThreshold")){
        onThresh = mainWindow->findChild<QSpinBox *>("rotaryOnThreshold");
        onThresh->setMinimum(3);
        connect(onThresh, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int)));
        connect(onThresh, SIGNAL(valueChanged(int)), this, SLOT(slotCalculateOffThreshFromOn(int)));
    } else {
        qDebug() << "lSlider GLobal Ui ***" << "rotaryOnThreshold" << "Not Found";
    }

    if(mainWindow->findChild<QSpinBox *>("rotaryOffThreshold")){
        offThresh = mainWindow->findChild<QSpinBox *>("rotaryOffThreshold");
        offThresh->setMinimum(1);
        connect(offThresh, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int)));
        connect(offThresh, SIGNAL(valueChanged(int)), this, SLOT(slotCheckOffThreshAgainstOn(int)));
    } else {
        qDebug() << "lSlider GLobal Ui ***" << "rotaryOffThreshold" << "Not Found";
    }



    //for some reason these pointers are used later for min/max setting and for value map setting-- don't fully understand at this point
    //these are no longer needed to set values because this is no longer a float value.
    //They are used for setting min max because they are set to different limits than the other spinboxes.
    rB1speed = mainWindow->findChild<QSpinBox *>("rB1outSpeed");
    rB2speed = mainWindow->findChild<QSpinBox *>("rB2outSpeed");
    rB3speed = mainWindow->findChild<QSpinBox *>("rB3outSpeed");
    rB4speed = mainWindow->findChild<QSpinBox *>("rB4outSpeed");
    //connect(rB1speed, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int)));
    //connect(rB2speed, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int)));
    //connect(rB3speed, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int)));
    //connect(rB4speed, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int)));

    //find Direction and location boxes (in spin box map, but specific pointer used here for direction enable/disable)
    rB1Direction = mainWindow->findChild<QSpinBox *>("rB1outDirection");
    rB2Direction = mainWindow->findChild<QSpinBox *>("rB2outDirection");
    rB3Direction = mainWindow->findChild<QSpinBox *>("rB3outDirection");
    rB4Direction = mainWindow->findChild<QSpinBox *>("rB4outDirection");
    rB1Location = mainWindow->findChild<QSpinBox *>("rB1outLocation");
    rB2Location = mainWindow->findChild<QSpinBox *>("rB2outLocation");
    rB3Location = mainWindow->findChild<QSpinBox *>("rB3outLocation");
    rB4Location = mainWindow->findChild<QSpinBox *>("rB4outLocation");

    rB1speed->installEventFilter(this);
    rB2speed->installEventFilter(this);
    rB3speed->installEventFilter(this);
    rB4speed->installEventFilter(this);
    ledInputChannel->installEventFilter(this);
    sensitivityDial->installEventFilter(this);
}

void RotaryEditPane::displayRotaryValues() {
    //disconnect param display labels during preset recall.
    disconnect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), this, SLOT(slotFocusParam(QWidget*,QWidget*))); //displays based on focus
    disconnect(this, SIGNAL(signalToLabels(QString)), this, SLOT(slotToLabels(QString))); //displays when value changed
    //get clicked rotary map
    rotaryMap = presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").
            toMap().value("Rotaries").toMap().value(QString("Rotary%1").arg(currentRotary)).toMap();

    //set rotary editpane title
    paneLabel->setText(QString("Rotary %1").arg(currentRotary+1));

    //clear focus before displaying values
    for(int i=0; i<rotaryParamList.count();i++){

        if(!rotaryUiBlacklist.contains(rotaryParamList.at(i))){
            if(spinBoxMap.contains(rotaryParamList.at(i))){
                spinBoxMap.value(QString(rotaryParamList.at(i)))->clearFocus();
            }
        }

    }

    //iterate through param names and set values in edit pane ui
    for(int i=0; i<rotaryParamList.count();i++)
    {
        if(spinBoxMap.contains(rotaryParamList.at(i)))
        {
            if(!rotaryUiBlacklist.contains(rotaryParamList.at(i)))
            {
                    if(rotaryParamList.at(i).contains("Channel", Qt::CaseSensitive))
                    {
                        spinBoxMap.value(QString(rotaryParamList.at(i)))->setValue(rotaryMap.value(QString(rotaryParamList.at(i))).toInt() + 1);
                    }
                    else {spinBoxMap.value(QString(rotaryParamList.at(i)))->setValue(rotaryMap.value(QString(rotaryParamList.at(i))).toInt());
                    }

                    //set up all spin boxes to have correct minimum / maximum and "off" text.
                    if(!rotaryParamList.at(i).contains("Channel", Qt::CaseSensitive) && !rotaryParamList.at(i).contains("PassThru", Qt::CaseSensitive))
                    {
                        spinBoxMap.value(QString(rotaryParamList.at(i)))->setMinimum(-1);
                        spinBoxMap.value(QString(rotaryParamList.at(i)))->setMaximum(127);
                        spinBoxMap.value(QString(rotaryParamList.at(i)))->setSpecialValueText("off");
                        if(rotaryParamList.at(i).contains("PressValue", Qt::CaseSensitive))
                        {
                            spinBoxMap.value(QString(rotaryParamList.at(i)))->setMinimum(0);
                            spinBoxMap.value(QString(rotaryParamList.at(i)))->setMaximum(127);
                            spinBoxMap.value(QString(rotaryParamList.at(i)))->setSpecialValueText("con");
                        }
                        if(rotaryParamList.at(i).contains("VelocityValue", Qt::CaseSensitive))
                        {
                            spinBoxMap.value(QString(rotaryParamList.at(i)))->setMinimum(0);
                            spinBoxMap.value(QString(rotaryParamList.at(i)))->setMaximum(127);
                            spinBoxMap.value(QString(rotaryParamList.at(i)))->setSpecialValueText("var");
                        }
                    }
                    else if(rotaryParamList.at(i).contains("Channel", Qt::CaseSensitive))
                    { //no off state, 1-16
                        spinBoxMap.value(QString(rotaryParamList.at(i)))->setMinimum(1);
                        spinBoxMap.value(QString(rotaryParamList.at(i)))->setMaximum(16);
                    }
                    if(QString(rotaryParamList.at(i)).contains("in"))
                    {
                        spinBoxMap.value(QString(rotaryParamList.at(i)))->hide();
                        QLabel *ledParamToHide = mainWindow->findChild<QLabel *>(QString("%1Label").arg(rotaryParamList.at(i)));
                        ledParamToHide->hide();
                    }


                if(rotaryParamList.at(i).contains("B1outSpeed", Qt::CaseSensitive) && !rotaryParamList.at(i).contains("PassThru", Qt::CaseSensitive)) {
                    //rB1speed->setValue(rotaryMap.value(QString(rotaryParamList.at(i))).toInt());
                    rB1speed->setMinimum(1);
                    rB1speed->setMaximum(31);
                    rB1speed->setSpecialValueText("");
                }
                if(rotaryParamList.at(i).contains("B2outSpeed", Qt::CaseSensitive) && !rotaryParamList.at(i).contains("PassThru", Qt::CaseSensitive)) {
                    //rB2speed->setValue(rotaryMap.value(QString(rotaryParamList.at(i))).toInt());
                    rB2speed->setMinimum(1);
                    rB2speed->setMaximum(31);
                    rB2speed->setSpecialValueText("");
                }
                if(rotaryParamList.at(i).contains("B3outSpeed", Qt::CaseSensitive) && !rotaryParamList.at(i).contains("PassThru", Qt::CaseSensitive)) {
                    //rB3speed->setValue(rotaryMap.value(QString(rotaryParamList.at(i))).toInt());
                    rB3speed->setMinimum(1);
                    rB3speed->setMaximum(31);
                    rB3speed->setSpecialValueText("");
                }
                if(rotaryParamList.at(i).contains("B4outSpeed", Qt::CaseSensitive) && !rotaryParamList.at(i).contains("PassThru", Qt::CaseSensitive)) {
                    //rB4speed->setValue(rotaryMap.value(QString(rotaryParamList.at(i))).toInt());
                    rB4speed->setMinimum(1);
                    rB4speed->setMaximum(31);
                    rB4speed->setSpecialValueText("");
                }
            }

        } else {
            if (rotaryParamList.at(i).contains("outNotePressMode"))
            {
                if(rotaryParamList.at(i).contains("rB1outNotePressMode"))
                {
                    modeMenuB1 = mainWindow->findChild<QComboBox *>(rotaryParamList.at(i));
                    modeMenuB1->setCurrentIndex(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("Rotaries").toMap().value(QString("Rotary%1").arg(currentRotary)).toMap().value(rotaryParamList.at(i)).toInt());
                }
                if(rotaryParamList.at(i).contains("rB2outNotePressMode"))
                {
                    modeMenuB2 = mainWindow->findChild<QComboBox *>(rotaryParamList.at(i));
                    modeMenuB2->setCurrentIndex(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("Rotaries").toMap().value(QString("Rotary%1").arg(currentRotary)).toMap().value(rotaryParamList.at(i)).toInt());
                }
                if(rotaryParamList.at(i).contains("rB3outNotePressMode"))
                {
                    modeMenuB3 = mainWindow->findChild<QComboBox *>(rotaryParamList.at(i));
                    modeMenuB3->setCurrentIndex(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("Rotaries").toMap().value(QString("Rotary%1").arg(currentRotary)).toMap().value(rotaryParamList.at(i)).toInt());
                }
                if(rotaryParamList.at(i).contains("rB4outNotePressMode"))
                {
                    modeMenuB4 = mainWindow->findChild<QComboBox *>(rotaryParamList.at(i));
                    modeMenuB4->setCurrentIndex(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("Rotaries").toMap().value(QString("Rotary%1").arg(currentRotary)).toMap().value(rotaryParamList.at(i)).toInt());
                }
            }
        }
    }

    //display on/off thresholds
    if(mainWindow->findChild<QSpinBox *>("rotaryOnThreshold")){
        onThresh->setValue(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("Rotaries").toMap().value("rotaryOnThreshold").toInt());
    }
    if(mainWindow->findChild<QSpinBox *>("rotaryOffThreshold")){
        offThresh->setValue(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("Rotaries").toMap().value("rotaryOffThreshold").toInt());
    }

    sensitivityDial->setValue(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("Rotaries").toMap().value("rotarySensitivity").toInt());


    //show values for velocity check boxes.
    bool rB1DirectionEnableVal = presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value
            ("ComponentSettings").toMap().value("Rotaries").toMap().value(QString("Rotary%1").arg(currentRotary)).toMap().value
            (QString("rB1outDirectionEnable")).toBool();
    rB1DirectionEnable->setChecked(rB1DirectionEnableVal);
    if(rB1DirectionEnableVal) {
        rB1Direction->setEnabled(true);
        rB1Location->setEnabled(false);
    }
    else {
        rB1Direction->setEnabled(false);
        rB1Location->setEnabled(true);
    }

    bool rB2DirectionEnableVal = presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value
            ("ComponentSettings").toMap().value("Rotaries").toMap().value(QString("Rotary%1").arg(currentRotary)).toMap().value
            (QString("rB2outDirectionEnable")).toBool();
    rB2DirectionEnable->setChecked(rB2DirectionEnableVal);
    if(rB2DirectionEnableVal) {
        rB2Direction->setEnabled(true);
        rB2Location->setEnabled(false);
    }
    else{
        rB2Direction->setEnabled(false);
        rB2Location->setEnabled(true);
    }

    bool rB3DirectionEnableVal = presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value
            ("ComponentSettings").toMap().value("Rotaries").toMap().value(QString("Rotary%1").arg(currentRotary)).toMap().value
            (QString("rB3outDirectionEnable")).toBool();
    rB3DirectionEnable->setChecked(rB3DirectionEnableVal);
    if(rB3DirectionEnableVal) {
        rB3Direction->setEnabled(true);
        rB3Location->setEnabled(false);
    }
    else{
        rB3Direction->setEnabled(false);
        rB3Location->setEnabled(true);
    }

    bool rB4DirectionEnableVal = presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value
            ("ComponentSettings").toMap().value("Rotaries").toMap().value(QString("Rotary%1").arg(currentRotary)).toMap().value
            (QString("rB4outDirectionEnable")).toBool();
    rB4DirectionEnable->setChecked(rB4DirectionEnableVal);
    if(rB4DirectionEnableVal) {
        rB4Direction->setEnabled(true);
        rB4Location->setEnabled(false);
    }
    else {
        rB4Direction->setEnabled(false);
        rB4Location->setEnabled(true);
    }

    //display local LED checkbox
    localLEDControl->setChecked(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value
                                ("ComponentSettings").toMap().value("Rotaries").toMap().value
                                (QString("rotaryLocalLEDControl")).toBool());

    ledInputChannel->setValue(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value
                                ("ComponentSettings").toMap().value("Rotaries").toMap().value
                                (QString("rotaryInChannel")).toInt() + 1);

    //qDebug()<<QString("Local LED Rotary %1").arg(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value
    //                                           ("ComponentSettings").toMap().value("Rotaries").toMap().value
    //                                         (QString("rotaryLocalLEDControl")).toBool());

    //connect display interface after preset recall.
    connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), this, SLOT(slotFocusParam(QWidget*,QWidget*)));
    connect(this, SIGNAL(signalToLabels(QString)), this, SLOT(slotToLabels(QString)));
}

void RotaryEditPane::slotCalculateOffThreshFromOn(int onThreshChanged){
    if(offThresh->value() > (onThresh->value() - 2)){
        offThresh->setValue(onThresh->value() - 2);
    }
    offThresh->setMaximum(onThresh->value() - 2);
}

void RotaryEditPane::slotCheckOffThreshAgainstOn(int offThreshChanged){
    if(offThreshChanged > (onThresh->value() - 2)){
        offThresh->setValue(onThresh->value() - 2);
    }
    offThresh->setMaximum(onThresh->value() - 2);
}

bool RotaryEditPane::eventFilter(QObject *object, QEvent *event){

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

void RotaryEditPane::slotEvents(QString string) {
    //format string from event into a single instance (rotary number)
    if(string.contains("rotaryButton", Qt::CaseSensitive)) {
        currentRotary = string.remove(0,12).toInt();//remove "rotaryButton" and convert last character to int
        displayRotaryValues();
    }
}

void RotaryEditPane::slotRecallPreset(int preset) {
    currentPreset = preset;
    displayRotaryValues();
    if(!prevObjectToLabel.contains("None")) {
        emit signalToLabels(prevObjectToLabel);
    }
}

void RotaryEditPane::slotValueChanged(int num) {
    focusCheck = qobject_cast<QWidget *>(QObject::sender());
    if(focusCheck->hasFocus()){
        if(focusCheck->objectName().contains("VelocityValue") || focusCheck->objectName().contains("PressValue")){
            if (num == 0){
                num = -1;
            }
        }
        emit signalValueChanged(currentPreset, "ComponentSettings", "Rotary", currentRotary, focusCheck->objectName(),num);
        emit signalToLabels(focusCheck->objectName());
    }
    //added the else to else if below
    else if(focusCheck->objectName() == "rB1outDirectionEnable" && num == 0){
        //get value in storage slot, set location value
        /*rB1Location->setValue(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value
                                                          ("ComponentSettings").toMap().value
                                                          ("Rotaries").toMap().value
                                                          (QString("Rotary%1").arg(currentRotary)).toMap().value
                                                          ("rB1LocStorage").toInt());
     //get current direction value, and store in storage
     emit signalValueChanged(currentPreset, "ComponentSettings", "Rotary", currentRotary, "rB1DirStorage",rB1Direction->value());*/
        //set direction to off, disable spin box, enable location
        rB1Direction->setValue(-1);
        emit signalValueChanged(currentPreset, "ComponentSettings", "Rotary", currentRotary, focusCheck->objectName(),num);
        emit signalToLabels(focusCheck->objectName());
    }
    else if(focusCheck->objectName() == "rB1outDirectionEnable" && num == 2){
        //get value in storage slot, set location value
        /*rB1Direction->setValue(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value
                                                          ("ComponentSettings").toMap().value
                                                          ("Rotaries").toMap().value
                                                          (QString("Rotary%1").arg(currentRotary)).toMap().value
                                                          ("rB1DirStorage").toInt());
     //get current direction value, and store in storage
     emit signalValueChanged(currentPreset, "ComponentSettings", "Rotary", currentRotary, "rB1LocStorage",rB1Location->value());*/
        //set direction to off, disable spin box, enable location
        emit signalValueChanged(currentPreset, "ComponentSettings", "Rotary", currentRotary, focusCheck->objectName(),num);
        emit signalToLabels(focusCheck->objectName());
        rB1Location->setValue(-1);
        emit signalValueChanged(currentPreset, "ComponentSettings", "Rotary", currentRotary, QString("rB1outLocation"), -1);
    }
    else if(focusCheck->objectName() == "rB2outDirectionEnable" && num == 0){
        //get value in storage slot, set location value
        /*rB2Location->setValue(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value
                                                          ("ComponentSettings").toMap().value
                                                          ("Rotaries").toMap().value
                                                          (QString("Rotary%1").arg(currentRotary)).toMap().value
                                                          ("rB2LocStorage").toInt());
     //get current direction value, and store in storage
     emit signalValueChanged(currentPreset, "ComponentSettings", "Rotary", currentRotary, "rB2DirStorage",rB2Direction->value());*/
        //set direction to off, disable spin box, enable location
        rB2Direction->setValue(-1);
        emit signalValueChanged(currentPreset, "ComponentSettings", "Rotary", currentRotary, focusCheck->objectName(),num);
        emit signalToLabels(focusCheck->objectName());
    }
    else if(focusCheck->objectName() == "rB2outDirectionEnable" && num == 2){
        //get value in storage slot, set location value
        /*rB2Direction->setValue(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value
                                                          ("ComponentSettings").toMap().value
                                                          ("Rotaries").toMap().value
                                                          (QString("Rotary%1").arg(currentRotary)).toMap().value
                                                          ("rB2DirStorage").toInt());
     //get current direction value, and store in storage
     emit signalValueChanged(currentPreset, "ComponentSettings", "Rotary", currentRotary, "rB2LocStorage",rB2Location->value());*/
        //set direction to off, disable spin box, enable location
        emit signalValueChanged(currentPreset, "ComponentSettings", "Rotary", currentRotary, focusCheck->objectName(),num);
        emit signalToLabels(focusCheck->objectName());
        rB2Location->setValue(-1);
        emit signalValueChanged(currentPreset, "ComponentSettings", "Rotary", currentRotary, QString("rB2outLocation"), -1);
    }
    else if(focusCheck->objectName() == "rB3outDirectionEnable" && num == 0){
        //get value in storage slot, set location value
        /*rB3Location->setValue(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value
                                                          ("ComponentSettings").toMap().value
                                                          ("Rotaries").toMap().value
                                                          (QString("Rotary%1").arg(currentRotary)).toMap().value
                                                          ("rB3LocStorage").toInt());
     //get current direction value, and store in storage
     emit signalValueChanged(currentPreset, "ComponentSettings", "Rotary", currentRotary, "rB3DirStorage",rB3Direction->value());*/
        //set direction to off, disable spin box, enable location
        rB3Direction->setValue(-1);
        emit signalValueChanged(currentPreset, "ComponentSettings", "Rotary", currentRotary, focusCheck->objectName(),num);
        emit signalToLabels(focusCheck->objectName());
    }
    else if(focusCheck->objectName() == "rB3outDirectionEnable" && num == 2){
        //get value in storage slot, set location value
        /*rB3Direction->setValue(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value
                                                          ("ComponentSettings").toMap().value
                                                          ("Rotaries").toMap().value
                                                          (QString("Rotary%1").arg(currentRotary)).toMap().value
                                                          ("rB3DirStorage").toInt());
     //get current direction value, and store in storage
     emit signalValueChanged(currentPreset, "ComponentSettings", "Rotary", currentRotary, "rB3LocStorage",rB3Location->value());*/
        //set direction to off, disable spin box, enable location
        emit signalValueChanged(currentPreset, "ComponentSettings", "Rotary", currentRotary, focusCheck->objectName(), num);
        emit signalToLabels(focusCheck->objectName());
        rB3Location->setValue(-1);
        emit signalValueChanged(currentPreset, "ComponentSettings", "Rotary", currentRotary, QString("rB3outLocation"), -1);
    }
    else if(focusCheck->objectName() == "rB4outDirectionEnable" && num == 0){
        //get value in storage slot, set location value
        /*rB4Location->setValue(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value
                                                          ("ComponentSettings").toMap().value
                                                          ("Rotaries").toMap().value
                                                          (QString("Rotary%1").arg(currentRotary)).toMap().value
                                                          ("rB4LocStorage").toInt());
     //get current direction value, and store in storage
     emit signalValueChanged(currentPreset, "ComponentSettings", "Rotary", currentRotary, "rB4DirStorage",rB4Direction->value());*/
        //set direction to off, disable spin box, enable location
        rB4Direction->setValue(-1);
        emit signalValueChanged(currentPreset, "ComponentSettings", "Rotary", currentRotary, focusCheck->objectName(),num);
        emit signalToLabels(focusCheck->objectName());
    }
    else if(focusCheck->objectName() == "rB4outDirectionEnable" && num == 2){
        //get value in storage slot, set location value
        /*rB4Direction->setValue(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value
                                                          ("ComponentSettings").toMap().value
                                                          ("Rotaries").toMap().value
                                                          (QString("Rotary%1").arg(currentRotary)).toMap().value
                                                          ("rB4DirStorage").toInt());
     //get current direction value, and store in storage
     emit signalValueChanged(currentPreset, "ComponentSettings", "Rotary", currentRotary, "rB4LocStorage",rB4Location->value());*/
        //set direction to off, disable spin box, enable location
        emit signalValueChanged(currentPreset, "ComponentSettings", "Rotary", currentRotary, focusCheck->objectName(),num);
        emit signalToLabels(focusCheck->objectName());
        rB4Location->setValue(-1);
        emit signalValueChanged(currentPreset, "ComponentSettings", "Rotary", currentRotary, QString("rB4outLocation"), -1);
    }

    else if(focusCheck->objectName().contains("Velocity")){
        emit signalValueChanged(currentPreset, "ComponentSettings", "Rotary", currentRotary, focusCheck->objectName(),num);
        emit signalToLabels(focusCheck->objectName());
    }
    else if(focusCheck->objectName().contains("Local")){
        emit signalValueChanged(currentPreset, "ComponentSettings", "Rotary", currentRotary, focusCheck->objectName(),num);
        emit signalToLabels(focusCheck->objectName());
    } else if(focusCheck->objectName().contains("outNotePressMode"))
    {
        emit signalValueChanged(currentPreset, "ComponentSettings", "Rotary", currentRotary, focusCheck->objectName(),num);
        emit signalToLabels(focusCheck->objectName());
    }
}


void RotaryEditPane::slotValueChanged(double num) {
    numDouble = int(num * 100.00);
    focusCheck = qobject_cast<QWidget *>(QObject::sender());
    emit signalValueChanged(currentPreset, "ComponentSettings", "Rotary", currentRotary, focusCheck->objectName(),numDouble);
    emit signalToLabels(focusCheck->objectName());
}


void RotaryEditPane::slotFocusParam(QWidget *oldObject, QWidget *nowObject){
    qDebug() << "RotaryEditPane called";
    if(nowObject != nullptr) {
        clicked = nowObject;
        if(clicked->parent()->objectName().contains("rotary")) {
            emit signalToLabels(clicked->objectName());
        }
    }
}

void RotaryEditPane::slotToLabels(QString parameter) {
    //display parameter on both rotaries.
    for(int i = 0; i < 2; i++) {
        rotaryLabel[i]->slotDisplayParameter(parameter, i, currentPreset);
    }
    prevObjectToLabel = parameter;

}

void RotaryEditPane::slotDirectionOrLocation(int enableSwitch) {
    QWidget *sender = qobject_cast<QWidget *>(QObject::sender());
    QString whichBank = sender->objectName().remove(0, 2);
    whichBank = whichBank.remove(1, whichBank.length());
    if(whichBank.contains("1")) {
        currentBank = 1; //Bank1
        if(enableSwitch) {
            rB1Direction->setEnabled(true);
            rB1Location->setEnabled(false);
        }
        else {
            rB1Direction->setEnabled(false);
            rB1Location->setEnabled(true);
        }
    }
    if(whichBank.contains("2")) {
        currentBank = 2;
        if(enableSwitch) {
            rB2Direction->setEnabled(true);
            rB2Location->setEnabled(false);
        }
        else {
            rB2Direction->setEnabled(false);
            rB2Location->setEnabled(true);
        }
    }
    if(whichBank.contains("3")) {
        currentBank = 3;
        if(enableSwitch) {
            rB3Direction->setEnabled(true);
            rB3Location->setEnabled(false);
        }
        else {
            rB3Direction->setEnabled(false);
            rB3Location->setEnabled(true);
        }
    }
    if(whichBank.contains("4")) {
        currentBank = 4;
        if(enableSwitch) {
            rB4Direction->setEnabled(true);
            rB4Location->setEnabled(false);
        }
        else {
            rB4Direction->setEnabled(false);
            rB4Location->setEnabled(true);
        }
    }
}

