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
    bool readTriggerType();
    int readTriggerPosition();

    void updateWindowSize(int);

    ~TriggerSetting();

private:
    Ui::TriggerSetting *ui;
    TriggerEnable triggerEnable;

    bool newTriggerStatus();

    bool currentTriggerStatus;
    int currentTriggerLevel;
    int currentTriggerChannel;
    bool currentTriggerType;
    int currentTriggerPosition;
    int windowPlotSize;

    void updateTriggerSettings();

signals:
    void TriggerHasBeenUpdated();


private slots:
    void okButtonPressed();
    void cancelButtonPressed();
    void applyButtonPressed();
    void unpauseButtonPressed();
};

#endif // TRIGGERSETTING_H
