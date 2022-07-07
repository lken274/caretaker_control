/******************************************************************************
 * @file main.c
 * Common source implementing the Caretaker library unit test.
 * This source file is shared between Windows and Linux library builds.
 * Special handling to load the library Windows DLL is handled with the
 * LIBCARETAKER_DYNAMIC_DLL macro defined in the Visual Studio project
 * configuration preprocessor definitions.
 *******************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "windows.h"

#include "libcaretaker_dynamic.hpp"

 /* Suppress unreferenced variable warning */
#define UNREFERENCED_VARIABLE(var) var = var

 /* Set the connection protocol to use. */
  //#define CONNECTION_TYPE LIBCT_DEVICE_CLASS_TCP
  //#define CONNECTION_TYPE LIBCT_DEVICE_CLASS_USB
#define CONNECTION_TYPE LIBCT_DEVICE_CLASS_BLE


/* Set TARGET_DEV_ADDRESS to the BLE or TCP address of the target device.
 * Or set TARGET_DEV_ADDRESS to ANY to connect to any device.
 */
//#define TARGET_DEV_ADDRESS "10.1.10.242" // Sample TCP address
//#define TARGET_DEV_ADDRESS "fc:f5:c4:3c:f5:aa" // Sample BLE address
#define TARGET_DEV_ADDRESS "ANY"

// Exit trigger
bool quit = false;

/******************************************************************************
 * LIBRARY CALLBACKS
 */

void LIBCTAPI on_device_discovered(libct_context_t* context, libct_device_t* device) {
    const char* name = libct_device_get_name(device);
    const char* address = libct_device_get_address(device);

    printf("%s: name=%s, address=%s, class=%d, state=0x%x\n",
        __func__,
        name,
        address,
        libct_device_get_class(device),
        libct_device_get_state(device)
    );

    if (
        strcmp(TARGET_DEV_ADDRESS, "ANY") == 0 ||
        strcmp(TARGET_DEV_ADDRESS, address) == 0
    ) {
        int status = libct_connect(context, device);
        if (status > LIBCT_STATUS_ERROR) {
            printf("%s : connecting...\n", __func__);
        }
        else {
            printf("%s : connect error=%d\n", __func__, status);
        }
    }
}

void LIBCTAPI on_discovery_timedout(libct_context_t* context) {
    UNREFERENCED_VARIABLE(context);

    printf("%s\n", __func__);
    quit = true;
}

void LIBCTAPI on_discovery_failed(libct_context_t* context, int error) {
    UNREFERENCED_VARIABLE(context);

    printf("%s : error=%d\n", __func__, error);
    quit = true;
}

void LIBCTAPI on_device_connected_ready(libct_context_t* context, libct_device_t* device) {
    UNREFERENCED_VARIABLE(device);

    /* start streaming data */
    int flags = (LIBCT_MONITOR_INT_PULSE |
        LIBCT_MONITOR_PARAM_PULSE |
        LIBCT_MONITOR_VITALS |
        LIBCT_MONITOR_VITALS2 |
        LIBCT_MONITOR_CUFF_PRESSURE |
        LIBCT_MONITOR_DEVICE_STATUS |
        LIBCT_MONITOR_BATTERY_INFO);

    libct_start_monitoring(context, flags);
}

void LIBCTAPI on_connect_error(libct_context_t* context, libct_device_t* device, const char* error) {
    UNREFERENCED_VARIABLE(context);
    UNREFERENCED_VARIABLE(device);

    printf("%s : error=%s\n", __func__, error);
    quit = true;
}

void LIBCTAPI on_connect_timedout(libct_context_t* context, libct_device_t* device) {
    UNREFERENCED_VARIABLE(context);
    UNREFERENCED_VARIABLE(device);

    printf("%s\n", __func__);
    quit = true;
}

void LIBCTAPI on_device_disconnected(libct_context_t* context, libct_device_t* device) {
    UNREFERENCED_VARIABLE(context);
    UNREFERENCED_VARIABLE(device);

    printf("%s\n", __func__);
}

void LIBCTAPI on_start_monitoring(libct_context_t* context, libct_device_t* device, int status) {
    UNREFERENCED_VARIABLE(context);
    UNREFERENCED_VARIABLE(device);

    printf("%s : status=%d\n", __func__, status);

    // start automatic calibration
    libct_cal_t cal_settings;
    cal_settings.type = LIBCT_AUTO_CAL;
    cal_settings.config.auto_cal.posture = LIBCT_POSTURE_SITTING;
    libct_start_measuring(context, &cal_settings);
}

void LIBCTAPI on_stop_monitoring(libct_context_t* context, libct_device_t* device, int status) {
    UNREFERENCED_VARIABLE(context);
    UNREFERENCED_VARIABLE(device);

    printf("%s : status=%d\n", __func__, status);
}

void LIBCTAPI on_start_measuring(libct_context_t* context, libct_device_t* device, int status) {
    UNREFERENCED_VARIABLE(context);
    UNREFERENCED_VARIABLE(device);

    printf("%s : status=%d\n", __func__, status);
}

void LIBCTAPI on_stop_measuring(libct_context_t* context, libct_device_t* device, int status) {
    UNREFERENCED_VARIABLE(context);

    printf("%s : status=%d\n", __func__, status);
}

void LIBCTAPI on_data_received(libct_context_t* context, libct_device_t* device, libct_stream_data_t* data) {
    UNREFERENCED_VARIABLE(context);
    UNREFERENCED_VARIABLE(device);

    if (data->device_status.valid) {
        printf("Device Status: \
               \n\tflags: %08lX\n",
            (long unsigned int) data->device_status.value);
    }

    libct_battery_info_t* battery_info = &data->battery_info;
    if (battery_info->valid) {
        printf("Battery Info: \
               \n\tvoltage: %d mV\n", battery_info->voltage);
    }

    unsigned int idx;

    int count = 0;
    libct_vitals_t* vitals = NULL;
    for_each_dp(data, idx, vitals, vitals) {
        if (vitals && vitals->valid) {
            count++;
            printf("Vitals DP%u\
                  \n\tsys: %d\
                  \n\tdia: %d\
                  \n\tmap: %d\
                  \n\thr: %d\
                  \n\tres: %d\
                  \n\tas: %d\
                  \n\tsqe: %d\n",
                idx,
                vitals->systolic,
                vitals->diastolic,
                vitals->map,
                vitals->heart_rate,
                vitals->respiration,
                vitals->as,
                vitals->sqe);
        }
    }

    count = 0;
    libct_vitals2_t* vitals2 = NULL;
    for_each_dp(data, idx, vitals2, vitals2) {
        if (vitals2 && vitals2->valid) {
            count++;
            printf("Vitals2 DP%u\
                  \n\tbv: %d\
                  \n\tco: %d\n",
                idx,
                vitals2->blood_volume,
                vitals2->cardiac_output);
        }
    }

    libct_cuff_pressure_t* cuff_pressure;
    for_each_dp(data, idx, cuff_pressure, cuff_pressure) {
        if (cuff_pressure && cuff_pressure->valid) {
            printf("Cuff Pressure DP%u\
                  \n\tvalue: %f\
                  \n\ttarget: %d\
                  \n\tsnr: %d\n",
                idx,
                cuff_pressure->value,
                cuff_pressure->target,
                cuff_pressure->snr);
        }
    }

    count = 0;
    short* sample;
    long long* timestamp;
    for_each_smpl(data, idx, sample, timestamp, raw_pulse) {
        count++;
    }
    if (count) {
        printf("Got %d raw pulse datapoints\n", count);
    }

    count = 0;
    for_each_smpl(data, idx, sample, timestamp, int_pulse) {
        count++;
    }
    if (count) {
        printf("Got %d int pulse datapoints\n", count);
    }
}

void LIBCTAPI on_data_error(libct_context_t* context, libct_device_t* device, const char* error) {
    UNREFERENCED_VARIABLE(context);
    UNREFERENCED_VARIABLE(device);

    printf("%s : error=%s\n", __func__, error);
}

void LIBCTAPI on_wrt_snr_min_rsp(libct_context_t* context, libct_device_t* device, int status) {
    UNREFERENCED_VARIABLE(context);
    UNREFERENCED_VARIABLE(device);

    printf("%s : status=%d\n", __func__, status);
}

void LIBCTAPI on_rd_snr_min_rsp(libct_context_t* context, libct_device_t* device, int snr, int status) {
    UNREFERENCED_VARIABLE(context);
    UNREFERENCED_VARIABLE(device);

    printf("%s : status=%d, snr=%d\n", __func__, status, snr);
}

void LIBCTAPI on_wrt_display_state_rsp(libct_context_t* context, libct_device_t* device, int status) {
    UNREFERENCED_VARIABLE(context);
    UNREFERENCED_VARIABLE(device);

    printf("%s : status=%d\n", __func__, status);
}

void LIBCTAPI on_rd_display_state_rsp(libct_context_t* context, libct_device_t* device, unsigned char state, int status) {
    UNREFERENCED_VARIABLE(context);
    UNREFERENCED_VARIABLE(device);

    printf("%s : status=%d, state=%d\n", __func__, status, state);
}

void LIBCTAPI on_wrt_recal_itvl_rsp(libct_context_t* context, libct_device_t* device, int status) {
    UNREFERENCED_VARIABLE(context);
    UNREFERENCED_VARIABLE(device);

    printf("%s : status=%d\n", __func__, status);
}

void LIBCTAPI on_rd_recal_itvl_rsp(libct_context_t* context, libct_device_t* device, unsigned int itvl, int status) {
    UNREFERENCED_VARIABLE(context);
    UNREFERENCED_VARIABLE(device);

    printf("%s : status=%d, itvl=%d\n", __func__, status, itvl);
}

void LIBCTAPI on_wrt_waveform_clamping(libct_context_t* context, libct_device_t* device, int status) {
    UNREFERENCED_VARIABLE(context);
    UNREFERENCED_VARIABLE(device);

    printf("%s : status=%d\n", __func__, status);
}

void LIBCTAPI on_wrt_median_filter(libct_context_t* context, libct_device_t* device, int status) {
    UNREFERENCED_VARIABLE(context);
    UNREFERENCED_VARIABLE(device);

    printf("%s : status=%d\n", __func__, status);
}

void LIBCTAPI on_rd_median_filter(libct_context_t* context, libct_device_t* device, int value, int status) {
    UNREFERENCED_VARIABLE(context);
    UNREFERENCED_VARIABLE(device);

    printf("%s : status=%d, value=%d\n", __func__, status, value);
}

void LIBCTAPI on_wrt_ambulatory_filter(libct_context_t* context, libct_device_t* device, int status) {
    UNREFERENCED_VARIABLE(context);
    UNREFERENCED_VARIABLE(device);

    printf("%s : status=%d\n", __func__, status);
}

void LIBCTAPI on_rd_ambulatory_filter(libct_context_t* context, libct_device_t* device, int value, int status) {
    UNREFERENCED_VARIABLE(context);
    UNREFERENCED_VARIABLE(device);

    printf("%s : status=%d, value=%d\n", __func__, status, value);
}

void LIBCTAPI on_diag_flush_rsp(libct_context_t* context, libct_device_t* device, int status) {
    UNREFERENCED_VARIABLE(context);
    UNREFERENCED_VARIABLE(device);

    printf("%s : status=%d\n", __func__, status);
}

void LIBCTAPI on_rd_cuff_pressure_rsp(libct_context_t* context, libct_device_t* device, int pressure, int status) {
    UNREFERENCED_VARIABLE(context);
    UNREFERENCED_VARIABLE(device);

    printf("%s : status=%d, pressure=%d\n", __func__, status, pressure);
}

void LIBCTAPI on_clr_status_rsp(libct_context_t* context, libct_device_t* device, int status) {
    UNREFERENCED_VARIABLE(context);
    UNREFERENCED_VARIABLE(device);

    printf("%s : status=%d\n", __func__, status);
}

void LIBCTAPI on_vent_cuff_rsp(libct_context_t* context, libct_device_t* device, int status) {
    UNREFERENCED_VARIABLE(context);
    UNREFERENCED_VARIABLE(device);

    printf("%s : status=%d\n", __func__, status);
}


/******************************************************************************
 */
int main(int argc, char** argv) {
    UNREFERENCED_VARIABLE(argc);
    UNREFERENCED_VARIABLE(argv);

    // Load libcaretaker_dynamic.dll
    if (!LOAD_LIBCARETAKER_DLL()) {
        printf("%s : libcaretaker_dynamic.dll load error", __func__);
        return 1;
    }

    // Setup library callbacks
    libct_app_callbacks_t app_callbacks;
    memset(&app_callbacks, 0, sizeof(app_callbacks));
    app_callbacks.on_device_discovered = on_device_discovered;
    app_callbacks.on_discovery_timedout = on_discovery_timedout;
    app_callbacks.on_discovery_failed = on_discovery_failed;
    app_callbacks.on_device_connected_ready = on_device_connected_ready;
    app_callbacks.on_connect_error = on_connect_error;
    app_callbacks.on_connect_timedout = on_connect_timedout;
    app_callbacks.on_device_disconnected = on_device_disconnected;
    app_callbacks.on_start_monitoring = on_start_monitoring;
    app_callbacks.on_stop_monitoring = on_stop_monitoring;
    app_callbacks.on_start_measuring = on_start_measuring;
    app_callbacks.on_stop_measuring = on_stop_measuring;
    app_callbacks.on_data_received = on_data_received;
    app_callbacks.on_data_error = on_data_error;
    app_callbacks.on_rd_snr_min_rsp = on_rd_snr_min_rsp;
    app_callbacks.on_wrt_snr_min_rsp = on_wrt_snr_min_rsp;
    app_callbacks.on_rd_display_state_rsp = on_rd_display_state_rsp;
    app_callbacks.on_wrt_display_state_rsp = on_wrt_display_state_rsp;
    app_callbacks.on_wrt_recal_itvl_rsp = on_wrt_recal_itvl_rsp;
    app_callbacks.on_rd_recal_itvl_rsp = on_rd_recal_itvl_rsp;
    app_callbacks.on_wrt_waveform_clamping = on_wrt_waveform_clamping;
    app_callbacks.on_wrt_median_filter = on_wrt_median_filter;
    app_callbacks.on_rd_median_filter = on_rd_median_filter;
    app_callbacks.on_wrt_ambulatory_filter = on_wrt_ambulatory_filter;
    app_callbacks.on_rd_ambulatory_filter = on_rd_ambulatory_filter;
    app_callbacks.on_diag_flush_rsp = on_diag_flush_rsp;
    app_callbacks.on_rd_cuff_pressure_rsp = on_rd_cuff_pressure_rsp;
    app_callbacks.on_clr_status_rsp = on_clr_status_rsp;
    app_callbacks.on_vent_cuff_rsp = on_vent_cuff_rsp;

    // Print build info
    libct_set_log_level(0);

    /* Initialize library context */
    libct_init_data_t init_data;
    init_data.device_class = CONNECTION_TYPE;
    libct_context_t* context = NULL;
    int status = libct_init(&context, &init_data, &app_callbacks);
    if (status <= LIBCT_STATUS_ERROR) {
        printf("%s : libct_init error : status=%d", __func__, status);
        UNLOAD_LIBCARETAKER_DLL();
        return 1;
    }

    // Start scanning, timeout after 5 minutes
    status = libct_start_discovery(context, 5 * 60000);
    if ( status <= LIBCT_STATUS_ERROR) {
        printf("%s : libct_start_discovery error : status=%d", __func__, status);
        libct_deinit(context);
        UNLOAD_LIBCARETAKER_DLL();
        return 1;
    }

    // Run indefinitely until console is closed
    for (; !quit; ) {
        Sleep(1000);
    }

    // Disconnect device
    libct_disconnect(context);

    // Release context
    libct_deinit(context);
    UNLOAD_LIBCARETAKER_DLL();
    return 0;
}

