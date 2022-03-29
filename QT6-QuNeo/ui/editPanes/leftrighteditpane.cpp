#include "leftrighteditpane.h"
#include <QDebug>

LeftrightEditPane::LeftrightEditPane(QVariantMap *variantMap, QVariantMap *variantMapCopy, QWidget *widget, QWidget *parent) :
    QWidget(parent)
{
    this->setStyleSheet("QLabel { font: 10px } QLineEdit { font: 12px } QCheckBox {font:9px} QSpinBox {font: 10px} QDoubleSpinBox {font: 10px}");

    mainWindow = widget;
    presetMap = variantMap;
    presetMapCopy = variantMapCopy;
    prevObjectToLabel = QString("None");
    currentPreset = 0;
    findUiElements();
    connect(mainWindow, SIGNAL(signalEvents(QString)), this, SLOT(slotEvents(QString)));

    for(int i = 0; i < 4; i++) {
        leftrightLabel[i] = new LeftrightLabels(mainWindow, presetMapCopy, 0);
    }
#ifdef Q_OS_MAC

#else
    QLabel *sensitivityLabel = mainWindow->findChild<QLabel *>("leftrightSensitivityLabel");
    QFont f( "Arial", 3, QFont::Normal);
    sensitivityLabel->setFont(f);
    sensitivityDial->move(9, 378);
    sensitivityLabel->move(3, 405);
#endif
}

void LeftrightEditPane::findUiElements()
{
    QTabWidget *leftrightTabs = mainWindow->findChild<QTabWidget *>("leftrightTabs");
    leftrightTabs->setCurrentIndex(0);
    leftrightTabs->setTabEnabled(1, false);
    //link left/right button edit pane label
    paneLabel = mainWindow->findChild<QLabel *>("leftrightButtonLabel");

    //get list of left/right button parameters (use Prest 0 and LeftRightButton0 as a template) **** below is one line of code in two lines ****
    leftrightParamList = presetMapCopy->value("Preset 0").toMap().value("ComponentSettings").
            toMap().value("LeftRightButtons").toMap().value(QString("LeftRightButton0")).toMap().keys();

    //iterate through list of params and populate map of spin box addresses linked by param name
    for(int i=0; i<leftrightParamList.count();i++) {

        if(mainWindow->findChild<QSpinBox *>(QString(leftrightParamList.at(i)))){

            if(!leftrightParamList.at(i).contains("Sensitivity", Qt::CaseSensitive)) {
                if(!leftrightParamList.at(i).contains("Enable", Qt::CaseSensitive)) {
                    spinBoxMap.insert(QString(leftrightParamList.at(i)),mainWindow->findChild<QSpinBox *>(QString(leftrightParamList.at(i))));
                    //set up all spin boxes to have correct minimum / maximum and "off" text.
                    if(!leftrightParamList.at(i).contains("Channel", Qt::CaseSensitive)) {
                        spinBoxMap.value(QString(leftrightParamList.at(i)))->setMinimum(-1);
                        spinBoxMap.value(QString(leftrightParamList.at(i)))->setMaximum(127);
                        spinBoxMap.value(QString(leftrightParamList.at(i)))->setSpecialValueText("off");
                        if(leftrightParamList.at(i).contains("VelocityValue", Qt::CaseSensitive))
                        {
                            spinBoxMap.value(QString(leftrightParamList.at(i)))->setMinimum(0);
                            spinBoxMap.value(QString(leftrightParamList.at(i)))->setMaximum(127);
                            spinBoxMap.value(QString(leftrightParamList.at(i)))->setSpecialValueText("var");
                        }
                        if(leftrightParamList.at(i).contains("PressValue", Qt::CaseSensitive))
                        {
                            spinBoxMap.value(QString(leftrightParamList.at(i)))->setMinimum(0);
                            spinBoxMap.value(QString(leftrightParamList.at(i)))->setMaximum(127);
                            spinBoxMap.value(QString(leftrightParamList.at(i)))->setSpecialValueText("con");
                        }
                    }
                    else if(leftrightParamList.at(i).contains("Channel", Qt::CaseSensitive)) {
                        //no off state, 1-16
                        spinBoxMap.value(QString(leftrightParamList.at(i)))->setMinimum(1);
                        spinBoxMap.value(QString(leftrightParamList.at(i)))->setMaximum(16);
                    }
                    connect(spinBoxMap.value(QString(leftrightParamList.at(i))), SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int)));
                }
            }

            //find checkboxes
        } else  if(mainWindow->findChild<QCheckBox *>(QString(leftrightParamList.at(i)))){

            if(mainWindow->findChild<QCheckBox *>("leftrightEnableSwitch")){
                enableBank = mainWindow->findChild<QCheckBox *>("leftrightEnableSwitch");
                connect(enableBank, SIGNAL(stateChanged(int)), this, SLOT(slotEnable(int)));
                connect(enableBank, SIGNAL(stateChanged(int)), this, SLOT(slotValueChanged(int)));
            }
        }

        //
        else if(mainWindow->findChild<QComboBox *>(QString(leftrightParamList.at(i)))){

            if(leftrightParamList.at(i).contains("leftrightLOutNotePressMode"))
            {
                modeMenuL = mainWindow->findChild<QComboBox *>(leftrightParamList.at(i));
                connect(modeMenuL, SIGNAL(currentIndexChanged(int)), this, SLOT(slotValueChanged(int)));
            }

            if(leftrightParamList.at(i).contains("leftrightROutNotePressMode"))
            {
                modeMenuR = mainWindow->findChild<QComboBox *>(leftrightParamList.at(i));
                connect(modeMenuR, SIGNAL(currentIndexChanged(int)), this, SLOT(slotValueChanged(int)));
            }
        }
        else {
            leftrightUiBlacklist.append(leftrightParamList.at(i));
            qDebug() << "LeftRight Button Ui ***" << leftrightParamList.at(i) << "Not Found";
        }

    }

    //install event filter in all the spinboxes so that
    //copy/paste commands can be intercepted while they are in focus
    for(int i=0; i<leftrightParamList.count();i++)
       {
           if(spinBoxMap.contains(leftrightParamList.at(i)))
           {
               spinBoxMap.value(QString(leftrightParamList.at(i)))->installEventFilter(this);
           }
       }

    //LeftRight Button Globals

    if(mainWindow->findChild<QDial *>("leftrightSensitivity")){
        sensitivityDial = mainWindow->findChild<QDial *>("leftrightSensitivity");
        connect(sensitivityDial, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int)));
        sensitivityDial->setMinimum(25);
        sensitivityDial->setMaximum(104);
        sensitivityDial->setSingleStep(1);
    } else {
        qDebug() << "LeftRight Button Ui ***" << "leftrightSensitivity" << "Not Found";
    }

    if(mainWindow->findChild<QCheckBox *>("leftrightLocalLEDControl")){
        localLEDControl = mainWindow->findChild<QCheckBox *>("leftrightLocalLEDControl");
        connect(localLEDControl, SIGNAL(stateChanged(int)), this, SLOT(slotValueChanged(int)));
    } else {
        qDebug() << "LeftRight Button Ui ***" << "leftrightLocalLEDControl" << "Not Found";
    }

    if(mainWindow->findChild<QSpinBox *>("leftrightInChannel")){
        ledInputChannel = mainWindow->findChild<QSpinBox *>("leftrightInChannel");
        connect(ledInputChannel, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int)));
    } else {
        qDebug() << "LeftRight Button Ui ***" << "leftrightInChannel" << "Not Found";
    }

    ledInputChannel->installEventFilter(this);
    sensitivityDial->installEventFilter(this);

}

void LeftrightEditPane::displayLeftrightValues() {

    //disconnect param display labels during preset recall.
    disconnect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), this, SLOT(slotFocusParam(QWidget*,QWidget*))); //displays based on focus
    disconnect(this, SIGNAL(signalToLabels(QString)), this, SLOT(slotToLabels(QString))); //displays when value changed

    //get clicked left/right map
    leftrightMap = presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").
            toMap().value("LeftRightButtons").toMap().value(QString("LeftRightButton%1").arg(currentLeftright)).toMap();

    //set transport editpane title
    paneLabel->setText(QString("For Horizontal Slider %1").arg(currentLeftright+1));

    //clear focus before displaying values
    for(int i=0; i<leftrightParamList.count();i++) {

        if(!leftrightUiBlacklist.contains(leftrightParamList.at(i))){
            if(spinBoxMap.contains(leftrightParamList.at(i))){
                spinBoxMap.value(QString(leftrightParamList.at(i)))->clearFocus();
            }
        }
    }

    //iterate through param names and set values in edit pane ui
    for(int i=0; i<leftrightParamList.count();i++) {
        if(!leftrightUiBlacklist.contains(leftrightParamList.at(i))){
            if(spinBoxMap.contains(leftrightParamList.at(i))){

                if(leftrightParamList.at(i).contains("Channel", Qt::CaseSensitive)){
                    spinBoxMap.value(QString(leftrightParamList.at(i)))->setValue(leftrightMap.value(QString(leftrightParamList.at(i))).toInt() + 1);}
                else {spinBoxMap.value(QString(leftrightParamList.at(i)))->setValue(leftrightMap.value(QString(leftrightParamList.at(i))).toInt());}
                if(QString(leftrightParamList.at(i)).contains("In")){
                    spinBoxMap.value(QString(leftrightParamList.at(i)))->hide();
                    QLabel *ledParamToHide = mainWindow->findChild<QLabel *>(QString("%1Label").arg(leftrightParamList.at(i)));
                    ledParamToHide->hide();
                    ledParamToHide = mainWindow->findChild<QLabel *>(QString("leftrightLInLabel"));
                    ledParamToHide->hide();
                    ledParamToHide = mainWindow->findChild<QLabel *>(QString("leftrightRInLabel"));
                    ledParamToHide->hide();
                }


            }
            if(mainWindow->findChild<QComboBox *>(leftrightParamList.at(i)))
            {
                if(leftrightParamList.at(i).contains("leftrightLOutNotePressMode"))
                {
                    modeMenuL = mainWindow->findChild<QComboBox *>(QString(leftrightParamList.at(i)));
                    modeMenuL->setCurrentIndex(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("LeftRightButtons").toMap().value(QString("LeftRightButton%1").arg(currentLeftright)).toMap().value(leftrightParamList.at(i)).toInt());
                }

                if(leftrightParamList.at(i).contains("leftrightROutNotePressMode"))
                {
                    modeMenuR = mainWindow->findChild<QComboBox *>(QString(leftrightParamList.at(i)));
                    modeMenuR->setCurrentIndex(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("LeftRightButtons").toMap().value(QString("LeftRightButton%1").arg(currentLeftright)).toMap().value(leftrightParamList.at(i)).toInt());
                }



            }
        }
    }
    enableBank->setChecked(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("LeftRightButtons").toMap().value(QString("LeftRightButton%1").arg(currentLeftright)).toMap().value("leftrightEnableSwitch").toBool());
    slotEnable(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("LeftRightButtons").toMap().value(QString("LeftRightButton%1").arg(currentLeftright)).toMap().value("leftrightEnableSwitch").toBool());

    //set values for global params in edit pane ui
    sensitivityDial->setValue(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("LeftRightButtons").toMap().value("leftrightSensitivity").toInt());
    localLEDControl->setChecked(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("LeftRightButtons").toMap().value("leftrightLocalLEDControl").toBool());
    ledInputChannel->setValue(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("LeftRightButtons").toMap().value("leftrightInChannel").toInt() + 1);


    //leftrightInChannel
    //connect display interface after preset recall
    connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), this, SLOT(slotFocusParam(QWidget*,QWidget*)));
    connect(this, SIGNAL(signalToLabels(QString)), this, SLOT(slotToLabels(QString)));
}

bool LeftrightEditPane::eventFilter(QObject *object, QEvent *event){
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

void LeftrightEditPane::slotEvents(QString string) {
    //qDebug()<<string;
    //format string from event into a single instance (transport number)
    if((string == QString("buttonButton4")) || (string == QString("buttonButton5")) || (string == QString("buttonButton6")) || (string == QString("buttonButton7"))) {
        //the line above makes sure the left/right edit pane is only looking at the correct buttonButtons
        currentLeftright = string.remove(0,12).toInt()-4;//remove "buttonButton" and convert last character to int
        //qDebug() << QString("Left Right Edit Pane: button %1").arg(currentLeftright);
        displayLeftrightValues();
    }
}

void LeftrightEditPane::slotRecallPreset(int preset) {
    currentPreset = preset;
    displayLeftrightValues();
    if(!prevObjectToLabel.contains("None")) {
        emit signalToLabels(prevObjectToLabel);
    }
}

void LeftrightEditPane::slotEnable(int enable) {
    if(enable){
        for (int i=0; i<leftrightParamList.count(); i++) {
            if(!leftrightUiBlacklist.contains(leftrightParamList.at(i))){
                if(spinBoxMap.contains(leftrightParamList.at(i))){
                    spinBoxMap.value(QString(leftrightParamList.at(i)))->setEnabled(false);
                }
                else if(mainWindow->findChild<QComboBox *>(leftrightParamList.at(i))){


                    if(leftrightParamList.at(i).contains("leftrightLOutNotePressMode"))
                    {
                        modeMenuL = mainWindow->findChild<QComboBox *>(leftrightParamList.at(i));
                        modeMenuL->setEnabled(false);
                    }

                    if(leftrightParamList.at(i).contains("leftrightROutNotePressMode"))
                    {
                        modeMenuR = mainWindow->findChild<QComboBox *>(leftrightParamList.at(i));
                        modeMenuR->setEnabled(false);
                    }
                }
            }
        }

        localLEDControl->setChecked(true);
        localLEDControl->setEnabled(false);
        paneLabel->setText(QString("For Horizontal Slider %1").arg(currentLeftright+1));

    } else {
        for (int i=0; i<leftrightParamList.count(); i++) {

            if(!leftrightUiBlacklist.contains(leftrightParamList.at(i))){
                if(spinBoxMap.contains(leftrightParamList.at(i))){
                    spinBoxMap.value(QString(leftrightParamList.at(i)))->setEnabled(true);
                }
                else if(mainWindow->findChild<QComboBox *>(leftrightParamList.at(i))){
                    if(leftrightParamList.at(i).contains("leftrightLOutNotePressMode"))
                    {
                        modeMenuL = mainWindow->findChild<QComboBox *>(leftrightParamList.at(i));
                        modeMenuL->setEnabled(true);
                    }

                    if(leftrightParamList.at(i).contains("leftrightROutNotePressMode"))
                    {
                        modeMenuR = mainWindow->findChild<QComboBox *>(leftrightParamList.at(i));
                        modeMenuR->setEnabled(true);
                    }
                }
            }
        }
        localLEDControl->setEnabled(true);
        paneLabel->setText(QString("Bank Control Off"));

    }
}


void LeftrightEditPane::slotValueChanged(int num) {
    focusCheck = qobject_cast<QWidget *>(QObject::sender());



    if(focusCheck->hasFocus()){
        //qDebug() << QString("From Left/Right Edit pane: %1 %2").arg(focusCheck->objectName()).arg(num);
        if(focusCheck->objectName().contains("VelocityValue") || focusCheck->objectName().contains("PressValue")){
            if (num == 0){
                num = -1;
            }
        }
        emit signalValueChanged(currentPreset, "ComponentSettings", "LeftRightButton", currentLeftright, focusCheck->objectName(),num);
        emit signalToLabels(focusCheck->objectName()); //whenever a value is changed, this should be emitted, but it shouldn't always have an effect

    }
    else if(focusCheck->objectName().contains("Velocity")
            || focusCheck->objectName().contains("Enable")
            || focusCheck->objectName().contains("Local")
            || focusCheck->objectName().contains("BankControl")
            || focusCheck->objectName().contains("NotePressMode")
            || focusCheck->objectName().contains("leftrightInChannel"))
    {
        emit signalValueChanged(currentPreset, "ComponentSettings", "LeftRightButton", currentLeftright, focusCheck->objectName(),num);
        emit signalToLabels(focusCheck->objectName()); //whenever a value is changed, this should be emitted, but it shoulde alwasy have an effect
    }
}

void LeftrightEditPane::slotToLabels(QString parameter) {
    for(int i = 0; i < 4; i++) {
        leftrightLabel[i]->slotDisplayParameter(parameter, i, currentPreset);
    }
    prevObjectToLabel = parameter;
}

void LeftrightEditPane::slotFocusParam(QWidget *oldObject, QWidget *nowObject) {
    Q_UNUSED(oldObject);
    qDebug() << "LeftrightEditPane called";
    if(nowObject != nullptr) {
        clicked = nowObject;
        if(clicked->parent()->objectName().contains("leftright")) {
            emit signalToLabels(clicked->objectName());
        }
    }
}
