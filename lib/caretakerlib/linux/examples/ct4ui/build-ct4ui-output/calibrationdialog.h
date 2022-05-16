#ifndef CALIBRATIONDIALOG_H
#define CALIBRATIONDIALOG_H

#include <QDialog>

namespace Ui {
class CalibrationDialog;
}

class CalibrationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CalibrationDialog(QWidget *parent = 0);
    ~CalibrationDialog();
    int getPosture();
    int getSystolic();
    int getDiastolic();


    static const int CD_CANCEL  = 0;
    static const int CD_AUTO    = 1;
    static const int CD_MANUAL  = 2;

    static const int POSTURE_SITTING    = 1;
    static const int POSTURE_RECLINING  = 2;
    static const int POSTURE_SUPINE     = 3;
    static const int POSTURE_STANDING   = 5;

signals:
    void cdExit(int status);

private slots:
    void cdCancel();
    void cdAuto();
    void cdManual();
    void onPostureLeft();
    void onPostureRight();



private:
    int posture;

    void displayPosture();
    Ui::CalibrationDialog *ui;
};

#endif // CALIBRATIONDIALOG_H
