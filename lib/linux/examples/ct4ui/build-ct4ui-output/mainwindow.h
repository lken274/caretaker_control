#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <stdint.h>
#include <QRect>
#include "qmutex.h"
#include "waveformviewer.h"
#include "vitalsbox.h"
#include "batterybox.h"
#include "snrbox.h"
#include "pressuretargetbox.h"
#include "calibrationdialog.h"
#include "caretaker.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(int argc = 0, char *argv[] = 0, QWidget *parent = 0);
    ~MainWindow();
    void showStatus(QString msg);
    int mBatteryVoltage;
    int mSnrValue;
    bool mUsrInitiatedDisconnect;
    bool mConnectionLost;

protected:
    void paintEvent(QPaintEvent *event);

private slots:
    void timerHandler();
    void onStart();
    void onStop();
    void onRecal();
    void onPlus();
    void onMinus();
    void onMenuConnect();
    void onMenuDisconnect();
    void onCalibrationDialogFinished(int status);

private:
    Ui::MainWindow *ui;
    QTimer timer;
    int pulseWaveformCursor;
    bool pulseWaveformInitialized;
    waveformViewer* rawPulseWaveform;
    waveformViewer* intPulseWaveform;
    VitalsBox* mHrBox;
    VitalsBox* mRespBox;
    VitalsBox* mBpBox;
    VitalsBox* mMapBox;
    BatteryBox* mBatteryBox;
    SnrBox* mSnrBox;
    VitalsBox* mCuffBox;
    PressureTargetBox* mTargetBox;
    CalibrationDialog* cd;

    // libcaretaker vairables
    libct_app_callbacks_t mCallbacks;
    libct_context_t* mContext;
    bool mMenuVisible;

    struct {
        QMutex mutex;
        QString msg;
    } mStatus;

    friend void on_data_received_cb(libct_context_t* context, libct_device_t* device, libct_stream_data_t* data);
};

#endif // MAINWINDOW_H
