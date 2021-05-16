#include "padbutton.h"
#include <QDebug>

PadButton::PadButton(PresetHandler *presetHandle, PadEditPane *editPaneWidget, int num, QWidget *mainWindow, QObject *parent) :
    QObject(parent)
{
    presetHandler = presetHandle;
    focusConnected = false;
    padEditPane = editPaneWidget;
    padButtonNumber = num;
    padButtonName = QString("padButton%1").arg(padButtonNumber);

    padButtonLabel = mainWindow->findChild<QLabel *>(QString("padButton%1").arg(padButtonNumber));
    padButtonLabel->installEventFilter(mainWindow);
    padTabPanes = mainWindow->findChild<QTabWidget *>(QString("padTabs"));
    connect(padTabPanes, SIGNAL(currentChanged(int)), this, SLOT(slotTabChange(int)));

    padButtonTransparent.load(QString(":Quneo/images/resources/images/padTransparent.png"));
    padButtonHighlight.load(QString(":Quneo/images/resources/images/padHighlight.png"));
    padButtonLabel->setPixmap(padButtonTransparent);

    connect(mainWindow, SIGNAL(signalEvents(QString)), this, SLOT(slotEvents(QString)));
    connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), this, SLOT(slotCheckToConnectFocus(QWidget*,QWidget*)));
    editPanes = mainWindow->findChild<QStackedWidget *>("EditPanes");
    startMessage = mainWindow->findChild<QLabel *>("startMessage");
    perPadSensitivityLabel = mainWindow->findChild<QLabel *>("padSensitivityPerPadLabel");
}

void PadButton::slotTabChange(int currentTab){
    padEditPane->slotUpdateXYText();
}

void PadButton::slotEvents(QString string){

    if(string == padButtonName){
        perPadSensitivityLabel->setText(QString("Pad %1 Sensitivity Adjust").arg(padButtonNumber + 1));
        padButtonLabel->setPixmap(padButtonHighlight);
        disconnect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), padEditPane, SLOT(slotFocusParam(QWidget*,QWidget*)));
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
        editPanes->setCurrentIndex(0);
        emit signalCurrentSensor(padButtonName);
        connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), padEditPane, SLOT(slotFocusParam(QWidget*,QWidget*)));
        presetHandler->slotConnectDisconnectDisplayAllLabels(true);
        focusConnected = true;
        padEditPane->slotUpdateXYText();
    }
    else {
        padButtonLabel->setPixmap(padButtonTransparent);
    }
}

void  PadButton::slotCheckToConnectFocus(QWidget* old, QWidget* now){
    if(now){
        if(!focusConnected){
            if(now->objectName().contains("pad") || now->objectName().contains("Gm") || now->objectName().contains("Dm") || now->objectName().contains("Grid")){
            connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), padEditPane, SLOT(slotFocusParam(QWidget*,QWidget*)));
            focusConnected = true;
 }}}
}
