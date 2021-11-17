#ifndef PADEDITPANE_H
#define PADEDITPANE_H

#include <QObject>
#include <QWidget>
//#include <QtGui>
#include <QtWidgets> // was QT Gui
#include "padlabels.h"

class PadEditPane : public QWidget
{
    Q_OBJECT
public:
    explicit PadEditPane(QVariantMap* variantMap, QVariantMap *variantMapCopy, QWidget *mainWindowAddress, QWidget *parent = 0);
    bool eventFilter(QObject *, QEvent *);

    QWidget *mainWindow;
    QVariantMap *presetMap;
    QVariantMap *presetMapCopy;
    QWidget *focusCheck;
    QWidget *clicked;
    QString prevObjectToLabel;

    void findUiElements();
    void displayPadValues();

    QLabel *paneLabel;
    QDial *sensitivityDial;
    QCheckBox *enableGridMode;
    QCheckBox *localLEDControl;

    int currentPreset;
    int currentPad;

    QVariantMap padMap;
    QStringList padParamList;//list of parameter names
    QStringList padUiBlackList;
    QMap<QString, QSpinBox*> spinBoxMap; //map of spin box address by param name in padParamList
    QMap<QString, QComboBox*> comboBoxMap;

    QString NoteNumberToName(int noteNumber);

    PadLabels *padLabel[16];

    //pointers for velocity check boxes
    QCheckBox *padDmVelocity;
    QCheckBox *padNWVelocity;
    QCheckBox *padNEVelocity;
    QCheckBox *padSEVelocity;
    QCheckBox *padSWVelocity;
    QTabWidget *padTabs;
    QSpinBox* cornerIsolation;

    //----Velocity Tables----//
    QMap<QString, QList<QVariant> > velocityTables;
    QList<QVariant> velTableNone;
    QList<QVariant> velTableLight;
    QList<QVariant> velTableMedium;
    QList<QVariant> velTableDynamic;
    QList<QVariant> velTableHard;
    QList<QVariant> velTableFull;
    //not used anymore. left it in case we want to experiment with new tables from colls later.
    QList<QVariant> velTableVariable;
    QStringList newTableValues;
    QList<QVariant> velTableVariableConverter;

    QStringList tableIDs;
    QComboBox *velTableMenu;
    QString VelTableID;

    //----Pad Sensitivity & Other Globals----//
    float globalPadSensitivity;
    float thisPadSensitivity;
    int calculatedSensitivityPerPad;
    QSpinBox *perPadSensitivityUI;
    QSpinBox *onThresh;
    QSpinBox *offThresh;
    QSpinBox *padOffset;
    QSpinBox *xSource;
    QSpinBox *ySource;
    int valXCC;
    int valYCC;
    QString justClicked;

    //------ LED Input Channels------//
    QSpinBox *ledDrumInChannel;
    QSpinBox *ledGridDiscreteInChannel;
    QSpinBox *ledGridDualInChannel;


    //-----Note Mode Menus-----//
    QComboBox *modeMenuDm;
    QComboBox *modeMenuNW;
    QComboBox *modeMenuNE;
    QComboBox *modeMenuSW;
    QComboBox *modeMenuSE;

    //------ Bank Transpose Spin Boxes ------///
    QSpinBox *backTransposeSpinBoxes[4];

signals:
    void signalValueChanged(int presetNum, QString settingsType, QString paramType, int typeNum, QString paramName, int paramValue);
    void signalToLabels(QString);
    void signalTableChanged(int presetNum, QString tableName, QList<QVariant> tableValue);
    void signalSelectPreset(QString);
    void signalTriggerCopy();
    void signalTriggerPaste();
    void signalCurrentSensor(QString);

public slots:
    void slotSelectVelocityTable(QString);
    void slotEvents(QString);
    void slotRecallPreset(int);
    void slotValueChanged(int);
    void slotEnableGridMode(bool);
    void slotToLabels(QString);
    void slotFocusParam(QWidget* oldObject, QWidget* newObject);
    void slotCalculatePadSensitivity(int);
    void slotCalculateOffThreshFromOn(int onThreshChanged);
    void slotCheckOffThreshAgainstOn(int offThreshChanged);
    //not used anymore. left it in case we want to experiment with new tables from colls later.
    void slotLoadVariableTable(QString);
    void slotChangeXText(int);
    void slotChangeYText(int);
    void slotUpdateXYText();

    //drum styles
    void slotPadSensitivityDefault();
    void slotPadSensitivityLow();
    void slotPadSensitivityMedium();
    void slotPadSensitivityHigh();
    void slotPadSensitivityExtreme();
};

#endif // PADEDITPANE_H
