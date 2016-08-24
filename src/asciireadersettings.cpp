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

AsciiReaderSettings::AsciiReaderSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AsciiReaderSettings)
{
    ui->setupUi(this);

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
