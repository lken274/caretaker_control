#ifndef BATTERYBOX_H
#define BATTERYBOX_H
#include "vitalsbox.h"

class BatteryBox : public VitalsBox
{
public:
    explicit BatteryBox(QFrame *parent = 0, QString valueColor = "gray");
    bool SetValue(int value);

private:
    QPushButton *mIconx;
    QPushButton *mIcon0;
    QPushButton *mIcon1;
    QPushButton *mIcon2;
    QPushButton *mIcon3;
    QPushButton *mIcon4;
    QPushButton *mIcon5;
    QPushButton *mIcon6;
    QPushButton *mIcon7;
    QPushButton *mIcon8;
    QPushButton *mIcon9;
    QPushButton *mIcon10;
};

#endif // BATTERYBOX_H
