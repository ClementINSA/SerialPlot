#ifndef TRIGGERDISABLE_H
#define TRIGGERDISABLE_H

#include <QWidget>


/*
 *  In this new code version, triggerdisable is now useless.
 *  However, I did not find the good way to remove all the
 *  files in relation with it.
 */




namespace Ui {
class TriggerDisable;
}

class TriggerDisable : public QWidget
{
    Q_OBJECT

public:
    explicit TriggerDisable(QWidget *parent = 0);
    QWidget* settingsWidget();
    ~TriggerDisable();

private:
    Ui::TriggerDisable *ui;
};

#endif // TRIGGERDISABLE_H
