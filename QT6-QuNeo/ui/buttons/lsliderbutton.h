#ifndef LSLIDERBUTTON_H
#define LSLIDERBUTTON_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QStackedWidget>
#include "lslidereditpane.h"
#include "presethandler.h"
#include "copypastehandler.h"

class LSliderButton : public QObject
{
    Q_OBJECT
public:
    explicit LSliderButton(PresetHandler *presetHandle, LSliderEditPane *editPaneWidget, int, QWidget *widget, QObject *parent = 0);
    int lSliderButtonNumber;
    QString lSliderButtonName;
    QLabel *lSliderButtonLabel;
    QPixmap lSliderButtonTransparent;
    QPixmap lSliderButtonHighlight;
    QStackedWidget *editPanes;
    LSliderEditPane *lSliderEditPane;
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

#endif // LSLIDERBUTTON_H
