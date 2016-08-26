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

#ifndef RESIZINGDATAS_H
#define RESIZINGDATAS_H

#include <QWidget>

class ResizingDatas
{
public:
    ResizingDatas(bool b=false, int a=0, float m=1);

    void setActivated(bool b=true);
    void setAdder(int a=0);
    void setMultiplier(float m=1);

    void setSizingDatas(bool b=false, int a=0, float m=1);


    bool getActivated();
    int getAdder();
    float getMultiplier();
    QString getSuffix();

    QString getModifiedChannelName(QString currentChannelName);
    QString getModifiedChannelName(int channelNumber);

    float getModifiedValue(const float currentValue);


private:
    bool activated;
    int adder;
    float multiplier;
    QString suffix;

    void updateSuffix ();
};

#endif // RESIZINGDATAS_H
