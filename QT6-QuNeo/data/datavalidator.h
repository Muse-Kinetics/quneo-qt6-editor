#ifndef DATAVALIDATOR_H
#define DATAVALIDATOR_H

#include <QWidget>
#include <QtWidgets> // was QT Gui
#include <QVariant>
#include <QFile>
#include <QDebug>
#include "qjson/src/parser.h"
#include "qjson/src/serializer.h"

class DataValidator : public QWidget
{
    Q_OBJECT
public:
    explicit DataValidator(QWidget *parent = 0);


    /*****JSON I/O*****/
    QFile *jsonFile;
    QByteArray jsonByteArray;
    QVariantMap jsonMasterMap;
    QVariantMap jsonQuNeoPresetsMap;

    QJson::Parser parser;
    QJson::Serializer serializer;
    bool ok;
    /******************/
    /******************/

    QVariantMap insertionMap[5]; //this is used insert nested maps in a more organized fashion-- pretty much just for readability
    QVariantMap deletionMap[5]; //this is used delete items from nested maps and repackage-- same as above, renamed for    "
    QVariantMap emptyMap; //this is used for embedded maps in the default map, like HSlider0, etc.

    /******************************* Default Maps *********************************/
    QVariantMap defaultMasterMap;

        QVariantMap defaultQuNeoPresetsMap;

            QVariantMap defaultSinglePresetMap;

                QVariantMap defaultComponentSettingsMap;

                    QVariantMap defaultHSlidersMap;
                        QVariantMap defaultHSlider0Map;
                        QVariantMap defaultHSlider1Map;
                        QVariantMap defaultHSlider2Map;
                        QVariantMap defaultHSlider3Map;

                    QVariantMap defaultLeftRightButtonsMap;
                        QVariantMap defaultLeftRightButton0Map;
                        QVariantMap defaultLeftRightButton1Map;
                        QVariantMap defaultLeftRightButton2Map;
                        QVariantMap defaultLeftRightButton3Map;

                    QVariantMap defaultLongSlidersMap;
                        QVariantMap defaultLongSliders0Map;

                    QVariantMap defaultModeButtonsMap;
                        QVariantMap defaultModeButton0Map;

                    QVariantMap defaultPadsMap;
                        QVariantMap defaultPad0Map;
                        QVariantMap defaultPad1Map;
                        QVariantMap defaultPad2Map;
                        QVariantMap defaultPad3Map;
                        QVariantMap defaultPad4Map;
                        QVariantMap defaultPad5Map;
                        QVariantMap defaultPad6Map;
                        QVariantMap defaultPad7Map;
                        QVariantMap defaultPad8Map;
                        QVariantMap defaultPad9Map;
                        QVariantMap defaultPad10Map;
                        QVariantMap defaultPad11Map;
                        QVariantMap defaultPad12Map;
                        QVariantMap defaultPad13Map;
                        QVariantMap defaultPad14Map;
                        QVariantMap defaultPad15Map;

                    QVariantMap defaultRhombusButtonsMap;
                        QVariantMap defaultRhombusButton0Map;

                    QVariantMap defaultRotariesMap;
                        QVariantMap defaultRotary0Map;
                        QVariantMap defaultRotary1Map;

                    QVariantMap defaultTransportButtonsMap;
                        QVariantMap defaultTransportButton0Map;
                        QVariantMap defaultTransportButton1Map;
                        QVariantMap defaultTransportButton2Map;

                    QVariantMap defaultUpDownButtonsMap;
                        QVariantMap defaultUpDownButton0Map;
                        QVariantMap defaultUpDownButton1Map;

                    QVariantMap defaultVSlidersMap;
                        QVariantMap defaultVSlider0Map;
                        QVariantMap defaultVSlider1Map;
                        QVariantMap defaultVSlider2Map;
                        QVariantMap defaultVSlider3Map;

    /************************************************************************/
    /************************************************************************/

QMap<QString, QVariant>::const_iterator mapIterator;
QMap<QString, QVariant>::const_iterator presetMapIterator;
QMap<QString, QVariant> tempDeletionMap;//map used to iterate through while in deletion process, otherwise iteration gets halted
    
signals:
    
public slots:

    //QuNeo JSON Validation
    void slotConstructDefultParamMap();
    void slotLoadJSON();
    void slotParseJSON();
    void slotCheckJSON();
    void slotSaveJSON();

    //Import Preset
    QVariantMap slotValidatePreset(QVariantMap);
    
};

#endif // DATAVALIDATOR_H
