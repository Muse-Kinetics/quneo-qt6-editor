#ifndef HSLIDERBUTTON_H
#define HSLIDERBUTTON_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QStackedWidget>
#include "hslidereditpane.h"
#include "presethandler.h"
#include "copypastehandler.h"

class HSliderButton : public QObject
{
    Q_OBJECT
public:
    explicit HSliderButton(PresetHandler *presetHandle, HSliderEditPane *editPaneWidget, int, QWidget *widget, QObject *parent = 0);
    HSliderEditPane *hSliderEditPane;
    int hSliderButtonNumber;
    QString hSliderButtonName;
    QLabel *hSliderButtonLabel;
    QPixmap hSliderButtonTransparent;
    QPixmap hSliderButtonHighlight;
    QStackedWidget* editPanes;
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

#endif // HSLIDERBUTTON_H
