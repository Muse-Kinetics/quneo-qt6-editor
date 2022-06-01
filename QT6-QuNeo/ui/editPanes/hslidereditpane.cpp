#include "hslidereditpane.h"

HSliderEditPane::HSliderEditPane(QVariantMap *variantMap, QVariantMap *variantMapCopy, QWidget *widget, QWidget *parent) :
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

    for(int i = 0; i < 4; i++) {
        hSliderLabel[i] = new HSliderLabels(mainWindow, presetMapCopy, 0);
    }
#ifdef Q_OS_MAC

#else
    QLabel *sensitivityLabel = mainWindow->findChild<QLabel *>("hSliderSensitivityLabel");
    QFont f( "Arial", 3, QFont::Normal);
    sensitivityLabel->setFont(f);
#endif
}

void HSliderEditPane::findUiElements()
{
    QTabWidget *hSliderTabs = mainWindow->findChild<QTabWidget *>("hSliderTabs");
    hSliderTabs->setCurrentIndex(0);

    hSliderParamList = presetMapCopy->
            value("Preset 0").toMap().
            value("ComponentSettings").toMap().
            value(QString("HSliders")).toMap().
            value(QString("HSlider0")).toMap().keys();

    for (int i = 0; i<hSliderParamList.count(); i++) {

        if(mainWindow->findChild<QSpinBox *>(QString(hSliderParamList.at(i)))){

            if (!hSliderParamList.at(i).contains("localLEDControl", Qt::CaseSensitive) && !hSliderParamList.at(i).contains("hSliderInChannel", Qt::CaseSensitive)) {
                if (!hSliderParamList.at(i).contains("Sensitivity", Qt::CaseSensitive)) {

                    spinBoxMap.insert(hSliderParamList.at(i), mainWindow->findChild<QSpinBox *>(QString(hSliderParamList.at(i))));

                    //set up all spin boxes to have correct minimum / maximum and "off" text.
                    if(!hSliderParamList.at(i).contains("Channel", Qt::CaseSensitive) && !hSliderParamList.at(i).contains("PassThru")) {
                        spinBoxMap.value(QString(hSliderParamList.at(i)))->setMinimum(-1);
                        spinBoxMap.value(QString(hSliderParamList.at(i)))->setMaximum(127);
                        spinBoxMap.value(QString(hSliderParamList.at(i)))->setSpecialValueText("off");
                        if(hSliderParamList.at(i).contains("VelocityValue", Qt::CaseSensitive))
                        {
                            spinBoxMap.value(QString(hSliderParamList.at(i)))->setMinimum(0);
                            spinBoxMap.value(QString(hSliderParamList.at(i)))->setMaximum(127);
                            spinBoxMap.value(QString(hSliderParamList.at(i)))->setSpecialValueText("var");
                        }
                        if(hSliderParamList.at(i).contains("PressValue", Qt::CaseSensitive))
                        {
                            spinBoxMap.value(QString(hSliderParamList.at(i)))->setMinimum(0);
                            spinBoxMap.value(QString(hSliderParamList.at(i)))->setMaximum(127);
                            spinBoxMap.value(QString(hSliderParamList.at(i)))->setSpecialValueText("con");
                        }
                    }else if(hSliderParamList.at(i).contains("PassThru")){
                        spinBoxMap.value(QString(hSliderParamList.at(i)))->setMinimum(1);
                        spinBoxMap.value(QString(hSliderParamList.at(i)))->setMaximum(127);

                        //hide pass thru stuff
                        spinBoxMap.value(QString(hSliderParamList.at(i)))->hide();
                        mainWindow->findChild<QLabel *>("hB1outLocPassThruRangeLabel")->hide();
                        mainWindow->findChild<QLabel *>("hB2outLocPassThruRangeLabel")->hide();
                        mainWindow->findChild<QLabel *>("hB3outLocPassThruRangeLabel")->hide();
                        mainWindow->findChild<QLabel *>("hB4outLocPassThruRangeLabel")->hide();

                    } else if(hSliderParamList.at(i).contains("Channel", Qt::CaseSensitive)) { //no off state, 1-16
                        spinBoxMap.value(QString(hSliderParamList.at(i)))->setMinimum(1);
                        spinBoxMap.value(QString(hSliderParamList.at(i)))->setMaximum(16);
                    }
                }
                connect(spinBoxMap.value(QString(hSliderParamList.at(i))), SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int)));
            }

            uiObject = mainWindow->findChild<QSpinBox *>(QString(hSliderParamList.at(i))); //Why is this here?

        } else  if(mainWindow->findChild<QCheckBox *>(QString(hSliderParamList.at(i))))
        {

        }
        else  if(mainWindow->findChild<QComboBox *>(QString(hSliderParamList.at(i)))){

            if(hSliderParamList.at(i).contains("outNotePressMode"))
            {
                if(hSliderParamList.at(i).contains("hB1outNotePressMode"))
                {
                    modeMenuB1 = mainWindow->findChild<QComboBox *>(hSliderParamList.at(i));
                    connect(modeMenuB1, SIGNAL(currentIndexChanged(int)), this, SLOT(slotValueChanged(int)));
                }
                if(hSliderParamList.at(i).contains("hB2outNotePressMode"))
                {
                    modeMenuB2 = mainWindow->findChild<QComboBox *>(hSliderParamList.at(i));
                    connect(modeMenuB2, SIGNAL(currentIndexChanged(int)), this, SLOT(slotValueChanged(int)));
                }
                if(hSliderParamList.at(i).contains("hB3outNotePressMode"))
                {
                    modeMenuB3 = mainWindow->findChild<QComboBox *>(hSliderParamList.at(i));
                    connect(modeMenuB3, SIGNAL(currentIndexChanged(int)), this, SLOT(slotValueChanged(int)));
                }
                if(hSliderParamList.at(i).contains("hB4outNotePressMode"))
                {
                    modeMenuB4 = mainWindow->findChild<QComboBox *>(hSliderParamList.at(i));
                    connect(modeMenuB4, SIGNAL(currentIndexChanged(int)), this, SLOT(slotValueChanged(int)));
                }
            }
        }
        else {
            hSliderUiBlackList.append(hSliderParamList.at(i));
            qDebug() << "HSlider Ui ***" << hSliderParamList.at(i) << "Not Found";
        }
    }

    //install event filter in all the spinboxes so that
    //copy/paste commands can be intercepted while they are in focus
    for(int i=0; i<hSliderParamList.count();i++)
       {
           if(spinBoxMap.contains(hSliderParamList.at(i)))
           {
               spinBoxMap.value(QString(hSliderParamList.at(i)))->installEventFilter(this);
           }
       }

    //HSlider Globals

    if(mainWindow->findChild<QLabel *>("hSliderName")){
        paneLabel = mainWindow->findChild<QLabel *>("hSliderName");
    } else {
        qDebug() << "HSlider GLobal Ui ***" << "hSliderName" << "Not Found";
    }

    if(mainWindow->findChild<QDial *>("hSliderSensitivity")){
        sensitivityDial = mainWindow->findChild<QDial *>("hSliderSensitivity");
        sensitivityDial->setMinimum(70);
        sensitivityDial->setMaximum(150);
        sensitivityDial->setSingleStep(1);
        connect(sensitivityDial, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int)));
    } else {
        qDebug() << "HSlider GLobal Ui ***" << "hSliderSensitivity" << "Not Found";
    }


    if(mainWindow->findChild<QCheckBox *>("hSliderLocalLEDControl")){
        localLEDControl = mainWindow->findChild<QCheckBox *>("hSliderLocalLEDControl");
        connect(localLEDControl, SIGNAL(stateChanged(int)), this, SLOT(slotValueChanged(int)));
    } else {
        qDebug() << "HSlider GLobal Ui ***" << "hSliderLocalLEDControl" << "Not Found";
    }

    if(mainWindow->findChild<QSpinBox *>("hSliderInChannel")){
        ledInputChannel = mainWindow->findChild<QSpinBox *>("hSliderInChannel");
        connect(ledInputChannel, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int)));
    } else {
        qDebug() << "HSlider GLobal Ui ***" << "hSliderInChannel" << "Not Found";
    }

    if(mainWindow->findChild<QSpinBox *>("hSliderOnThreshold")){
        onThresh = mainWindow->findChild<QSpinBox *>("hSliderOnThreshold");
        onThresh->setMinimum(3);
        connect(onThresh, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int)));
        connect(onThresh, SIGNAL(valueChanged(int)), this, SLOT(slotCalculateOffThreshFromOn(int)));
    } else {
        qDebug() << "HSlider GLobal Ui ***" << "hSliderOnThreshold" << "Not Found";
    }

    if(mainWindow->findChild<QSpinBox *>("hSliderOffThreshold")){
        offThresh = mainWindow->findChild<QSpinBox *>("hSliderOffThreshold");
        offThresh->setMinimum(1);
        connect(offThresh, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int)));
        connect(offThresh, SIGNAL(valueChanged(int)), this, SLOT(slotCheckOffThreshAgainstOn(int)));
    } else {
        qDebug() << "HSlider GLobal Ui ***" << "hSliderOffThreshold" << "Not Found";
    }

    ledInputChannel->installEventFilter(this);
    onThresh->installEventFilter(this);
    offThresh->installEventFilter(this);
    sensitivityDial->installEventFilter(this);

}

void HSliderEditPane::displayHSliderValues()
{
    //disconnect param display labels during preset recall.
    disconnect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), this, SLOT(slotFocusParam(QWidget*,QWidget*))); //displays based on focus
    disconnect(this, SIGNAL(signalToLabels(QString)), this, SLOT(slotToLabels(QString))); //displays when value changed
    //get clicked slider map
    hSliderMap = presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value
            ("HSliders").toMap().value(QString("HSlider%1").arg(currentHSlider)).toMap();

    //set edit pane title
    paneLabel->setText(QString("Horizontal Slider %1").arg(currentHSlider+1));

    //clear focus before displaying values
    for(int i=0; i<hSliderParamList.count(); i++) {
        if(!hSliderUiBlackList.contains(hSliderParamList.at(i))){
            if(spinBoxMap.contains(hSliderParamList.at(i))){
                if (!hSliderParamList.at(i).contains("localLEDControl", Qt::CaseSensitive)) {
                    spinBoxMap.value(QString(hSliderParamList.at(i)))->clearFocus();
                }
            }
        }
    }

    //qDebug() << "keys" << hSliderMap.keys();
    //iterate through param names and set values in edit pane ui
    for(int i=0; i<hSliderParamList.count(); i++)
    {
        if(!hSliderUiBlackList.contains(hSliderParamList.at(i)))
        {
            if(mainWindow->findChild<QSpinBox *>(hSliderParamList.at(i)))
            {
                if (!hSliderParamList.at(i).contains("Sensitivity", Qt::CaseSensitive))
                {
                    if (!hSliderParamList.at(i).contains("localLEDControl", Qt::CaseSensitive) && !hSliderParamList.at(i).contains("InChannel", Qt::CaseSensitive))
                    {
                        if(hSliderParamList.at(i).contains("Channel", Qt::CaseSensitive))
                        {
                            //qDebug() << "offset" << hSliderParamList.at(i);
                            spinBoxMap.value(QString(hSliderParamList.at(i)))->setValue(hSliderMap.value(QString(hSliderParamList.at(i))).toInt() + 1);
                        }
                        else
                        {
                            spinBoxMap.value(QString(hSliderParamList.at(i)))->setValue(hSliderMap.value(QString(hSliderParamList.at(i))).toInt());
                        }
                        if(QString(hSliderParamList.at(i)).contains("in"))
                        {
                            spinBoxMap.value(QString(hSliderParamList.at(i)))->hide();
                            QLabel *ledParamToHide = mainWindow->findChild<QLabel *>(QString("%1Label").arg(hSliderParamList.at(i)));
                            ledParamToHide->hide();
                        }
                    }
                }
            }
            else if(mainWindow->findChild<QComboBox *>(hSliderParamList.at(i)))
            {
                if (hSliderParamList.at(i).contains("outNotePressMode"))
                {
                    if(hSliderParamList.at(i).contains("hB1outNotePressMode"))
                    {
                        modeMenuB1 = mainWindow->findChild<QComboBox *>(hSliderParamList.at(i));
                        modeMenuB1->setCurrentIndex(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("HSliders").toMap().value(QString("HSlider%1").arg(currentHSlider)).toMap().value(hSliderParamList.at(i)).toInt());
                    }
                    if(hSliderParamList.at(i).contains("hB2outNotePressMode"))
                    {
                        modeMenuB2 = mainWindow->findChild<QComboBox *>(hSliderParamList.at(i));
                        modeMenuB2->setCurrentIndex(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("HSliders").toMap().value(QString("HSlider%1").arg(currentHSlider)).toMap().value(hSliderParamList.at(i)).toInt());
                    }
                    if(hSliderParamList.at(i).contains("hB3outNotePressMode"))
                    {
                        modeMenuB3 = mainWindow->findChild<QComboBox *>(hSliderParamList.at(i));
                        modeMenuB3->setCurrentIndex(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("HSliders").toMap().value(QString("HSlider%1").arg(currentHSlider)).toMap().value(hSliderParamList.at(i)).toInt());
                    }
                    if(hSliderParamList.at(i).contains("hB4outNotePressMode"))
                    {
                        modeMenuB4 = mainWindow->findChild<QComboBox *>(hSliderParamList.at(i));
                        modeMenuB4->setCurrentIndex(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("HSliders").toMap().value(QString("HSlider%1").arg(currentHSlider)).toMap().value(hSliderParamList.at(i)).toInt());
                    }
                }
            }
        }
    }

    //set values for sensitivity dial in edit pane ui
    sensitivityDial->setValue(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value
                              ("ComponentSettings").toMap().value("HSliders").toMap().value("hSliderSensitivity").toInt());

    //display local LED checkbox
    localLEDControl->setChecked(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("HSliders").toMap().value("hSliderLocalLEDControl").toBool());

    ledInputChannel->setValue(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("HSliders").toMap().value("hSliderInChannel").toInt() +1);
    //display on/off thresholds
    if(mainWindow->findChild<QSpinBox *>("hSliderOnThreshold")){
        onThresh->setValue(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("HSliders").toMap().value("hSliderOnThreshold").toInt());
    }
    if(mainWindow->findChild<QSpinBox *>("hSliderOffThreshold")){
        offThresh->setValue(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("HSliders").toMap().value("hSliderOffThreshold").toInt());
    }

    //connect display interface after preset recall.
    connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), this, SLOT(slotFocusParam(QWidget*,QWidget*)));
    connect(this, SIGNAL(signalToLabels(QString)), this, SLOT(slotToLabels(QString)));
}


void HSliderEditPane::slotCalculateOffThreshFromOn(int onThreshChanged){
    Q_UNUSED(onThreshChanged);
    if(offThresh->value() > (onThresh->value() - 2)){
        offThresh->setValue(onThresh->value() - 2);
    }
    offThresh->setMaximum(onThresh->value() - 2);
}

void HSliderEditPane::slotCheckOffThreshAgainstOn(int offThreshChanged){
    if(offThreshChanged > (onThresh->value() - 2)){
        offThresh->setValue(onThresh->value() - 2);
    }
    offThresh->setMaximum(onThresh->value() - 2);
}

bool HSliderEditPane::eventFilter(QObject *object, QEvent *event){
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

void HSliderEditPane::slotEvents(QString string)
{
    if(string.contains("hSliderButton", Qt::CaseSensitive)) {
        currentHSlider = string.remove(0,13).toInt();
        displayHSliderValues();
    }
}

void HSliderEditPane::slotRecallPreset(int preset)
{
    currentPreset = preset;
    displayHSliderValues();
    if(!prevObjectToLabel.contains("None")) {
        emit signalToLabels(prevObjectToLabel);
    }
}

void HSliderEditPane::slotValueChanged(int num)
{
    focusCheck = qobject_cast<QWidget *>(QObject::sender());
    if(focusCheck->hasFocus() && !focusCheck->objectName().contains("hSliderInChannel")){

        if(focusCheck->objectName().contains("VelocityValue") || focusCheck->objectName().contains("PressValue")){
            if (num == 0){
                num = -1;
            }
        }
        emit signalValueChanged(currentPreset, "ComponentSettings", "HSlider", currentHSlider, focusCheck->objectName(),num);
        emit signalToLabels(focusCheck->objectName()); //whenever a value is changed, this should be emitted, but it shouldn't always have an effect.
    }
    else if(focusCheck->objectName().contains("Velocity")
            ||focusCheck->objectName().contains("Enable")
            ||focusCheck->objectName().contains("Local")
            ||focusCheck->objectName().contains("outNotePressMode")
            ||focusCheck->objectName().contains("hSliderInChannel"))
    {
        emit signalValueChanged(currentPreset, "ComponentSettings", "HSlider", currentHSlider, focusCheck->objectName(),num);
        emit signalToLabels(focusCheck->objectName());
    }
}

void HSliderEditPane::slotToLabels(QString parameter) {
    for(int i = 0; i < 4; i++) {  //display parameter on all 4 sliders.
        hSliderLabel[i]->slotDisplayParameter(parameter, i, currentPreset);
    }
    prevObjectToLabel = parameter;
}

void HSliderEditPane::slotFocusParam(QWidget *oldObject, QWidget *nowObject) {
    Q_UNUSED(oldObject);
    qDebug() << "HSliderEditPane called";
    if(nowObject != nullptr) {
        clicked = nowObject;
        if(clicked->parent()->objectName().contains("hSlider")) {
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
