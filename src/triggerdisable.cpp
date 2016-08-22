#include "triggerdisable.h"
#include "ui_triggerdisable.h"

TriggerDisable::TriggerDisable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TriggerDisable)
{
    ui->setupUi(this);
}

TriggerDisable::~TriggerDisable()
{
    delete ui;
}

QWidget* TriggerDisable::settingsWidget()
{
    return this;
}
