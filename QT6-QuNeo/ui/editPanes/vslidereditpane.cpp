#include "vslidereditpane.h"

VSliderEditPane::VSliderEditPane(QVariantMap *variantMap, QVariantMap *variantMapCopy, QWidget *widget, QWidget *parent) :
    QWidget(parent)
{
    //this->setStyleSheet("QLabel { font: 10px } QLineEdit { font: 12px } QCheckBox {font:9px} QSpinBox {font: 10px} QDoubleSpinBox {font: 10px}");

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

    connect(sensitivityDial, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int)));

    for(int i = 0; i < 4; i++){
        vSliderLabel[i] = new VSliderLabels(mainWindow, presetMapCopy, 0);
    }
#ifdef Q_OS_MAC

#else
    QLabel *sensitivityLabel = mainWindow->findChild<QLabel *>("vSliderSensitivityLabel");
    QFont f( "Arial", 3, QFont::Normal);
    sensitivityLabel->setFont(f);
#endif
}

bool VSliderEditPane::eventFilter(QObject *object, QEvent *event){
    Q_UNUSED(object);
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

void VSliderEditPane::slotEvents(QString string)
{
    if(string.contains("vSliderButton", Qt::CaseSensitive)){
        currentVSlider = string.remove(0,13).toInt();
        displayVSliderValues();
    }
}

void VSliderEditPane::findUiElements()
{
    QTabWidget *vSliderTabs = mainWindow->findChild<QTabWidget *>("vSliderTabs");
    vSliderTabs->setCurrentIndex(0);

    vSliderParamList = presetMapCopy->value("Preset 0").toMap().value("ComponentSettings").toMap().value(QString("VSliders")).toMap().value(QString("VSlider0")).toMap().keys();
    for (int i = 0; i<vSliderParamList.count(); i++){

        if(mainWindow->findChild<QSpinBox *>(QString(vSliderParamList.at(i)))){

            if (!vSliderParamList.at(i).contains("Sensitivity", Qt::CaseSensitive) && !vSliderParamList.at(i).contains("localLEDControl", Qt::CaseSensitive)){
                spinBoxMap.insert(vSliderParamList.at(i), mainWindow->findChild<QSpinBox *>(QString(vSliderParamList.at(i))));
                //set up all spin boxes to have correct minimum / maximum and "off" text.
                if(!vSliderParamList.at(i).contains("Channel", Qt::CaseSensitive) && !vSliderParamList.at(i).contains("PassThru")){
                    spinBoxMap.value(QString(vSliderParamList.at(i)))->setMinimum(-1);
                    spinBoxMap.value(QString(vSliderParamList.at(i)))->setMaximum(127);
                    spinBoxMap.value(QString(vSliderParamList.at(i)))->setSpecialValueText("off");
                    if(vSliderParamList.at(i).contains("VelocityValue", Qt::CaseSensitive))
                    {
                        spinBoxMap.value(QString(vSliderParamList.at(i)))->setMinimum(0);
                        spinBoxMap.value(QString(vSliderParamList.at(i)))->setMaximum(127);
                        spinBoxMap.value(QString(vSliderParamList.at(i)))->setSpecialValueText("var");
                    }
                    if(vSliderParamList.at(i).contains("PressValue", Qt::CaseSensitive))
                    {
                        spinBoxMap.value(QString(vSliderParamList.at(i)))->setMinimum(0);
                        spinBoxMap.value(QString(vSliderParamList.at(i)))->setMaximum(127);
                        spinBoxMap.value(QString(vSliderParamList.at(i)))->setSpecialValueText("con");
                    }
                }
                else if (vSliderParamList.at(i).contains("PassThru")){
                    spinBoxMap.value(QString(vSliderParamList.at(i)))->setMinimum(1);
                    spinBoxMap.value(QString(vSliderParamList.at(i)))->setMaximum(127);

                    //hide pas thru stuff
                    spinBoxMap.value(QString(vSliderParamList.at(i)))->hide();
                    mainWindow->findChild<QLabel *>("vB1outLocPassThruRangeLabel")->hide();
                    mainWindow->findChild<QLabel *>("vB2outLocPassThruRangeLabel")->hide();
                    mainWindow->findChild<QLabel *>("vB3outLocPassThruRangeLabel")->hide();
                    mainWindow->findChild<QLabel *>("vB4outLocPassThruRangeLabel")->hide();

                }
                else if(vSliderParamList.at(i).contains("Channel", Qt::CaseSensitive)){ //no off state, 1-16
                    spinBoxMap.value(QString(vSliderParamList.at(i)))->setMinimum(1);
                    spinBoxMap.value(QString(vSliderParamList.at(i)))->setMaximum(16);}
            }
            connect(spinBoxMap.value(QString(vSliderParamList.at(i))), SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int)));

            uiObject = mainWindow->findChild<QSpinBox *>(QString(vSliderParamList.at(i)));

        } else  if(mainWindow->findChild<QCheckBox *>(QString(vSliderParamList.at(i)))){

            if(mainWindow->findChild<QCheckBox *>("hB1outVelocity")){
                vB1Velocity = mainWindow->findChild<QCheckBox *>("vB1outVelocity");
                connect(vB1Velocity, SIGNAL(stateChanged(int)), this, SLOT(slotValueChanged(int)));
            }
            if(mainWindow->findChild<QCheckBox *>("vB2outVelocity")){
                vB2Velocity = mainWindow->findChild<QCheckBox *>("vB2outVelocity");
                connect(vB2Velocity, SIGNAL(stateChanged(int)), this, SLOT(slotValueChanged(int)));
            }
            if(mainWindow->findChild<QCheckBox *>("vB3outVelocity")){
                vB3Velocity = mainWindow->findChild<QCheckBox *>("vB3outVelocity");
                connect(vB3Velocity, SIGNAL(stateChanged(int)), this, SLOT(slotValueChanged(int)));
            }
            if(mainWindow->findChild<QCheckBox *>("vB4outVelocity")){
                vB4Velocity = mainWindow->findChild<QCheckBox *>("vB4outVelocity");
                connect(vB4Velocity, SIGNAL(stateChanged(int)), this, SLOT(slotValueChanged(int)));
            }

        }  else  if(mainWindow->findChild<QComboBox *>(QString(vSliderParamList.at(i)))){

            if(vSliderParamList.at(i).contains("outNotePressMode"))
            {
                if(vSliderParamList.at(i).contains("vB1outNotePressMode"))
                {
                    modeMenuB1 = mainWindow->findChild<QComboBox *>(vSliderParamList.at(i));
                    connect(modeMenuB1, SIGNAL(currentIndexChanged(int)), this, SLOT(slotValueChanged(int)));
                }
                if(vSliderParamList.at(i).contains("vB2outNotePressMode"))
                {
                    modeMenuB2 = mainWindow->findChild<QComboBox *>(vSliderParamList.at(i));
                    connect(modeMenuB2, SIGNAL(currentIndexChanged(int)), this, SLOT(slotValueChanged(int)));
                }
                if(vSliderParamList.at(i).contains("vB3outNotePressMode"))
                {
                    modeMenuB3 = mainWindow->findChild<QComboBox *>(vSliderParamList.at(i));
                    connect(modeMenuB3, SIGNAL(currentIndexChanged(int)), this, SLOT(slotValueChanged(int)));
                }
                if(vSliderParamList.at(i).contains("vB4outNotePressMode"))
                {
                    modeMenuB4 = mainWindow->findChild<QComboBox *>(vSliderParamList.at(i));
                    connect(modeMenuB4, SIGNAL(currentIndexChanged(int)), this, SLOT(slotValueChanged(int)));
                }
            }
        }
        else {
            vSliderUiBlackList.append(vSliderParamList.at(i));
            qDebug() << "VSlider Ui ***" << vSliderParamList.at(i) << "Not Found";
        }
    }
    //install event filter in all the spinboxes so that
    //copy/paste commands can be intercepted while they are in focus
    for(int i=0; i<vSliderParamList.count();i++)
       {
           if(spinBoxMap.contains(vSliderParamList.at(i)))
           {
               spinBoxMap.value(QString(vSliderParamList.at(i)))->installEventFilter(this);
           }
       }

    paneLabel = mainWindow->findChild<QLabel *>("vSliderName");
    sensitivityDial = mainWindow->findChild<QDial *>("vSliderSensitivity");

    if(mainWindow->findChild<QSpinBox *>("vSliderOnThreshold")){
        onThresh = mainWindow->findChild<QSpinBox *>("vSliderOnThreshold");
        onThresh->setMinimum(3);
        connect(onThresh, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int)));
        connect(onThresh, SIGNAL(valueChanged(int)), this, SLOT(slotCalculateOffThreshFromOn(int)));
    } else {
        qDebug() << "vSlider GLobal Ui ***" << "vSliderOnThreshold" << "Not Found";
    }

    if(mainWindow->findChild<QSpinBox *>("vSliderOffThreshold")){
        offThresh = mainWindow->findChild<QSpinBox *>("vSliderOffThreshold");
        offThresh->setMinimum(1);
        connect(offThresh, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int)));
        connect(offThresh, SIGNAL(valueChanged(int)), this, SLOT(slotCheckOffThreshAgainstOn(int)));
    } else {
        qDebug() << "vSlider GLobal Ui ***" << "vSliderOffThreshold" << "Not Found";
    }

    localLEDControl = mainWindow->findChild<QCheckBox *>("vSliderLocalLEDControl");
    connect(localLEDControl, SIGNAL(stateChanged(int)), this, SLOT(slotValueChanged(int)));

    ledInputChannel = mainWindow->findChild<QSpinBox *>("vSliderInChannel");
    connect(ledInputChannel, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int)));

    ledInputChannel->installEventFilter(this);
    onThresh->installEventFilter(this);
    offThresh->installEventFilter(this);
    sensitivityDial->installEventFilter(this);

}

void VSliderEditPane::displayVSliderValues()
{
    //disconnect param display labels during preset recall.
    disconnect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), this, SLOT(slotFocusParam(QWidget*,QWidget*))); //displays based on focus
    disconnect(this, SIGNAL(signalToLabels(QString)), this, SLOT(slotToLabels(QString))); //displays when value changed

    //get clicked slider map
    vSliderMap = presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("VSliders").toMap().value(QString("VSlider%1").arg(currentVSlider)).toMap();

    //set edit pane title
    paneLabel->setText(QString("Vertical Slider %1").arg(currentVSlider+1));

    //clear focus before displaying values
    for(int i=0; i<vSliderParamList.count();i++){
        if(!vSliderUiBlackList.contains(vSliderParamList.at(i))){
            if(spinBoxMap.contains(vSliderParamList.at(i))){
                if (!vSliderParamList.at(i).contains("localLEDControl", Qt::CaseSensitive)){
                    spinBoxMap.value(QString(vSliderParamList.at(i)))->clearFocus();
                }
            }
        }
    }

    //iterate through param names and set values in edit pane ui
    for(int i = 0; i<vSliderParamList.count(); i++)
    {
        if(!vSliderUiBlackList.contains(vSliderParamList.at(i)))
        {
            if(mainWindow->findChild<QSpinBox *>(vSliderParamList.at(i)))
            {
                if (!vSliderParamList.at(i).contains("Sensitivity", Qt::CaseSensitive))
                {
                    if (!vSliderParamList.at(i).contains("localLEDControl", Qt::CaseSensitive))
                    {
                        if(vSliderParamList.at(i).contains("Channel", Qt::CaseSensitive))
                        {
                            spinBoxMap.value(QString(vSliderParamList.at(i)))->setValue(vSliderMap.value(QString(vSliderParamList.at(i))).toInt() + 1);
                        }
                        else
                        {
                            spinBoxMap.value(QString(vSliderParamList.at(i)))->setValue(vSliderMap.value(QString(vSliderParamList.at(i))).toInt());
                        }
                        if(QString(vSliderParamList.at(i)).contains("in"))
                        {
                            spinBoxMap.value(QString(vSliderParamList.at(i)))->hide();
                            QLabel *ledParamToHide = mainWindow->findChild<QLabel *>(QString("%1Label").arg(vSliderParamList.at(i)));
                            ledParamToHide->hide();
                        }
                    }
                }
            } else if(mainWindow->findChild<QComboBox *>(vSliderParamList.at(i))){


                if (vSliderParamList.at(i).contains("outNotePressMode"))
                {
                    if(vSliderParamList.at(i).contains("vB1outNotePressMode"))
                    {
                        modeMenuB1 = mainWindow->findChild<QComboBox *>(vSliderParamList.at(i));
                        modeMenuB1->setCurrentIndex(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("VSliders").toMap().value(QString("VSlider%1").arg(currentVSlider)).toMap().value(vSliderParamList.at(i)).toInt());
                    }
                    if(vSliderParamList.at(i).contains("vB2outNotePressMode"))
                    {
                        modeMenuB2 = mainWindow->findChild<QComboBox *>(vSliderParamList.at(i));
                        modeMenuB2->setCurrentIndex(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("VSliders").toMap().value(QString("VSlider%1").arg(currentVSlider)).toMap().value(vSliderParamList.at(i)).toInt());
                    }
                    if(vSliderParamList.at(i).contains("vB3outNotePressMode"))
                    {
                        modeMenuB3 = mainWindow->findChild<QComboBox *>(vSliderParamList.at(i));
                        modeMenuB3->setCurrentIndex(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("VSliders").toMap().value(QString("VSlider%1").arg(currentVSlider)).toMap().value(vSliderParamList.at(i)).toInt());
                    }
                    if(vSliderParamList.at(i).contains("vB4outNotePressMode"))
                    {
                        modeMenuB4 = mainWindow->findChild<QComboBox *>(vSliderParamList.at(i));
                        modeMenuB4->setCurrentIndex(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("VSliders").toMap().value(QString("VSlider%1").arg(currentVSlider)).toMap().value(vSliderParamList.at(i)).toInt());
                    }
                }

            }
        }
    }

    sensitivityDial->setValue(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value
                              ("ComponentSettings").toMap().value("VSliders").toMap().value("vSliderSensitivity").toInt());

    //display local LED checkbox
    localLEDControl->setChecked(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("VSliders").toMap().value("vSliderLocalLEDControl").toBool());

    ledInputChannel->setValue(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("VSliders").toMap().value("vSliderInChannel").toInt() + 1);
    //display on/off thresholds
    if(mainWindow->findChild<QSpinBox *>("vSliderOnThreshold")){
        onThresh->setValue(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("VSliders").toMap().value("vSliderOnThreshold").toInt());
    }
    if(mainWindow->findChild<QSpinBox *>("vSliderOffThreshold")){
        offThresh->setValue(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("VSliders").toMap().value("vSliderOffThreshold").toInt());
    }

    //connect display interface after preset recall.
    connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), this, SLOT(slotFocusParam(QWidget*,QWidget*)));
    connect(this, SIGNAL(signalToLabels(QString)), this, SLOT(slotToLabels(QString)));
}

void VSliderEditPane::slotCalculateOffThreshFromOn(int onThreshChanged){
    Q_UNUSED(onThreshChanged);
    if(offThresh->value() > (onThresh->value() - 2)){
        offThresh->setValue(onThresh->value() - 2);
    }
    offThresh->setMaximum(onThresh->value() - 2);
}

void VSliderEditPane::slotCheckOffThreshAgainstOn(int offThreshChanged){
    if(offThreshChanged > (onThresh->value() - 2)){
        offThresh->setValue(onThresh->value() - 2);
    }
    offThresh->setMaximum(onThresh->value() - 2);
}

void VSliderEditPane::slotRecallPreset(int preset){
    currentPreset = preset;
    displayVSliderValues();
    if(!prevObjectToLabel.contains("None")){
        emit signalToLabels(prevObjectToLabel);}
}

void VSliderEditPane::slotValueChanged(int num){

    focusCheck = qobject_cast<QWidget *>(QObject::sender());
    if(focusCheck->hasFocus()){
        if(focusCheck->objectName().contains("VelocityValue") || focusCheck->objectName().contains("PressValue")){
            if (num == 0){
                num = -1;
            }
        }
        //int presetNum, QString settingsType, QString paramType, int typeNum, QVariantMap paramValMap
        emit signalValueChanged(currentPreset, "ComponentSettings", "VSlider", currentVSlider, focusCheck->objectName(),num);
        emit signalToLabels(focusCheck->objectName()); //whenever a value is changed, this should be emitted, but it shouldn't always have an effect.
    }
    else if(focusCheck->objectName().contains("Velocity")
            ||focusCheck->objectName().contains("Enable")
            ||focusCheck->objectName().contains("Local")
            ||focusCheck->objectName().contains("BankControl")
            ||focusCheck->objectName().contains("outNotePressMode")){
        emit signalValueChanged(currentPreset, "ComponentSettings", "VSlider", currentVSlider, focusCheck->objectName(),num);
        emit signalToLabels(focusCheck->objectName());
    }
}


void VSliderEditPane::slotToLabels(QString parameter){
    for(int i = 0; i < 4; i++){  //display parameter on all 4 sliders.
        vSliderLabel[i]->slotDisplayParameter(parameter, i, currentPreset);}
    prevObjectToLabel = parameter;
}

void VSliderEditPane::slotFocusParam(QWidget *oldObject, QWidget *nowObject){
    Q_UNUSED(oldObject);
    qDebug() << "VSliderEditPane called";
    if(nowObject != nullptr){
        clicked = nowObject;
        if(clicked->parent()->objectName().contains("vSlider")){
            emit signalToLabels(clicked->objectName());
            if(clicked->objectName().contains("B1")){
                currentBank = 1;}
            if(clicked->objectName().contains("B2")){
                currentBank = 2;}
            if(clicked->objectName().contains("B3")){
                currentBank = 3;}
            if(clicked->objectName().contains("B4")){
                currentBank = 4;}
        }}
}
