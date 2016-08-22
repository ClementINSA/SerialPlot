#include "triggerenable.h"
#include "ui_triggerenable.h"

TriggerEnable::TriggerEnable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TriggerEnable)
{
    ui->setupUi(this);
}

TriggerEnable::~TriggerEnable()
{
    delete ui;
}

QWidget* TriggerEnable::settingsWidget()
{
    return this;
}

void TriggerEnable::defaultTriggerChannel(int channel = 0)
{
    ui->sbTriggerChannel->setValue(channel);
}

void TriggerEnable::defaultTriggerLevel(int level = 0)
{
    ui->sbTriggerLevel->setValue(level);
}

void TriggerEnable::defaultTriggerType(bool type = true)
{
    int index = 0;
    if (type == false) index = 1;
    ui->cbTriggerType->setCurrentIndex(index);
}

int TriggerEnable::newTriggerChannel()
{
    return ui->sbTriggerChannel->value();
}

bool TriggerEnable::newTriggerType()
{
    bool b = true; // overpassing is prioritary
    if (ui->cbTriggerType->currentIndex() == 1) b = false;
    return b;
}

int TriggerEnable::newTriggerLevel()
{
    return ui->sbTriggerLevel->value();
}
