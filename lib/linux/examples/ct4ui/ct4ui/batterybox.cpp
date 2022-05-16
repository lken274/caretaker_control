#include "batterybox.h"
#include <QVariant>
#include <QDebug>
#include <QPainter>

BatteryBox::BatteryBox(QFrame *parent, QString valueColor) : VitalsBox(parent, valueColor)
{
    mIconx = parent->findChild<QPushButton *>(QString("battIconx"));
    mIconx->setStyleSheet("background-color : black;");
    mIconx->setVisible(true);  // default to dead batt
    mIcon = mIconx;

    mIcon0 = parent->findChild<QPushButton *>(QString("battIcon0"));
    mIcon0->setStyleSheet("background-color : black;");
    mIcon0->setVisible(false);

    mIcon1 = parent->findChild<QPushButton *>(QString("battIcon1"));
    mIcon1->setStyleSheet("background-color : black;");
    mIcon1->setVisible(false);

    mIcon2 = parent->findChild<QPushButton *>(QString("battIcon2"));
    mIcon2->setStyleSheet("background-color : black;");
    mIcon2->setVisible(false);

    mIcon3 = parent->findChild<QPushButton *>(QString("battIcon3"));
    mIcon3->setStyleSheet("background-color : black;");
    mIcon3->setVisible(false);

    mIcon4 = parent->findChild<QPushButton *>(QString("battIcon4"));
    mIcon4->setStyleSheet("background-color : black;");
    mIcon4->setVisible(false);

    mIcon5 = parent->findChild<QPushButton *>(QString("battIcon5"));
    mIcon5->setStyleSheet("background-color : black;");
    mIcon5->setVisible(false);

    mIcon6 = parent->findChild<QPushButton *>(QString("battIcon6"));
    mIcon6->setStyleSheet("background-color : black;");
    mIcon6->setVisible(false);

    mIcon7 = parent->findChild<QPushButton *>(QString("battIcon7"));
    mIcon7->setStyleSheet("background-color : black;");
    mIcon7->setVisible(false);

    mIcon8 = parent->findChild<QPushButton *>(QString("battIcon8"));
    mIcon8->setStyleSheet("background-color : black;");
    mIcon8->setVisible(false);

    mIcon9 = parent->findChild<QPushButton *>(QString("battIcon9"));
    mIcon9->setStyleSheet("background-color : black;");
    mIcon9->setVisible(false);

    mIcon10 = parent->findChild<QPushButton *>(QString("battIcon10"));
    mIcon10->setStyleSheet("background-color : black;");
    mIcon10->setVisible(false);
}

bool BatteryBox::SetValue(int value)
{

    if (mIcon != NULL) mIcon->setVisible(false);
    // device has a 12 step bar graph display.  Mimic it using values supplied by Cory
    if (value >= 4100) {
        mIcon = mIcon10;
    } else if (value >= 4021) {
       mIcon = mIcon9;
    } else if (value >= 3939) {
        mIcon = mIcon8;
    } else if (value >= 3857) {
        mIcon = mIcon7;
    } else if (value >= 3775) {
      mIcon = mIcon6;
    } else if (value >= 3693) {
       mIcon = mIcon5;
    } else if (value >= 3611) {
       mIcon = mIcon5;
    } else if (value >= 3529) {
        mIcon = mIcon3;
    } else if (value >= 3447) {
        mIcon = mIcon2;
    } else if (value >= 3365) {
        mIcon = mIcon1;
    } else if (value >= 3283) {
        mIcon = mIcon0;
    } else  {
        mIcon = mIconx;
    }

    mIcon->setVisible(true);

    QString v = QVariant(value / 100).toString() + "V";

    if (v.length() == 3) {
        v.insert(1,".");
    } else {
        // invalid value from device
        v = "0.0V";
    }
    mValue->setText(v);
    return false;
}
