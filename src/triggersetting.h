#ifndef TRIGGERSETTING_H
#define TRIGGERSETTING_H

#include "triggerenable.h"
#include <QWidget>
#include <iostream>

namespace Ui {
class TriggerSetting;
}

class TriggerSetting : public QWidget
{
    Q_OBJECT

public:
    explicit TriggerSetting(QWidget *parent = 0);

    bool readTriggerStatus();
    int readTriggerLevel();
    int readTriggerChannel();
    bool readcurrentTriggerType();

    ~TriggerSetting();

private:
    Ui::TriggerSetting *ui;
    TriggerEnable triggerEnable;

    bool newTriggerStatus();

    bool currentTriggerStatus;
    int currentTriggerLevel;
    int currentTriggerChannel;
    bool currentTriggerType;

    void updateTriggerSettings();

signals:
    void TriggerHasBeenUpdated();


private slots:
    void okButtonPressed();
    void cancelButtonPressed();
};

#endif // TRIGGERSETTING_H
