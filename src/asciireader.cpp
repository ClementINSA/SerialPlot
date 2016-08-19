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
#define INITIALNUMOFCHANNELS     (5)

AsciiReader::AsciiReader(QIODevice* device, ChannelManager* channelMan, QObject *parent) :
    AbstractReader(device, channelMan, parent)
{
    paused = false;
    discardFirstLine = true;
    sampleCount = 0;
    valuesSeparator = DEFAULT_VALUES_SEPARATOR;
    ChannelsSequence.resize(INITIALNUMOFCHANNELS);
    ChannelsSequence = setDefaultChannelsSequence(ChannelsSequence.length());

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

    connect(&_settingsWidget, &AsciiReaderSettings::channelsSequenceChanged,
            this, &AsciiReader::onChannelsSequenceChanged);

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

        printAsciiMessages(line.constData());
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

        line = treatLine(line).toLatin1();

        onMessagePrinting(line.constData());

        // Note: When data coming from pseudo terminal is buffered by
        // system CR is converted to LF for some reason. This causes
        // empty lines in the input when the port is just opened.
        if (line.isEmpty())
        {
            continue;
        }

        // Data separation with the valuesSeparator content (and not a default char)
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
        // parsing lines in normal sequence
        /*
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
        */

        // parsing lines following user sequence's
        // TODO : this part can be improved !
        for (unsigned ci = 0; ci < numReadChannels; ci++)
        {
            int iUser;
            if (ChannelsSequence.length() < numReadChannels)
            {
                // It's better to avoid to try to reach a non existing number
                iUser = 0;
            }
            else
            {
                iUser = ChannelsSequence[ci];
            }
            if (iUser > 0 && iUser <= numReadChannels)
            {
                bool ok;
                double channelSample = separatedValues[iUser-1].toDouble(&ok);
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
            else
            {
                double channelSample = 0;
                _channelMan->addChannelData(ci, &channelSample, 1);
                sampleCount++;
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

void AsciiReader::printAsciiMessages (QString msg)
{
    _settingsWidget.printAsciiMessages(msg);
}

void AsciiReader::onChannelsSequenceChanged(QString line)
{
    if (line.isEmpty())
    {
        ChannelsSequence = setDefaultChannelsSequence(ChannelsSequence.length());
    }
    else
    {
        line = treatLine(line);
        // It can be interesting to remove also all non numeric characters (except ',')
        line = line.remove((QRegExp(QString::fromUtf8("[-`~!@#$%^&*()_—+=|:;<>«».?/{}\'\"\\\[\\\]\\\\]"))));
        auto separatedValues = line.split(',');

        if (separatedValues.length() > ChannelsSequence.length())
        {
            ChannelsSequence.resize(separatedValues.length());
        }

        for (int i = 0; i<separatedValues.length(); i++)
        {
            ChannelsSequence[i]=separatedValues[i].toInt();
        }
        for (int i = separatedValues.length(); i<ChannelsSequence.length(); i++)
        {
            ChannelsSequence[i]= 0;
        }


    }
    // for debug only
    printChannelsSequence(ChannelsSequence);
}

QVector<int> AsciiReader::setDefaultChannelsSequence(int size)
{
    QVector<int> mySequence(size);
    // By default all channels must be printed in the "normal" sequence
    for (int i = 0; i < size; i++)
    {
        mySequence[i] = i+1;
    }
    return mySequence;
}

// This function is used mainly for debbuging but can also be used to give more information to user
void AsciiReader::printChannelsSequence(QVector<int> oneVector)
{
    printAsciiMessages("Channels Sequence has been set to : ");
    QString str;
    for (int i = 0; i < oneVector.size(); ++i)
    {
        if (i >= 0)
        {
            str += " ";
            str += QString::number(oneVector[i]);
        }
    }
    printAsciiMessages(str);
}

// This function permits to remove from the line given
// -> EOL
// -> whitespaces
// -> non numeric characters
QString AsciiReader::treatLine(QString line)
{
    // parse data
    line = line.trimmed();

    // NOTE : the line above does not remove internal whitespaces and must be completed
    // following line permits to remove internal whitespaces
    if (valuesSeparator != ' ') // do not remove whitespaces if valueSeparator is a whitespace
    {
        line = line.replace(" ", "");
    }

    // Removing from the line all the alphabetics caracters
    char a = 97;
    char A = 65;
    for (int i = 0; i < 26; i++)
    {
        line = line.replace((char) a, "");
        line = line.replace((char) A, "");
        a = a+1;
        A = A+1;
    }

    return line;
}
