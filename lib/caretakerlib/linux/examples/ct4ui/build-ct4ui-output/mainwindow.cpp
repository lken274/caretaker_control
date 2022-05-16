#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "waveformviewer.h"
#include "qpainter.h"
#include "qtimer.h"
#include "qdebug.h"
#include "pulsewaveform.h"
#include "qcolordialog.h"
#include <iostream>
#include <fcntl.h>
#include <unistd.h>

#define SUPPRESS_WARNING(var) (var) = (var)

// turn on extra logging, mostly useful for running V&V tests
#define VERBOSE_DEBUG
#ifdef VERBOSE_DEBUG
#define verbose_debug(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
#define verbose_debug(fmt, ...) do {} while (0)
#endif

#ifdef VERBOSE_DEBUG
void DumpHex(const void* data, size_t size) {
    char ascii[17];
    size_t i, j;
    ascii[16] = '\0';
    for (i = 0; i < size; ++i) {
        fprintf(stderr, "%02X ", ((unsigned char*)data)[i]);
        if (((unsigned char*)data)[i] >= ' ' && ((unsigned char*)data)[i] <= '~') {
            ascii[i % 16] = ((unsigned char*)data)[i];
        } else {
            ascii[i % 16] = '.';
        }
        if ((i+1) % 8 == 0 || i+1 == size) {
            fprintf(stderr, " ");
            if ((i+1) % 16 == 0) {
                fprintf(stderr, "|  %s \n", ascii);
            } else if (i+1 == size) {
                ascii[(i+1) % 16] = '\0';
                if ((i+1) % 16 <= 8) {
                    fprintf(stderr, " ");
                }
                for (j = (i+1) % 16; j < 16; ++j) {
                    fprintf(stderr, "   ");
                }
                fprintf(stderr, "|  %s \n", ascii);
            }
        }
    }
}

#endif

/* simulated data sample count */
int numSimulatedSamples = sizeof(simulated_adc_readings) / sizeof(uint16_t);

/* async cb for write recalibrate interval */
void LIBCTAPI on_wrt_vitals_filter_rsp_cb(libct_context_t* context, libct_device_t* device, int status)
{
    SUPPRESS_WARNING(context);
    SUPPRESS_WARNING(device);
    verbose_debug("Vitals filter written status=%d\n", status);
}

/* async cb for read recalibrate interval */
void LIBCTAPI on_rd_vitals_filter_rsp_cb(libct_context_t* context, libct_device_t* device, int filter, int status)
{
    SUPPRESS_WARNING(context);
    SUPPRESS_WARNING(device);
    verbose_debug("Vitals filter read status=%d filter=%d\n", status, filter);
}

/* async cb for write recalibrate interval */
void LIBCTAPI on_wrt_recal_itvl_rsp_cb(libct_context_t* context, libct_device_t* device, int status)
{
    SUPPRESS_WARNING(context);
    SUPPRESS_WARNING(device);
    verbose_debug("Recalibrate interval written status=%d\n", status);
}

/* async cb for read recalibrate interval */
void LIBCTAPI on_rd_recal_itvl_rsp_cb(libct_context_t* context, libct_device_t* device, unsigned int itvl, int status)
{
    SUPPRESS_WARNING(context);
    SUPPRESS_WARNING(device);
    verbose_debug("Recalibrate interval read status=%d interval=%d\n", status, itvl);
}

/* async cb for write recalibrate interval */
void LIBCTAPI on_wrt_snr_min_rsp_cb(libct_context_t* context, libct_device_t* device, int status)
{
    SUPPRESS_WARNING(context);
    SUPPRESS_WARNING(device);
    verbose_debug("Recalibrate interval written status=%d\n", status);
}

/* async cb for read recalibrate interval */
void LIBCTAPI on_rd_snr_min_rsp_cb(libct_context_t* context, libct_device_t* device, int snr, int status)
{
    SUPPRESS_WARNING(context);
    SUPPRESS_WARNING(device);
    verbose_debug("Recalibrate interval read status=%d interval=%d\n", status, snr);
}

/* async cb for read recalibrate interval */
void LIBCTAPI on_diag_flush_rsp_cb(libct_context_t* context, libct_device_t* device, int status)
{
    SUPPRESS_WARNING(context);
    SUPPRESS_WARNING(device);
    verbose_debug("Diagnostic flush status=%d\n", status);
}

/* async cb for read recalibrate interval */
void LIBCTAPI on_wrt_waveform_clamping_cb(libct_context_t* context, libct_device_t* device, int status)
{
    SUPPRESS_WARNING(context);
    SUPPRESS_WARNING(device);
    verbose_debug("Waveform Clamping status=%d\n", status);
}

/* libcaretaker callback to notify a device was found */
void LIBCTAPI on_device_discovered_cb(libct_context_t* context, libct_device_t* device) {
    libct_stop_discovery(context);
    int status = libct_connect(context, device);

     MainWindow* window = (MainWindow*) libct_get_app_specific_data(context);
     if ( status > LIBCT_STATUS_ERROR ) {
        const char* name = libct_device_get_name(device);
        if (name)
            window->showStatus(QString("Connecting to ") + name + "...");
        else
            window->showStatus("Connecting...");
     } else {
         window->showStatus(QString("Connect error (") + status + ")");
     }
}

/* libcaretaker callback to notify scanning timed out */
void LIBCTAPI on_discovery_timedout_cb(libct_context_t* context) {
    MainWindow* window = (MainWindow*) libct_get_app_specific_data(context);
    window->showStatus("Scan timed out");
}

/* libcaretaker callback  to notify scan error */
void LIBCTAPI on_discovery_failed_cb(libct_context_t* context, int error) {
    SUPPRESS_WARNING(error);
    MainWindow* window = (MainWindow*) libct_get_app_specific_data(context);
    window->showStatus("Scan error");
}

/* libcaretaker callback to notify connection is established */
void LIBCTAPI on_device_connected_not_ready_cb(libct_context_t* context, libct_device_t* device) {
    SUPPRESS_WARNING(device);
    MainWindow* window = (MainWindow*) libct_get_app_specific_data(context);
    window->mUsrInitiatedDisconnect = false;
    window->mConnectionLost = false;
    window->showStatus("Connected. Device not ready.");
}

/* libcaretaker callback to notify connection is established */
void LIBCTAPI on_device_connected_ready_cb(libct_context_t* context, libct_device_t* device) {
    SUPPRESS_WARNING(device);
    MainWindow* window = (MainWindow*) libct_get_app_specific_data(context);
    window->showStatus("Connected. Press start to begin.");
}

/* libcaretaker callback to notify connection error */
void LIBCTAPI on_connect_error_cb(libct_context_t* context, libct_device_t* device, const char* error) {
    SUPPRESS_WARNING(device);
    SUPPRESS_WARNING(error);
    MainWindow* window = (MainWindow*) libct_get_app_specific_data(context);
    window->showStatus("Connect error");
}

/* libcaretaker callback to notify connection timed out */
void LIBCTAPI on_connect_timedout_cb(libct_context_t* context, libct_device_t* device) {
    SUPPRESS_WARNING(device);
    MainWindow* window = (MainWindow*) libct_get_app_specific_data(context);
    window->showStatus("Connect timed out");
}

/* libcaretaker callback  to notify disconnect events */
void LIBCTAPI on_device_disconnected_cb(libct_context_t* context, libct_device_t* device) {
    SUPPRESS_WARNING(device);
    MainWindow* window = (MainWindow*) libct_get_app_specific_data(context);
    // auto connect if the connection was dropped
    if ( !window->mUsrInitiatedDisconnect ) {
        window->showStatus("Connection lost! Reconnecting");
        window->mConnectionLost = true;
    } else {
        window->showStatus("Disconnected");
    }
}

/* libcaretaker callback to notify the device will start producing data */
void LIBCTAPI on_start_monitoring_cb(libct_context_t* context, libct_device_t* device, int status) {
    MainWindow* window = (MainWindow*) libct_get_app_specific_data(context);
    if ( status > LIBCT_STATUS_ERROR ) {
       const char* name = libct_device_get_name(device);
       if (name)
           window->showStatus(QString("Monitoring ") + name);
       else
           window->showStatus("Monitoring");
    } else {
        window->showStatus(QString("Monitor error (") + status + ")");
    }
}

/* libcaretaker callback to notify the device is taking measurments */
void LIBCTAPI on_start_measuring_cb(libct_context_t* context, libct_device_t* device, int status) {
    SUPPRESS_WARNING(context);
    SUPPRESS_WARNING(device);
    SUPPRESS_WARNING(status);
}

/* libcaretaker callback to notify data from the device */
void LIBCTAPI on_data_received_cb(libct_context_t* context, libct_device_t* device, libct_stream_data_t* data) {
    SUPPRESS_WARNING(device);
    MainWindow* window = (MainWindow*) libct_get_app_specific_data(context);

    // update displayed pulse rate waveform
    unsigned int idx;
    libct_pulse_t* pulse;
    for_each_dp(data, idx, pulse, raw_pulse) {
        if ( pulse && pulse->valid ) {
            window->rawPulseWaveform->add(pulse->timestamp, pulse->value);
        }
    }

    // update displayed pulse pressure waveform
    for_each_dp(data, idx, pulse, int_pulse) {
        if ( pulse && pulse->valid ) {
            if ( pulse && pulse->valid ) {
                window->intPulseWaveform->add(pulse->timestamp, pulse->value);
            }
        }
    }

    // update displayed vitals
    libct_vitals_t* vitals = libct_get_last_dp(data, vitals);
    if ( vitals && vitals->valid ) {
        window->mHrBox->SetValue(vitals->heart_rate);
        window->mRespBox->SetValue(vitals->respiration);
        window->mMapBox->SetValue(vitals->map);
        window-> mBpBox->SetValue(vitals->systolic, vitals->diastolic);

        verbose_debug("Vitals: (status bits): bp=%d map=%d hr=%d resp=%d int_err=%d d_err=%d p12_err=%d\n", 
            vitals->bp_status, vitals->map_status, vitals->hr_status, vitals->respiration_status, 
            vitals->integration_error, vitals->differentiation_error, vitals->p12_finder_error);

        verbose_debug("Vitals: (contd) p3_err=%d min_oor=%d max_oor=%d slope_oor=%d\n",
            vitals->p3_finder_eError, vitals->min_index_out_of_range, 
            vitals->max_index_out_of_range, vitals->slope_out_of_range);

        verbose_debug("Vitals: hr=%d resp=%d map=%d sys=%d dia=%d\n", 
            vitals->heart_rate, vitals->respiration, vitals->map, vitals->systolic, vitals->diastolic);
    }

#ifdef VERBOSE_DEBUG
    // log vitals2 and pp for v&v testing
    libct_vitals2_t* vitals2 = libct_get_last_dp(data, vitals2);
    if (vitals2 && vitals2->valid) {
        verbose_debug("Vitals2: bv=%d co=%d pr=%f ibi=%d lvet=%d p2p1=%f hrcomp=%f\n",
            vitals2->blood_volume, vitals2->cardiac_output, vitals2->pr, vitals2->ibi, 
            vitals2->lvet, vitals2->p2p1, vitals2->hrComp);
        verbose_debug("Vitals2: (reserved fields) :\n");
        DumpHex((char *)&vitals2->reserved[0], sizeof(vitals2->reserved));
    }

    libct_param_pulse_t* param_pulse = libct_get_last_dp(data, param_pulse);
    if (param_pulse && param_pulse->valid) {
        verbose_debug("PP: t0=%d t1=%d t2=%d t3=%d p0=%d p1=%d p2=%d p3=%d\n",
            param_pulse->t0, param_pulse->t1, param_pulse->t2, param_pulse->t3, 
            param_pulse->p0, param_pulse->p1, param_pulse->p2, param_pulse->p3);

        verbose_debug("PP Waveform:\n");
        DumpHex(param_pulse->waveform, param_pulse->waveform_len);
    }
#endif

    // update displayed cuff pressure / store snr
    libct_cuff_pressure_t* cuff = libct_get_last_dp(data, cuff_pressure);
    if ( cuff && cuff->valid ) {
        window->mSnrValue = cuff->snr;
        window->mCuffBox->SetValue(cuff->value);
        window->mTargetBox->SetValue(cuff->target);
        verbose_debug("Cuff: snr=%d target=%d value=%d\n", cuff->snr, cuff->target, cuff->value);
    }

    //  store battery info
    if ( data->battery_info.valid) {
        window->mBatteryVoltage = data->battery_info.voltage;
        verbose_debug("Batt: voltage=%d\n", data->battery_info.voltage);
    }

    // update displayed device status
    if ( data->device_status.valid) {

        verbose_debug("status : calibrated=%d manual_mode=%d auto_mode=%d calibrating=%d calibration_failed=%d\n",
            data->device_status.calibrated, data->device_status.manual_cal_mode, data->device_status.auto_cal_mode, 
            data->device_status.calibrating, data->device_status.calibration_failed);

        verbose_debug("status : bad_cuff=%d cuff_too_loose=%d cuff_too_tight=%d motion_event=%d poor_signal=%d\n",
            data->device_status.bad_cuff, data->device_status.cuff_too_loose, data->device_status.cuff_too_tight, 
            data->device_status.motion_event, data->device_status.poor_signal);

        verbose_debug("status : weak_signal=%d posture=%d charging=%d charge_complete=%d\n", 
            data->device_status.weak_signal, data->device_status.posture, data->device_status.charging, data->device_status.chargeComplete);

        verbose_debug("status : pda_en=%d sim=%d inflated=%d clk_w=%d batt_low=%d temp_crit=%d\n",
            data->device_status.pda_enabled, data->device_status.simulation_enabled, data->device_status.inflated_indicator,
            data->device_status.clock_wrap_around, data->device_status.battery_voltage_low, data->device_status.critical_temperature);

        verbose_debug("status : %llx\n", data->device_status.value);

        if ( data->device_status.calibrated) {
            window->showStatus("Calibrated");
        }
        if ( data->device_status.manual_cal_mode) {
            window->showStatus("Calibrated (manual mode)");
        }
        if ( data->device_status.auto_cal_mode) {
            window->showStatus("Calibrated (auto mode)");
        }
        if ( data->device_status.calibrating) {
            window->showStatus("Calibrating...");
        }
        if ( data->device_status.calibration_failed) {
            window->showStatus("Calibration failed");
        }
        if ( data->device_status.bad_cuff) {
            window->showStatus("Bad cuff");
        }
        if ( data->device_status.cuff_too_loose) {
            window->showStatus("Cuff too loose");
        }
        if ( data->device_status.cuff_too_tight) {
            window->showStatus("Cuff too tight");
        }
        if ( data->device_status.motion_event) {
            window->showStatus("Motion detected");
        }
        if ( data->device_status.poor_signal) {
            window->showStatus("Poor signal");
        }
        if ( data->device_status.weak_signal) {
            window->showStatus("Weak signal");
        }
    }
}

/* libcaretaker callback to notify data receive error */
void LIBCTAPI on_data_error_cb(libct_context_t* context, libct_device_t* device, const char* error) {
    SUPPRESS_WARNING(context);
    SUPPRESS_WARNING(device);
    printf("Data error: %s\n", error);
}

MainWindow::MainWindow(int argc, char *argv[], QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle(QString("Libcaretaker Sample App"));

    // Setup waveform windows
    // xres is set to 180/440 to fit approximately 6 seconds of pulse data in the waveform window.
    // Assuming 4 xfers per second, input rate = 4 * 10 points per second, so max buffer size = 6 * 40 = 240 points.
    // The app needs to display 180 points every 6 seconds, or at least 60 points every second to keep up, which
    // should be met since the app refreshes every 100ms, or 10 times per second, so on average 6 points should
    // be displayed each refresh cycle if the input keeps up. (See timer and timerHandler() definitions for the
    // refresh rate.)
    const int width = 440, height = 130;
    const float xres = 180.0 / width;

    // if running as root we need to adjust y-offset to account for menubar placement on the window
    SUPPRESS_WARNING(argv);
    mMenuVisible = (argc > 1);
    int yoffset = (mMenuVisible) ? menuBar()->geometry().height() : 0;

    rawPulseWaveform = new waveformViewer(QRect(20,20+yoffset,width,height), 10, 30, 10, 10, xres);
    intPulseWaveform = new waveformViewer(QRect(20,180+yoffset,width,height), 10, 30, 10, 10, xres);

    // instantiate vitals boxes
    mHrBox      = new VitalsBox(ui->hr, QString("darkseagreen"));
    mRespBox    = new VitalsBox(ui->resp, QString("aqua"));
    mBpBox      = new VitalsBox(ui->bp, QString("firebrick"));
    mMapBox     = new VitalsBox(ui->map, QString("crimson"));
    mBatteryBox = new BatteryBox(ui->batt, QString("gray"));
    mSnrBox     =  new SnrBox(ui->snr, QString("gray"));
    mCuffBox    = new VitalsBox(ui->cuff, QString("gray"));
    mTargetBox  = new PressureTargetBox(ui->target, QString("gray"));

    // setup to display simualated data for testing
    pulseWaveformInitialized = true;
    pulseWaveformCursor = 0;

    // wire up buttons
    connect(&timer, SIGNAL(timeout()), this, SLOT(timerHandler()));
    connect(ui->startButton, SIGNAL(clicked()), this, SLOT(onStart()));
    connect(ui->recalButton, SIGNAL(clicked()), this, SLOT(onRecal()));
    connect(ui->stopButton, SIGNAL(clicked()), this, SLOT(onStop()));
    connect(ui->targetPlus, SIGNAL(clicked()), this, SLOT(onPlus()));
    connect(ui->targetMinus, SIGNAL(clicked()), this, SLOT(onMinus()));

    if ( mMenuVisible ) {
        showStatus(QString("Use the Options menu to connect to a Caretaker 4 device"));
    } else {
        showStatus(QString("Press start to connect to a Caretaker 4 device"));
    }

    // wire up dropdown menu (on ubuntu, it is on the top of the screen apple style, not on window)
    connect(ui->actionQuit, &QAction::triggered, this, QApplication::quit);
    connect(ui->actionConnect,SIGNAL(triggered()), this, SLOT(onMenuConnect()));
    connect(ui->actionDisconnect,SIGNAL(triggered()), this, SLOT(onMenuDisconnect()));

    // enable libcaretaker debug messages
    libct_set_log_level(0);

    // initialize library callbacks
    memset(&mCallbacks, 0, sizeof(mCallbacks));
    mCallbacks.on_device_discovered             = on_device_discovered_cb;
    mCallbacks.on_discovery_timedout            = on_discovery_timedout_cb;
    mCallbacks.on_discovery_failed              = on_discovery_failed_cb;
    mCallbacks.on_device_connected_not_ready    = on_device_connected_not_ready_cb;
    mCallbacks.on_device_connected_ready        = on_device_connected_ready_cb;
    mCallbacks.on_connect_error                 = on_connect_error_cb;
    mCallbacks.on_connect_timedout              = on_connect_timedout_cb;
    mCallbacks.on_device_disconnected           = on_device_disconnected_cb;
    mCallbacks.on_start_monitoring              = on_start_monitoring_cb;
    mCallbacks.on_start_measuring               = on_start_measuring_cb;
    mCallbacks.on_data_received                 = on_data_received_cb;
    mCallbacks.on_data_error                    = on_data_error_cb;
    mCallbacks.on_rd_recal_itvl_rsp             = on_rd_recal_itvl_rsp_cb;
    mCallbacks.on_wrt_recal_itvl_rsp            = on_wrt_recal_itvl_rsp_cb;
    mCallbacks.on_rd_snr_min_rsp                = on_rd_snr_min_rsp_cb;
    mCallbacks.on_wrt_snr_min_rsp               = on_wrt_snr_min_rsp_cb;
    mCallbacks.on_diag_flush_rsp                = on_diag_flush_rsp_cb;
    mCallbacks.on_wrt_waveform_clamping         = on_wrt_waveform_clamping_cb;
    mCallbacks.on_rd_vitals_filter              = on_rd_vitals_filter_rsp_cb;
    mCallbacks.on_wrt_vitals_filter             = on_wrt_vitals_filter_rsp_cb;

    // setup to connect to a BLE CareTaker4 device
    libct_init_data_t initData;
    memset(&initData, 0, sizeof(initData));
    initData.device_class = LIBCT_DEVICE_CLASS_BLE_CARETAKER4;

    // initialize a device context
    mContext = NULL;
    int status = libct_init(&mContext, &initData, &mCallbacks);
    if ( status <= LIBCT_STATUS_ERROR) {
        showStatus(QString("Init error (") + status + ")");
    }

    /* set instance data to use the callbacks */
    libct_set_app_specific_data(mContext, this);

    // initialize local display values
    mBatteryVoltage = 0;
    mSnrValue = 0;
    mUsrInitiatedDisconnect = false;
    mConnectionLost = false;

    // set 7.5ms CI through debugfs, hci library call worked 95% of the time, and caused conn issues
    if (access("/sys/kernel/debug/bluetooth/hci0/conn_min_interval", F_OK ) != -1) {
        // raspberry pi
        if (system("echo 6 > /sys/kernel/debug/bluetooth/hci0/conn_min_interval")) {
            verbose_debug("Unable to set min connection interval\n");
        }
        if (system("echo 6 > /sys/kernel/debug/bluetooth/hci0/conn_max_interval")) {
            verbose_debug("Unable to set max connection interval\n");
        }
    } else if (access("/sys/kernel/debug/bluetooth/hci1/conn_min_interval", F_OK ) != -1) {
        // linux vm with dongle case
        if (system("echo 6 > /sys/kernel/debug/bluetooth/hci1/conn_min_interval")) {
            verbose_debug("Unable to set min connection interval\n");
        }
        if (system("echo 6 > /sys/kernel/debug/bluetooth/hci1/conn_max_interval")) {
            verbose_debug("Unable to set max connection interval\n");
        }
    } else {
        verbose_debug("Debugfs method of setting connection interval not found. BT will be SLOW\n");
    }


    // start timer to update realtime waveforms and status
    this->timer.start();
}

MainWindow::~MainWindow()
{
    this->timer.stop();

    libct_deinit(mContext);
    mContext = NULL;

    delete ui;
    delete rawPulseWaveform;
    delete intPulseWaveform;
    delete mHrBox;
    delete mRespBox;
    delete mBpBox;
    delete mMapBox;
    delete mBatteryBox;
    delete mSnrBox;
    delete mCuffBox;
    delete mTargetBox;
}

void MainWindow::showStatus(QString msg) {
    mStatus.mutex.lock();
    mStatus.msg = msg;
    mStatus.mutex.unlock();
}

void MainWindow::onMenuConnect()
{
    int status=libct_start_discovery(mContext, 20000);
    if ( status >= LIBCT_STATUS_OK ) {
        showStatus(QString("Scanning..."));
    } else {
        showStatus(QString("Scan error (") + status + ")");
    }
}

void MainWindow::onMenuDisconnect()
{
    int status;
    libct_device_t* device = libct_get_device(mContext);
    if ( libct_device_connecting(device) || libct_device_connected(device) ) {
        mUsrInitiatedDisconnect = true;
        status=libct_disconnect(mContext);
        if ( status < LIBCT_STATUS_OK ) {
            showStatus(QString("Disconnect error (") + status + ")");
        }
        showStatus(QString("Disconnected"));
    } else {
        status = libct_stop_discovery(mContext);
        if ( mMenuVisible ) {
            showStatus(QString("Use the Options menu to connect to a Caretaker 4 device"));
        } else {
            showStatus(QString("Press start to connect to a Caretaker 4 device"));
        }
    }
}

void MainWindow::onCalibrationDialogFinished(int status)
{
    switch (status) {
        case CalibrationDialog::CD_CANCEL:
            break;

        // do automatic calibration
        case CalibrationDialog::CD_AUTO:
        {
            libct_cal_t cal;
            cal.type = LIBCT_AUTO_CAL;
            cal.config.auto_cal.posture = cd->getPosture();
            libct_start_measuring(mContext, &cal);
            //showStatus(QString("Calibrating..."));
        }
        break;

        // do manual calibration
        case CalibrationDialog::CD_MANUAL:
        {
            libct_cal_t cal;
            cal.type = LIBCT_MANUAL_CAL;
            cal.config.manual_cal.settings.systolic = cd->getSystolic();
            cal.config.manual_cal.settings.diastolic = cd->getDiastolic();
            libct_start_measuring(mContext, &cal);
            //showStatus(QString("Calibrating..."));
        }
        break;
    }

    cd->close();
    delete cd;
}

void MainWindow::onStart()
{
    libct_device_t* device = libct_get_device(mContext);
    /* Start button has dual function according to connected state.
     * (1) connect to device, or (2) calibrate bp measurments. */
    if ( libct_device_connected(device) ) {
        // calibrate bp measuremnts
        // instantiate the calibration dialog just before showing because we need to center
        // it above the main window
        cd = new CalibrationDialog(this);

        // these flags kill the title bar and the close / minmax icons
        cd->setWindowFlags(Qt::Window |  Qt::CustomizeWindowHint);

        libct_start_monitoring(mContext,
                               ( LIBCT_MONITOR_INT_PULSE |
                                LIBCT_MONITOR_RAW_PULSE |
                                LIBCT_MONITOR_VITALS |
                                LIBCT_MONITOR_VITALS2 |
#ifdef VERBOSE_DEBUG
                                LIBCT_MONITOR_PARAM_PULSE |
                                LIBCT_MONITOR_CAL_CURVE_DATA |
#endif
                                LIBCT_MONITOR_CUFF_PRESSURE |
                                LIBCT_MONITOR_DEVICE_STATUS |
                                LIBCT_MONITOR_BATTERY_INFO));
        connect(cd,SIGNAL(cdExit(int)),this,SLOT(onCalibrationDialogFinished(int)));
        cd->show();
    } else if( !libct_device_connecting(device) ) {
        // connect to device
        this->timer.start(10);

        int status=libct_start_discovery(mContext, 20000);
        if ( status >= LIBCT_STATUS_OK ) {
            showStatus(QString("Scanning..."));
        } else {
            showStatus(QString("Scan error (") + status + ")");
        }
    }

    qDebug() << "Start Pressed";
}

void MainWindow::onRecal()
{
    libct_device_t* device = libct_get_device(mContext);
    if ( libct_device_connected(device) ) {
        libct_recalibrate(mContext);
    }
    qDebug() << "Recal Pressed";
}

void MainWindow::onStop()
{
    qDebug() << "Stop Pressed";

    libct_device_t* device = libct_get_device(mContext);
    if ( libct_device_discovering(device) ) {
        libct_stop_discovery(mContext);
    }

    if ( libct_device_measuring(device) ) {
        libct_stop_measuring(mContext);
    }

    if ( libct_device_monitoring(device) ) {
        libct_stop_monitoring(mContext);
    }

    if ( libct_device_connected(device) ) {
        showStatus("Connected. Press start to begin.");
    } else {
    if ( mMenuVisible )
        showStatus(QString("Use the Options menu to connect to a Caretaker 4 device"));
    else
        showStatus(QString("Press start to connect to a Caretaker 4 device"));
    }
}

void MainWindow::onPlus()
{
    libct_inc_cuff_pressure(mContext);
    qDebug() << "Plus Pressed";
}

void MainWindow::onMinus()
{
    libct_dec_cuff_pressure(mContext);
    qDebug() << "Minus Pressed";
}

void MainWindow::timerHandler()
{
   static long ticks = 0;
   ticks++;

   // re-establish connection if was lost
   // timeout after 1 hour
   if ( (ticks % 1000 == 0) && mConnectionLost ) {
       mConnectionLost = false;
       libct_disconnect(mContext);
       libct_start_discovery(mContext, 60*60*1000);
   }

   // update realtime measurements every 100ms
   if ( ticks % 10 ) {
       mStatus.mutex.lock();
       QString msg = mStatus.msg;
       mStatus.mutex.unlock();
       statusBar()->showMessage(msg);

       // these get set on the ui thread to circumvent a segfault
       // related to doing icon visibility on another thread
       mBatteryBox->SetValue(mBatteryVoltage);
       mSnrBox->SetValue(mSnrValue);
       repaint();
   }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    // kill warning
    event = event;

    if (pulseWaveformInitialized) {
        rawPulseWaveform->draw(&painter);
        intPulseWaveform->draw(&painter);
    }
}
