#ifndef VSLIDERBUTTON_H
#define VSLIDERBUTTON_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QStackedWidget>
#include "vslidereditpane.h"
#include "presethandler.h"
#include "copypastehandler.h"

class VSliderButton : public QObject
{
    Q_OBJECT
public:
    explicit VSliderButton(PresetHandler *presetHandle, VSliderEditPane *editPaneWidget, int, QWidget *widget, QObject *parent = 0);
    int vSliderButtonNumber;
    QString vSliderButtonName;
    QLabel *vSliderButtonLabel;
    QPixmap vSliderButtonTransparent;
    QPixmap vSliderButtonHighlight;
    QStackedWidget *editPanes;
    VSliderEditPane *vSliderEditPane;
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

#endif // VSLIDERBUTTON_H
