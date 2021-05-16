#ifndef SYSEXFORMAT_H
#define SYSEXFORMAT_H

#include <QObject>
#include <QApplication>
//#include <QtGui>
#include <QtWidgets> // was QT Gui
#include "presethandler.h"



enum {manufacturer_id1,manufacturer_id2,manufacturer_id3,manufacturer_id4,product,format, ID};

class SysExFormat : public QObject
{
    Q_OBJECT
public:
    explicit SysExFormat(QVariantMap *variantMap, QObject *parent = 0);

    QVariantMap* presetMapsCopyPointer;

    QByteArray presetSysExByteArray;
    QFile* presetSysEx[16];
    QFile* loadPresetSysEx[16];

    QStringList pad_str;
    QStringList rot_str;
    QStringList long_str;
    QStringList hslid_str;
    QStringList vslid_str;
    QStringList lrswitch_str;
    QStringList udswitch_str;
    QStringList rhswitch_str;
    QStringList tbutt_str;
    QStringList mswitch_str;
    QStringList globals_str;

    //******ENCODING******//
    unsigned char startPrestet;
    unsigned char endPreset;
    unsigned char sx_ident[ID];
    unsigned char sysex_start;
    unsigned char sysex_end;
    unsigned char sysex_encode_len;
    unsigned int crc;
    unsigned char midi_hi_bits;
    unsigned char midi_hi_count;
    unsigned int size;
    unsigned char SX_PACKET_START;
    unsigned int SX_ENCODE_LEN;
    unsigned int PRESET_LENGTH;
    unsigned char PRESET_START;
    unsigned int PRESET_LENGTH_MSB;
    unsigned int PRESET_LENGTH_LSB;

    unsigned int count;
    unsigned int sumByte;

    void sysex_out(unsigned char);
    void midi_buffer_put_core(unsigned char);
    void midi_chunk_init();
    void midi_sx_encode_char(unsigned char);
    void midi_sx_encode_int(unsigned int);
    void midi_sx_encode_crc_char(unsigned char);
    void midi_sx_encode_crc_int(unsigned int);
    void midi_sx_flush();
    void send_sx_stop();
    void crc_byte(unsigned char);
    void crc_init();
    void lineInit();
    void encode(QString, int);

    void sendPreset(unsigned int);

signals:
    void sigSysExByteArray(QByteArray*);

public slots:
    void slotEncodePreset(int);
    void slotWriteFile(int);

};

#endif // SYSEXFORMAT_H
