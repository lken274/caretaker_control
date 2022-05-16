#ifndef PRESSURETARGETBOX_H
#define PRESSURETARGETBOX_H
#include <QLabel>
#include <QPushButton>

class PressureTargetBox
{
public:
    PressureTargetBox(QFrame *parent = 0, QString valueColor = "gray");
    bool SetValue(int value1);

public:
    QLabel      *mLabel;
    QLabel      *mValue;
    QPushButton *mPlusButton;
    QPushButton *mMinusButton;
};

#endif // PRESSURETARGETBOX_H
