#ifndef ROTARYBUTTON_H
#define ROTARYBUTTON_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QStackedWidget>
#include "rotaryeditpane.h"
#include "presethandler.h"
#include "copypastehandler.h"

class RotaryButton : public QObject
{
    Q_OBJECT
public:
    explicit RotaryButton(PresetHandler *presetHandle, RotaryEditPane *editPaneWidget, int, QWidget *widget, QObject *parent = 0);
    int rotaryButtonNumber;
    QString rotaryButtonName;
    QLabel *rotaryButtonLabel;
    QPixmap rotaryButtonTransparent;
    QPixmap rotaryButtonHighlight;
    QStackedWidget *editPanes;
    RotaryEditPane *rotaryEditPane;
    bool focusConnected;
    QLabel *startMessage;
    PresetHandler *presetHandler;
    CopyPasteHandler *copyPasteHandle;

signals:

    void signalCurrentSensor(QString);

public slots:
    void slotEvents(QString);
    void slotCheckToConnectFocus(QWidget* old, QWidget* now);

};

#endif // ROTARYBUTTON_H
