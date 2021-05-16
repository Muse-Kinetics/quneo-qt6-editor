#ifndef PADBUTTON_H
#define PADBUTTON_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QStackedWidget>
#include "padeditpane.h"
#include "presethandler.h"
#include "copypastehandler.h"

class PadButton : public QObject
{
    Q_OBJECT
public:
    explicit PadButton(PresetHandler *presetHandle, PadEditPane *editPaneWidget, int, QWidget *widget, QObject *parent = 0);

    int padButtonNumber;
    QString padButtonName;
    QLabel *padButtonLabel;
    QPixmap padButtonTransparent;
    QPixmap padButtonHighlight;
    QStackedWidget* editPanes;
    QTabWidget *padTabPanes;
    PadEditPane *padEditPane;
    bool focusConnected;
    QLabel *startMessage;
    PresetHandler *presetHandler;
    QLabel *perPadSensitivityLabel;
    CopyPasteHandler *copyPasteHandle;

signals:

    void signalCurrentSensor(QString);

public slots:
    void slotEvents(QString);
    void slotCheckToConnectFocus(QWidget* old, QWidget* now);
    void slotTabChange(int);
};

#endif // PADBUTTON_H
