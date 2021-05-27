#include "vsliderbutton.h"
#include <QDebug>

VSliderButton::VSliderButton(PresetHandler *presetHandle, VSliderEditPane *editPaneWidget, int num, QWidget *mainWindow, QObject *parent) :
    QObject(parent)
{
    presetHandler = presetHandle;
    focusConnected = false;
    vSliderEditPane = editPaneWidget;
    vSliderButtonNumber = num;
    vSliderButtonName = QString("vSliderButton%1").arg(vSliderButtonNumber);
    vSliderButtonLabel = mainWindow->findChild<QLabel *>(QString("vSliderButton%1").arg(vSliderButtonNumber));
    vSliderButtonLabel->installEventFilter(mainWindow);

    vSliderButtonTransparent.load(QString(":Quneo/images/resources/images/vSliderTransparent.png"));
    vSliderButtonHighlight.load(QString(":Quneo/images/resources/images/vSliderHighlight.png"));
    vSliderButtonLabel->setPixmap(vSliderButtonTransparent);

    connect(mainWindow, SIGNAL(signalEvents(QString)), this, SLOT(slotEvents(QString)));
    editPanes = mainWindow->findChild<QStackedWidget *>("EditPanes");
    connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), this, SLOT(slotCheckToConnectFocus(QWidget*,QWidget*)));
    startMessage = mainWindow->findChild<QLabel *>("startMessage");
}

void VSliderButton::slotEvents(QString string){

    if(string == vSliderButtonName){
        //qDebug() << QString("Vertical Slider %1 was clicked!").arg(vSliderButtonNumber);
        vSliderButtonLabel->setPixmap(vSliderButtonHighlight);
        disconnect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), vSliderEditPane, SLOT(slotFocusParam(QWidget*,QWidget*)));
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
        editPanes->setCurrentIndex(7);
        emit signalCurrentSensor(vSliderButtonName);
        connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), vSliderEditPane, SLOT(slotFocusParam(QWidget*,QWidget*)));
        presetHandler->slotConnectDisconnectDisplayAllLabels(true);
        focusConnected = true;
    }
    else {
        vSliderButtonLabel->setPixmap(vSliderButtonTransparent);
    }
}

void VSliderButton::slotCheckToConnectFocus(QWidget *old, QWidget *now){

    if(now){
        if(!focusConnected && now->objectName().contains("vB")){
            connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), vSliderEditPane, SLOT(slotFocusParam(QWidget*,QWidget*)));
            focusConnected = true;
 }}

}
