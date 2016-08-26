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

#include "triggerenable.h"
#include "ui_triggerenable.h"

// For debbug only
#include <iostream>


TriggerEnable::TriggerEnable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TriggerEnable)
{
    ui->setupUi(this);

    connect (ui->hsTriggerPosition, SIGNAL (valueChanged(int)), this, SLOT (onSliderModified(int)));
}

TriggerEnable::~TriggerEnable()
{
    delete ui;
}

QWidget* TriggerEnable::settingsWidget()
{
    return this;
}

void TriggerEnable::defaultTriggerChannel(int channel = 0)
{
    ui->sbTriggerChannel->setValue(channel);
}

void TriggerEnable::defaultTriggerLevel(int level = 0)
{
    ui->sbTriggerLevel->setValue(level);
}

void TriggerEnable::defaultTriggerType(bool type = true)
{
    int index = 0;
    if (type == false) index = 1;
    ui->cbTriggerType->setCurrentIndex(index);
}

void TriggerEnable::defaultTriggerPosition(int position = 50)
{
    ui->hsTriggerPosition->setValue(position);
}

int TriggerEnable::newTriggerChannel()
{
    return ui->sbTriggerChannel->value();
}

bool TriggerEnable::newTriggerType()
{
    bool b = true; // overpassing is prioritary
    if (ui->cbTriggerType->currentIndex() == 1) b = false;
    return b;
}

int TriggerEnable::newTriggerLevel()
{
    return ui->sbTriggerLevel->value();
}

int TriggerEnable::newTriggerPosition()
{
    return ui->hsTriggerPosition->sliderPosition();
}

void TriggerEnable::onSliderModified(int sliderPosition)
{
    int samplesBefore = (sliderPosition * currentNumberOfSamples)/100;
    int samplesAfter = ((100 - sliderPosition)*currentNumberOfSamples)/100;
    QString strSamplesBefore = QString::number(samplesBefore);
    QString strSamplesAfter = QString::number(samplesAfter);
    //QString myString = "Trigger Position (" + strSliderPosition + " sample";
    QString myString = "(";
    QString myString2;

    if (samplesBefore == 0) myString = myString + "no";
    else myString = myString + strSamplesBefore;

    myString = myString + " sample";
    if (samplesBefore > 1) myString = myString + 's';
    myString = myString + " before trigger, ";

    ui->labelTriggerPosition->setText(myString);

    if (samplesAfter == 0) myString2 = "no";
    else myString2 = strSamplesAfter;

    myString2 = myString2 + " sample";
    if (samplesAfter > 1) myString2 = myString2 + 's';
    myString2 = myString2 + " after";

    myString2 = myString2 + ')';
    ui->labelTriggerPosition_2->setText(myString2);
}

void TriggerEnable::setCurrentNumberOfSamples(int nb)
{
    currentNumberOfSamples = nb;
}
