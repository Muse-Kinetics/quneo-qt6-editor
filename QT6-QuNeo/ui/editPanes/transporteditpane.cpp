#include "transporteditpane.h"
#include <QDebug>

TransportEditPane::TransportEditPane(QVariantMap *variantMap, QVariantMap *variantMapCopy, QWidget *widget, QWidget *parent) :
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

    for(int i = 0; i < 3; i++) {
        transportLabel[i] = new TransportLabels(mainWindow, presetMapCopy, 0);
    }
#ifdef Q_OS_MAC

#else
    QLabel *sensitivityLabel = mainWindow->findChild<QLabel *>("transportSensitivityLabel");
    QFont f( "Arial", 3, QFont::Normal);
    sensitivityLabel->setFont(f);
    sensitivityDial->move(9, 378);
    sensitivityLabel->move(3, 405);
#endif
}

void TransportEditPane::findUiElements() {
    QTabWidget *transportTabs = mainWindow->findChild<QTabWidget *>("transportTabs");
    transportTabs->setCurrentIndex(0);
    transportTabs->setTabEnabled(1, false);
    //link transport button edit pane master label
    paneLabel = mainWindow->findChild<QLabel *>("transportButtonLabel");

    //get list of transport button parameters (use Prest 0 and TransportButton0 as a template) **** below is one line of code in two lines ****
    transportParamList = presetMapCopy->value("Preset 0").toMap().value("ComponentSettings").
            toMap().value("TransportButtons").toMap().value(QString("TransportButton0")).toMap().keys();

    //iterate through list of params and populate map of spin box addresses linked by param name
    for(int i=0; i<transportParamList.count();i++) {
       if(mainWindow->findChild<QSpinBox *>(QString(transportParamList.at(i)))){
           if(!transportParamList.at(i).contains("Sensitivity", Qt::CaseSensitive) && !transportParamList.at(i).contains("LocalLEDControl", Qt::CaseSensitive)) {
                spinBoxMap.insert(QString(transportParamList.at(i)),mainWindow->findChild<QSpinBox *>(QString(transportParamList.at(i))));
                //set up all spin boxes to have correct minimum / maximum and "off" text.
                if(!transportParamList.at(i).contains("Channel", Qt::CaseSensitive)) {
                    spinBoxMap.value(QString(transportParamList.at(i)))->setMinimum(-1);
                    spinBoxMap.value(QString(transportParamList.at(i)))->setMaximum(127);
                    spinBoxMap.value(QString(transportParamList.at(i)))->setSpecialValueText("off");
                    if(transportParamList.at(i).contains("VelocityValue", Qt::CaseSensitive))
                    {
                        spinBoxMap.value(QString(transportParamList.at(i)))->setMinimum(0);
                        spinBoxMap.value(QString(transportParamList.at(i)))->setMaximum(127);
                        spinBoxMap.value(QString(transportParamList.at(i)))->setSpecialValueText("var");
                    }
                    if(transportParamList.at(i).contains("PressValue", Qt::CaseSensitive))
                    {
                        spinBoxMap.value(QString(transportParamList.at(i)))->setMinimum(0);
                        spinBoxMap.value(QString(transportParamList.at(i)))->setMaximum(127);
                        spinBoxMap.value(QString(transportParamList.at(i)))->setSpecialValueText("con");
                    }
                }
                else if(transportParamList.at(i).contains("Channel", Qt::CaseSensitive)) {
                    //no off state, 1-16
                    spinBoxMap.value(QString(transportParamList.at(i)))->setMinimum(1);
                    spinBoxMap.value(QString(transportParamList.at(i)))->setMaximum(16);
                }
                connect(spinBoxMap.value(QString(transportParamList.at(i))), SIGNAL(valueChanged(int)),
                        this, SLOT(slotValueChanged(int)));
            }

        } else  if(mainWindow->findChild<QCheckBox *>(QString(transportParamList.at(i)))){

            if(mainWindow->findChild<QCheckBox *>("transportOutVelocity")){
                transportVelocity = mainWindow->findChild<QCheckBox *>("transportOutVelocity");
                connect(transportVelocity, SIGNAL(stateChanged(int)), this, SLOT(slotValueChanged(int)));
            }

        } else  if(mainWindow->findChild<QComboBox *>(QString(transportParamList.at(i)))){

           if(transportParamList.at(i).contains("NotePressMode")){
               if(transportParamList.at(i).contains("transportOutNotePressMode"))
               {
                   modeMenu = mainWindow->findChild<QComboBox *>(transportParamList.at(i));
                   connect(modeMenu, SIGNAL(currentIndexChanged(int)), this, SLOT(slotValueChanged(int)));
               }
           }

       }else {
            transportUiBlacklist.append(transportParamList.at(i));
            qDebug() << "Transport Button Ui ***" << transportParamList.at(i) << "Not Found";
        }

    }
    //install event filter in all the spinboxes so that
    //copy/paste commands can be intercepted while they are in focus
    for(int i=0; i<transportParamList.count();i++)
       {
           if(spinBoxMap.contains(transportParamList.at(i)))
           {
               spinBoxMap.value(QString(transportParamList.at(i)))->installEventFilter(this);
           }
       }

    if((sensitivityDial = mainWindow->findChild<QDial *>("transportSensitivity"))){
        sensitivityDial = mainWindow->findChild<QDial *>("transportSensitivity");
        connect(sensitivityDial, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int)));

    } else {
        qDebug() << "Transport Button Ui ***" << "transportSensitivity" << "Not Found";
    }

    if(mainWindow->findChild<QCheckBox *>("transportLocalLEDControl")){
        localLEDControl = mainWindow->findChild<QCheckBox *>("transportLocalLEDControl");
        connect(localLEDControl, SIGNAL(stateChanged(int)), this, SLOT(slotValueChanged(int)));

    } else {
        qDebug() << "Transport Button Ui ***" << "transportLocalLEDControl" << "Not Found";
    }

    if(mainWindow->findChild<QSpinBox *>("transportInChannel")){
        ledInputChannel = mainWindow->findChild<QSpinBox *>("transportInChannel");
        connect(ledInputChannel, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int)));

    } else {
        qDebug() << "Transport Button Ui ***" << "transportInChannel" << "Not Found";
    }

    ledInputChannel->installEventFilter(this);
    sensitivityDial->installEventFilter(this);

}

void TransportEditPane::displayTransportValues() {

    //disconnect param display labels during preset recall
    disconnect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), this, SLOT(slotFocusParam(QWidget*,QWidget*))); //displays based on focus
    disconnect(this, SIGNAL(signalToLabels(QString)), this, SLOT(slotToLabels(QString))); //displays when value changed

    //get clicked transport map
    transportMap = presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").
            toMap().value("TransportButtons").toMap().value(QString("TransportButton%1").arg(currentTransport)).toMap();

    //set transport editpane title
    //paneLabel->setText(QString("Transport %1").arg(currentTransport));
    if(currentTransport == 0) {
        paneLabel->setText(QString("Record"));
    }
    else if (currentTransport == 1) {
        paneLabel->setText(QString("Stop"));
    }
    else if (currentTransport == 2) {
        paneLabel->setText(QString("Play"));
    }

    //clear focus before displaying values
    for(int i=0; i<transportParamList.count();i++) {
        if(!transportUiBlacklist.contains(transportParamList.at(i))){
            if(spinBoxMap.contains(transportParamList.at(i))){
            if(!transportParamList.at(i).contains("LocalLEDControl", Qt::CaseSensitive)) {
                spinBoxMap.value(QString(transportParamList.at(i)))->clearFocus();
                }
            }
        }
    }

    //iterate through param names and set values in edit pane ui
    for(int i=0; i<transportParamList.count();i++) {
        if(!transportUiBlacklist.contains(transportParamList.at(i))){
            if(spinBoxMap.contains(transportParamList.at(i))){
                if(transportParamList.at(i).contains("Channel", Qt::CaseSensitive)){
                    spinBoxMap.value(QString(transportParamList.at(i)))->setValue(transportMap.value(QString(transportParamList.at(i))).toInt() + 1);}
                else {spinBoxMap.value(QString(transportParamList.at(i)))->setValue(transportMap.value(QString(transportParamList.at(i))).toInt());}
                if(QString(transportParamList.at(i)).contains("In")){
                    spinBoxMap.value(QString(transportParamList.at(i)))->hide();
                    QLabel *ledParamToHide = mainWindow->findChild<QLabel *>(QString("%1Label").arg(transportParamList.at(i)));
                    //ledParamToHide->hide();
                    //ledParamToHide = mainWindow->findChild<QLabel *>(QString("transportInLabel"));
                    //ledParamToHide->hide();
                }
            }
            else if(mainWindow->findChild<QComboBox *>(QString(transportParamList.at(i)))){
                modeMenu = mainWindow->findChild<QComboBox *>(QString(transportParamList.at(i)));
                modeMenu->setCurrentIndex(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("TransportButtons").toMap().value(QString("TransportButton%1").arg(currentTransport)).toMap().value(QString(transportParamList.at(i))).toInt());
            }
        }
    }

    if(sensitivityDial){
    sensitivityDial->setValue(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("TransportButtons").toMap().value("transportSensitivity").toInt());
    }

    if(transportVelocity){
    //transportVelocity->setChecked(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("TransportButtons").toMap().value(QString("TransportButton%1").arg(currentTransport)).toMap().value("transportOutVelocity").toBool());
    }

    if(localLEDControl){
    localLEDControl->setChecked(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("TransportButtons").toMap().value("transportLocalLEDControl").toBool());
    }

    if(ledInputChannel){
    ledInputChannel->setValue(presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap().value("ComponentSettings").toMap().value("TransportButtons").toMap().value("transportInChannel").toInt() + 1);
    }

    //connect display interface after preset recall
    connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), this, SLOT(slotFocusParam(QWidget*,QWidget*)));
    connect(this, SIGNAL(signalToLabels(QString)), this, SLOT(slotToLabels(QString)));
}

bool TransportEditPane::eventFilter(QObject *object, QEvent *event){

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

void TransportEditPane::slotEvents(QString string) {
    //qDebug() << "STRING:" << QString(string);
    //format string from event into a single instance (transport number)
    if((string == QString("buttonButton1")) || (string == QString("buttonButton2")) || (string == QString("buttonButton3"))) {
        //the line above makes sure the transport edit pane is only looking at the correct buttonButtons
        currentTransport = string.remove(0,12).toInt()-1;//remove "buttonButton" and convert last character to int
        qDebug() << QString("Transport Edit Pane: button %1").arg(currentTransport);
        displayTransportValues();
    }
}

void TransportEditPane::slotRecallPreset(int preset) {
    currentPreset = preset;
    displayTransportValues();
    if(!prevObjectToLabel.contains("None")) {
        emit signalToLabels(prevObjectToLabel);
    }
}

void TransportEditPane::slotValueChanged(int num) {
    focusCheck = qobject_cast<QWidget *>(QObject::sender());
    if(focusCheck->hasFocus()){

        if(focusCheck->objectName().contains("VelocityValue") || focusCheck->objectName().contains("PressValue")){
            if (num == 0){
                num = -1;
            }
        }
        //qDebug()<<QString("From transport edit pane: %1 %2").arg(focusCheck->objectName()).arg(num);
        emit signalValueChanged(currentPreset, "ComponentSettings", "TransportButton", currentTransport, focusCheck->objectName(), num);
        emit signalToLabels(focusCheck->objectName()); //whenever a value is changed, this should be emitted, but it shouldn't always have an effect
    }
    else if(focusCheck->objectName().contains("Velocity")
            || focusCheck->objectName().contains("BankControl")
            || focusCheck->objectName().contains("Enable")
            || focusCheck->objectName().contains("Local")
            || focusCheck->objectName().contains("transportOutNotePressMode")){
        emit signalValueChanged(currentPreset, "ComponentSettings", "TransportButton", currentTransport, focusCheck->objectName(),num);
        emit signalToLabels(focusCheck->objectName()); //whenever a value is changed, this should be emitted, but it shouldn't always have an effect
    }
}

void TransportEditPane::slotToLabels(QString parameter) {
    for(int i = 0; i < 3; i++) {
        transportLabel[i]->slotDisplayParameter(parameter, i, currentPreset);
    }
    prevObjectToLabel = parameter;
}

void TransportEditPane::slotFocusParam(QWidget *oldObject, QWidget *nowObject) {
    qDebug() << "TransportEditPane called";
    if(nowObject) {
        clicked = nowObject;
        if(clicked->parent()->objectName().contains("transport")) {
            emit signalToLabels(clicked->objectName());
        }
    }
}
