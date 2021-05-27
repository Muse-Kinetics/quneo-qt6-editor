#include "rhombuseditpane.h"
#include <QDebug>

RhombusEditPane::RhombusEditPane(QVariantMap *variantMap, QVariantMap *variantMapCopy, QWidget *widget, QWidget *parent) :
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


    for(int i = 0; i < 1; i++) {
        rhombusLabel[i] = new RhombusLabels(mainWindow, presetMapCopy, 0);
    }
#ifdef Q_OS_MAC

#else
    QLabel *sensitivityLabel = mainWindow->findChild<QLabel *>("rhombusSensitivityLabel");
    QFont f( "Arial", 3, QFont::Normal);
    sensitivityLabel->setFont(f);
    sensitivityDial->move(9, 378);
    sensitivityLabel->move(3, 405);
#endif
}

void RhombusEditPane::findUiElements()
{
    QTabWidget *rhombusTabs = mainWindow->findChild<QTabWidget *>("rhombusTabs");
    rhombusTabs->setCurrentIndex(0);
    rhombusTabs->setTabEnabled(1, false);
    //link rhombus button edit pane label
    paneLabel = mainWindow->findChild<QLabel *>("rhombusButtonLabel");

    //get list of rhombus button parameters (use Prest 0 and RhombusButton as a template) **** below is one line of code in two lines ****
    rhombusParamList = presetMapCopy->value("Preset 0").toMap().value("ComponentSettings").
            toMap().value(QString("RhombusButtons")).toMap().value(QString("RhombusButton0")).toMap().keys();

    //iterate through list of params and populate map of spin box addresses linked by param name
    for(int i=0; i<rhombusParamList.count();i++) {

        if(mainWindow->findChild<QSpinBox *>(QString(rhombusParamList.at(i))))
        {

                if(!rhombusParamList.at(i).contains("Sensitivity", Qt::CaseSensitive)) {
                    if(!rhombusParamList.at(i).contains("Enable", Qt::CaseSensitive)) {
                        if(!rhombusParamList.at(i).contains("BankControl", Qt::CaseSensitive)) {
                            spinBoxMap.insert(QString(rhombusParamList.at(i)),mainWindow->findChild<QSpinBox *>(QString(rhombusParamList.at(i))));
                            //set up all spin boxes to have correct minimum / maximum and "off" text.
                            if(!rhombusParamList.at(i).contains("Channel", Qt::CaseSensitive)) {
                                spinBoxMap.value(QString(rhombusParamList.at(i)))->setMinimum(-1);
                                spinBoxMap.value(QString(rhombusParamList.at(i)))->setMaximum(127);
                                spinBoxMap.value(QString(rhombusParamList.at(i)))->setSpecialValueText("off");
                                if(rhombusParamList.at(i).contains("VelocityValue", Qt::CaseSensitive))
                                {
                                    spinBoxMap.value(QString(rhombusParamList.at(i)))->setMinimum(0);
                                    spinBoxMap.value(QString(rhombusParamList.at(i)))->setMaximum(127);
                                    spinBoxMap.value(QString(rhombusParamList.at(i)))->setSpecialValueText("var");
                                }
                                if(rhombusParamList.at(i).contains("PressValue", Qt::CaseSensitive))
                                {
                                    spinBoxMap.value(QString(rhombusParamList.at(i)))->setMinimum(0);
                                    spinBoxMap.value(QString(rhombusParamList.at(i)))->setMaximum(127);
                                    spinBoxMap.value(QString(rhombusParamList.at(i)))->setSpecialValueText("con");
                                }
                            }
                            else if(rhombusParamList.at(i).contains("Channel", Qt::CaseSensitive)) {
                                //no off state, 1-16
                                spinBoxMap.value(QString(rhombusParamList.at(i)))->setMinimum(1);
                                spinBoxMap.value(QString(rhombusParamList.at(i)))->setMaximum(16);
                            }
                            connect(spinBoxMap.value(QString(rhombusParamList.at(i))), SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int)));
                        }
                    }
                }




        } else  if(mainWindow->findChild<QCheckBox *>(QString(rhombusParamList.at(i)))){

            if(mainWindow->findChild<QCheckBox *>("rhombusEnableSwitch")){
                enableBank = mainWindow->findChild<QCheckBox *>("rhombusEnableSwitch");
                connect(enableBank, SIGNAL(stateChanged(int)), this, SLOT(slotEnable(int)));
                connect(enableBank, SIGNAL(stateChanged(int)), this, SLOT(slotValueChanged(int)));
            }
            if(mainWindow->findChild<QCheckBox *>("rhombusOutVelocity")){
                rhombusVelocity = mainWindow->findChild<QCheckBox *>("rhombusOutVelocity");
                connect(rhombusVelocity, SIGNAL(stateChanged(int)), this, SLOT(slotValueChanged(int)));
            }
            if(mainWindow->findChild<QCheckBox *>("rhombusLocalLEDControl")){
                localLEDControl = mainWindow->findChild<QCheckBox *>("rhombusLocalLEDControl");
                connect(localLEDControl, SIGNAL(stateChanged(int)), this, SLOT(slotValueChanged(int)));
            }

        } else if(mainWindow->findChild<QComboBox *>(QString(rhombusParamList.at(i)))){
            if(mainWindow->findChild<QComboBox *>("rhombusBankControl")){
                bankControl = mainWindow->findChild<QComboBox *>("rhombusBankControl");
                connect(bankControl, SIGNAL(currentIndexChanged(int)), this, SLOT(slotBankControl(int)));
                connect(bankControl, SIGNAL(currentIndexChanged(int)), this, SLOT(slotValueChanged(int)));
            }
            if(rhombusParamList.at(i).contains("rhombusOutNotePressMode")){
                modeMenu = mainWindow->findChild<QComboBox *>(rhombusParamList.at(i));
                connect(modeMenu, SIGNAL(currentIndexChanged(int)), this, SLOT(slotValueChanged(int)));
            }

        } else {
            rhombusUiBlacklist.append(rhombusParamList.at(i));
            qDebug() << "Rhombus Button Ui ***" << rhombusParamList.at(i) << "Not Found";
        }
    }

    //install event filter in all the spinboxes so that
    //copy/paste commands can be intercepted while they are in focus
    for(int i=0; i<rhombusParamList.count();i++)
       {
           if(spinBoxMap.contains(rhombusParamList.at(i)))
           {
               spinBoxMap.value(QString(rhombusParamList.at(i)))->installEventFilter(this);
           }
       }

    if(mainWindow->findChild<QDial *>("rhombusSensitivity")){
        sensitivityDial = mainWindow->findChild<QDial *>("rhombusSensitivity");
        sensitivityDial->setMinimum(25);
        sensitivityDial->setMaximum(104);
        sensitivityDial->setSingleStep(1);
        connect(sensitivityDial, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int)));
    } else {
        qDebug() << "Rhombus Button Ui ***" << "rhombusSensitivity" << "Not Found";
    }


    if(mainWindow->findChild<QSpinBox *>("rhombusInChannel"))
    {
        ledInChannel = mainWindow->findChild<QSpinBox *>("rhombusInChannel");
        connect(ledInChannel, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int)));

    }
    else
    {
        qDebug() << "Rhombus Button Ui ***" << "rhombusInChannel" << "Not Found";
    }

    if(mainWindow->findChild<QLabel *>("rhombusButtonLabel")){
        bankDestination = mainWindow->findChild<QLabel *>("rhombusButtonLabel");
    } else {
        qDebug() << "Rhombus Button Ui ***" << "rhombusButtonLabel" << "Not Found";
    }

    ledInChannel->installEventFilter(this);
    sensitivityDial->installEventFilter(this);
}

void RhombusEditPane::displayRhombusValues() {

    //disconnect param display labels during preset recall
    disconnect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), this, SLOT(slotFocusParam(QWidget*,QWidget*))); //displays based on focus
    disconnect(this, SIGNAL(signalToLabels(QString)), this, SLOT(slotToLabels(QString)));

    //get clicked rhombus map
    rhombusMap = presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").
            toMap().value("RhombusButtons").toMap().value(QString("RhombusButton%1").arg(currentRhombus)).toMap();

    //set rhombus editpane title
    paneLabel->setText(QString("for Rotaries"));

    //clear focus before displaying values
    for(int i=0; i<rhombusParamList.count();i++) {

        if(!rhombusUiBlacklist.contains(rhombusParamList.at(i))){

            // if(!rhombusParamList.at(i).contains("Velocity", Qt::CaseSensitive) && !rhombusParamList.at(i).contains("LocalLEDControl", Qt::CaseSensitive)) {
            //if(!rhombusParamList.at(i).contains("Enable", Qt::CaseSensitive)) {
            //if(!rhombusParamList.at(i).contains("BankControl", Qt::CaseSensitive)) {


            if(spinBoxMap.contains(rhombusParamList.at(i))){


                spinBoxMap.value(QString(rhombusParamList.at(i)))->clearFocus();
                if(QString(rhombusParamList.at(i)).contains("In")){
                    spinBoxMap.value(QString(rhombusParamList.at(i)))->hide();
                    QLabel *ledParamToHide = mainWindow->findChild<QLabel *>(QString("%1Label").arg(rhombusParamList.at(i)));
                    ledParamToHide->hide();
                    ledParamToHide = mainWindow->findChild<QLabel *>(QString("rhombusInLabel"));
                    ledParamToHide->hide();
                }
            }
            //}
            //}
            // }
        }

    }
    //iterate through param names and set values in edit pane ui
    for(int i=0; i<rhombusParamList.count();i++) {
        if(!rhombusUiBlacklist.contains(rhombusParamList.at(i))){

            if(spinBoxMap.contains(rhombusParamList.at(i))){

                if(!rhombusParamList.at(i).contains("Sensitivity", Qt::CaseSensitive)) {
                    if(!rhombusParamList.at(i).contains("LocalLEDControl", Qt::CaseSensitive)) {
                        if(!rhombusParamList.at(i).contains("Enable", Qt::CaseSensitive)) {
                            if(!rhombusParamList.at(i).contains("BankControl", Qt::CaseSensitive)) {
                                if(rhombusParamList.at(i).contains("Channel", Qt::CaseSensitive)){
                                    spinBoxMap.value(QString(rhombusParamList.at(i)))->setValue(rhombusMap.value(QString(rhombusParamList.at(i))).toInt() + 1);

                                } else {
                                    spinBoxMap.value(QString(rhombusParamList.at(i)))->setValue(rhombusMap.value(QString(rhombusParamList.at(i))).toInt());
                                }
                            }
                        }
                    }
                    else if (rhombusParamList.at(i).contains("Velocity", Qt::CaseSensitive)) {
                        // rhombusVelocity->setChecked(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("RhombusButtons").toMap().value("RhombusButton0").toMap().value("rhombusOutVelocity").toBool());
                    }
                }
            }
            else if (rhombusParamList.at(i).contains("rhombusOutNotePressMode")){
                modeMenu = mainWindow->findChild<QComboBox *>(rhombusParamList.at(i));
                modeMenu->setCurrentIndex(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("RhombusButtons").toMap().value(QString("RhombusButton%1").arg(currentRhombus)).toMap().value(rhombusParamList.at(i)).toInt());
            }
            else if(rhombusParamList.at(i).contains("Enable", Qt::CaseSensitive)) {
                //if it is an enable switch, set the ui object and call slotEnable with the stored bool value.
                enableBank->setChecked(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("RhombusButtons").toMap().value("RhombusButton0").toMap().value("rhombusEnableSwitch").toBool());
                slotBankControl(bankControl->currentIndex());
                slotEnable(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("RhombusButtons").toMap().value("RhombusButton0").toMap().value("rhombusEnableSwitch").toBool());
            }
            else if (rhombusParamList.at(i).contains("BankControl", Qt::CaseSensitive)) {
                bankControl->setCurrentIndex(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("RhombusButtons").toMap().value("RhombusButton0").toMap().value("rhombusBankControl").toInt());
                slotBankControl(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("RhombusButtons").toMap().value("RhombusButton0").toMap().value("rhombusBankControl").toInt());
            }
        }
    }
    sensitivityDial->setValue(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("RhombusButtons").toMap().value("rhombusSensitivity").toInt());
    localLEDControl->setChecked(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("RhombusButtons").toMap().value("rhombusLocalLEDControl").toBool());
    ledInChannel->setValue(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("RhombusButtons").toMap().value("rhombusInChannel").toInt() + 1);

    //connect display interface after preset recall
    connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), this, SLOT(slotFocusParam(QWidget*,QWidget*)));
    connect(this, SIGNAL(signalToLabels(QString)), this, SLOT(slotToLabels(QString)));
}

bool RhombusEditPane::eventFilter(QObject *object, QEvent *event){

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

void RhombusEditPane::slotEvents(QString string) {
    //format string from event into a single instance (rhombus number)
    if((string == QString("buttonButton8"))) {
        //the line above makes sure the rhombus edit pane is only looking at the correct buttonButtons
        currentRhombus = string.remove(0,12).toInt()-8;//remove "buttonButton" and convert last character to int
        qDebug() << QString("Rhombus Edit Pane: button %1").arg(currentRhombus);
        displayRhombusValues();
    }
}

void RhombusEditPane::slotRecallPreset(int preset) {
    currentPreset = preset;
    displayRhombusValues();
    if(!prevObjectToLabel.contains("None")) {
        emit signalToLabels(prevObjectToLabel);
    }
}

void RhombusEditPane::slotEnable(int enable) {
    if(enable) {
        for (int i=0; i<rhombusParamList.count(); i++) {
            if(!rhombusUiBlacklist.contains(rhombusParamList.at(i))){
                    if(!rhombusParamList.at(i).contains("Sensitivity")){
                        if(!rhombusParamList.at(i).contains("BankControl")){
                            if(!rhombusParamList.at(i).contains("Enable")){
                                if(spinBoxMap.contains(rhombusParamList.at(i))){
                                    spinBoxMap.value(QString(rhombusParamList.at(i)))->setEnabled(false);
                                }
                                else if(rhombusParamList.at(i).contains("Mode")){
                                    modeMenu = mainWindow->findChild<QComboBox *>(rhombusParamList.at(i));
                                    modeMenu->setEnabled(false);
                                }
                            }
                        }
                    }

                bankControl->setEnabled(true);
            }

        }
        //rhombusVelocity->setEnabled(false);
        localLEDControl->setChecked(true);
        localLEDControl->setEnabled(false);
        slotBankControl(bankControl->currentIndex());

    } else {
        for (int i=0; i<rhombusParamList.count(); i++) {
            if(!rhombusUiBlacklist.contains(rhombusParamList.at(i))){

                    if(!rhombusParamList.at(i).contains("Sensitivity")){
                        if(!rhombusParamList.at(i).contains("BankControl")){
                            if(!rhombusParamList.at(i).contains("Enable")){
                                if(spinBoxMap.contains(rhombusParamList.at(i))){
                                    spinBoxMap.value(QString(rhombusParamList.at(i)))->setEnabled(true);
                                }
                                else if(rhombusParamList.at(i).contains("Mode")){
                                    modeMenu = mainWindow->findChild<QComboBox *>(rhombusParamList.at(i));
                                    modeMenu->setEnabled(true);
                                }
                            }
                        }
                    }

                bankControl->setEnabled(false);
            }

        }
        //rhombusVelocity->setEnabled(true);
        localLEDControl->setEnabled(true);
        bankDestination->setText("Bank Control Off");
    }
}

void RhombusEditPane::slotBankControl(int menuChoice) {
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

void RhombusEditPane::slotValueChanged(int num) {
    focusCheck = qobject_cast<QWidget *>(QObject::sender());
    if(focusCheck->hasFocus()){
        //qDebug() << QString("From Rhombus Edit pane: %1 %2").arg(focusCheck->objectName()).arg(num);

        if(focusCheck->objectName().contains("VelocityValue") || focusCheck->objectName().contains("PressValue")){
            if (num == 0){
                num = -1;
            }
        }
        emit signalValueChanged(currentPreset, "ComponentSettings", "RhombusButton", currentRhombus,focusCheck->objectName(),num);
        emit signalToLabels(focusCheck->objectName());
    }
    else if(focusCheck->objectName().contains("Velocity")
            ||focusCheck->objectName().contains("Enable")
            ||focusCheck->objectName().contains("Local")
            ||focusCheck->objectName().contains("BankControl")
            || focusCheck->objectName().contains("rhombusOutNotePressMode")){
        emit signalValueChanged(currentPreset, "ComponentSettings", "RhombusButton", currentRhombus, focusCheck->objectName(), num);
    }
}

void RhombusEditPane::slotToLabels(QString parameter) {
    for(int i = 0; i < 1; i++) {
        rhombusLabel[i]->slotDisplayParameter(parameter, i, currentPreset);
    }
    prevObjectToLabel = parameter;
}

void RhombusEditPane::slotFocusParam(QWidget *oldObject, QWidget *nowObject) {
    if (nowObject != nullptr) {
        clicked = nowObject;
        if(clicked->parent()->objectName().contains("rhombus")) {
            emit signalToLabels(clicked->objectName());
        }
    }
}
