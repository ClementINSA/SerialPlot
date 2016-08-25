#ifndef RESIZINGDATAS_H
#define RESIZINGDATAS_H

#include <QWidget>

class ResizingDatas
{
public:
    ResizingDatas(bool b=false, int a=0, float m=1);

    void setActivated(bool b=true);
    void setAdder(int a=0);
    void setMultiplier(float m=1);

    void setSizingDatas(bool b=false, int a=0, float m=1);


    bool getActivated();
    int getAdder();
    float getMultiplier();
    QString getSuffix();

    QString getModifiedChannelName(QString currentChannelName);
    QString getModifiedChannelName(int channelNumber);

    float getModifiedValue(const float currentValue);


private:
    bool activated;
    int adder;
    float multiplier;
    QString suffix;

    void updateSuffix ();
};

#endif // RESIZINGDATAS_H
