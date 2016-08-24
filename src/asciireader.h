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

#ifndef ASCIIREADER_H
#define ASCIIREADER_H


#include "abstractreader.h"
#include "asciireadersettings.h"

class AsciiReader : public AbstractReader
{
    Q_OBJECT

public:
    explicit AsciiReader(QIODevice* device, ChannelManager* channelMan, QObject *parent = 0);
    QWidget* settingsWidget();
    unsigned numOfChannels();
    void enable(bool enabled = true);
    void setvaluesSeparator (QChar newSeparator);
    void setChannelsSequence (QString strChannelSequence);

    // to use trigger
    void setTriggerStatus(bool);
    void setTriggerLevel(int);
    void setTriggerChannel(int);
    void setTriggerType(bool);
    void setTriggerPosition(int);
    void setTriggerWindowSize(int);
    void setTriggerLauch (bool);

public slots:
    void pause(bool);

signals :
    void messagePrinting(QString);
    void triggerHasFinished();

private:
    AsciiReaderSettings _settingsWidget;
    unsigned _numOfChannels;
    /// number of channels will be determined from incoming data
    unsigned autoNumOfChannels;
    bool paused;

    // We may have (usually true) started reading in the middle of a
    // line, so its a better idea to just discard first line.
    bool discardFirstLine;

    QChar valuesSeparator;

    QVector<int> channelsSequence;
    QVector<int> setDefaultChannelsSequence(int size);

    QList<QByteArray> reOrderChannels(QList<QByteArray>, QVector<int>, int);

    void printAsciiMessages (QString);

    QString treatLine (QString line);

    // for debug only
    void printChannelsSequence (QVector<int>);

    // to use trigger
    bool triggerStatus;
    int triggerLevel;
    int triggerChannel;
    bool triggerType;
    int triggerPosition;
    int triggerWindowSize;

    bool triggerPreLauch;
    bool triggerLauch;
    int remainingSamples;

    // to use Regex
    bool regexActivated;
    QRegExp theRegexp;

    int checkUserChannel(int userChannel, int channelMax);

    bool cleanPlot();

private slots:
    void onDataReady();
    void onValuesSeparatorChanged(QChar);
    void onChannelsSequenceChanged(QString);
    void onMessagePrinting(QString);
    void onRegexChanged(QRegExp, bool);
};

#endif // ASCIIREADER_H
