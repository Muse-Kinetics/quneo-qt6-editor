#include "hsliderbutton.h"
#include <QDebug>

HSliderButton::HSliderButton(PresetHandler *presetHandle, HSliderEditPane *editPaneWidget, int num, QWidget *mainWindow, QObject *parent) :
    QObject(parent)
{
    presetHandler = presetHandle;
    focusConnected = false;
    hSliderEditPane = editPaneWidget;
    hSliderButtonNumber = num;
    hSliderButtonName = QString("hSliderButton%1").arg(hSliderButtonNumber);
    hSliderButtonLabel = mainWindow->findChild<QLabel *>(QString("hSliderButton%1").arg(hSliderButtonNumber));
    hSliderButtonLabel->installEventFilter(mainWindow);

    hSliderButtonTransparent.load(QString(":Quneo/images/resources/images/hSliderTransparent.png"));
    hSliderButtonHighlight.load(QString(":Quneo/images/resources/images/horizontalSliderHighlight.png"));
    hSliderButtonLabel->setPixmap(hSliderButtonTransparent);

    connect(mainWindow, SIGNAL(signalEvents(QString)), this, SLOT(slotEvents(QString)));
    editPanes = mainWindow->findChild<QStackedWidget *>("EditPanes");
    startMessage = mainWindow->findChild<QLabel *>("startMessage");
    connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), this, SLOT(slotCheckToConnectFocus(QWidget*,QWidget*)));
}

void HSliderButton::slotEvents(QString string){
    if(string == hSliderButtonName){
        hSliderButtonLabel->setPixmap(hSliderButtonHighlight);
        disconnect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), hSliderEditPane, SLOT(slotFocusParam(QWidget*,QWidget*)));
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
        editPanes->setCurrentIndex(6);
        emit signalCurrentSensor(hSliderButtonName);
        connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), hSliderEditPane, SLOT(slotFocusParam(QWidget*,QWidget*)));
        presetHandler->slotConnectDisconnectDisplayAllLabels(true);
        focusConnected = true;
    }
    else {
        hSliderButtonLabel->setPixmap(hSliderButtonTransparent);
    }
}

void  HSliderButton::slotCheckToConnectFocus(QWidget* old, QWidget* now){
    if(now){
        if(!focusConnected && now->objectName().contains("hB")){
            connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), hSliderEditPane, SLOT(slotFocusParam(QWidget*,QWidget*)));
            focusConnected = true;
 }}
}

