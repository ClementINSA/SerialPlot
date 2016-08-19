/*
  Copyright © 2016 Hasan Yavuz Özderya

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

#include <QtDebug>
#include <iostream>

#include "asciireader.h"

/// If set to this value number of channels is determined from input
#define NUMOFCHANNELS_AUTO   (0)

AsciiReader::AsciiReader(QIODevice* device, ChannelManager* channelMan, QObject *parent) :
    AbstractReader(device, channelMan, parent)
{
    paused = false;
    discardFirstLine = true;
    sampleCount = 0;
    valuesSeparator = DEFAULT_VALUES_SEPARATOR;

    _numOfChannels = _settingsWidget.numOfChannels();
    autoNumOfChannels = (_numOfChannels == NUMOFCHANNELS_AUTO);

    connect(&_settingsWidget, &AsciiReaderSettings::numOfChannelsChanged,
            [this](unsigned value)
            {
                _numOfChannels = value;
                autoNumOfChannels = (_numOfChannels == NUMOFCHANNELS_AUTO);
                if (!autoNumOfChannels)
                {
                    emit numOfChannelsChanged(value);
                }
            });

    connect(device, &QIODevice::aboutToClose, [this](){discardFirstLine=true;});

    connect(&_settingsWidget, &AsciiReaderSettings::valuesSeparatorChanged,
            this, &AsciiReader::onValuesSeparatorChanged);



}

QWidget* AsciiReader::settingsWidget()
{
    return &_settingsWidget;
}

unsigned AsciiReader::numOfChannels()
{
    // do not allow '0'
    if (_numOfChannels == 0)
    {
        return 1;
    }
    else
    {
        return _numOfChannels;
    }
}

// TODO: this could be a part of AbstractReader
void AsciiReader::enable(bool enabled)
{
    if (enabled)
    {
        discardFirstLine = true;
        QObject::connect(_device, &QIODevice::readyRead,
                         this, &AsciiReader::onDataReady);
    }
    else
    {
        QObject::disconnect(_device, 0, this, 0);
    }
}

void AsciiReader::pause(bool enabled)
{
    paused = enabled;
}


// CLEMENT
// Permits to set the datas separator
void AsciiReader::setvaluesSeparator(QChar newSeparator)
{
    valuesSeparator = newSeparator;
}


void AsciiReader::onDataReady()
{



    while(_device->canReadLine())
    {
        QByteArray line = _device->readLine();

        onMessagePrinting(line.constData());
        std::cerr << (line.constData()) << std::endl;


        // discard only once when we just started reading
        if (discardFirstLine)
        {
            discardFirstLine = false;
            continue;
        }

        // discard data if paused
        if (paused)
        {
            continue;
        }

        // parse data
        line = line.trimmed();
        // CLEMENT
        // NOTE : the line above does not remove internal whitespaces and must be completed
        // following line permits to remove internal whitespaces
        if (valuesSeparator != ' ') // do not remove whitespaces if valueSeparator is a whitespace
        {
            line = line.replace(" ", "");
        }

        // Note: When data coming from pseudo terminal is buffered by
        // system CR is converted to LF for some reason. This causes
        // empty lines in the input when the port is just opened.
        if (line.isEmpty())
        {
            continue;
        }

        // CLEMENT
        // Data separation with the valuesSeparator content (and not a default char)
        //auto separatedValues = line.split(',');
        auto separatedValues = line.split(valuesSeparator.toLatin1());

        unsigned numReadChannels; // effective number of channels to read
        unsigned numComingChannels = separatedValues.length();

        if (autoNumOfChannels)
        {
            // did number of channels changed?
            if (numComingChannels != _numOfChannels)
            {
                _numOfChannels = numComingChannels;
                emit numOfChannelsChanged(numComingChannels);
            }
            numReadChannels = numComingChannels;
        }
        else if (numComingChannels >= _numOfChannels)
        {
            numReadChannels = _numOfChannels;
        }
        else // there is missing channel data
        {
            numReadChannels = separatedValues.length();
            qWarning() << "Incoming data is missing data for some channels!";
        }

        // parse read line
        for (unsigned ci = 0; ci < numReadChannels; ci++)
        {
            bool ok;
            double channelSample = separatedValues[ci].toDouble(&ok);
            if (ok)
            {
                _channelMan->addChannelData(ci, &channelSample, 1);
                sampleCount++;
            }
            else
            {
                qWarning() << "Data parsing error for channel: " << ci;
            }
        }
        emit dataAdded();
    }
}

void AsciiReader::onValuesSeparatorChanged(QChar newSeparator)
{
    valuesSeparator = newSeparator;
}

void AsciiReader::onMessagePrinting(QString Message)
{
    emit messagePrinting(Message);
}
