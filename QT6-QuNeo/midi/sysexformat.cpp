#include "sysexformat.h"
#include <QDebug>

SysExFormat::SysExFormat(QVariantMap* variantMap, QObject *parent) :
    QObject(parent)
{

    qDebug() << "Initialize SysExFormat";

    pad_str  <<
                "enableGrid"<<
                "padChannel"<<
                "outDmNote"<<
                "outDmNotePressMode"<<
                "outDmPress"<<
                "outDmPressValue"<<
                "outDmVelocityValue"<<
                "outDmXCC"<<
                "outDmXYReturn"<<
                "outDmXReturn"<<
                "outDmYCC"<<
                "outDmYReturn"<<
                "padSensitivityPerPad"<<


                "padChannel"<<
                "outGmNoteNW"<<
                "outGmNotePressModeNW"<<
                "outGmPressNW"<<
                "outGmPressValueNW"<<
                "outGmVelocityValueNW"<<


                "padChannel"<<
                "outGmNoteNE"<<
                "outGmNotePressModeNE"<<
                "outGmPressNE"<<
                "outGmPressValueNE"<<
                "outGmVelocityValueNE"<<


                "padChannel"<<
                "outGmNoteSE"<<
                "outGmNotePressModeSE"<<
                "outGmPressSE"<<
                "outGmPressValueSE"<<
                "outGmVelocityValueSE"<<


                "padChannel"<<
                "outGmNoteSW"<<
                "outGmNotePressModeSW"<<
                "outGmPressSW"<<
                "outGmPressValueSW"<<
                "outGmVelocityValueSW";

    rot_str
            <<
               "rB1Channel"<<
               "rB1outLocation"<<
               "rB1outNote"<<
               "rB1outNotePressMode"<<
               "rB1outPress"<<
               "rB1outPressValue"<<
               "rB1outVelocityValue"<<
               "rB1outLocPassThruRange"<<

               "rB1outDirection"<<
               "rB1outDirectionEnable"<<
               "rB1outSpeed"<<



               "rB2Channel"<<
               "rB2outLocation"<<
               "rB2outNote"<<
               "rB2outNotePressMode"<<
               "rB2outPress"<<
               "rB2outPressValue"<<
               "rB2outVelocityValue"<<
               "rB2outLocPassThruRange"<<

               "rB2outDirection"<<
               "rB2outDirectionEnable"<<
               "rB2outSpeed"<<



               "rB3Channel"<<
               "rB3outLocation"<<
               "rB3outNote"<<
               "rB3outNotePressMode"<<
               "rB3outPress"<<
               "rB3outPressValue"<<
               "rB3outVelocityValue"<<
               "rB3outLocPassThruRange"<<

               "rB3outDirection"<<
               "rB3outDirectionEnable"<<
               "rB3outSpeed"<<



               "rB4Channel"<<
               "rB4outLocation"<<
               "rB4outNote"<<
               "rB4outNotePressMode"<<
               "rB4outPress"<<
               "rB4outPressValue"<<
               "rB4outVelocityValue"<<
               "rB4outLocPassThruRange"<<

               "rB4outDirection"<<
               "rB4outDirectionEnable"<<
               "rB4outSpeed";




    long_str
            <<

               "lB1Channel"<<
               "lB1outLocation"<<
               "lB1outNote"<<
               "lB1outNotePressMode"<<
               "lB1outPress"<<
               "lB1outPressValue"<<
               "lB1outVelocityValue"<<
               "lB1outLocPassThruRange"<<

               "lB1outWidth"<<

               "lB2Channel"<<
               "lB2outLocation"<<
               "lB2outNote"<<
               "lB2outNotePressMode"<<
               "lB2outPress"<<
               "lB2outPressValue"<<
               "lB2outVelocityValue"<<
               "lB2outLocPassThruRange"<<

               "lB2outWidth"<<

               "lB3Channel"<<
               "lB3outLocation"<<
               "lB3outNote"<<
               "lB3outNotePressMode"<<
               "lB3outPress"<<
               "lB3outPressValue"<<
               "lB3outVelocityValue"<<
               "lB3outLocPassThruRange"<<

               "lB3outWidth"<<

               "lB4Channel"<<
               "lB4outLocation"<<
               "lB4outNote"<<
               "lB4outNotePressMode"<<
               "lB4outPress"<<
               "lB4outPressValue"<<
               "lB4outVelocityValue"<<
               "lB4outLocPassThruRange"<<

               "lB4outWidth";



    hslid_str
            <<
               "hB1Channel"<<
               "hB1outLocation"<<
               "hB1outNote"<<
               "hB1outNotePressMode"<<
               "hB1outPress"<<
               "hB1outPressValue"<<
               "hB1outVelocityValue"<<
               "hB1outLocPassThruRange"<<


               "hB2Channel"<<
               "hB2outLocation"<<
               "hB2outNote"<<
               "hB2outNotePressMode"<<
               "hB2outPress"<<
               "hB2outPressValue"<<
               "hB2outVelocityValue"<<
               "hB2outLocPassThruRange"<<


               "hB3Channel"<<
               "hB3outLocation"<<
               "hB3outNote"<<
               "hB3outNotePressMode"<<
               "hB3outPress"<<
               "hB3outPressValue"<<
               "hB3outVelocityValue"<<
               "hB3outLocPassThruRange"<<


               "hB4Channel"<<
               "hB4outLocation"<<
               "hB4outNote"<<
               "hB4outNotePressMode"<<
               "hB4outPress"<<
               "hB4outPressValue"<<
               "hB4outVelocityValue"<<
               "hB4outLocPassThruRange";





    vslid_str
            <<
               "vB1Channel"<<
               "vB1outLocation"<<
               "vB1outNote"<<
               "vB1outNotePressMode"<<
               "vB1outPress"<<
               "vB1outPressValue"<<
               "vB1outVelocityValue"<<
               "vB1outLocPassThruRange"<<


               "vB2Channel"<<
               "vB2outLocation"<<
               "vB2outNote"<<
               "vB2outNotePressMode"<<
               "vB2outPress"<<
               "vB2outPressValue"<<
               "vB2outVelocityValue"<<
               "vB2outLocPassThruRange"<<


               "vB3Channel"<<
               "vB3outLocation"<<
               "vB3outNote"<<
               "vB3outNotePressMode"<<
               "vB3outPress"<<
               "vB3outPressValue"<<
               "vB3outVelocityValue"<<
               "vB3outLocPassThruRange"<<


               "vB4Channel"<<
               "vB4outLocation"<<
               "vB4outNote"<<
               "vB4outNotePressMode"<<
               "vB4outPress"<<
               "vB4outPressValue"<<
               "vB4outVelocityValue"<<
               "vB4outLocPassThruRange";




    lrswitch_str
            <<
               "leftrightEnableSwitch"<<

               "leftrightChannel"<<
               "leftrightLOutNote"<<
               "leftrightLOutNotePressMode"<<
               "leftrightLOutPress"<<
               "leftrightLOutPressValue"<<
               "leftrightLOutVelocityValue"<<


               "leftrightChannel"<<
               "leftrightROutNote"<<
               "leftrightROutNotePressMode"<<
               "leftrightROutPress"<<
               "leftrightROutPressValue"<<
               "leftrightROutVelocityValue";







    udswitch_str
            <<
               "updownEnableSwitch"<<
               "updownBankControl"<<

               "updownChannel"<<
               "updownUOutNote"<<
               "updownUOutNotePressMode"<<
               "updownUOutPress"<<
               "updownUOutPressValue"<<
               "updownUOutVelocityValue"<<


               "updownChannel"<<
               "updownDOutNote"<<
               "updownDOutNotePressMode"<<
               "updownDOutPress"<<
               "updownDOutPressValue"<<
               "updownDOutVelocityValue";





    rhswitch_str
            <<
               "rhombusEnableSwitch"<<
               "rhombusBankControl"<<
               "rhombusInNoteG"<<
               "rhombusInNoteR"<<

               "rhombusChannel"<<
               "rhombusOutNote"<<
               "rhombusOutNotePressMode"<<
               "rhombusOutPress"<<
               "rhombusOutPressValue"<<
               "rhombusOutVelocityValue";





    tbutt_str
            <<
               "transportChannel"<<
               "transportOutNote"<<
               "transportOutNotePressMode"<<
               "transportOutPress"<<
               "transportOutPressValue"<<
               "transportOutVelocityValue";



    mswitch_str
            <<
               "modeOutVelocityValue"<<
               "modeEnableSwitch"<<
               "modeChannel"<<
               "modeOutNote"<<
               "modeOutPress";

    //set variant map to preset copy
    presetMapsCopyPointer =  variantMap;

    //set data and load file arrays to correct path - this is now done by forming
//    for(int i = 0; i < 16; i ++){
//        QString saveSyx = QString("../../../sysex/data/dataPreset%1.syx").arg(i);
//        QString loadSyx = QString("../../../sysex/loadPresets/loadPreset%1.syx").arg(i);

//        presetSysEx[i] = new QFile(saveSyx);
//        loadPresetSysEx[i] = new QFile(loadSyx);


//        if(presetSysEx[i]->open(QIODevice::ReadWrite)){
//            qDebug("presetFound");
//        } else {
//            qDebug() << "preset not found";
//        }

//        if(loadPresetSysEx[i]->open(QIODevice::ReadWrite)){
//            qDebug("presetFound");
//        } else {
//            qDebug() << "preset not found";
//        }

//        //presetSysEx[i]->open(QIODevice::ReadWrite);
//        qDebug() << "presetSysex[" << i << "] file: " << saveSyx << " debug: " << presetSysEx[i]->isReadable();
//        qDebug() << "loadPresetSysEx[" << i << "] file: " << loadSyx << "debug: " << loadPresetSysEx[i]->isReadable();

//    }

    //initialize encoding variables and sysex id
    size = 0;
    midi_hi_bits = 0;
    midi_hi_count = 0;
    crc = 0;

    SX_PACKET_START = 0x01;
    SX_ENCODE_LEN = 7;

    sx_ident[manufacturer_id1]  = 0x00;
    sx_ident[manufacturer_id2]  = 0x01;
    sx_ident[manufacturer_id3]  = 0x5F;
    sx_ident[manufacturer_id4]  = 0x7A;
    sx_ident[product]           = 0x1E;
    sx_ident[format]            = 0x00;

    sysex_start = 0xF0;
    sysex_end = 0xF7;
}

void SysExFormat::slotEncodePreset(int presetNum){

    qDebug() << "slotEncodePreset called - presetNum: " << presetNum;

    PRESET_LENGTH = 1279; //****************** Doublecheck the fly.
    PRESET_START = 0xA1;
    PRESET_LENGTH_MSB = (PRESET_LENGTH >> 8);
    PRESET_LENGTH_LSB = (PRESET_LENGTH & 0xFF);

    count = 0;
    sumByte = 0;
    presetSysExByteArray.clear();
    //presetSysEx[presetNum]->open(QIODevice::ReadWrite);
    //presetSysEx[presetNum]->resize(0);

    //begin sysex byte
    sysex_out(sysex_start);

    // identification bytes
    for(int i=0; i<ID; i++){
        sysex_out(sx_ident[i]);
    }

    //some zeros
    for(int i=0; i<26; i++){
        sysex_out(0);
    }

    //packet start signal byte
    sysex_out(SX_PACKET_START);

    //initialize crc and midi chunk
    crc_init();
    midi_chunk_init();

    //preamble type 2bytes
    midi_sx_encode_crc_char(0x00);
    midi_sx_encode_crc_char(0x02); // start of text

    //message id 2 bytes
    midi_sx_encode_crc_char(0x22); // load preset
    midi_sx_encode_crc_char(0x20);
    //crc
    midi_sx_encode_crc_int(crc);

    midi_sx_flush();
    sendPreset(presetNum);
    sysex_out(sysex_end);

    slotWriteFile(presetNum);
}

void SysExFormat::slotLoadPreset(int presetNum){

    qDebug() << "slotLoadPreset called - presetNum: " << presetNum;

    // reset the sysex encoding values and bytearray
    count = 0;
    sumByte = 0;
    presetSysExByteArray.clear();

    //begin sysex byte
    sysex_out(sysex_start);

    // identification bytes
    for(int i=0; i<ID; i++){
        sysex_out(sx_ident[i]);
    }

    //packet start signal byte
    sysex_out(SX_PACKET_START);

    //initialize crc and midi chunk
    crc_init();
    midi_chunk_init();

    //preamble type 2bytes
    midi_sx_encode_crc_char(0x00);
    midi_sx_encode_crc_char(0x02);

    //preamble length param 2 bytes
    midi_sx_encode_crc_char(0x30);      // load preset
    midi_sx_encode_crc_char(presetNum); // preset number

    //crc
    midi_sx_encode_crc_int(crc);        // encode the two bytes CRC

    midi_sx_flush();                    // finish 8bit encoding
    sysex_out(sysex_end);


}

void SysExFormat::slotWriteFile(int presetNum){

    qDebug() << "slotWriteFile - presetNum: " << presetNum;

    //presetSysEx[presetNum]->resize(0);
    //presetSysEx[presetNum]->write(presetSysExByteArray);
    //presetSysEx[presetNum]->close();

    //emit sigSysExByteArray(&presetSysExByteArray); //emit byte array to mididevice access to send to deivce
}


void SysExFormat::sendPreset(unsigned int presetNum){

    qDebug() << "sendPreset called - presetNum: " << presetNum;
    //reset global count
    count = 0;
    sumByte = 0;
    midi_sx_encode_char(PRESET_START);
    midi_sx_encode_char(presetNum);
    midi_sx_encode_char(PRESET_LENGTH_MSB);
    midi_sx_encode_char(PRESET_LENGTH_LSB);
    crc_init();

    //--------------------------------Component Iterations------------------------------------//
    //iterate through pads
    for(int j = 0; j<16; j++){
        for(int i = 0; i< pad_str.size(); i++){

            int a = presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
                    toMap().value("ComponentSettings").
                    toMap().value("Pads").
                    toMap().value(QString("Pad%1").arg(j)).
                    toMap().value(pad_str.at(i)).toInt();

            encode(pad_str.at(i), a);
        }
    }

    //rotaries
    for(int j = 0; j<2; j++){
        for(int i = 0; i< rot_str.size(); i++){

            int a = presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
                    toMap().value("ComponentSettings").
                    toMap().value("Rotaries").
                    toMap().value(QString("Rotary%1").arg(j)).
                    toMap().value(rot_str.at(i)).toInt();

            if (rot_str.at(i) == "rB1outSpeed" ||
                    rot_str.at(i) == "rB2outSpeed" ||
                    rot_str.at(i) == "rB2outSpeed" ||		//for rotary speed, break into msb and lsb
                    rot_str.at(i) == "rB3outSpeed" ||
                    rot_str.at(i) == "rB4outSpeed") {
                unsigned int lsb = a & 0xFF;
                unsigned int msb = a >> 8;
                //println("\t\tMSB = ", d2h(msb),"  LSB = ", d2h(lsb))
                encode(rot_str.at(i) + " -MSB", msb);
                encode(rot_str.at(i) + " -LSB", lsb);
            } else  {
                encode(rot_str.at(i), a);
            }
        }
    }

    //long slider
    for(int j = 0; j<1; j++){
        for(int i = 0; i< long_str.size(); i++){

            int a = presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
                    toMap().value("ComponentSettings").
                    toMap().value("LongSliders").
                    toMap().value(QString("LongSlider%1").arg(j)).
                    toMap().value(long_str.at(i)).toInt();

            encode(long_str.at(i), a);
        }
    }

    //h sliders
    for(int j = 0; j<4; j++){
        for(int i = 0; i< hslid_str.size(); i++){

            int a = presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
                    toMap().value("ComponentSettings").
                    toMap().value("HSliders").
                    toMap().value(QString("HSlider%1").arg(j)).
                    toMap().value(hslid_str.at(i)).toInt();

            encode(hslid_str.at(i), a);
        }
    }

    //v sliders
    for(int j = 0; j<4; j++){
        for(int i = 0; i< vslid_str.size(); i++){

            int a = presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
                    toMap().value("ComponentSettings").
                    toMap().value("VSliders").
                    toMap().value(QString("VSlider%1").arg(j)).
                    toMap().value(vslid_str.at(i)).toInt();

            encode(vslid_str.at(i), a);
        }
    }

    //lr buttons
    for(int j = 0; j<4; j++){
        for(int i = 0; i< lrswitch_str.size(); i++){

            int a = presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
                    toMap().value("ComponentSettings").
                    toMap().value("LeftRightButtons").
                    toMap().value(QString("LeftRightButton%1").arg(j)).
                    toMap().value(lrswitch_str.at(i)).toInt();

            encode(lrswitch_str.at(i), a);

        }
    }

    //ud buttons
    for(int j = 0; j<2; j++){
        for(int i = 0; i< udswitch_str.size(); i++){

            int a  = presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
                    toMap().value("ComponentSettings").
                    toMap().value("UpDownButtons").
                    toMap().value(QString("UpDownButton%1").arg(j)).
                    toMap().value(udswitch_str.at(i)).toInt();

            encode(udswitch_str.at(i), a);
        }
    }

    //rhom button
    for(int j = 0; j<1; j++){
        for(int i = 0; i< rhswitch_str.size(); i++){

            int a = presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
                    toMap().value("ComponentSettings").
                    toMap().value("RhombusButtons").
                    toMap().value(QString("RhombusButton%1").arg(j)).
                    toMap().value(rhswitch_str.at(i)).toInt();

            encode(rhswitch_str.at(i), a);

        }
    }

    //tansp button
    for(int j = 0; j<3; j++){
        for(int i = 0; i< tbutt_str.size(); i++){

            int a = presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
                    toMap().value("ComponentSettings").
                    toMap().value("TransportButtons").
                    toMap().value(QString("TransportButton%1").arg(j)).
                    toMap().value(tbutt_str.at(i)).toInt();

            encode(tbutt_str.at(i), a);
        }
    }

    //mode button
    for(int j = 0; j<1; j++){
        for(int i = 0; i< mswitch_str.size(); i++){

            int a  = presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
                    toMap().value("ComponentSettings").
                    toMap().value("ModeButtons").
                    toMap().value(QString("ModeButton%1").arg(j)).
                    toMap().value(mswitch_str.at(i)).toInt();

            encode(mswitch_str.at(i), a);

        }
    }

    //--------------------------------Globals Iterations------------------------------------//


    //*********************
    encode("padBankChangeMode",
           presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
           toMap().value("ComponentSettings").
           toMap().value("Pads").
           toMap().value("padBankChangeMode").
           toInt());

    //*********************
    encode("hSliderInChannel",
           presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
           toMap().value("ComponentSettings").
           toMap().value("HSliders").
           toMap().value("hSliderInChannel").
           toInt());

    //*********************
    encode("leftrightInChannel",
           presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
           toMap().value("ComponentSettings").
           toMap().value("LeftRightButtons").
           toMap().value("leftrightInChannel").
           toInt());

    //*********************
    encode("lSliderInChannel",
           presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
           toMap().value("ComponentSettings").
           toMap().value("LongSliders").
           toMap().value("lSliderInChannel").
           toInt());

    //*********************
    encode("padDrumInChannel",
           presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
           toMap().value("ComponentSettings").
           toMap().value("Pads").
           toMap().value("padDrumInChannel").
           toInt());

    //*********************
    encode("padGridDiscreteInChannel",
           presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
           toMap().value("ComponentSettings").
           toMap().value("Pads").
           toMap().value("padGridDiscreteInChannel").
           toInt());

    //*********************
    encode("padGridDualInChannel",
           presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
           toMap().value("ComponentSettings").
           toMap().value("Pads").
           toMap().value("padGridDualInChannel").
           toInt());

    //*********************
    encode("rhombusInChannel",
           presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
           toMap().value("ComponentSettings").
           toMap().value("RhombusButtons").
           toMap().value("rhombusInChannel").
           toInt());

    //*********************
    encode("rotaryInChannel",
           presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
           toMap().value("ComponentSettings").
           toMap().value("Rotaries").
           toMap().value("rotaryInChannel").
           toInt());

    //*********************
    encode("transportInChannel",
           presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
           toMap().value("ComponentSettings").
           toMap().value("TransportButtons").
           toMap().value("transportInChannel").
           toInt());

    //*********************
    encode("updownInChannel",
           presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
           toMap().value("ComponentSettings").
           toMap().value("UpDownButtons").
           toMap().value("updownInChannel").
           toInt());

    //*********************
    encode("vSliderInChannel",
           presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
           toMap().value("ComponentSettings").
           toMap().value("VSliders").
           toMap().value("vSliderInChannel").
           toInt());

    //*********************
    encode("bank1TransposeInterval",
           presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
           toMap().value("ComponentSettings").
           toMap().value("Pads").
           toMap().value("bank1TransposeInterval").
           toInt());

    //*********************
    encode("bank2TransposeInterval",
           presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
           toMap().value("ComponentSettings").
           toMap().value("Pads").
           toMap().value("bank2TransposeInterval").
           toInt());

    //*********************
    encode("bank3TransposeInterval",
           presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
           toMap().value("ComponentSettings").
           toMap().value("Pads").
           toMap().value("bank3TransposeInterval").
           toInt());

    //*********************
    encode("bank4TransposeInterval",
           presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
           toMap().value("ComponentSettings").
           toMap().value("Pads").
           toMap().value("bank4TransposeInterval").
           toInt());

    //*********************
    encode("vSlidOffThreshold",
           presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
           toMap().value("ComponentSettings").
           toMap().value("VSliders").
           toMap().value("vSliderOffThreshold").
           toInt());


    //*********************
    encode("vSlidOnThreshold",
           presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
           toMap().value("ComponentSettings").
           toMap().value("VSliders").
           toMap().value("vSliderOnThreshold").
           toInt());


    //*********************
    encode("udOffThreshold",
           presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
           toMap().value("ComponentSettings").
           toMap().value("UpDownButtons").
           toMap().value("updownOffThreshold").
           toInt());

    //*********************
    encode("udOnThreshold",
           presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
           toMap().value("ComponentSettings").
           toMap().value("UpDownButtons").
           toMap().value("updownOnThreshold").
           toInt());

    //*********************
    encode("transOffThreshold",
           presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
           toMap().value("ComponentSettings").
           toMap().value("TransportButtons").
           toMap().value("transportOffThreshold").
           toInt());

    //*********************
    encode("transOnThreshold",
           presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
           toMap().value("ComponentSettings").
           toMap().value("TransportButtons").
           toMap().value("transportOnThreshold").
           toInt());


    //*********************
    encode("rotOffThreshold",
           presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
           toMap().value("ComponentSettings").
           toMap().value("Rotaries").
           toMap().value("rotaryOffThreshold").
           toInt());


    //*********************
    encode("rotOnThreshold",
           presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
           toMap().value("ComponentSettings").
           toMap().value("Rotaries").
           toMap().value("rotaryOnThreshold").
           toInt());

    //*********************
    encode("rhOffThreshold",
           presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
           toMap().value("ComponentSettings").
           toMap().value("RhombusButtons").
           toMap().value("rhombusOffThreshold").
           toInt());

    //*********************
    encode("rhOnThreshold",
           presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
           toMap().value("ComponentSettings").
           toMap().value("RhombusButtons").
           toMap().value("rhombusOnThreshold").
           toInt());

    //*********************
    encode("modeOffThreshold",
           presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
           toMap().value("ComponentSettings").
           toMap().value("ModeButtons").
           toMap().value("modeOffThreshold").
           toInt());

    //*********************
    encode("modeOnThreshold",
           presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
           toMap().value("ComponentSettings").
           toMap().value("ModeButtons").
           toMap().value("modeOnThreshold").
           toInt());

    //*********************
    encode("lSlidOffThreshold",
           presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
           toMap().value("ComponentSettings").
           toMap().value("LongSliders").
           toMap().value("lSliderOffThreshold").
           toInt());

    //*********************
    encode("lSlidOnThreshold",
           presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
           toMap().value("ComponentSettings").
           toMap().value("LongSliders").
           toMap().value("lSliderOnThreshold").
           toInt());

    //*********************
    encode("lrOffThreshold",
           presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
           toMap().value("ComponentSettings").
           toMap().value("LeftRightButtons").
           toMap().value("leftrightOffThreshold").
           toInt());

    //*********************
    encode("lrOnThreshold",
           presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
           toMap().value("ComponentSettings").
           toMap().value("LeftRightButtons").
           toMap().value("leftrightOnThreshold").
           toInt());

    //*********************
    encode("hSliderOffThreshold",
           presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
           toMap().value("ComponentSettings").
           toMap().value("HSliders").
           toMap().value("hSliderOffThreshold").
           toInt());

    //*********************
    encode("hSliderOnThreshold",
           presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
           toMap().value("ComponentSettings").
           toMap().value("HSliders").
           toMap().value("hSliderOnThreshold").
           toInt());


    //*********************
    encode("padOffset",
           presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
           toMap().value("ComponentSettings").
           toMap().value("Pads").
           toMap().value("padOffset").
           toInt());

    //*********************
    encode("padOffThreshold",
           presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
           toMap().value("ComponentSettings").
           toMap().value("Pads").
           toMap().value("padOffThreshold").
           toInt());

    //*********************
    encode("padOnThreshold",
           presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
           toMap().value("ComponentSettings").
           toMap().value("Pads").
           toMap().value("padOnThreshold").
           toInt());

    //*********************
    encode("cornerIsolation",
           presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
           toMap().value("ComponentSettings").
           toMap().value("Pads").
           toMap().value("cornerIsolation").
           toInt());

    //*********************
    encode("padSensitivity",
           presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
           toMap().value("ComponentSettings").
           toMap().value("Pads").
           toMap().value("padSensitivity").
           toInt());

    //*********************
    encode("rotarySensitivity",
           presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
           toMap().value("ComponentSettings").
           toMap().value("Rotaries").
           toMap().value("rotarySensitivity").
           toInt());

    //*********************
    encode("lSliderSensitivity",
           presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
           toMap().value("ComponentSettings").
           toMap().value("LongSliders").
           toMap().value("lSliderSensitivity").
           toInt());

    //*********************
    encode("hSliderSensitivity",
           presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
           toMap().value("ComponentSettings").
           toMap().value("HSliders").
           toMap().value("hSliderSensitivity").
           toInt());

    //*********************
    encode("vSliderSensitivity",
           presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
           toMap().value("ComponentSettings").
           toMap().value("VSliders").
           toMap().value("vSliderSensitivity").
           toInt());

    //*********************
    encode("leftrightSensitivity", presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
           toMap().value("ComponentSettings").
           toMap().value("LeftRightButtons").
           toMap().value("leftrightSensitivity").
           toInt());

    //*********************
    encode("updownSensitivity", presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
           toMap().value("ComponentSettings").
           toMap().value("UpDownButtons").
           toMap().value("updownSensitivity").
           toInt());

    //*********************
    encode("rhombusSensitivity", presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
           toMap().value("ComponentSettings").
           toMap().value("RhombusButtons").
           toMap().value("rhombusSensitivity").
           toInt());

    //*********************
    encode("transportSensitivity", presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
           toMap().value("ComponentSettings").
           toMap().value("TransportButtons").
           toMap().value("transportSensitivity").
           toInt());

    //*********************
    encode("modeSensitivity", presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
           toMap().value("ComponentSettings").
           toMap().value("ModeButtons").
           toMap().value("modeSensitivity").
           toInt());





    //*********************
    encode("padLocalLEDControl", presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
           toMap().value("ComponentSettings").
           toMap().value("Pads").
           toMap().value("localLEDControl").
           toInt());

    //*********************
    encode("rotaryLocalLEDControl", presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
           toMap().value("ComponentSettings").
           toMap().value("Rotaries").
           toMap().value("rotaryLocalLEDControl").
           toInt());

    //*********************
    encode("lSliderLocalLEDControl", presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
           toMap().value("ComponentSettings").
           toMap().value("LongSliders").
           toMap().value("lSliderLocalLEDControl").
           toInt());

    //*********************
    encode("hSliderLocalLEDControl", presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
           toMap().value("ComponentSettings").
           toMap().value("HSliders").
           toMap().value("hSliderLocalLEDControl").
           toInt());

    //*********************
    encode("vSliderLocalLEDControl", presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
           toMap().value("ComponentSettings").
           toMap().value("VSliders").
           toMap().value("vSliderLocalLEDControl").
           toInt());

    //*********************
    encode("leftrightLocalLEDControl", presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
           toMap().value("ComponentSettings").
           toMap().value("LeftRightButtons").
           toMap().value("leftrightLocalLEDControl").
           toInt());

    //*********************
    encode("updownLocalLEDControl", presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
           toMap().value("ComponentSettings").
           toMap().value("UpDownButtons").
           toMap().value("updownLocalLEDControl").
           toInt());

    //*********************
    encode("rhombusLocalLEDControl", presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
           toMap().value("ComponentSettings").
           toMap().value("RhombusButtons").
           toMap().value("rhombusLocalLEDControl").
           toInt());

    //*********************
    encode("transportLocalLEDControl", presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
           toMap().value("ComponentSettings").
           toMap().value("TransportButtons").
           toMap().value("transportLocalLEDControl").
           toInt());

    //*********************
    encode("modeLocalLEDControl", presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
           toMap().value("ComponentSettings").
           toMap().value("ModeButtons").
           toMap().value("modeLocalLEDControl").
           toInt());


    for(int i=0; i<128; i++){

        encode("velocity - ", presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
               toMap().value("ComponentSettings").
               toMap().value("Pads").
               toMap().value("padVelocityTable").toList().at(i).toInt());

        /* qDebug() << "table val" << i << presetMapsCopyPointer->value(QString("Preset %1").arg(presetNum)).
                   toMap().value("ComponentSettings").
                   toMap().value("Pads").
                   toMap().value("padVelocityTable").toList().at(i).toInt();*/
    }


    midi_sx_encode_char(0);
    midi_sx_encode_char(0);
    midi_sx_encode_char(sumByte);

    midi_sx_flush();
}

//******************************ENCODING Functions******************************//
void SysExFormat::sysex_out(unsigned char val){

    //qDebug() << "valllll" << val;
    presetSysExByteArray.append(val);
}

void SysExFormat::midi_buffer_put_core(unsigned char val){
    //print(d2h(write_char) + "  \t");
    sysex_out(val);
    //size++;
}

void SysExFormat::midi_chunk_init(){
    midi_hi_bits = midi_hi_count = 0;
}

//byte encoding
void SysExFormat::midi_sx_encode_char(unsigned char val){

    midi_hi_bits |= (val & 0x80);
    midi_hi_bits >>= 1;
    midi_buffer_put_core(val & 0x7f);
    if (++midi_hi_count == SX_ENCODE_LEN) {
        midi_hi_count = 0;
        midi_buffer_put_core(midi_hi_bits);
        //	print('*');

    }

}

void SysExFormat::midi_sx_encode_int(unsigned int val){
    midi_sx_encode_char(val>>8);
    midi_sx_encode_char(val);
}

//crc encoding
void SysExFormat::midi_sx_encode_crc_char(unsigned char val){
    crc_byte(val);
    midi_sx_encode_char(val);
}

void SysExFormat::midi_sx_encode_crc_int(unsigned int val){

    midi_sx_encode_crc_char(val>>8);
    midi_sx_encode_crc_char(val & 0xFF);
}

void SysExFormat::midi_sx_flush(){

    while(midi_hi_count){
        midi_sx_encode_crc_char(0);
    }
}

void SysExFormat::send_sx_stop(){

}

void SysExFormat::crc_byte(unsigned char val){

    unsigned int temp = 0;
    unsigned int quick = 0;
    //if we represent crc at start as 0xHHLL
    temp = (crc >> 8) ^ val;    //xor 8 bit val with upper byte of crc (0x00HH ^ val) = 0x00XX
    crc &= 0xFFFF;
    crc <<= 8;				    // left shift crc now 0xLL00
    crc &= 0xFFFF;
    quick = ((temp ^ (temp >> 4)) & 0xFFFF);	//0x00XX ^ 0x000X = 0x00XY
    crc ^= quick;  				// 0xLL00 ^ 0x00XY = 0xLLXY
    crc &= 0xFFFF;				//effect of all this is to preserve the information in
    //LSB (LL) intact, while mixing the new data and the old MSB thoroughly

    quick <<= 5;				//hash	(0x00XY << 5) = 0xNNN0	 (quick * 2 to the fifth)
    quick &= 0xFFFF;
    crc ^= quick;				//hash
    crc &= 0xFFFF;
    quick <<= 7;				//hash	(0xNNN0 << 7) = 0xN000
    quick &= 0xFFFF;			//	 (quick * 2 to the seventh)
    crc ^= quick; 				//hash
    crc &= 0xFFFF;
}

void SysExFormat::crc_init(){
    crc = 0xffff;
}

void SysExFormat::lineInit(){

}

void SysExFormat::encode(QString name, int b){

    char a = b;
    midi_sx_encode_crc_char(a);

    if (a < 0)			//write text file for use in explicitly initializing variables
    { a += 256; }

    /*if (typeof(eeprom) != 'undefined')
            {	f.writeByte(a); }
            if (typeof(out_text) != 'undefined')
            {	out_text.writeln(a, ',  \t\t//',name);}*/

    //print('   count - ', count++);
    sumByte += a;
    sumByte &= 0xFF;
}


