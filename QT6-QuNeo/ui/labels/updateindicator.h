#ifndef UPDATEINDICATOR_H
#define UPDATEINDICATOR_H

#include <QObject>
#include <QWidget>
//#include <QtGui>
#include <QtWidgets> // was QT Gui

class UpdateIndicator : public QWidget
{
    Q_OBJECT
public:
    explicit UpdateIndicator(QWidget *widget, QVariantMap * presetMapOrig, QVariantMap * presetMapModified, QWidget *parent = 0);

    bool redBlack;
    bool alreadyOn;
    QVariantMap * presetOrig;
    QVariantMap * presetCopy;

    int origValue;
    int copyValue;

    QStringList ParamList;
    QStringList GlobalParamList;

    QWidget* mainWindow;
    QPushButton* indicatorButton;
    QTimer* buttonFlasher;

    bool presetModified[16];
    QComboBox *presetMenu;
    QPushButton *updateButton;
    int presetNum;

signals:
    void signalStartButtonFlasher();
    void signalStopButtonFlasher();

public slots:

    bool slotCompareMaps(int presetNumber);
    //slotCompareMaps is a function used in multiple places. returns true if current preset has unsaved changes, false if it has none.
    bool slotCheckForUnsavedPresets(); //checks all 16 presets and returns true if any are modified and unsaved.
    bool slotCheckThisPreset(int preset);//called when a preset is recalled. function calls compareMaps and then slotPresetModified.
    void slotPresetModified(bool);//controls graphics by starting or stopping qtimer, filters out repeat calls to turn on indicator.
    void slotFlasher();//called every 500ms by QTimer
    void slotStartButtonFlasher();
    void slotStopButtonFlasher();

};

#endif // UPDATEINDICATOR_H
