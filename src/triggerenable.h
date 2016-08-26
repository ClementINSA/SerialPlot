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

#ifndef TRIGGERENABLE_H
#define TRIGGERENABLE_H

#include <QWidget>

namespace Ui {
class TriggerEnable;
}

class TriggerEnable : public QWidget
{
    Q_OBJECT

public:
    explicit TriggerEnable(QWidget *parent = 0);
    QWidget* settingsWidget();

    void defaultTriggerChannel(int channel);
    void defaultTriggerLevel(int level);
    void defaultTriggerType(bool type);
    void defaultTriggerPosition(int position);

    int newTriggerChannel();
    int newTriggerLevel();
    bool newTriggerType(); // true = overpassing   false = underpassing
    int newTriggerPosition();

    void setCurrentNumberOfSamples (int);

    ~TriggerEnable();

private:
    Ui::TriggerEnable *ui;

    int currentNumberOfSamples;

private slots:
    void onSliderModified(int sliderPosition);
};

#endif // TRIGGERENABLE_H
