#include <QFrame>
#include <QVariant>
#include "pressuretargetbox.h"

PressureTargetBox::PressureTargetBox(QFrame *parent, QString valueColor)
{
    mLabel = parent->findChild<QLabel *>(QString(parent->objectName()).append("Label"));
    if (mLabel != NULL) mLabel->setStyleSheet("color : white;");

    mPlusButton = parent->findChild<QPushButton *>(QString(parent->objectName()).append("Plus"));
    mMinusButton = parent->findChild<QPushButton *>(QString(parent->objectName()).append("Minus"));
    mValue = parent->findChild<QLabel *>(QString(parent->objectName()).append("Value"));
    if (mValue != NULL) mValue->setStyleSheet(QString("color : %1;").arg(valueColor));
    mPlusButton->setStyleSheet("background-color: gray;");
    mMinusButton->setStyleSheet("background-color: gray;");
}

bool PressureTargetBox::SetValue(int value1)
{
    if ((value1 >= 0) && (value1 <= 300)) {
        mValue->setText(QVariant(value1).toString());
        return true;
    }
    return false;
}
