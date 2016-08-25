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

#ifndef ASCIIREADERSETTINGS_H
#define ASCIIREADERSETTINGS_H

#include <QWidget>
#define DEFAULT_VALUES_SEPARATOR ','

namespace Ui {
class AsciiReaderSettings;
}

class AsciiReaderSettings : public QWidget
{
    Q_OBJECT

public:
    explicit AsciiReaderSettings(QWidget *parent = 0);
    ~AsciiReaderSettings();

    unsigned numOfChannels();
    QChar syncValuesSeparator();
    QString syncChannelsSequence();
    QRegExp syncRegex();

    void printAsciiMessages(QString);

    void putChannelsInDataResizerSelector(int numberOfChannelsToPrint);
    void putDatasResizingSettingsInIHM(bool b, int a, float m);

signals:
    void numOfChannelsChanged(unsigned);
    void channelsSequenceChanged(QString);
    void valuesSeparatorChanged(QChar);
    void regexChanged(QRegExp, bool);
    void dataResizingChanged(int, bool, int, float);
    void dataResizingCurrentChannelChanged(int);

private:
    Ui::AsciiReaderSettings *ui;
    int previousIndex;

private slots:
    void onValuesSeparatorEdited();
    void onChannelsSequenceEdited();
    void onRegexEdited();
    void onRegexChecked(bool);
    void onDataResizingChanged();
    void onDataResizingChannelChanged(int channel);
};

#endif // ASCIIREADERSETTINGS_H
