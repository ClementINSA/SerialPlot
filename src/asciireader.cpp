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
    channelsSequence.resize(INITIALNUMOFCHANNELS);
    channelsSequence = setDefaultChannelsSequence(channelsSequence.length());

    remainingSamples = 100;

    triggerLauch = false;
    triggerLauch = false;

    regexActivated = false;
    theRegexp;

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

    connect(&_settingsWidget, &AsciiReaderSettings::regexChanged, this, &AsciiReader::onRegexChanged);

    // for datas resizing
    connect(&_settingsWidget, &AsciiReaderSettings::dataResizingChanged, this, &AsciiReader::onDataResizingSettingsChanged);
    connect(&_settingsWidget, &AsciiReaderSettings::dataResizingCurrentChannelChanged, this, &AsciiReader::onDataResizingChannelChanged);
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
        //std::cerr << (line.constData()) << std::endl;


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

        // Note: When data coming from pseudo terminal is buffered by
        // system CR is converted to LF for some reason. This causes
        // empty lines in the input when the port is just opened.
        if (line.isEmpty())
        {
            continue;
        }

        // discard line if not matching with regex
        if (regexActivated == true && theRegexp.indexIn(line.constData()) == -1)
        {
            continue;
        }

        // removing unwanted characters
        line = treatLine(line).toLatin1();

        // log printing
        onMessagePrinting(line.constData());

        // datas separation
        auto separatedValues = line.split(valuesSeparator.toLatin1());

        // channels counting
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

        // updating data Resizer selector
        _settingsWidget.putChannelsInDataResizerSelector(numReadChannels);

        // parsing read line
        separatedValues = reOrderChannels(separatedValues, channelsSequence, numReadChannels);

        // applying datas resizing
        separatedValues = reSizeChannels(separatedValues, dataResizerSettings);

        // ploting datas
        for (int ci = 0; ci < numReadChannels; ci++)
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
                // To get a better plotting, we print a 0 if a data is not correct
                channelSample = 0;
                qWarning() << "Data parsing error for channel: " << ci;
                _channelMan->addChannelData(ci, &channelSample, 1);
                sampleCount++;
            }
        }
        emit dataAdded();

        // counting remaining samples to plot
        if (triggerStatus == true && triggerLauch == true)
        {
            remainingSamples --;
        }
        else
        {
            remainingSamples = 100;
        }


        // checking lauching conditions of trigger
        if (triggerLauch == false && triggerStatus == true)
        {
            // 2 conditions : "prelauching" condition and lauching condition
            // for Overpassing : trigger must first go under the start condition and then go over
            // for Underpassing : trigger must first go over the start condition and then go under
            bool ok;
            int channelToWatch = checkUserChannel(triggerChannel, numReadChannels);
            double sample = separatedValues[channelToWatch].toDouble(&ok);  // TODO : user can chose channel : we have to check his choice !!!!
            if (ok)
            {
                if (triggerPreLauch == false)
                {
                    if ((triggerType == true && sample < triggerLevel) || (triggerType == false && sample > triggerLevel))
                    {
                        triggerPreLauch = true;
                    }
                }
                else
                {
                    if ((triggerType == true && sample > triggerLevel) || (triggerType == false && sample < triggerLevel))
                    {
                        triggerLauch = true;
                        remainingSamples = (triggerWindowSize - (triggerPosition*triggerWindowSize)/100);
                        triggerPreLauch = false;
                    }
                }
            }
        }

        // stoping plotting if all expected samples have been plotted
        if (remainingSamples<1)
        {
            // STOP !!!!
            triggerLauch = false;
            paused = true;
            pause(true);
            emit triggerHasFinished();
            remainingSamples = 100;
        }
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
        channelsSequence = setDefaultChannelsSequence(channelsSequence.length());
    }
    else
    {
        line = treatLine(line);
        // It can be interesting to remove also all non numeric characters (except ',')
        line = line.remove((QRegExp(QString::fromUtf8("[-`~!@#$%^&*()_—+=|:;<>«».?/{}\'\"\\\[\\\]\\\\]"))));
        auto separatedValues = line.split(',');

        if (separatedValues.length() > channelsSequence.length())
        {
            channelsSequence.resize(separatedValues.length());
        }

        for (int i = 0; i<separatedValues.length(); i++)
        {
            channelsSequence[i]=separatedValues[i].toInt();
        }
        for (int i = separatedValues.length(); i<channelsSequence.length(); i++)
        {
            channelsSequence[i]= 0;
        }


    }
    // for debug only
    printChannelsSequence(channelsSequence);
}

void AsciiReader::onRegexChanged(QRegExp newRegex, bool newEnable)
{
    if (newEnable == false)
    {
        regexActivated = false;
        // in this case it is no use to change theRegexp
    }
    else
    {
        regexActivated = true;
        theRegexp = newRegex;
    }
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

void AsciiReader::setTriggerStatus(bool newTriggerStatus)
{
    triggerStatus = newTriggerStatus;
}

void AsciiReader::setTriggerLevel(int newTriggerLevel)
{
    triggerLevel = newTriggerLevel;
}

void AsciiReader::setTriggerChannel(int newTriggerChannel)
{
    triggerChannel = newTriggerChannel;
}

void AsciiReader::setTriggerType(bool newTriggerType)
{
    triggerType = newTriggerType;
}

void AsciiReader::setTriggerPosition(int position)
{
    triggerPosition = position;
}

void AsciiReader::setTriggerWindowSize(int windowSize)
{
    triggerWindowSize = windowSize;
}

void AsciiReader::setTriggerLauch(bool lauch)
{
    triggerLauch = lauch;
    triggerPreLauch = lauch;
}

bool AsciiReader::cleanPlot()
{
    double channelSample = 0;
    for (int i = 0; i <_channelMan->numOfSamples(); i++)
    {
        for (int j = 0; j < _numOfChannels; j++)
        {
            _channelMan->addChannelData(j, &channelSample, 1);
        }
    }
    emit dataAdded();
    return true;
}

QList<QByteArray> AsciiReader::reOrderChannels(QList<QByteArray> currentSequenceChannels, QVector<int> newSequenceExpected, int numReadChannels)
{
    // The purpose fo this function is to change the content of normalSequenceChannels to
    // match with the newSequenceExpected by the user
    // If the given sequence can be reached, some small changes will be made.
    QList<QByteArray> reOrderredChannels = currentSequenceChannels;

    for (int ci = 0; ci < numReadChannels; ci++)
    {
        int iUser;
        if (newSequenceExpected.length() < numReadChannels)
        {
            // It's better to avoid to try to reach a non existing number
            iUser = 0;
        }
        else
        {
            iUser = newSequenceExpected[ci];
        }

        if (iUser > 0 && iUser <= numReadChannels)
        {
            // users count from 1, computers from 0
            reOrderredChannels[ci] = currentSequenceChannels[iUser-1];
        }
        else
        {
            reOrderredChannels[ci] = 0;
        }
    }
    return reOrderredChannels;
}

QList<QByteArray> AsciiReader::reSizeChannels (QList<QByteArray>currentSamples, QMap<int,ResizingDatas> resizingSettings)
{
    QList<QByteArray> resizedSamples = currentSamples;
    for (int i = 0; i < currentSamples.length(); i++)
    {
        if (resizingSettings.count(i) == 1)
        {
            resizedSamples[i].setNum(resizingSettings[i].getModifiedValue(resizedSamples[i].toDouble()), 'f',1);
        }
    }
    return resizedSamples;
}

int AsciiReader::checkUserChannel(int userChannel, int channelMax)
{
    // If user channel is reachable, give back the user channel (shift by 1 cause user count from 1)
    // If not give back 0 (whatever happens, channel 0 exists)
    int newChannel;
    if (userChannel > 0 && userChannel < channelMax) newChannel = userChannel - 1;
    else newChannel = 0;
    return newChannel;
}

void AsciiReader::onDataResizingSettingsChanged(int channel, bool activated, int adder, float multiplier)
{
    if (channel >= 0)
    {
        if (dataResizerSettings.count(channel) == 1)
        {
            // this channel has already been set : just update it
            dataResizerSettings[channel].setSizingDatas(activated, adder, multiplier);
        }
        else
        {
            // this channel has never been set : create it
            ResizingDatas newDatas (activated, adder, multiplier);
            dataResizerSettings.insert(channel, newDatas);
        }
    }
}

void AsciiReader::onDataResizingChannelChanged(int channel)
{
    _settingsWidget.putDatasResizingSettingsInIHM( dataResizerSettings[channel].getActivated(),
                                                   dataResizerSettings[channel].getAdder(),
                                                   dataResizerSettings[channel].getMultiplier());
}
