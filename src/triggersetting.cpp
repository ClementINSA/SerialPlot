#include "triggersetting.h"
#include "ui_triggersetting.h"

TriggerSetting::TriggerSetting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TriggerSetting)
{
    ui->setupUi(this);

    ui->rbEnableTrigger->setChecked(currentTriggerStatus);

    connect (ui->rbEnableTrigger, &QCheckBox::toggled, [this](bool checked)
        {
            ui->horizontalLayout->addWidget(triggerEnable.settingsWidget(),1);

            if (checked)
            {
                triggerEnable.setEnabled(true);
                triggerEnable.defaultTriggerChannel(currentTriggerChannel);
                triggerEnable.defaultTriggerLevel(currentTriggerLevel);
                triggerEnable.defaultTriggerType(currentTriggerType);
            }

            else triggerEnable.setEnabled(false);
        });

    connect (ui->pbOk, SIGNAL (released()), this, SLOT (okButtonPressed()));

    connect (ui->pbCancel, SIGNAL (released()), this, SLOT (cancelButtonPressed()));

}

TriggerSetting::~TriggerSetting()
{
    delete ui;
}

bool TriggerSetting::newTriggerStatus()
{
   bool b;
   if (ui != NULL) b = ui->rbEnableTrigger->isChecked();
   else b = false;
   return b;
}

void TriggerSetting::okButtonPressed()
{
    currentTriggerStatus = newTriggerStatus();
    currentTriggerLevel = triggerEnable.newTriggerLevel();
    currentTriggerChannel = triggerEnable.newTriggerChannel();
    currentTriggerType = triggerEnable.newTriggerType();
    emit TriggerHasBeenUpdated();
    this->hide();
}

void TriggerSetting::cancelButtonPressed()
{
    this->hide();
}

bool TriggerSetting::readTriggerStatus()
{
    return currentTriggerStatus;
}

int TriggerSetting::readTriggerLevel()
{
    return currentTriggerLevel;
}

int TriggerSetting::readTriggerChannel()
{
    return currentTriggerChannel;
}

bool TriggerSetting::readcurrentTriggerType()
{
    return currentTriggerType;
}

