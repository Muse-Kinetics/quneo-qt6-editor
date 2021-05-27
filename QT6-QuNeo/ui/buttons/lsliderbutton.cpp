#include "lsliderbutton.h"
#include <QDebug>

LSliderButton::LSliderButton(PresetHandler *presetHandle, LSliderEditPane *editPaneWidget, int num, QWidget *mainWindow, QObject *parent) :
    QObject(parent)
{
    presetHandler = presetHandle;
    focusConnected = false;
    lSliderEditPane = editPaneWidget;
    lSliderButtonNumber = num;
    lSliderButtonName = QString("lSliderButton%1").arg(lSliderButtonNumber);
    lSliderButtonLabel = mainWindow->findChild<QLabel *>(QString("lSliderButton%1").arg(lSliderButtonNumber));
    lSliderButtonLabel->installEventFilter(mainWindow);

    lSliderButtonTransparent.load(QString(":Quneo/images/resources/images/lSliderTransparent.png"));
    lSliderButtonHighlight.load(QString(":Quneo/images/resources/images/longSliderHighlight.png"));
    lSliderButtonLabel->setPixmap(lSliderButtonTransparent);

    connect(mainWindow, SIGNAL(signalEvents(QString)), this, SLOT(slotEvents(QString)));
    editPanes = mainWindow->findChild<QStackedWidget *>("EditPanes");
    startMessage = mainWindow->findChild<QLabel *>("startMessage");
    connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), this, SLOT(slotCheckToConnectFocus(QWidget*,QWidget*)));
}

void LSliderButton::slotEvents(QString string){

    if(string == lSliderButtonName){
        lSliderButtonLabel->setPixmap(lSliderButtonHighlight);
        disconnect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), lSliderEditPane, SLOT(slotFocusParam(QWidget*,QWidget*)));
        presetHandler->slotConnectDisconnectDisplayAllLabels(false);
        focusConnected = false;
        if(!startMessage->isHidden()){
            startMessage->hide();
            // EB commented out to avoid crash
            //connect(presetHandler->saveButton, SIGNAL(clicked()), presetHandler, SLOT(slotSave()));
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
        editPanes->setCurrentIndex(8);
        emit signalCurrentSensor(lSliderButtonName);
        connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), lSliderEditPane, SLOT(slotFocusParam(QWidget*,QWidget*)));
        presetHandler->slotConnectDisconnectDisplayAllLabels(true);
        focusConnected = true;
    }
    else {
        lSliderButtonLabel->setPixmap(lSliderButtonTransparent);
    }
}

void LSliderButton::slotCheckToConnectFocus(QWidget *old, QWidget *now){

    if(now){
        if(!focusConnected && now->objectName().contains("lB")){
            connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), lSliderEditPane, SLOT(slotFocusParam(QWidget*,QWidget*)));
            focusConnected = true;
 }}

}
