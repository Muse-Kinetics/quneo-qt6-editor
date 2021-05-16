#include "rotarybutton.h"
#include <QDebug>

RotaryButton::RotaryButton(PresetHandler *presetHandle, RotaryEditPane *editPaneWidget, int num, QWidget *mainWindow, QObject *parent) :
    QObject(parent)
{
    presetHandler = presetHandle;
    rotaryEditPane = editPaneWidget;
    focusConnected = false;
    rotaryButtonNumber = num;
    rotaryButtonName = QString("rotaryButton%1").arg(rotaryButtonNumber);
    rotaryButtonLabel = mainWindow->findChild<QLabel *>(QString("rotaryButton%1").arg(rotaryButtonNumber));
    rotaryButtonLabel->installEventFilter(mainWindow);

    rotaryButtonTransparent.load(QString(":Quneo/images/resources/images/rotaryTransparent.png"));
    rotaryButtonHighlight.load(QString(":Quneo/images/resources/images/rotaryHighlight.png"));
    rotaryButtonLabel->setPixmap(rotaryButtonTransparent);

    connect(mainWindow, SIGNAL(signalEvents(QString)), this, SLOT(slotEvents(QString)));
    editPanes = mainWindow->findChild<QStackedWidget *>("EditPanes");
    connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), this, SLOT(slotCheckToConnectFocus(QWidget*,QWidget*)));
    startMessage = mainWindow->findChild<QLabel *>("startMessage");
}

void RotaryButton::slotEvents(QString string){

    if(string == rotaryButtonName){
        rotaryButtonLabel->setPixmap(rotaryButtonHighlight);
        disconnect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), rotaryEditPane, SLOT(slotFocusParam(QWidget*,QWidget*)));
        presetHandler->slotConnectDisconnectDisplayAllLabels(false);
        focusConnected = false;
        if(!startMessage->isHidden()){
            startMessage->hide();
            connect(presetHandler->saveButton, SIGNAL(clicked()), presetHandler, SLOT(slotSave()));
            copyPasteHandle->copyAct->setDisabled(false);
            copyPasteHandle->copySensorAct->setDisabled(false);
            copyPasteHandle->revertFactoryAct->setDisabled(false);
            copyPasteHandle->clearPreset->setDisabled(false);
            copyPasteHandle->importPreset->setDisabled(false);
            copyPasteHandle->exportPreset->setDisabled(false);
            copyPasteHandle->exportAllPresets->setDisabled(false);
            copyPasteHandle->revertAllPresets->setDisabled(false);
            copyPasteHandle->saveAllPresets->setDisabled(false);
            copyPasteHandle->savePreset->setDisabled(false);
        }
        editPanes->setCurrentIndex(9);
        emit signalCurrentSensor(rotaryButtonName);
        connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), rotaryEditPane, SLOT(slotFocusParam(QWidget*,QWidget*)));
        presetHandler->slotConnectDisconnectDisplayAllLabels(true);
        focusConnected = true;
    }
    else {
        rotaryButtonLabel->setPixmap(rotaryButtonTransparent);
    }
}

void RotaryButton::slotCheckToConnectFocus(QWidget *old, QWidget *now){

    if(now){
        if(!focusConnected && now->objectName().contains("rB")){
            connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), rotaryEditPane, SLOT(slotFocusParam(QWidget*,QWidget*)));
            focusConnected = true;
 }}

}
