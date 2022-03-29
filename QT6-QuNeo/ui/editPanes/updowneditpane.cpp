#include "updowneditpane.h"
#include <QDebug>

UpdownEditPane::UpdownEditPane(QVariantMap *variantMap, QVariantMap *variantMapCopy, QWidget *widget, QWidget *parent) :
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
    sensitivityDial->setMinimum(25);
    sensitivityDial->setMaximum(104);
    sensitivityDial->setSingleStep(1);

    for(int i = 0; i < 2; i++) {
        updownLabel[i] = new UpdownLabels(mainWindow, presetMapCopy, 0);
    }
#ifdef Q_OS_MAC

#else
    QLabel *sensitivityLabel = mainWindow->findChild<QLabel *>("updownSensitivityLabel");
    QFont f( "Arial", 3, QFont::Normal);
    sensitivityLabel->setFont(f);
    sensitivityDial->move(9, 378);
    sensitivityLabel->move(3, 405);
#endif
}

void UpdownEditPane::findUiElements()
{
    QTabWidget *updownTabs = mainWindow->findChild<QTabWidget *>("updownTabs");
    updownTabs->setCurrentIndex(0);
    updownTabs->setTabEnabled(1, false);
    //link up/down button edit pane label
    paneLabel = mainWindow->findChild<QLabel *>("updownButtonLabel");

    //get list of up/down button parameters (use Prest 0 and UpDownButton0 as a template) **** below is one line of code in two lines ****
    updownParamList = presetMapCopy->value("Preset 0").toMap().value("ComponentSettings").
            toMap().value("UpDownButtons").toMap().value(QString("UpDownButton0")).toMap().keys();

    //iterate through list of params and populate map of spin box addresses linked by param name
    for(int i=0; i<updownParamList.count();i++) {
        if(mainWindow->findChild<QSpinBox *>(QString(updownParamList.at(i)))){
            if(!updownParamList.at(i).contains("Sensitivity", Qt::CaseSensitive)) {
                if(!updownParamList.at(i).contains("Enable", Qt::CaseSensitive)) {
                    if(!updownParamList.at(i).contains("BankControl", Qt::CaseSensitive)) {
                        spinBoxMap.insert(QString(updownParamList.at(i)),mainWindow->findChild<QSpinBox *>(QString(updownParamList.at(i))));
                        //set up all spin boxes to have correct minimum / maximum and "off" text.
                        if(!updownParamList.at(i).contains("Channel", Qt::CaseSensitive)) {
                            spinBoxMap.value(QString(updownParamList.at(i)))->setMinimum(-1);
                            spinBoxMap.value(QString(updownParamList.at(i)))->setMaximum(127);
                            spinBoxMap.value(QString(updownParamList.at(i)))->setSpecialValueText("off");
                            if(updownParamList.at(i).contains("VelocityValue", Qt::CaseSensitive))
                            {
                                spinBoxMap.value(QString(updownParamList.at(i)))->setMinimum(0);
                                spinBoxMap.value(QString(updownParamList.at(i)))->setMaximum(127);
                                spinBoxMap.value(QString(updownParamList.at(i)))->setSpecialValueText("var");
                            }
                            if(updownParamList.at(i).contains("PressValue", Qt::CaseSensitive))
                            {
                                spinBoxMap.value(QString(updownParamList.at(i)))->setMinimum(0);
                                spinBoxMap.value(QString(updownParamList.at(i)))->setMaximum(127);
                                spinBoxMap.value(QString(updownParamList.at(i)))->setSpecialValueText("con");
                            }
                        }
                        else if(updownParamList.at(i).contains("Channel", Qt::CaseSensitive)) {
                            //no off state, 1-16
                            spinBoxMap.value(QString(updownParamList.at(i)))->setMinimum(1);
                            spinBoxMap.value(QString(updownParamList.at(i)))->setMaximum(16);
                        }
                        connect(spinBoxMap.value(QString(updownParamList.at(i))), SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int)));
                    }
                }
            }


        } else  if(mainWindow->findChild<QCheckBox *>(QString(updownParamList.at(i)))){

            if(mainWindow->findChild<QCheckBox *>("updownEnableSwitch")){
                enableBank = mainWindow->findChild<QCheckBox *>("updownEnableSwitch");
                connect(enableBank, SIGNAL(stateChanged(int)), this, SLOT(slotEnable(int)));
                connect(enableBank, SIGNAL(stateChanged(int)), this, SLOT(slotValueChanged(int)));
            }
            if(mainWindow->findChild<QCheckBox *>("updownUOutVelocity")){
                updownUVelocity = mainWindow->findChild<QCheckBox *>("updownUOutVelocity");
                connect(updownUVelocity, SIGNAL(stateChanged(int)), this, SLOT(slotValueChanged(int)));

            }
            if(mainWindow->findChild<QCheckBox *>("updownDOutVelocity")){
                updownDVelocity = mainWindow->findChild<QCheckBox *>("updownDOutVelocity");
                connect(updownDVelocity, SIGNAL(stateChanged(int)), this, SLOT(slotValueChanged(int)));
            }

        } else if(mainWindow->findChild<QComboBox *>(QString(updownParamList.at(i))))
        {

            if(mainWindow->findChild<QComboBox *>("updownBankControl"))
            {
                bankControl = mainWindow->findChild<QComboBox *>("updownBankControl");
                connect(bankControl, SIGNAL(currentIndexChanged(int)), this, SLOT(slotBankControl(int)));
                connect(bankControl, SIGNAL(currentIndexChanged(int)), this, SLOT(slotValueChanged(int)));
            }
            if(updownParamList.at(i).contains("NotePressMode")){

                if(updownParamList.at(i).contains("updownUOutNotePressMode"))
                {
                    modeMenuU = mainWindow->findChild<QComboBox *>(updownParamList.at(i));
                    connect(modeMenuU, SIGNAL(currentIndexChanged(int)), this, SLOT(slotValueChanged(int)));
                }

                if(updownParamList.at(i).contains("updownDOutNotePressMode"))
                {
                    modeMenuD = mainWindow->findChild<QComboBox *>(updownParamList.at(i));
                    connect(modeMenuD, SIGNAL(currentIndexChanged(int)), this, SLOT(slotValueChanged(int)));
                }


            }
        } else {
            updownUiBlacklist.append(updownParamList.at(i));
            qDebug() << "UpDown Button Ui ***" << updownParamList.at(i) << "Not Found";
        }
    }

    //install event filter in all the spinboxes so that
    //copy/paste commands can be intercepted while they are in focus
    for(int i=0; i<updownParamList.count();i++)
       {
           if(spinBoxMap.contains(updownParamList.at(i)))
           {
               spinBoxMap.value(QString(updownParamList.at(i)))->installEventFilter(this);
           }
       }

    if(mainWindow->findChild<QLabel *>("updownButtonLabel")){
        bankDestination = mainWindow->findChild<QLabel *>("updownButtonLabel");
    } else {
        updownUiBlacklist.append("updownButtonLabel");
        qDebug() << "UpDown Button Ui ***" << "updownButtonLabel" << "Not Found";
    }

    if(mainWindow->findChild<QDial *>("updownSensitivity")){
        sensitivityDial = mainWindow->findChild<QDial *>("updownSensitivity");
        connect(sensitivityDial, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int)));
    } else {
        updownUiBlacklist.append("updownSensitivity");
        qDebug() << "UpDown Button Ui ***" << "updownSensitivity" << "Not Found";
    }

    if(mainWindow->findChild<QCheckBox *>("updownLocalLEDControl")){
        localLEDControl = mainWindow->findChild<QCheckBox *>("updownLocalLEDControl");
        connect(localLEDControl, SIGNAL(stateChanged(int)), this, SLOT(slotValueChanged(int)));
    } else {
        updownUiBlacklist.append("updownLocalLEDControl");
        qDebug() << "UpDown Button Ui ***" << "updownLocalLEDControl" << "Not Found";
    }

    if(mainWindow->findChild<QSpinBox *>("updownInChannel")){
        ledInputChannel = mainWindow->findChild<QSpinBox *>("updownInChannel");
        connect(ledInputChannel, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int)));
    } else {
        updownUiBlacklist.append("updownInChannel");
        qDebug() << "UpDown Button Ui ***" << "updownInChannel" << "Not Found";
    }

    ledInputChannel->installEventFilter(this);
    sensitivityDial->installEventFilter(this);
}

void UpdownEditPane::displayUpdownValues() {

    //disconnect param display labels during preset recall.
    disconnect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), this, SLOT(slotFocusParam(QWidget*,QWidget*))); //displays based on focus
    disconnect(this, SIGNAL(signalToLabels(QString)), this, SLOT(slotToLabels(QString))); //displays when value changed

    //get clicked up/down map
    updownMap = presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").
            toMap().value("UpDownButtons").toMap().value(QString("UpDownButton%1").arg(currentUpdown)).toMap();

    //set transport editpane title
    paneLabel->setText(QString("for Vertical Sliders"));

    //clear focus before displaying values
    for(int i=0; i<updownParamList.count();i++) {
        if(!updownUiBlacklist.contains(updownParamList.at(i))){
            if(!updownParamList.at(i).contains("LocalLEDControl", Qt::CaseSensitive)) {
                if(!updownParamList.at(i).contains("Enable", Qt::CaseSensitive)) {
                    if(!updownParamList.at(i).contains("BankControl", Qt::CaseSensitive)) {

                        if(spinBoxMap.contains(updownParamList.at(i))){
                            spinBoxMap.value(QString(updownParamList.at(i)))->clearFocus();
                        }
                    }
                }
            }
        }
    }

    //iterate through param names and set values in edit pane ui
    for(int i=0; i<updownParamList.count();i++) {
        if(!updownUiBlacklist.contains(updownParamList.at(i))){

            if(spinBoxMap.contains(updownParamList.at(i))){

                if(!updownParamList.at(i).contains("LocalLEDControl", Qt::CaseSensitive)) {
                    if(!updownParamList.at(i).contains("Sensitivity", Qt::CaseSensitive)) {
                        if(!updownParamList.at(i).contains("Enable", Qt::CaseSensitive)) {
                            if(!updownParamList.at(i).contains("BankControl", Qt::CaseSensitive)) {
                                if(updownParamList.at(i).contains("Channel", Qt::CaseSensitive)){
                                    spinBoxMap.value(QString(updownParamList.at(i)))->setValue(updownMap.value(QString(updownParamList.at(i))).toInt() + 1);}
                                else {spinBoxMap.value(QString(updownParamList.at(i)))->setValue(updownMap.value(QString(updownParamList.at(i))).toInt());}
                                if(QString(updownParamList.at(i)).contains("In")){
                                    spinBoxMap.value(QString(updownParamList.at(i)))->hide();
                                    QLabel *ledParamToHide = mainWindow->findChild<QLabel *>(QString("%1Label").arg(updownParamList.at(i)));
                                    ledParamToHide->hide();
                                    ledParamToHide = mainWindow->findChild<QLabel *>(QString("updownUInLabel"));
                                    ledParamToHide->hide();
                                    ledParamToHide = mainWindow->findChild<QLabel *>(QString("updownDInLabel"));
                                    ledParamToHide->hide();
                                }
                            }
                        }
                    }
                }
            }
            else if (updownParamList.at(i).contains("NotePressMode")){

                if(updownParamList.at(i).contains("updownUOutNotePressMode"))
                {
                    modeMenuU->setCurrentIndex(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("UpDownButtons").toMap().value(QString("UpDownButton%1").arg(currentUpdown)).toMap().value(updownParamList.at(i)).toInt());
                }

                if(updownParamList.at(i).contains("updownDOutNotePressMode"))
                {
                    modeMenuD->setCurrentIndex(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("UpDownButtons").toMap().value(QString("UpDownButton%1").arg(currentUpdown)).toMap().value(updownParamList.at(i)).toInt());
                }

            }

            else if (updownParamList.at(i).contains("Enable", Qt::CaseSensitive)) {
                //if it is an enable switch, set the ui object and call slotEnable with the stored bool value.
                enableBank->setChecked(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("UpDownButtons").toMap().value(QString("UpDownButton%1").arg(currentUpdown)).toMap().value("updownEnableSwitch").toBool());
                slotBankControl(bankControl->currentIndex());
                slotEnable(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("UpDownButtons").toMap().value(QString("UpDownButton%1").arg(currentUpdown)).toMap().value("updownEnableSwitch").toBool());
            }
            else if (updownParamList.at(i).contains("BankControl")){
                bankControl->setCurrentIndex(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("UpDownButtons").toMap().value(QString("UpDownButton%1").arg(currentUpdown)).toMap().value("updownBankControl").toInt());
                slotBankControl(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("UpDownButtons").toMap().value("UpDownButton0").toMap().value("updownBankControl").toInt());
            }
        }
    }

    sensitivityDial->setValue(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("UpDownButtons").toMap().value("updownSensitivity").toInt());
    localLEDControl->setChecked(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("UpDownButtons").toMap().value("updownLocalLEDControl").toBool());
    ledInputChannel->setValue(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("UpDownButtons").toMap().value("updownInChannel").toInt() + 1);

    //connect display interface after preset recall
    connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), this, SLOT(slotFocusParam(QWidget*,QWidget*)));
    connect(this, SIGNAL(signalToLabels(QString)), this, SLOT(slotToLabels(QString)));
}

bool UpdownEditPane::eventFilter(QObject *object, QEvent *event){
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

void UpdownEditPane::slotEvents(QString string) {
    //format string from event into a single instance (up/down number)
    if((string == QString("buttonButton9")) || (string == QString("buttonButton10"))) {
        //the line above makes sure the up/down edit pane is only looking at the correct buttonButtons
        currentUpdown = string.remove(0,12).toInt()-9;//remove "buttonButton" and convert last character to int
        displayUpdownValues();
    }
}

void UpdownEditPane::slotRecallPreset(int preset) {
    currentPreset = preset;
    displayUpdownValues();
    if(!prevObjectToLabel.contains("None")) {
        emit signalToLabels(prevObjectToLabel);
    }
}

void UpdownEditPane::slotEnable(int enable) {
    if(enable) {
        for(int i=0; i<updownParamList.count(); i++) {

            if(!updownUiBlacklist.contains(updownParamList.at(i))){

                if(spinBoxMap.contains(updownParamList.at(i))){

                    if(!updownParamList.at(i).contains("Enable") && !updownParamList.at(i).contains("Control")) {
                        spinBoxMap.value(QString(updownParamList.at(i)))->setEnabled(false);
                    }
                }
                else if(updownParamList.at(i).contains("Mode")){
                    //modeMenus = mainWindow->findChild<QComboBox *>(updownParamList.at(i));
                    modeMenuU->setEnabled(false);
                    modeMenuD->setEnabled(false);
                }
                bankControl->setEnabled(true);
            }
        }

        localLEDControl->setChecked(true);
        localLEDControl->setEnabled(false);
        slotBankControl(bankControl->currentIndex());

    } else {
        for (int i=0; i<updownParamList.count(); i++) {
            if(!updownUiBlacklist.contains(updownParamList.at(i))){

                if(spinBoxMap.contains(updownParamList.at(i))){

                    if(!updownParamList.at(i).contains("Enable") && !updownParamList.at(i).contains("Control")) {
                        spinBoxMap.value(QString(updownParamList.at(i)))->setEnabled(true);
                    }
                }

                else if(updownParamList.at(i).contains("Mode")){
                    // modeMenus = mainWindow->findChild<QComboBox *>(updownParamList.at(i));
                    modeMenuU->setEnabled(true);
                    modeMenuD->setEnabled(true);
                }
                bankControl->setEnabled(false);
            }
        }
        localLEDControl->setEnabled(true);
        bankDestination->setText("Bank Control Off");
    }
}

void UpdownEditPane::slotBankControl(int menuChoice) {
    switch(menuChoice) {
    case 0: {
        bankDestination->setText("For Rotaries");
        break;
    }
    case 1: {
        bankDestination->setText("For Vertical Sliders");
        break;
    }
    case 2: {
        bankDestination->setText("For Long Slider");
        break;
    }
    case 3: {
        bankDestination->setText("For Pads");
        break;
    }
    }
}

void UpdownEditPane::slotValueChanged(int num) {
    focusCheck = qobject_cast<QWidget *>(QObject::sender());
    if(focusCheck->hasFocus()){
        if(focusCheck->objectName().contains("VelocityValue") || focusCheck->objectName().contains("PressValue")){
            if (num == 0){
                num = -1;
            }
        }
        emit signalValueChanged(currentPreset, "ComponentSettings", "UpDownButton", currentUpdown,focusCheck->objectName(),num);
        emit signalToLabels(focusCheck->objectName()); //whenever a value is changed, this should be emitted, but it shouldn't always have an effect
    }
    else if(focusCheck->objectName().contains("Velocity")
            ||focusCheck->objectName().contains("BankControl")
            ||focusCheck->objectName().contains("Enable")
            ||focusCheck->objectName().contains("Local")
            || focusCheck->objectName().contains("NotePressMode")){
        emit signalValueChanged(currentPreset, "ComponentSettings", "UpDownButton", currentUpdown, focusCheck->objectName(),num);
        emit signalToLabels(focusCheck->objectName()); //whenever a value is changed, this should be emitted, but it shouldn't always have an effect
    }
}

void UpdownEditPane::slotToLabels(QString parameter) {
    for(int i = 0; i < 2; i++) {
        updownLabel[i]->slotDisplayParameter(parameter, i, currentPreset);
    }
    prevObjectToLabel = parameter;
}

void UpdownEditPane::slotFocusParam(QWidget *oldObject, QWidget *nowObject) {
    Q_UNUSED(oldObject);
    qDebug() << "UpdownEditPane called";
    if(nowObject != nullptr) {
        clicked = nowObject;
        if(clicked->parent()->objectName().contains("updown")) {
            emit signalToLabels(clicked->objectName());
        }
    }
}
