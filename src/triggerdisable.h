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

#ifndef TRIGGERDISABLE_H
#define TRIGGERDISABLE_H

#include <QWidget>


/*
 *  In this new code version, triggerdisable is now useless.
 *  However, I did not find the good way to remove all the
 *  files in relation with it.
 */




namespace Ui {
class TriggerDisable;
}

class TriggerDisable : public QWidget
{
    Q_OBJECT

public:
    explicit TriggerDisable(QWidget *parent = 0);
    QWidget* settingsWidget();
    ~TriggerDisable();

private:
    Ui::TriggerDisable *ui;
};

#endif // TRIGGERDISABLE_H
