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

#include "utils.h"

#include "asciireadersettings.h"
#include "ui_asciireadersettings.h"

//debug only
#include <iostream>


AsciiReaderSettings::AsciiReaderSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AsciiReaderSettings)
{
    ui->setupUi(this);

    putChannelsInDataResizerSelector(1);

    previousIndex = ui->cbChannels->currentIndex();

    // Note: if directly connected we get a runtime warning on incompatible signal arguments
    connect(ui->spNumOfChannels, SELECT<int>::OVERLOAD_OF(&QSpinBox::valueChanged),
            [this](int value)
            {
                emit numOfChannelsChanged(value);
            });

    connect(ui->spValuesSeparator, &QLineEdit::textChanged, this, &AsciiReaderSettings::onValuesSeparatorEdited);

    connect(ui->spChannelsSequence, &QLineEdit::textChanged, this, &AsciiReaderSettings::onChannelsSequenceEdited);

    connect(ui->spRegex, &QLineEdit::textChanged, this, &AsciiReaderSettings::onRegexEdited);

    connect(ui->cbRegexChecked, SIGNAL (toggled(bool)), this, SLOT (onRegexChecked(bool)));

    connect(ui->cbChannels, SIGNAL (currentIndexChanged(int)), this, SLOT (onDataResizingChannelChanged(int)));

    connect(ui->cbEnableResizing, &QCheckBox::toggled, this, &AsciiReaderSettings::onDataResizingChanged); // not sure to be good
    connect(ui->sbAdder, &QSpinBox::editingFinished, this, &AsciiReaderSettings::onDataResizingChanged);
    connect(ui->sbMultiplicator, &QSpinBox::editingFinished, this, &AsciiReaderSettings::onDataResizingChanged);

}

AsciiReaderSettings::~AsciiReaderSettings()
{
    delete ui;
}

unsigned AsciiReaderSettings::numOfChannels()
{
    return ui->spNumOfChannels->value();
}

void AsciiReaderSettings::onValuesSeparatorEdited()
{
    emit valuesSeparatorChanged(syncValuesSeparator());
}

void AsciiReaderSettings::onChannelsSequenceEdited()
{
    emit channelsSequenceChanged(syncChannelsSequence());
}

void AsciiReaderSettings::onRegexEdited()
{
    if(ui->cbRegexChecked->isChecked())
    {
        emit regexChanged(syncRegex(), true);
    }
    else
    {
        QRegExp null;
        emit regexChanged(null, false);
    }
}

void AsciiReaderSettings::onRegexChecked(bool)
{
    if(ui->cbRegexChecked->isChecked())
    {
        emit regexChanged(syncRegex(), true);
    }
    else
    {
        QRegExp null;
        emit regexChanged(null, false);
    }
}

void AsciiReaderSettings::onDataResizingChanged()
{
    int c = ui->cbChannels->currentIndex();
    bool b = ui->cbEnableResizing->isChecked();
    int a = ui->sbAdder->value();
    float m = ui->sbMultiplicator->value();

    emit dataResizingChanged(c, b, a, m);
}

void AsciiReaderSettings::onDataResizingChannelChanged(int channel)
{
    // send current datas
    int c = previousIndex;
    bool b = ui->cbEnableResizing->isChecked();
    int a = ui->sbAdder->value();
    float m = ui->sbMultiplicator->value();

    emit dataResizingChanged(c, b, a, m);

    // to keep in mind where we are
    previousIndex = channel;

    // restaure datas inrelationship with this channel (as datas are in AsciiReader)
    emit dataResizingCurrentChannelChanged(channel);

}

QChar AsciiReaderSettings::syncValuesSeparator()
{
    // You must avoid to read an empty line
    QChar mychar;
    if (ui->spValuesSeparator->text().length() != 0)
    {
        mychar = ui->spValuesSeparator->text().at(0);
    }
    else
    {
        mychar = DEFAULT_VALUES_SEPARATOR;
    }
    return mychar;
}

QString AsciiReaderSettings::syncChannelsSequence()
{
    return ui->spChannelsSequence->text();
}

QRegExp AsciiReaderSettings::syncRegex()
{
    QRegExp myRegexp(ui->spRegex->text());
    return myRegexp;
}

void AsciiReaderSettings::printAsciiMessages(QString msg)
{
    if (ui != NULL) ui->ptAsciiMessages->appendPlainText(msg);
}

void AsciiReaderSettings::putChannelsInDataResizerSelector(int numberOfChannelsToPrint)
{
    int currentNumberOfChannelsPrinted = ui->cbChannels->count();

    if (currentNumberOfChannelsPrinted == numberOfChannelsToPrint)
    {
        // do nothing : all is right
    }
    else
    {
        if (currentNumberOfChannelsPrinted < numberOfChannelsToPrint)
        {
            // print some more
            for (int i = currentNumberOfChannelsPrinted; i < numberOfChannelsToPrint; i++)
            {
                QString string;
                string.setNum((i+1));
                string = "Channel" + string;
                ui->cbChannels->insertItem(i, string, i);
            }
        }
        else
        {
            // stop printing some
            for (int i = currentNumberOfChannelsPrinted; i > numberOfChannelsToPrint; i--)
            {
                ui->cbChannels->removeItem(i-1); // carefult, last index is currentNumberOfChannelsPrinted - 1
            }
        }
    }

}

void AsciiReaderSettings::putDatasResizingSettingsInIHM(bool b, int a, float m)
{
    ui->cbEnableResizing->setChecked(b);
    ui->sbAdder->setValue(a);
    ui->sbMultiplicator->setValue(m);
}
