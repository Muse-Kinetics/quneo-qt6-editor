#include "buttonbutton.h"
#include <QDebug>

ButtonButton::ButtonButton(PresetHandler *presetHandle, LeftrightEditPane *leftrightEditPane, UpdownEditPane *updownEditPane, RhombusEditPane *rhombusEditPane, TransportEditPane *transportEditPane, int num, QWidget *mainWindow, QObject *parent) :
    QObject(parent)
{
    presetHandler = presetHandle;
    leftrightEdit = leftrightEditPane;
    updownEdit = updownEditPane;
    rhombusEdit = rhombusEditPane;
    transportEdit = transportEditPane;

    focusConnected = false;
    buttonButtonNumber = num;
    buttonButtonName = QString("buttonButton%1").arg(buttonButtonNumber);
    buttonButtonLabel = mainWindow->findChild<QLabel *>(QString("buttonButton%1").arg(buttonButtonNumber));
    buttonButtonLabel->installEventFilter(mainWindow);

    if(num == 4 || num == 5 || num == 6 || num == 7){
        buttonButtonHighlight.load(QString(":Quneo/images/resources/images/highlightButton4.png"));
    } else if (num == 9 || num == 10){
        buttonButtonHighlight.load(QString(":Quneo/images/resources/images/highlightButton9.png"));
    } else {
        buttonButtonHighlight.load(QString(":Quneo/images/resources/images/highlightButton%1.png").arg(buttonButtonNumber));
    }

    buttonButtonTransparent.load(QString(":Quneo/images/resources/images/modeTransparent.png"));
    buttonButtonLabel->setPixmap(buttonButtonTransparent);

    connect(mainWindow, SIGNAL(signalEvents(QString)), this, SLOT(slotEvents(QString)));
    editPanes = mainWindow->findChild<QStackedWidget *>("EditPanes");
    startMessage = mainWindow->findChild<QLabel *>("startMessage");
    connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), this, SLOT(slotCheckToConnectFocus(QWidget*,QWidget*)));
}

void ButtonButton::slotEvents(QString string){

    if(string == buttonButtonName){
        buttonButtonLabel->setPixmap(buttonButtonHighlight);
        disconnect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), transportEdit, SLOT(slotFocusParam(QWidget*,QWidget*)));
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
        presetHandler->slotConnectDisconnectDisplayAllLabels(false);
        editPanes->setCurrentIndex(1);
        qDebug() << "emit signalCurrentSensor: " << buttonButtonName;
        emit signalCurrentSensor(buttonButtonName);
        connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), transportEdit, SLOT(slotFocusParam(QWidget*,QWidget*)));
        presetHandler->slotConnectDisconnectDisplayAllLabels(true);
        if(string == "buttonButton0"){//mode button
            if(!startMessage->isHidden()){
                startMessage->hide();
                // EB commented out to avoid crash
                //connect(presetHandler->saveButton, SIGNAL(clicked()), presetHandler, SLOT(slotSave()));
            }
            presetHandler->slotConnectDisconnectDisplayAllLabels(false);
            editPanes->setCurrentIndex(2);
            presetHandler->slotConnectDisconnectDisplayAllLabels(true);
        }
        if(string == "buttonButton4" || string == "buttonButton5" || string == "buttonButton6" || string == "buttonButton7"){
            disconnect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), leftrightEdit, SLOT(slotFocusParam(QWidget*,QWidget*)));
            if(!startMessage->isHidden()){
                startMessage->hide();
                // EB commented out to avoid crash
                //connect(presetHandler->saveButton, SIGNAL(clicked()), presetHandler, SLOT(slotSave()));
            }
            presetHandler->slotConnectDisconnectDisplayAllLabels(false);
            editPanes->setCurrentIndex(3);
            connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), leftrightEdit, SLOT(slotFocusParam(QWidget*,QWidget*)));
            presetHandler->slotConnectDisconnectDisplayAllLabels(true);
        }
        if(string == "buttonButton8"){
            disconnect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), rhombusEdit, SLOT(slotFocusParam(QWidget*,QWidget*)));
            if(!startMessage->isHidden()){
                startMessage->hide();
                // EB commented out to avoid crash
                //connect(presetHandler->saveButton, SIGNAL(clicked()), presetHandler, SLOT(slotSave()));
            }
            presetHandler->slotConnectDisconnectDisplayAllLabels(false);
            editPanes->setCurrentIndex(4);
            connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), rhombusEdit, SLOT(slotFocusParam(QWidget*,QWidget*)));
            presetHandler->slotConnectDisconnectDisplayAllLabels(true);
        }
        if(string == "buttonButton9" ||  string == "buttonButton10"){
            disconnect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), updownEdit, SLOT(slotFocusParam(QWidget*,QWidget*)));
            if(!startMessage->isHidden()){
                startMessage->hide();
                // EB commented out to avoid crash
                //connect(presetHandler->saveButton, SIGNAL(clicked()), presetHandler, SLOT(slotSave()));
            }
            presetHandler->slotConnectDisconnectDisplayAllLabels(false);
            editPanes->setCurrentIndex(5);
            connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), updownEdit, SLOT(slotFocusParam(QWidget*,QWidget*)));
            presetHandler->slotConnectDisconnectDisplayAllLabels(true);
        }
    }
    else {
        buttonButtonLabel->setPixmap(buttonButtonTransparent);
    }
}
void ButtonButton::slotCheckToConnectFocus(QWidget* old, QWidget* now){
    if(now){
        if(!focusConnected){
              if(now->objectName().contains("transport")){
              connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), transportEdit, SLOT(slotFocusParam(QWidget*,QWidget*)));}
              if(now->objectName().contains("updown")){
              connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), updownEdit, SLOT(slotFocusParam(QWidget*,QWidget*)));}
              if(now->objectName().contains("rhombus")){
              connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), rhombusEdit, SLOT(slotFocusParam(QWidget*,QWidget*)));}
              if(now->objectName().contains("leftright")){
              connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), leftrightEdit, SLOT(slotFocusParam(QWidget*,QWidget*)));}
              focusConnected = true;
 }}}


