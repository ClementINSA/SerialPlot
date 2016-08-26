/*
  Copyright © 2016 Clément Chaduc
  This file is part of serialplot.

  serialplot is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  serialplot is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with serialplot.  If not, see <http://www.gnu.org/licenses/>.
*/

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
