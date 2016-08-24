#include "triggersetting.h"
#include "ui_triggersetting.h"

TriggerSetting::TriggerSetting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TriggerSetting)
{
    ui->setupUi(this);

    currentTriggerStatus = false;

    ui->rbEnableTrigger->setChecked(currentTriggerStatus);

    connect (ui->rbEnableTrigger, &QCheckBox::toggled, [this](bool checked)
        {
            ui->horizontalLayout->addWidget(triggerEnable.settingsWidget(),1);

            if (checked)
            {
                triggerEnable.setEnabled(true);
                triggerEnable.defaultTriggerChannel(1);
                triggerEnable.defaultTriggerLevel(0);
                triggerEnable.defaultTriggerType(true);
                triggerEnable.defaultTriggerPosition(50);

                triggerEnable.setCurrentNumberOfSamples(windowPlotSize);
            }

            else triggerEnable.setEnabled(false);
        });

    connect (ui->pbOk, SIGNAL (released()), this, SLOT (okButtonPressed()));

    connect (ui->pbCancel, SIGNAL (released()), this, SLOT (cancelButtonPressed()));

    connect (ui->pbApply, SIGNAL (released()), this, SLOT (applyButtonPressed()));

    connect (ui->pbUnpause, SIGNAL (released()), this, SLOT (unpauseButtonPressed()));
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
    currentTriggerPosition = triggerEnable.newTriggerPosition();
    emit TriggerHasBeenUpdated();
    this->hide();
}

void TriggerSetting::cancelButtonPressed()
{
    this->hide();
}

void TriggerSetting::applyButtonPressed()
{
    currentTriggerStatus = newTriggerStatus();
    currentTriggerLevel = triggerEnable.newTriggerLevel();
    currentTriggerChannel = triggerEnable.newTriggerChannel();
    currentTriggerType = triggerEnable.newTriggerType();
    currentTriggerPosition = triggerEnable.newTriggerPosition();
    emit TriggerHasBeenUpdated();
}

void TriggerSetting::unpauseButtonPressed()
{
    currentTriggerStatus = newTriggerStatus();
    currentTriggerLevel = triggerEnable.newTriggerLevel();
    currentTriggerChannel = triggerEnable.newTriggerChannel();
    currentTriggerType = triggerEnable.newTriggerType();
    currentTriggerPosition = triggerEnable.newTriggerPosition();
    emit TriggerHasBeenUpdated();
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

bool TriggerSetting::readTriggerType()
{
    return currentTriggerType;
}

int TriggerSetting::readTriggerPosition()
{
    return currentTriggerPosition;
}

void TriggerSetting::updateWindowSize(int newSize)
{
    windowPlotSize = newSize;
    triggerEnable.setCurrentNumberOfSamples(newSize);
}
