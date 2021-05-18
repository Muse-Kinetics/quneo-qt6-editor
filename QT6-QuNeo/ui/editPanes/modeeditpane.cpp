#include "modeeditpane.h"
#include <QDebug>

ModeEditPane::ModeEditPane(QVariantMap *variantMap, QVariantMap *variantMapCopy, QWidget *widget, QWidget *parent) :
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
        modeLabel[i] = new ModeLabels(mainWindow, presetMapCopy, 0);
    }
}

void ModeEditPane::findUiElements()
{
    //get list of mode button parameters
    modeParamList = presetMapCopy->value("Preset 0").toMap().value("ComponentSettings").
            toMap().value(QString("ModeButtons")).toMap().value(QString("ModeButton0")).toMap().keys();

    //iterate through list of params and populate map of spin box addresses linked by param name
    for(int i=0; i<modeParamList.count();i++) {

        if(mainWindow->findChild<QSpinBox *>(QString(modeParamList.at(i)))){

        if(!modeParamList.at(i).contains("Sensitivity", Qt::CaseSensitive) && !modeParamList.at(i).contains("Velocity", Qt::CaseSensitive)) {
            if(!modeParamList.at(i).contains("Enable", Qt::CaseSensitive)) {
                spinBoxMap.insert(QString(modeParamList.at(i)),mainWindow->findChild<QSpinBox *>(QString(modeParamList.at(i))));
                //set up all spin boxes to have correct minimum / maximum and "off" text.
                if(!modeParamList.at(i).contains("Channel", Qt::CaseSensitive)) {
                    spinBoxMap.value(QString(modeParamList.at(i)))->setMinimum(-1);
                    spinBoxMap.value(QString(modeParamList.at(i)))->setMaximum(127);
                    spinBoxMap.value(QString(modeParamList.at(i)))->setSpecialValueText("off");
                }
                else if(modeParamList.at(i).contains("Channel", Qt::CaseSensitive)) { //no off state, 1-16
                    spinBoxMap.value(QString(modeParamList.at(i)))->setMinimum(1);
                    spinBoxMap.value(QString(modeParamList.at(i)))->setMaximum(16);
                }
                connect(spinBoxMap.value(QString(modeParamList.at(i))), SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int)));
            }
        }

        } if(mainWindow->findChild<QCheckBox *>(QString(modeParamList.at(i)))){

            if(mainWindow->findChild<QCheckBox *>("modeEnableSwitch")){
                enableMIDI = mainWindow->findChild<QCheckBox *>("modeEnableSwitch");
                connect(enableMIDI, SIGNAL(stateChanged(int)), this, SLOT(slotEnable(int)));
                connect(enableMIDI, SIGNAL(stateChanged(int)), this, SLOT(slotValueChanged(int)));
            }

            if(mainWindow->findChild<QCheckBox *>("modeOutVelocity")){
                modeVelocity = mainWindow->findChild<QCheckBox *>("modeOutVelocity");
                connect(modeVelocity, SIGNAL(stateChanged(int)), this, SLOT(slotValueChanged(int)));

            }

        } else {
            modeUiBlacklist.append(modeParamList.at(i));
            qDebug() << "Mode Button Ui ***" << modeParamList.at(i) << "Not Found";
        }


    }

    if(mainWindow->findChild<QDial *>("modeSensitivity")){
        sensitivityDial = mainWindow->findChild<QDial *>("modeSensitivity");
        sensitivityDial->setMinimum(25);
        sensitivityDial->setMaximum(104);
        sensitivityDial->setSingleStep(1);
        connect(sensitivityDial, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int)));
    } else {
        qDebug() << "Mode Button Ui ***" << "modeSensitivity" << "Not Found";
    }

    if(mainWindow->findChild<QCheckBox *>("modeLocalLEDControl")){
        localLEDControl = mainWindow->findChild<QCheckBox *>("modeLocalLEDControl");
        connect(localLEDControl, SIGNAL(stateChanged(int)), this, SLOT(slotValueChanged(int)));
    } else {
        qDebug() << "Mode Button Ui ***" << "modeLocalLEDControl" << "Not Found";
    }
}

void ModeEditPane::displayModeValues() {

    //disconnect param display labels during preset recall
    disconnect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), this, SLOT(slotFocusParam(QWidget*,QWidget*)));
    disconnect(this, SIGNAL(signalToLabels(QString)), this, SLOT(slotToLabels(QString)));

    //get clicked mode map
    modeMap = presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").
            toMap().value("ModeButtons").toMap().value(QString("ModeButton%1").arg(currentMode)).toMap();

    //clear focus before displaying values
    for(int i=0; i<modeParamList.count();i++) {
        if(!modeUiBlacklist.contains(modeParamList.at(i))){
            if(!modeParamList.at(i).contains("Enable", Qt::CaseSensitive) && !modeParamList.at(i).contains("Velocity", Qt::CaseSensitive) && !modeParamList.at(i).contains("LocalLEDControl", Qt::CaseSensitive)) {
                spinBoxMap.value(QString(modeParamList.at(i)))->clearFocus();
            }
        }
    }

    //iterate through param names and set values in edit pane ui
    for(int i=0; i<modeParamList.count();i++) {

        if(!modeUiBlacklist.contains(modeParamList.at(i))){
            if(!modeParamList.at(i).contains("Sensitivity", Qt::CaseSensitive) && !modeParamList.at(i).contains("Velocity", Qt::CaseSensitive) && !modeParamList.at(i).contains("LocalLEDControl", Qt::CaseSensitive)) {
                if(!modeParamList.at(i).contains("Enable", Qt::CaseSensitive)) {
                    spinBoxMap.value(QString(modeParamList.at(i)))->setValue(modeMap.value(QString(modeParamList.at(i))).toInt());
                }
                else {
                    enableMIDI->setChecked(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value
                                           ("ComponentSettings").toMap().value("ModeButtons").toMap().value(QString("ModeButton%1").arg
                                                                                                            (currentMode)).toMap().value("modeEnableSwitch").toBool());
                    slotEnable(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value
                               ("ComponentSettings").toMap().value("ModeButtons").toMap().value(QString("ModeButton%1").arg
                                                                                                (currentMode)).toMap().value("modeEnableSwitch").toBool());
                }
            }

        }
    }
    sensitivityDial->setValue(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value
        ("ComponentSettings").toMap().value("ModeButtons").toMap().value("modeSensitivity").toInt());

    localLEDControl->setChecked(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("ModeButtons").toMap().value("modeLocalLEDControl").toBool());
    //connect display interface after preset recall
    connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), this, SLOT(slotFocusParam(QWidget*,QWidget*)));
    connect(this, SIGNAL(signalToLabels(QString)), this, SLOT(slotToLabels(QString)));
}

void ModeEditPane::slotEvents(QString string) {
    //qDebug()<<string;

    //format string from event into a single instance
    if((string == QString("buttonButton0"))) {
        //the line above makes sure the mode edit pane is only looking at the correct buttonButton
        currentMode = string.remove(0,12).toInt(); //remove "buttonButton" and convert the last character to int
        qDebug() << QString("Mode Edit Pane: button %1").arg(currentMode);
        displayModeValues();
    }
}

void ModeEditPane::slotRecallPreset(int preset) {
    currentPreset = preset;
    displayModeValues();
    qDebug()<<QString("ModeEditPane Preset %1").arg(currentPreset);
    if(!prevObjectToLabel.contains("None")) {
        emit signalToLabels(prevObjectToLabel);
    }
}

void ModeEditPane::slotEnable(int enable) {
    if(enable) {
        for(int i=1; i<modeParamList.count(); i++) {
            if(!modeUiBlacklist.contains(modeParamList.at(i))){
                if(!QString("%1").arg(modeParamList.at(i)).contains("Velocity") && !modeParamList.at(i).contains("LocalLEDControl", Qt::CaseSensitive)) {
                    if(!QString("%1").arg(modeParamList.at(i)).contains("Enable")) {
                        spinBoxMap.value(QString(modeParamList.at(i)))->setEnabled(true);
                    }
                }

            }
        }
        modeVelocity->setEnabled(true);
    } else {
        for(int i=1; i<modeParamList.count(); i++) {
            if(!modeUiBlacklist.contains(modeParamList.at(i))){
                if(!QString("%1").arg(modeParamList.at(i)).contains("Velocity") && !modeParamList.at(i).contains("LocalLEDControl", Qt::CaseSensitive)) {
                    if(!QString("%1").arg(modeParamList.at(i)).contains("Enable")) {
                        spinBoxMap.value(QString(modeParamList.at(i)))->setEnabled(false);
                    }
                }
            }

        }
        modeVelocity->setEnabled(false);
    }
}

void ModeEditPane::slotValueChanged(int num) {
    focusCheck = qobject_cast<QWidget *>(QObject::sender());
    //qDebug()<<QString("From Mode edit pane: %1 %2").arg(focusCheck->objectName()).arg(num);
    emit signalValueChanged(currentPreset, "ComponentSettings", "ModeButton", currentMode, focusCheck->objectName(), num);
    emit signalToLabels(focusCheck->objectName()); //whenever a value is changed this should be emitte, but it shouldn't always have an effect
}

void ModeEditPane::slotToLabels(QString parameter) {
    for(int i = 0; i < 1; i++) {
        modeLabel[i]->slotDisplayParameter(parameter, i, currentPreset);
    }
    prevObjectToLabel = parameter;
}

void ModeEditPane::slotFocusParam(QWidget *oldObject, QWidget *nowObject) {
    qDebug() << "ModeEditPane called";
    if(nowObject->isEnabled()) {
        clicked = nowObject;
        if(clicked->parent()->objectName().contains("mode")) {
            emit signalToLabels(clicked->objectName());
        }
    }
}
