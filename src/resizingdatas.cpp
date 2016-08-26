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

#include "resizingdatas.h"

ResizingDatas::ResizingDatas(bool b, int a, float m)
{
    activated = b;
    adder = a;
    multiplier = m;
    updateSuffix();
}

void ResizingDatas::setActivated(bool b)
{
    activated = b;
}

void ResizingDatas::setAdder(int a)
{
    adder = a;
}

void ResizingDatas::setMultiplier(float m)
{
    multiplier = m;
}

void ResizingDatas::setSizingDatas(bool b, int a, float m)
{
    activated = b;
    adder = a;
    multiplier = m;
    updateSuffix();
}

bool ResizingDatas::getActivated()
{
    return activated;
}

int ResizingDatas::getAdder()
{
    return adder;
}

float ResizingDatas::getMultiplier()
{
    return multiplier;
}

QString ResizingDatas::getSuffix()
{
    updateSuffix();
    return suffix;
}

QString ResizingDatas::getModifiedChannelName(int channelNumber)
{
    QString strChannelNumber;
    strChannelNumber.setNum(channelNumber);
    QString channelName = "Channel" + strChannelNumber; // in the language of Shakespeare
    //QString channelName = "Canal" + strChannelNumber; // in the language of Moliere
    //QString channelName = "Kanal" + strChannelNumber; // in the language of Goethe
    return getModifiedChannelName(channelName);
}


QString ResizingDatas::getModifiedChannelName(QString currentChannelName)
{
    updateSuffix();
    QString newChannelName;
    if (activated == true) newChannelName = '(';
    newChannelName = newChannelName + currentChannelName + suffix;
    return newChannelName;
}

float ResizingDatas::getModifiedValue(const float currentValue)
{
    float r = currentValue;
    if (activated == true)
    {
        r = (currentValue*multiplier) + adder;
    }
    return r;
}

void ResizingDatas::updateSuffix()
{
    if (! activated)
    {
        suffix = "";
    }
    else
    {
        QString strMultiplicator;
        strMultiplicator.setNum(multiplier, 'f', 1);  // to get a number XX.X
        QString strAdder;
        strAdder.setNum(adder);

        // this solution supposes to also add a '(' at the beggining of channel name
        suffix = " x " + strMultiplicator + ") + " + strAdder;
    }
}
