#include "globalparameters.h"
#include <QDebug>

GlobalParameters::GlobalParameters(QVariantMap *variantMap, QVariantMap *variantMapCopy, QWidget *widget, QWidget *parent) :
    QWidget(parent)
{
    this->setStyleSheet("QLabel { font: 10px } QLineEdit { font: 12px } QCheckBox {font:9px} QSpinBox {font: 10px} QDoubleSpinBox {font: 10px}");


    mainWindow = widget;
    presetMap = variantMap;
    presetMapCopy = variantMapCopy;
    currentPreset = 0;
    //findUiElements();

    presetName = mainWindow->findChild<QLineEdit *>("presetName");
    connect(presetName,SIGNAL(textEdited(QString)),this,SLOT(slotPresetNameChanged(QString)));

    presetNumMap = presetMapCopy->value(QString("Preset 0")).toMap();
    presetName->setText(QString(presetNumMap.value("presetName").toString()));

}

void GlobalParameters::slotPresetNameChanged(QString presetName){
    qDebug() << "slotPresetNameChanged called";
    emit signalPresetNameChanged(presetName);
}

void GlobalParameters::slotRecallPreset(int preset){
    currentPreset = preset;
    presetNumMap = presetMapCopy->value(QString("Preset %1").arg(currentPreset)).toMap();
    presetNumMap.value("presetName").toString();
    qDebug()<< presetNumMap.value("presetName").toString();
    presetName->setText(QString(presetNumMap.value("presetName").toString()));
}
