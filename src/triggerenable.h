#ifndef TRIGGERENABLE_H
#define TRIGGERENABLE_H

#include <QWidget>

namespace Ui {
class TriggerEnable;
}

class TriggerEnable : public QWidget
{
    Q_OBJECT

public:
    explicit TriggerEnable(QWidget *parent = 0);
    QWidget* settingsWidget();

    void defaultTriggerChannel(int channel);
    void defaultTriggerLevel(int level);
    void defaultTriggerType(bool type);

    int newTriggerChannel();
    int newTriggerLevel();
    bool newTriggerType(); // true = overpassing   false = underpassing

    ~TriggerEnable();

private:
    Ui::TriggerEnable *ui;
};

#endif // TRIGGERENABLE_H
