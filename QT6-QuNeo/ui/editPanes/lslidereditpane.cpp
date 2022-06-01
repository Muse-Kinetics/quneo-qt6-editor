#include "lslidereditpane.h"

LSliderEditPane::LSliderEditPane(QVariantMap *variantMap, QVariantMap *variantMapCopy, QWidget *widget, QWidget *parent) :
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



    for(int i = 0; i < 1; i++){
        lSliderLabel[i] = new LSliderLabels(mainWindow, presetMapCopy, 0);
    }

#ifdef Q_OS_MAC

#else
    QLabel *sensitivityLabel = mainWindow->findChild<QLabel *>("lSliderSensitivityLabel");
    QFont f( "Arial", 3, QFont::Normal);
    sensitivityLabel->setFont(f);


    QPushButton *buttonPointer = mainWindow->findChild<QPushButton *>("updateButton");
    QFont ff( "Verdana", 10, QFont::Normal);
    buttonPointer->setFont(ff);
    buttonPointer = mainWindow->findChild<QPushButton *>("updateAll");
    buttonPointer->setFont(ff);
    buttonPointer = mainWindow->findChild<QPushButton *>("saveButton");
    buttonPointer->setFont(ff);
    buttonPointer = mainWindow->findChild<QPushButton *>("revertButton");
    buttonPointer->setFont(ff);

#endif



}

bool LSliderEditPane::eventFilter(QObject *object, QEvent *event){
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

void LSliderEditPane::slotEvents(QString string)
{
    if(string.contains("lSliderButton", Qt::CaseSensitive)){
        displayLSliderValues();
    }

}

void LSliderEditPane::findUiElements()
{
    QTabWidget *lSliderTabs = mainWindow->findChild<QTabWidget *>("lSliderTabs");
    lSliderTabs->setCurrentIndex(0);

    lSliderParamList = presetMapCopy->value("Preset 0").toMap().value("ComponentSettings").toMap().value(QString("LongSliders")).toMap().value(QString("LongSlider0")).toMap().keys();
    for (int i = 0; i<lSliderParamList.count(); i++){

        if(mainWindow->findChild<QSpinBox *>(QString(lSliderParamList.at(i)))){

            if (!lSliderParamList.at(i).contains("localLEDControl", Qt::CaseSensitive)){
                if (!lSliderParamList.at(i).contains("Sensitivity", Qt::CaseSensitive)){
                    spinBoxMap.insert(lSliderParamList.at(i), mainWindow->findChild<QSpinBox *>(QString(lSliderParamList.at(i))));
                    //set up all spin boxes to have correct minimum / maximum and "off" text.
                    if(!lSliderParamList.at(i).contains("Channel", Qt::CaseSensitive) && !lSliderParamList.at(i).contains("PassThru", Qt::CaseSensitive)){
                        spinBoxMap.value(QString(lSliderParamList.at(i)))->setMinimum(-1);
                        spinBoxMap.value(QString(lSliderParamList.at(i)))->setMaximum(127);
                        spinBoxMap.value(QString(lSliderParamList.at(i)))->setSpecialValueText("off");

                        if(lSliderParamList.at(i).contains("PressValue", Qt::CaseSensitive)){
                            spinBoxMap.value(QString(lSliderParamList.at(i)))->setMinimum(0);
                            spinBoxMap.value(QString(lSliderParamList.at(i)))->setMaximum(127);
                            spinBoxMap.value(QString(lSliderParamList.at(i)))->setSpecialValueText("con");
                        }
                        if(lSliderParamList.at(i).contains("VelocityValue", Qt::CaseSensitive)){
                            spinBoxMap.value(QString(lSliderParamList.at(i)))->setMinimum(0);
                            spinBoxMap.value(QString(lSliderParamList.at(i)))->setMaximum(127);
                            spinBoxMap.value(QString(lSliderParamList.at(i)))->setSpecialValueText("var");
                        }
                    }
                    else if(lSliderParamList.at(i).contains("PassThru", Qt::CaseSensitive)){
                        spinBoxMap.value(QString(lSliderParamList.at(i)))->setMinimum(1);
                        spinBoxMap.value(QString(lSliderParamList.at(i)))->setMaximum(127);

                        //hide pass thru stuff
                        spinBoxMap.value(QString(lSliderParamList.at(i)))->hide();
                        mainWindow->findChild<QLabel *>("lB1outLocPassThruRangeLabel")->hide();
                        mainWindow->findChild<QLabel *>("lB2outLocPassThruRangeLabel")->hide();
                        mainWindow->findChild<QLabel *>("lB3outLocPassThruRangeLabel")->hide();
                        mainWindow->findChild<QLabel *>("lB4outLocPassThruRangeLabel")->hide();
                    }
                    else if(lSliderParamList.at(i).contains("Channel", Qt::CaseSensitive)){ //no off state, 1-16
                        spinBoxMap.value(QString(lSliderParamList.at(i)))->setMinimum(1);
                        spinBoxMap.value(QString(lSliderParamList.at(i)))->setMaximum(16);}
                }
                connect(spinBoxMap.value(QString(lSliderParamList.at(i))), SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int)));
            }

        } else if(mainWindow->findChild<QCheckBox *>(QString(lSliderParamList.at(i)))){

            if(mainWindow->findChild<QCheckBox *>("lB1outVelocity")){
                lB1Velocity = mainWindow->findChild<QCheckBox *>("lB1outVelocity");
                connect(lB1Velocity, SIGNAL(stateChanged(int)), this, SLOT(slotValueChanged(int)));
            }
            if(mainWindow->findChild<QCheckBox *>("lB2outVelocity")){
                lB2Velocity = mainWindow->findChild<QCheckBox *>("lB2outVelocity");
                connect(lB2Velocity, SIGNAL(stateChanged(int)), this, SLOT(slotValueChanged(int)));
            }
            if(mainWindow->findChild<QCheckBox *>("lB3outVelocity")){
                lB3Velocity = mainWindow->findChild<QCheckBox *>("lB3outVelocity");
                connect(lB3Velocity, SIGNAL(stateChanged(int)), this, SLOT(slotValueChanged(int)));
            }
            if(mainWindow->findChild<QCheckBox *>("lB4outVelocity")){
                lB4Velocity = mainWindow->findChild<QCheckBox *>("lB4outVelocity");
                connect(lB4Velocity, SIGNAL(stateChanged(int)), this, SLOT(slotValueChanged(int)));
            }

        }  else if(mainWindow->findChild<QComboBox *>(QString(lSliderParamList.at(i)))){

            if(lSliderParamList.at(i).contains("outNotePressMode")){

                if(lSliderParamList.at(i).contains("lB1outNotePressMode"))
                {
                    modeMenuB1 = mainWindow->findChild<QComboBox *>(lSliderParamList.at(i));
                    connect(modeMenuB1, SIGNAL(currentIndexChanged(int)), this, SLOT(slotValueChanged(int)));
                }
                if(lSliderParamList.at(i).contains("lB2outNotePressMode"))
                {
                    modeMenuB2 = mainWindow->findChild<QComboBox *>(lSliderParamList.at(i));
                    connect(modeMenuB2, SIGNAL(currentIndexChanged(int)), this, SLOT(slotValueChanged(int)));
                }
                if(lSliderParamList.at(i).contains("lB3outNotePressMode"))
                {
                    modeMenuB3 = mainWindow->findChild<QComboBox *>(lSliderParamList.at(i));
                    connect(modeMenuB3, SIGNAL(currentIndexChanged(int)), this, SLOT(slotValueChanged(int)));
                }
                if(lSliderParamList.at(i).contains("lB4outNotePressMode"))
                {
                    modeMenuB4 = mainWindow->findChild<QComboBox *>(lSliderParamList.at(i));
                    connect(modeMenuB4, SIGNAL(currentIndexChanged(int)), this, SLOT(slotValueChanged(int)));
                }
            }
        }
        else {
            lSliderUiBlacklist.append(lSliderParamList.at(i));
            qDebug() << "LSlider Ui ***" << lSliderParamList.at(i) << "Not Found";
        }
    }

    //install event filter in all the spinboxes so that
    //copy/paste commands can be intercepted while they are in focus
    for(int i=0; i<lSliderParamList.count();i++)
       {
           if(spinBoxMap.contains(lSliderParamList.at(i)))
           {
               spinBoxMap.value(QString(lSliderParamList.at(i)))->installEventFilter(this);
           }
       }

    //LSlider Globals
    if(mainWindow->findChild<QDial *>("lSliderSensitivity")){
        sensitivityDial = mainWindow->findChild<QDial *>("lSliderSensitivity");
        sensitivityDial->setMinimum(70);
        sensitivityDial->setMaximum(150);
        sensitivityDial->setSingleStep(1);
        connect(sensitivityDial, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int)));
    } else {
        qDebug() << "LSlider Ui ***" << "lSliderSensitivity" << "Not Found";
    }

    if(mainWindow->findChild<QCheckBox *>("lSliderLocalLEDControl")){
        localLEDControl = mainWindow->findChild<QCheckBox *>("lSliderLocalLEDControl");
        connect(localLEDControl, SIGNAL(stateChanged(int)), this, SLOT(slotValueChanged(int)));
    } else {
        qDebug() << "LSlider Ui ***" << "lSliderLocalLEDControl" << "Not Found";
    }

    if(mainWindow->findChild<QSpinBox *>("lSliderInChannel")){
        ledInputChannel = mainWindow->findChild<QSpinBox *>("lSliderInChannel");
        connect(ledInputChannel, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int)));
    } else {
        qDebug() << "LSlider Ui ***" << "lSliderInChannel" << "Not Found";
    }

    if(mainWindow->findChild<QSpinBox *>("lSliderOnThreshold")){
        onThresh = mainWindow->findChild<QSpinBox *>("lSliderOnThreshold");
        onThresh->setMinimum(3);
        connect(onThresh, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int)));
        connect(onThresh, SIGNAL(valueChanged(int)), this, SLOT(slotCalculateOffThreshFromOn(int)));
    } else {
        qDebug() << "lSlider GLobal Ui ***" << "lSliderOnThreshold" << "Not Found";
    }

    if(mainWindow->findChild<QSpinBox *>("lSliderOffThreshold")){
        offThresh = mainWindow->findChild<QSpinBox *>("lSliderOffThreshold");
        offThresh->setMinimum(1);
        connect(offThresh, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int)));
        connect(offThresh, SIGNAL(valueChanged(int)), this, SLOT(slotCheckOffThreshAgainstOn(int)));
    } else {
        qDebug() << "lSlider GLobal Ui ***" << "lSliderOffThreshold" << "Not Found";
    }

    ledInputChannel->installEventFilter(this);
    onThresh->installEventFilter(this);
    offThresh->installEventFilter(this);
    sensitivityDial->installEventFilter(this);

}

void LSliderEditPane::displayLSliderValues()
{
    //disconnect param display labels during preset recall.
    disconnect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), this, SLOT(slotFocusParam(QWidget*,QWidget*))); //displays based on focus
    disconnect(this, SIGNAL(signalToLabels(QString)), this, SLOT(slotToLabels(QString))); //displays when value changed
    //get clicked slider map
    lSliderMap = presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("LongSliders").toMap().value("LongSlider0").toMap();

    //clear focus before displaying values
    for(int i=0; i<lSliderParamList.count();i++){
        if(!lSliderUiBlacklist.contains(lSliderParamList.at(i))){
            if(spinBoxMap.contains(lSliderParamList.at(i))){
                if (!lSliderParamList.at(i).contains("localLEDControl", Qt::CaseSensitive)){
                    spinBoxMap.value(QString(lSliderParamList.at(i)))->clearFocus();
                }
            }
        }
    }

    //iterate through param names and set values in edit pane ui
    for(int i = 0; i<lSliderParamList.count(); i++){
        if(!lSliderUiBlacklist.contains(lSliderParamList.at(i))){
            if(spinBoxMap.contains(lSliderParamList.at(i))){
                if(!lSliderParamList.at(i).contains("Sensitivity", Qt::CaseSensitive)){
                    if(!lSliderParamList.at(i).contains("localLEDControl", Qt::CaseSensitive)){
                        if(lSliderParamList.at(i).contains("Channel", Qt::CaseSensitive)){
                            spinBoxMap.value(QString(lSliderParamList.at(i)))->setValue(lSliderMap.value(QString(lSliderParamList.at(i))).toInt() + 1);}
                        else {spinBoxMap.value(QString(lSliderParamList.at(i)))->setValue(lSliderMap.value(QString(lSliderParamList.at(i))).toInt());}}
                    if(QString(lSliderParamList.at(i)).contains("in")){
                        spinBoxMap.value(QString(lSliderParamList.at(i)))->hide();
                        QLabel *ledParamToHide = mainWindow->findChild<QLabel *>(QString("%1Label").arg(lSliderParamList.at(i)));
                        ledParamToHide->hide();
                    }
                }
            }
            else if(mainWindow->findChild<QComboBox *>(lSliderParamList.at(i))){
                if(lSliderParamList.at(i).contains("NotePressMode")){
                    if (lSliderParamList.at(i).contains("outNotePressMode"))
                    {
                        if(lSliderParamList.at(i).contains("lB1outNotePressMode"))
                        {
                            modeMenuB1 = mainWindow->findChild<QComboBox *>(lSliderParamList.at(i));
                            modeMenuB1->setCurrentIndex(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("LongSliders").toMap().value(QString("LongSlider0")).toMap().value(lSliderParamList.at(i)).toInt());
                        }
                        if(lSliderParamList.at(i).contains("lB2outNotePressMode"))
                        {
                            modeMenuB2 = mainWindow->findChild<QComboBox *>(lSliderParamList.at(i));
                            modeMenuB2->setCurrentIndex(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("LongSliders").toMap().value(QString("LongSlider0")).toMap().value(lSliderParamList.at(i)).toInt());
                        }
                        if(lSliderParamList.at(i).contains("lB3outNotePressMode"))
                        {
                            modeMenuB3 = mainWindow->findChild<QComboBox *>(lSliderParamList.at(i));
                            modeMenuB3->setCurrentIndex(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("LongSliders").toMap().value(QString("LongSlider0")).toMap().value(lSliderParamList.at(i)).toInt());
                        }
                        if(lSliderParamList.at(i).contains("lB4outNotePressMode"))
                        {
                            modeMenuB4 = mainWindow->findChild<QComboBox *>(lSliderParamList.at(i));
                            modeMenuB4->setCurrentIndex(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("LongSliders").toMap().value(QString("LongSlider0")).toMap().value(lSliderParamList.at(i)).toInt());
                        }
                    }
                }
            }
        }
    }

    //display sensitivity dial value.
    sensitivityDial->setValue(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("LongSliders").toMap().value("lSliderSensitivity").toInt());
    //display local LED checkbox
    localLEDControl->setChecked(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("LongSliders").toMap().value("lSliderLocalLEDControl").toBool());
    ledInputChannel->setValue(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("LongSliders").toMap().value("lSliderInChannel").toInt() + 1);
    //display on/off thresholds
    if(mainWindow->findChild<QSpinBox *>("lSliderOnThreshold")){
        onThresh->setValue(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("LongSliders").toMap().value("lSliderOnThreshold").toInt());
    }
    if(mainWindow->findChild<QSpinBox *>("lSliderOffThreshold")){
        offThresh->setValue(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("LongSliders").toMap().value("lSliderOffThreshold").toInt());
    }
    //connect display interface after preset recall.
    connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), this, SLOT(slotFocusParam(QWidget*,QWidget*)));
    connect(this, SIGNAL(signalToLabels(QString)), this, SLOT(slotToLabels(QString)));
}

void LSliderEditPane::slotCalculateOffThreshFromOn(int onThreshChanged){
    Q_UNUSED(onThreshChanged);
    if(offThresh->value() > (onThresh->value() - 2)){
        offThresh->setValue(onThresh->value() - 2);
    }
    offThresh->setMaximum(onThresh->value() - 2);
}

void LSliderEditPane::slotCheckOffThreshAgainstOn(int offThreshChanged){
    if(offThreshChanged > (onThresh->value() - 2)){
        offThresh->setValue(onThresh->value() - 2);
    }
    offThresh->setMaximum(onThresh->value() - 2);
}


void LSliderEditPane::slotRecallPreset(int preset)
{
    currentPreset = preset;
    displayLSliderValues();
    if(!prevObjectToLabel.contains("None"))
    {
        emit signalToLabels(prevObjectToLabel);
    }
}

void LSliderEditPane::slotValueChanged(int num){

    focusCheck = qobject_cast<QWidget *>(QObject::sender());
    if(focusCheck->hasFocus()){
        //int presetNum, QString settingsType, QString paramType, int typeNum, QVariantMap paramValMap

        if(focusCheck->objectName().contains("VelocityValue") || focusCheck->objectName().contains("PressValue")){
            if (num == 0){
                num = -1;
            }
        }
        emit signalValueChanged(currentPreset, "ComponentSettings", "LongSlider", 0, focusCheck->objectName(),num);
        emit signalToLabels(focusCheck->objectName());
    }
    else if(focusCheck->objectName().contains("Local")){
        emit signalValueChanged(currentPreset, "ComponentSettings", "LongSlider", 0, focusCheck->objectName(),num);
        emit signalToLabels(focusCheck->objectName());
    }
    else if(focusCheck->objectName().contains("Velocity")){
        emit signalValueChanged(currentPreset, "ComponentSettings", "LongSlider", 0, focusCheck->objectName(),num);
        emit signalToLabels(focusCheck->objectName());
    } else if(focusCheck->objectName().contains("outNotePressMode")){
        emit signalValueChanged(currentPreset, "ComponentSettings", "LongSlider", 0, focusCheck->objectName(),num);
        emit signalToLabels(focusCheck->objectName());
    }
}

void LSliderEditPane::slotFocusParam(QWidget *oldObject, QWidget *nowObject){
    Q_UNUSED(oldObject);
    qDebug() << "LSliderEditPane called";
    if(nowObject != nullptr){
        clicked = nowObject;
        if(clicked->parent()->objectName().contains("lSlider")){
            emit signalToLabels(clicked->objectName());
            if(clicked->objectName().contains("B1")){
                currentBank = 1;}
            if(clicked->objectName().contains("B2")){
                currentBank = 2;}
            if(clicked->objectName().contains("B3")){
                currentBank = 3;}
            if(clicked->objectName().contains("B4")){
                currentBank = 4;}
        }
    }
}

void LSliderEditPane::slotToLabels(QString parameter){

    for(int i = 0; i < 1; i++){  //display parameter on all 4 sliders.
        lSliderLabel[i]->slotDisplayParameter(parameter, i, currentPreset);}
    prevObjectToLabel = parameter;

}
