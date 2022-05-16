#include "snrbox.h"
#include <QVariant>
#include <QDebug>

SnrBox::SnrBox(QFrame *parent, QString valueColor): VitalsBox(parent, valueColor)
{
    mIcon0 = parent->findChild<QPushButton *>(QString("snrIcon0"));
    mIcon0->setStyleSheet("background-color : black;");
    mIcon0->setVisible(true);  // default to 0 snr
    mIcon = mIcon0;

    mIcon1 = parent->findChild<QPushButton *>(QString("snrIcon1"));
    mIcon1->setStyleSheet("background-color : black;");
    mIcon1->setVisible(false);

    mIcon2 = parent->findChild<QPushButton *>(QString("snrIcon2"));
    mIcon2->setStyleSheet("background-color : black;");
    mIcon2->setVisible(false);

    mIcon3 = parent->findChild<QPushButton *>(QString("snrIcon3"));
    mIcon3->setStyleSheet("background-color : black;");
    mIcon3->setVisible(false);

    mIcon4 = parent->findChild<QPushButton *>(QString("snrIcon4"));
    mIcon4->setStyleSheet("background-color : black;");
    mIcon4->setVisible(false);

    mIcon5 = parent->findChild<QPushButton *>(QString("snrIcon5"));
    mIcon5->setStyleSheet("background-color : black;");
    mIcon5->setVisible(false);

    mIcon6 = parent->findChild<QPushButton *>(QString("snrIcon6"));
    mIcon6->setStyleSheet("background-color : black;");
    mIcon6->setVisible(false);

    mIcon7 = parent->findChild<QPushButton *>(QString("snrIcon7"));
    mIcon7->setStyleSheet("background-color : black;");
    mIcon7->setVisible(false);

    mIcon8 = parent->findChild<QPushButton *>(QString("snrIcon8"));
    mIcon8->setStyleSheet("background-color : black;");
    mIcon8->setVisible(false);

    mIcon9 = parent->findChild<QPushButton *>(QString("snrIcon9"));
    mIcon9->setStyleSheet("background-color : black;");
    mIcon9->setVisible(false);

    mIcon10 = parent->findChild<QPushButton *>(QString("snrIcon10"));
    mIcon10->setStyleSheet("background-color : black;");
    mIcon10->setVisible(false);
}


bool SnrBox::SetValue(int value)
{
    int pct = value / 10;
    int segment = 0;
    // snr comes in as 1000 to 0 - divide by ten and express as percentage

    if ((pct > 1000) || (pct < 0)) pct = 0;

    // select segment to display

    for (int i = 0; i < 10; i++) {
        if (i * 10 >= pct) break;
        segment++;
    }
    if (mIcon != NULL) mIcon->setVisible(false);
    switch (segment) {
    case 0:
        mIcon = mIcon0;
        break;
    case 1:
        mIcon = mIcon1;
        break;
    case 2:
        mIcon = mIcon2;
        break;
    case 3:
        mIcon = mIcon3;
        break;
    case 4:
        mIcon = mIcon4;
        break;
    case 5:
        mIcon = mIcon5;
        break;
    case 6:
        mIcon = mIcon6;
        break;
    case 7:
        mIcon = mIcon7;
        break;
    case 8:
        mIcon = mIcon8;
        break;
    case 9:
        mIcon = mIcon9;
        break;
    case 10:
        mIcon = mIcon10;
        break;
    default:
        mIcon = mIcon0;
        break;
    }

    mIcon->setVisible(true);
    mValue->setText(QString::number(segment * 10)+"%");

    return false;
}
