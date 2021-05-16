#ifndef BUTTONBUTTON_H
#define BUTTONBUTTON_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QStackedWidget>
#include "leftrighteditpane.h"
#include "updowneditpane.h"
#include "rhombuseditpane.h"
#include "transporteditpane.h"
#include "presethandler.h"
#include "copypastehandler.h"

class ButtonButton : public QObject
{
    Q_OBJECT
public:
    explicit ButtonButton(PresetHandler *presetHandle, LeftrightEditPane *leftrightEditPane, UpdownEditPane *updownEditPane, RhombusEditPane *rhombusEditPane, TransportEditPane *transportEditPane, int, QWidget *widget, QObject *parent = 0);
    int buttonButtonNumber;
    QString buttonButtonName;
    QLabel *buttonButtonLabel;
    QPixmap buttonButtonTransparent;
    QPixmap buttonButtonHighlight;
    QStackedWidget* editPanes;

    LeftrightEditPane *leftrightEdit;
    UpdownEditPane *updownEdit;
    RhombusEditPane *rhombusEdit;
    TransportEditPane *transportEdit;

    bool focusConnected;
    QLabel *startMessage;
    PresetHandler *presetHandler;
    CopyPasteHandler *copyPasteHandle;

signals:

    void signalCurrentSensor(QString);

public slots:
    void slotEvents(QString);
    void slotCheckToConnectFocus(QWidget*, QWidget*);
    
};

#endif // BUTTONBUTTON_H
