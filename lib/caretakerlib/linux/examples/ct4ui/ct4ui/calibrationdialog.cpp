#include "calibrationdialog.h"
#include "ui_calibrationdialog.h"

CalibrationDialog::CalibrationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CalibrationDialog)
{
    ui->setupUi(this);
    move(parent->window()->frameGeometry().topLeft() +
         parent->window()->rect().center() - rect().center());

    posture = POSTURE_SITTING;
    displayPosture();

    connect(ui->cdCancel,SIGNAL(clicked()), this, SLOT(cdCancel()));
    connect(ui->cdStartAuto,SIGNAL(clicked()), this, SLOT(cdAuto()));
    connect(ui->cdStartManual,SIGNAL(clicked()), this, SLOT(cdManual()));
    connect(ui->cdPostureLeft, SIGNAL(clicked()), this, SLOT(onPostureLeft()));
    connect(ui->cdPostureRight, SIGNAL(clicked()), this, SLOT(onPostureRight()));
}

void CalibrationDialog::displayPosture()
{
    switch (posture) {
    case POSTURE_SITTING:
        ui->cdPostureLeft->setEnabled(false);
        ui->cdPostureIcon->setIcon(QPixmap(":res/posture_sitting.png"));
        ui->cdPostureIcon->setText(QString("Sitting"));
        break;
    case POSTURE_RECLINING:
        ui->cdPostureLeft->setEnabled(true);
        ui->cdPostureIcon->setIcon(QPixmap(":res/posture_reclining.png"));
        ui->cdPostureIcon->setText(QString("Reclining"));
        break;
    case POSTURE_SUPINE:
        ui->cdPostureRight->setEnabled(true);
        ui->cdPostureIcon->setIcon(QPixmap(":res/posture_supine.png"));
        ui->cdPostureIcon->setText(QString("Supine"));
        break;
    case POSTURE_STANDING:
        ui->cdPostureRight->setEnabled(false);
        ui->cdPostureIcon->setIcon(QPixmap(":res/posture_standing.png"));
        ui->cdPostureIcon->setText(QString("Standing"));
        break;
    }
}

void CalibrationDialog::onPostureLeft()
{
    switch (posture) {
    case POSTURE_SITTING:
        break;
    case POSTURE_RECLINING:
        posture = POSTURE_SITTING;
        break;
    case POSTURE_SUPINE:
        posture = POSTURE_RECLINING;
        break;
    case POSTURE_STANDING:
        posture = POSTURE_SUPINE;
        break;
    }
    displayPosture();
}

void CalibrationDialog::onPostureRight()
{
    switch (posture) {
    case POSTURE_SITTING:
        posture = POSTURE_RECLINING;
        break;
    case POSTURE_RECLINING:
        posture = POSTURE_SUPINE;
        break;
    case POSTURE_SUPINE:
        posture = POSTURE_STANDING;
        break;
    case POSTURE_STANDING:
        break;
    }
    displayPosture();
}

int CalibrationDialog::getPosture()
{
    return posture;
}

int CalibrationDialog::getSystolic()
{
    return ui->cdSystolic->value();
}

int CalibrationDialog::getDiastolic()
{
    return ui->cdDiastolic->value();
}

void CalibrationDialog::cdCancel()
{
    emit cdExit(CD_CANCEL);
}

void CalibrationDialog::cdAuto()
{
    emit cdExit(CD_AUTO);
}

void CalibrationDialog::cdManual()
{
    emit cdExit(CD_MANUAL);
}
CalibrationDialog::~CalibrationDialog()
{
    delete ui;
}
