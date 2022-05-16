#include <stdint.h>
#include "vitalsbox.h"
#include <QVariant>


VitalsBox::VitalsBox(QFrame *parent, QString valueColor)
{
    mIcon = parent->findChild<QPushButton *>(QString(parent->objectName()).append("Icon"));
    if (mIcon != NULL) mIcon->setStyleSheet("background-color : black;");

    mLabel = parent->findChild<QLabel *>(QString(parent->objectName()).append("Label"));
    if (mLabel != NULL) mLabel->setStyleSheet("color : white;");

    mValue = parent->findChild<QLabel *>(QString(parent->objectName()).append("Value"));
    if (mValue != NULL) mValue->setStyleSheet(QString("color : %1;").arg(valueColor));
}

bool VitalsBox::SetValue(int value1)
{
    if ((value1 >= 0) && (value1 <= 300)) {
        mValue->setText(QVariant(value1).toString());
        return true;
    }
    return false;
}

bool VitalsBox::SetValue(int value1, int value2)
{
    if ((value1 >= 0) && (value1 <= 300) && (value2 < 200) && (value2 >= 0)) {
        mValue->setText(QVariant(value1).toString() + " / " + QVariant(value2).toString());
        return true;
    }
    return false;
}
