#ifndef GLOBALPARAMETERS_H
#define GLOBALPARAMETERS_H

#include <QWidget>
#include <QObject>
//#include <QtGui>
#include <QtWidgets> // was QT Gui

class GlobalParameters : public QWidget
{
    Q_OBJECT
public:
    explicit GlobalParameters(QVariantMap *variantMap, QVariantMap *variantMapCopy, QWidget *widget, QWidget *parent = 0);

    QWidget *mainWindow;
    QVariantMap *presetMap;
    QVariantMap *presetMapCopy;
    QLineEdit *presetName;
    QVariantMap presetNumMap;

    //void findUiElements();

    int currentPreset;
    
signals:
    void signalPresetNameChanged(QString);
    
public slots:
    void slotRecallPreset(int);
    void slotPresetNameChanged(QString);
    
};

#endif // GLOBALPARAMETERS_H
