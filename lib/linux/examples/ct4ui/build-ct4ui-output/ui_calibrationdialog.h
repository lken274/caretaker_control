/********************************************************************************
** Form generated from reading UI file 'calibrationdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CALIBRATIONDIALOG_H
#define UI_CALIBRATIONDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_CalibrationDialog
{
public:
    QLabel *cdLabel;
    QFrame *cdHorizontalDivider;
    QFrame *line;
    QPushButton *cdCancel;
    QPushButton *cdStartAuto;
    QPushButton *cdStartManual;
    QLabel *cdLabelStartAuto;
    QLabel *cdLabelStartManual;
    QSpinBox *cdDiastolic;
    QSpinBox *cdSystolic;
    QLabel *label;
    QPushButton *cdPostureLeft;
    QPushButton *cdPostureRight;
    QPushButton *cdPostureIcon;

    void setupUi(QDialog *CalibrationDialog)
    {
        if (CalibrationDialog->objectName().isEmpty())
            CalibrationDialog->setObjectName(QStringLiteral("CalibrationDialog"));
        CalibrationDialog->resize(320, 240);
        cdLabel = new QLabel(CalibrationDialog);
        cdLabel->setObjectName(QStringLiteral("cdLabel"));
        cdLabel->setGeometry(QRect(10, 10, 301, 17));
        cdLabel->setStyleSheet(QStringLiteral("color:steelblue;"));
        cdLabel->setAlignment(Qt::AlignCenter);
        cdHorizontalDivider = new QFrame(CalibrationDialog);
        cdHorizontalDivider->setObjectName(QStringLiteral("cdHorizontalDivider"));
        cdHorizontalDivider->setGeometry(QRect(10, 20, 301, 20));
        cdHorizontalDivider->setStyleSheet(QStringLiteral("color: steelblue;"));
        cdHorizontalDivider->setFrameShadow(QFrame::Plain);
        cdHorizontalDivider->setLineWidth(1);
        cdHorizontalDivider->setFrameShape(QFrame::HLine);
        line = new QFrame(CalibrationDialog);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(150, 30, 20, 171));
        line->setStyleSheet(QStringLiteral("color : steelblue;"));
        line->setFrameShadow(QFrame::Plain);
        line->setLineWidth(1);
        line->setFrameShape(QFrame::VLine);
        cdCancel = new QPushButton(CalibrationDialog);
        cdCancel->setObjectName(QStringLiteral("cdCancel"));
        cdCancel->setGeometry(QRect(0, 200, 321, 41));
        cdCancel->setFocusPolicy(Qt::NoFocus);
        cdCancel->setAutoDefault(true);
        cdCancel->setFlat(false);
        cdStartAuto = new QPushButton(CalibrationDialog);
        cdStartAuto->setObjectName(QStringLiteral("cdStartAuto"));
        cdStartAuto->setGeometry(QRect(10, 130, 141, 61));
        cdStartAuto->setFocusPolicy(Qt::NoFocus);
        QIcon icon;
        icon.addFile(QStringLiteral(":/res/cal_auto_default.png"), QSize(), QIcon::Normal, QIcon::Off);
        cdStartAuto->setIcon(icon);
        cdStartAuto->setIconSize(QSize(48, 48));
        cdStartAuto->setAutoDefault(false);
        cdStartAuto->setFlat(false);
        cdStartManual = new QPushButton(CalibrationDialog);
        cdStartManual->setObjectName(QStringLiteral("cdStartManual"));
        cdStartManual->setGeometry(QRect(168, 130, 151, 61));
        cdStartManual->setFocusPolicy(Qt::NoFocus);
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/res/cal_manual_default.png"), QSize(), QIcon::Normal, QIcon::Off);
        cdStartManual->setIcon(icon1);
        cdStartManual->setIconSize(QSize(48, 48));
        cdStartManual->setAutoDefault(false);
        cdStartManual->setFlat(false);
        cdLabelStartAuto = new QLabel(CalibrationDialog);
        cdLabelStartAuto->setObjectName(QStringLiteral("cdLabelStartAuto"));
        cdLabelStartAuto->setGeometry(QRect(10, 40, 141, 20));
        QFont font;
        font.setPointSize(10);
        cdLabelStartAuto->setFont(font);
        cdLabelStartAuto->setAlignment(Qt::AlignCenter);
        cdLabelStartManual = new QLabel(CalibrationDialog);
        cdLabelStartManual->setObjectName(QStringLiteral("cdLabelStartManual"));
        cdLabelStartManual->setGeometry(QRect(170, 40, 141, 20));
        cdLabelStartManual->setFont(font);
        cdLabelStartManual->setAlignment(Qt::AlignCenter);
        cdDiastolic = new QSpinBox(CalibrationDialog);
        cdDiastolic->setObjectName(QStringLiteral("cdDiastolic"));
        cdDiastolic->setGeometry(QRect(250, 80, 61, 41));
        cdDiastolic->setFocusPolicy(Qt::ClickFocus);
        cdDiastolic->setMinimum(20);
        cdDiastolic->setMaximum(250);
        cdDiastolic->setValue(80);
        cdSystolic = new QSpinBox(CalibrationDialog);
        cdSystolic->setObjectName(QStringLiteral("cdSystolic"));
        cdSystolic->setGeometry(QRect(170, 80, 61, 41));
        cdSystolic->setFocusPolicy(Qt::ClickFocus);
        cdSystolic->setMinimum(50);
        cdSystolic->setMaximum(300);
        cdSystolic->setValue(120);
        label = new QLabel(CalibrationDialog);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(230, 90, 16, 17));
        label->setAlignment(Qt::AlignCenter);
        cdPostureLeft = new QPushButton(CalibrationDialog);
        cdPostureLeft->setObjectName(QStringLiteral("cdPostureLeft"));
        cdPostureLeft->setGeometry(QRect(10, 80, 21, 41));
        cdPostureLeft->setFocusPolicy(Qt::NoFocus);
        cdPostureRight = new QPushButton(CalibrationDialog);
        cdPostureRight->setObjectName(QStringLiteral("cdPostureRight"));
        cdPostureRight->setGeometry(QRect(130, 80, 21, 41));
        cdPostureRight->setFocusPolicy(Qt::NoFocus);
        cdPostureIcon = new QPushButton(CalibrationDialog);
        cdPostureIcon->setObjectName(QStringLiteral("cdPostureIcon"));
        cdPostureIcon->setEnabled(true);
        cdPostureIcon->setGeometry(QRect(40, 80, 81, 41));
        QFont font1;
        font1.setPointSize(9);
        cdPostureIcon->setFont(font1);
        cdPostureIcon->setFocusPolicy(Qt::NoFocus);
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/res/posture_sitting.png"), QSize(), QIcon::Normal, QIcon::Off);
        cdPostureIcon->setIcon(icon2);
        cdPostureIcon->setIconSize(QSize(32, 32));
        cdPostureIcon->setAutoDefault(false);
        cdPostureIcon->setFlat(true);

        retranslateUi(CalibrationDialog);

        QMetaObject::connectSlotsByName(CalibrationDialog);
    } // setupUi

    void retranslateUi(QDialog *CalibrationDialog)
    {
        CalibrationDialog->setWindowTitle(QApplication::translate("CalibrationDialog", "Dialog", 0));
        cdLabel->setText(QApplication::translate("CalibrationDialog", "Start With Automatic or Manual Calibration", 0));
        cdCancel->setText(QApplication::translate("CalibrationDialog", "Cancel", 0));
        cdStartAuto->setText(QApplication::translate("CalibrationDialog", "Start", 0));
        cdStartManual->setText(QApplication::translate("CalibrationDialog", "Start", 0));
        cdLabelStartAuto->setText(QApplication::translate("CalibrationDialog", "Automatic Calibration", 0));
        cdLabelStartManual->setText(QApplication::translate("CalibrationDialog", "Manual Calibration", 0));
        label->setText(QApplication::translate("CalibrationDialog", "/", 0));
        cdPostureLeft->setText(QApplication::translate("CalibrationDialog", "<", 0));
        cdPostureRight->setText(QApplication::translate("CalibrationDialog", ">", 0));
        cdPostureIcon->setText(QApplication::translate("CalibrationDialog", "Sitting", 0));
    } // retranslateUi

};

namespace Ui {
    class CalibrationDialog: public Ui_CalibrationDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CALIBRATIONDIALOG_H
