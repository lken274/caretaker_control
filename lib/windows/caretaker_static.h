/************************************************************************************************
 * @file caretaker.h
 * Header file defining libcaretaker API to monitor Caretaker devices.
 * This is the main header file to include in your application to use libcaretaker.
 *
 * Version: 1.6.5
 * Build Date: Thu 10/11/2018 10:50:12.11
 *
 * Change History
 * --------------
 *
 * Date         Version     Comment
 * ----------   -------     -----------------------------------------------------------------------
 * 05-01-2-18   1.5.2       Initial release.
 * 07-26-2-18   1.5.3       Added Windows library bug fixes.
 * 08-02-2-18   1.6.1       Added Windows library support for TI CC2540 BLE dongles.
 */
#ifndef LIBCT_CARETAKER_H
#define LIBCT_CARETAKER_H
#include <stdbool.h>
#ifdef __cplusplus
extern "C" {
#endif
/**
 * \defgroup unmanaged_device Unmanaged Device Information
 * This modules describes the interface to retrieve general information about the Caretaker device.
 */
 /**
  * \defgroup unmanaged_primary_api Unmanaged Primary API
  * The group of primary functions that are required to connect to a Caretaker device and monitor data.
  */
/**
 * \defgroup unmanaged_secondary_api Unmanaged Secondary API
 * The group of auxiliary functions and macros available to parse, read and write additional Caretaker
 * device data.
 */
#ifndef LIBCTAPI
#if defined(_WIN32) || defined(__CYGWIN__) || defined(__MSYS__)
#include <windows.h>
/** 
 * The library calling convention.
 * Some platforms, such as Windows, support many calling conventions which specify the order that
 * function arguments are pushed onto and popped from the call stack, and how values are returned
 * from the called functions. The stack can be corrupted if the library and application do not use
 * the same calling convention. As such, LIBCTAPI is defined to abstract the underlying OS calling
 * convention, and it must be specified in the declarations and definitions of library functions and
 * callbacks exposed to the application to ensure the function caller (the application) and function
 * callee (libcaretaker) agree on the same calling convention to avoid corrupting the stack.
 */
#define LIBCTAPI WINAPI
/**
 * Static or dynamic linking on Windows
 * This macro is updated by the Windows build scripts to set it for static or dynamic linking
 * as follows.
 *
 * Dynamic Linking
 * \code
 * #define LIBCTEXPORT extern _declspec(dllimport)
 * \endcode
 *
 * Static Linking
 * \code
 * #define LIBCTEXPORT extern
 * \endcode
 */
#ifndef LIBCTEXPORT
#define LIBCTEXPORT extern
#endif
#else
/** 
 * The library calling convention.
 * Some platforms, such as Windows, support many calling conventions which specify the order that
 * function arguments are pushed onto and popped from the call stack, and how values are returned
 * from the called functions. The stack can be corrupted if the library and application do not use
 * the same calling convention. As such, LIBCTAPI is defined to abstract the underlying OS calling
 * convention, and it must be specified in the declarations and definitions of library functions and
 * callbacks exposed to the application to ensure the function caller (the application) and function
 * callee (libcaretaker) agree on the same calling convention to avoid corrupting the stack. */
#define LIBCTAPI
#define LIBCTEXPORT extern
#endif
#endif
/**
 * \class libct_status_t
 * Function return status codes.
 */
enum libct_status_t {
    /* NOTE: The common status codes (defined in libctcommon) are redefined here to avoid including
     * libctcommon header file libct.h in caretaker.h to enforce only one (1) header file (caretaker.h)
     * is deployed with the library to external customers.
     */
    /** Common status (CT_STATUS_OK) indicating success. */
    LIBCT_COMMON_STATUS_OK = 0,
    /** Common status (CT_STATUS_ERROR) indicating generic error. */
    LIBCT_COMMON_STATUS_ERROR = -1,
    /** Common status (CT_ASSERT_ERROR) indicating assertion error. */
    LIBCT_COMMON_ASSERT_ERROR = -2,
    /** Common status (CT_CHECKSUM_ERROR) indicating command checksum error. */
    LIBCT_COMMON_CHECKSUM_ERROR = -3,
    /* NOTE: Status codes >= -100 are reserved for libctcommon.
     * As such, libcaretaker specific status codes must be <= -101.
     */
    /** Function return status indicating success. */
    LIBCT_STATUS_OK = LIBCT_COMMON_STATUS_OK,
    /** Function return status indicating generic error. */
    LIBCT_STATUS_ERROR = LIBCT_COMMON_STATUS_ERROR,
    /** Function return status indicating invalid argument. */
    LIBCT_STATUS_ILLEGAL_PARAM_ERROR = -101,
    /** Function return status indicating failure to allocate memory. */
    LIBCT_STATUS_NOMEM_ERROR = -102,
    /** Function return status indicating error during initialization. */
    LIBCT_STATUS_INIT_ERROR = -103,
    /** Function return status indicating error sending data to the device. */
    LIBCT_STATUS_SEND_ERROR = -104,
    /** Function return status indicating error reading data from the device. */
    LIBCT_STATUS_READ_ERROR = -105,
    /** Function return status indicating error writing data to the device. */
    LIBCT_STATUS_WRITE_ERROR = -106,
};
/**
 * Checks a function return status for failure.
 * This macro returns true if the specified status indicates error, and return false otherwise.
 */
#define LIBCT_FAILED(status) ((status)<=LIBCT_STATUS_ERROR)
 /**
 * Checks a function return status for success.
 * This macro returns true if the specified status indicates success, and return false otherwise.
 */
#define LIBCT_SUCCEEDED(status) ((status)>=LIBCT_STATUS_OK)
/**
 * \class libct_device_state_t
 * The Caretaker device states.
 */
enum libct_device_state_t {
    /**
     * State flag indicating library initialization is not complete for the specified context.
     */
    LIBCT_STATE_UNITIALIZED = (1<<0),
    /**
     * State flag indicating the device is being discovered, i.e., the library is actively scanning for the device.
     */
    LIBCT_STATE_DISCOVERING = (1<<1),
    /**
     * State flag indicating a connection is being established with the device.
     */
    LIBCT_STATE_CONNECTING = (1<<2),
    /**
     * State flag indicating an active connection is established with the device.
     */
    LIBCT_STATE_CONNECTED = (1<<3),
    /**
     * State flag indicating the connection to the device is disconnecting.
     */
    LIBCT_STATE_DISCONNECTING = (1<<4),
    /**
     * State flag indicating a previously connected device is now disconnected.
     */
    LIBCT_STATE_DISCONNECTED = (1<<5),
    /**
     * State flag indicating the application the device is producing data.
     * @note Vitals data is being produced only while the LIBCT_STATE_MEASURING flag is set.
     */
    LIBCT_STATE_MONITIORING = (1<<6),
    /**
     * State flag indicating the device is producing vitals data.
     */
    LIBCT_STATE_MEASURING = (1<<7),
};
/**
 * \class libct_device_class_t
 * Classes of devices that can be monitored by this library.
 */
enum libct_device_class_t {
    /**
     * Device class specifying unknown devices.
     * @note This class is used for error handling so you should not specify it when
     *       initializing a context.
     */
    LIBCT_DEVICE_CLASS_UNKOWN=0,
    /** Device class specifying a Bluetooth Low Energy (BLE) CareTaker4 device. */
    LIBCT_DEVICE_CLASS_BLE_CARETAKER4 =1,
};
/**
 * \class libct_cal_type_t
 * The Caretaker calibration types.
 */
enum libct_cal_type_t {
    /** Automatic calibration using internal blood pressure settings. */
    LIBCT_AUTO_CAL =1,
    /** Manual calibration requiring external blood pressure settings. */
    LIBCT_MANUAL_CAL =2
};
/**
 * \class libct_posture_t
 * Patient postures.
 */
enum libct_posture_t {
    /** Medical posture when the patient is sitting. */
    LIBCT_POSTURE_SITTING=1,
    /** Medical posture when the patient is supine. */
    LIBCT_POSTURE_SUPINE=3,
};
/**
 * \class libct_monitor_flags_t
 * Data monitor flags passed to libct_start_monitoring()
 */
enum libct_monitor_flags_t {
    /**
     * \ingroup libct_monitor_flags_t
     * Set this flag in the flags passed to libct_start_monitoring() to stream the pulse pressure 
     * (integrated pulse) waveform from the device. Note the raw pulse waveform will be derived
     * locally from the pulse pressure waveform and the application will receive both the \ref 
     * libct_stream_data_t.raw_pulse "raw pulse" and \ref libct_stream_data_t.int_pulse "integrated 
     * pulse" waveform data in \ref libct_stream_data_t "stream data" packets delivered to the application
     * \ref libct_app_callbacks_t.on_data_received "on_data_received()" callback function when
     * this flag is set.
     */
    LIBCT_MONITOR_INT_PULSE = (1<<0),
    /**
     * Set this flag in the flags passed to libct_start_monitoring() to stream the pulse rate (raw pulse)
     * waveform from the device. Note the pulse pressure (integrated pulse) waveform will be derived locally 
     * from the pulse rate waveform and the application will receive both the \ref libct_stream_data_t.raw_pulse
     * "raw pulse" and \ref libct_stream_data_t.int_pulse "integrated pulse" waveform data in
     * \ref libct_stream_data_t "stream data" packets delivered to the application
     * \ref libct_app_callbacks_t.on_data_received "on_data_received()" callback function when this
     * flag is set.
     */
    LIBCT_MONITOR_RAW_PULSE = (1<<1),
    /**
     * Set this flag in the flags passed to libct_start_monitoring() to stream parameterize waveform
     * from the device. The application will receive \ref libct_stream_data_t.param_pulse
     * "parameterize pulse" waveform data in \ref libct_stream_data_t "stream data" packets delivered
     * to the application \ref libct_app_callbacks_t.on_data_received "on_data_received()" callback
     * function when this flag is set.
     */
    LIBCT_MONITOR_PARAM_PULSE = (1<<2),
    /**
     * Set this flag in the flags passed to libct_start_monitoring() to stream
     * vitals data from the device. The application will receive \ref libct_stream_data_t.vitals
     * "vitals" data in \ref libct_stream_data_t "stream data" packets delivered to the application
     * \ref libct_app_callbacks_t.on_data_received "on_data_received()" callback function when this
     * flag is set.
     */
    LIBCT_MONITOR_VITALS = (1<<3),
    /**
     * Set this flag in the flags passed to libct_start_monitoring() to stream secondary vitals
     * from the device. The application will receive \ref libct_stream_data_t.vitals2
     * "vitals2" data in \ref libct_stream_data_t "stream data" packets delivered to the
     * application \ref libct_app_callbacks_t.on_data_received "on_data_received()" callback
     * function when this flag is set.
     *
     * @note The secondary vitals are for internal use or research only.
     */
    LIBCT_MONITOR_VITALS2 = (1<<4),
    /**
     * Set this flag in the flags passed to libct_start_monitoring() to stream pulse oximetry data
     * from the device. The application will receive \ref libct_stream_data_t.pulse_ox
     * "pulse ox" data in \ref libct_stream_data_t "stream data" packets delivered to the application
     * \ref libct_app_callbacks_t.on_data_received "on_data_received()" callback function when this
     * flag is set.
     * @note The caretaker does not report pulse oximetry data today but may in the future.
     * @note Reserved for future use.
     */
    LIBCT_MONITOR_PULSE_OX = (1<<5),
    /**
     * Set this flag in the flags passed to libct_start_monitoring() to stream temperature data from
     * the device. The application will receive \ref libct_stream_data_t.temperature "temperature"
     * data in \ref libct_stream_data_t "stream data" packets delivered to the application
     * \ref libct_app_callbacks_t.on_data_received "on_data_received()" callback function when this
     * flag is set.
     * @note the caretaker does not report temperature data today but may in the future.
     * @note Reserved for future use.
     */
    LIBCT_MONITOR_TEMPERATURE = (1<<6),
    /**
     * Set this flag in the flags passed to libct_start_monitoring() to stream cuff pressure
     * data from the device. The application will receive \ref libct_stream_data_t.cuff_pressure
     * "cuff pressure" data in \ref libct_stream_data_t "stream data" packets delivered to the
     * application \ref libct_app_callbacks_t.on_data_received "on_data_received()" callback
     * function when this flag is set.
     */
    LIBCT_MONITOR_CUFF_PRESSURE = (1<<7),
    /**
     * Set this flag in the flags passed to libct_start_monitoring() to stream device status flags
     * from the device. The application will receive \ref libct_stream_data_t.device_status
     * "device status" data in \ref libct_stream_data_t "stream data" packets delivered to the
     * application \ref libct_app_callbacks_t.on_data_received "on_data_received()" callback
     * function when this flag is set.
     */
    LIBCT_MONITOR_DEVICE_STATUS = (1<<8),
    /**
     * Set this flag in the flags passed to libct_start_monitoring() to stream battery information
     * from the device. The application will receive \ref libct_stream_data_t.battery_info
     * "battery info" data in \ref libct_stream_data_t "stream data" packets delivered to the
     * application \ref libct_app_callbacks_t.on_data_received "on_data_received()" callback
     * function when this flag is set.
     */
    LIBCT_MONITOR_BATTERY_INFO = (1<<9),
    /**
     * Set this flag in the flags passed to libct_start_monitoring() to stream calibration curve
     * data from the device. The application will receive \ref libct_stream_data_t.cal_curve
     * "calibration curve" data in \ref libct_stream_data_t "stream data" packets delivered to the
     * application \ref libct_app_callbacks_t.on_data_received "on_data_received()" callback
     * function when this flag is set.
     *
     * @note The cal curve data is for internal use or research only.
     */
    LIBCT_MONITOR_CAL_CURVE_DATA = (1<<10),
    #if 0 /* not implemented */
    /**
     * Set this flag in the flags passed to libct_start_monitoring() to stream blood pressure
     * calibrated settings at the device. The application will receive
     * \ref libct_stream_data_t.bp_calib_params "calibration data" in \ref libct_stream_data_t
     * "stream data" packets delivered to the application \ref libct_app_callbacks_t.on_data_received
     * "on_data_received()" callback function when this flag is set.
     */
    LIBCT_MONITOR_BP_CAL_PARAMS = (1<<11),
    #endif
};
/**
 * \class libct_context_t
 * An opaque type representing a library instance associated with (or bound to) a device the application is monitoring.
 * The context is used internally to manage the library instance so its data structure is not exposed to the application.
 * As such, the application cannot create a library context explicitly. A library context can only be created by
 * calling libct_init() to initialize a library instance, which sets the context pointer passed in the first
 * argument. If the call succeeded, the application can use the context to call other library functions, but
 * must call libct_deinit() to destroy the context when it is no longer needed. Destroying the context releases
 * resources that were allocated when the context was initialized, so the application is required to call libct_deinit()
 * to release the context, and not doing so will leak system resources.
 *\code
 * // Initialize library instance, which returns a device context pointer.
 * libct_context_t* context = NULL;
 * int status = libct_init(&context, &init_data, &app_callbacks);
 * if ( LIBCT_FAILED(status) {
 *     // Handle error
 *     return status;
 * }
 *
 * // Connect to a device and monitor data (code not shown)
 *
 * // Destroy context
 *  libct_deinit(context);
 *\endcode
 */
typedef struct libct_context_t libct_context_t;
/* Forward declarations */
struct libct_device_t;
typedef struct libct_device_t libct_device_t;
/**
 * \class libct_bp_settings_t
 * Structure to write the caretaker manual blood pressure settings.
 */
typedef struct libct_bp_settings_t {
    /**
     * Systolic pressure setting used for blood pressure calibration.
     * Acceptable range [30, 250].
     */
    unsigned short systolic;
    /**
     * Diastolic pressure setting used for blood pressure calibration.
     * Acceptable range [10, 150].
     */
    unsigned short diastolic;
}  libct_bp_settings_t;
/**
 * \class libct_device_status_t
 * Device status data point within the libct_stream_data_t packet.
 */
// MUST match CTAppDeviceStatus defined in libct.h exactly.
typedef struct libct_device_status_t {
    /**
     * The other fields are valid when this field is non-zero (true) and invalid otherwise.
     */
    bool valid;
    /**
     * Integer value representing logically OR of all status flags, which
     * essentially is the raw value from the device.
     */
    long long value;
    /** An indicator of whether the system PDA measurement system is enabled */
    bool pda_enabled;
    /** An indicator of whether the system is in simulation mode. */
    bool simulation_enabled;
    /**
     * An indicator of whether the system is currently running closed
     * loop pressure control.
     */
    bool pressure_control_indicator;
    /** An indicator of whether the system has been inflated to pressure. */
    bool inflated_indicator;
    /** The system clock (time since reset) has wrapped around its index. */
    bool clock_wrap_around;
    /** The battery voltage sensor has indicated the battery is near drop-out. */
    bool battery_voltage_low;
    /** The on-board temperature sensor has detected critically high temperature */
    bool critical_temperature;
    /** The pump has violated an overrun condition. */
    bool pump_overrun;
    /** The BLE Temperature Sensor is paired and actively communicating with CareTaker. */
    bool ble_temperature_sensor_paired;
    /** The a BLE hand-held device is paired and actively communicating with CareTaker */
    bool ble_handheld_paired;
    /** TThe current stream control status bit of the BLE stream. */
    bool ble_stream_control;
    /** The current stream control status bit of the cellular stream */
    bool cellular_control;
    /** The current stream control status bit of the serial stream */
    bool serial_stream_control;
    /** The system has been started and running in auto-calibration mode */
    bool auto_cal_mode;
    /** The system has been started and running in manual calibration mode. */
    bool manual_cal_mode;
    /** The system is having trouble getting a good reading due to too much motion. */
    bool motion_event;
    /**
     * The system failed to calibrate or timed out process signals so
     * measurements were aborted.*/
    bool poor_signal;
    /**
     * There are valid vital signs measurements.
     * This is used to notify the GUI if data should be displayed or hidden.*/
    bool data_valid;
    /** The system is currently calibrating the blood pressure system */
    bool calibrating;
    /**  The system has current valid calibration */
    bool calibrated;
    /**
     * The system has finished finding the oscillometric curve and is processing
     * the beta (offset) value.
     */
    bool beta_processing;
    /**
     * Cuff did not inflate to expected value within timeout.
     */
    bool inflate_failed;
    /**
     * The calibration values were out of range or oscillometric curve had
     * invalid shape.
     */
    bool calibration_failed;
    /**
     * Too much movement. The calibration offset calculation failed to identify
     * pulses due to movement.
     */
    bool calibration_offset_failed;
    /** The systems has gone greater than 3 minutes without a valid heart beat */
    bool no_pulse_timeout;
    /** The calibration pump up identified the cuff was too loose */
    bool cuff_too_loose;
    /** The calibration pump up identified the cuff was too tight */
    bool cuff_too_tight;
    /** Calibration oscillometric curve amplitude is too weak to verify reading. */
    bool weak_signal;
    /** The cuff is not holding pressure as expected. */
    bool bad_cuff;
    /** The Bluetooth module is advertising. */
    bool ble_adv;
    /** An automatic recalibration will be occurring shortly. */
    bool recal_soon;
    /** Auto-calibration failed too many consecutive times try manual calibration. */
    bool too_many_fails;
    /** Auto-calibration percentage complete */
    short autocal_pct;
    /** The device is charging */
    bool charging;
    /** Charging complete */
    bool chargeComplete;
    /** Posture */
    short posture;
	/** Invalid input received in the last command. */
	bool invalid_data_entry;
} libct_device_status_t;
/**
 * \class libct_temperature_t
 * Temperature data point within the libct_stream_data_t packet.
 * @note Reserved for future use.
 */
// MUST match CTAppTempData structure defined in libct.h exactly.
typedef struct libct_temperature_t {
    /**
     * The other fields are valid when this field is non-zero (true) and invalid otherwise.
     */
    bool valid;
    /** The temperature value. */
    int value;
    /** Time stamp from the device associated with the data. */
    unsigned int timestamp;
} libct_temperature_t;
/**
 * \class libct_battery_info_t
 * Battery info data point within the libct_stream_data_t packet.
 */
// MUST match CTAppBatteryData defined in libct.h exactly.
typedef struct libct_battery_info_t {
    /** The other fields are valid when this field is non-zero (true) and invalid otherwise */
    bool valid;
    /** The battery voltage in millivolts. */
    int voltage;
    /** Time stamp from the device associated with the data. */
    unsigned int timestamp;
} libct_battery_info_t;
/**
 * \class libct_pulse_ox_t
 * Pulse oximetry data point within the libct_stream_data_t packet.
 * @note Reserved for future use.
 */
// PulseOximetry MUST match CTAppPulseOxiData byte format defined in libct.h exactly.
typedef struct libct_pulse_ox_t {
    /**
     * The other fields are valid when this field is non-zero (true) and invalid otherwise.
     */
    bool valid;
    /** Blood oxygen level (percentage). */
    int sao2;
    /** Pulse rate in beats per minute (30-200BPM). */
    int pulse_rate;
    /** Time stamp from the device associated with the data. */
    unsigned int timestamp;
} libct_pulse_ox_t;
/**
 * \class libct_vitals2_t
 * Secondary Vitals data point within the libct_stream_data_t packet.
 * @note The secondary vitals are for internal use or research only.
 */
// Secondary Vitals MUST match CTAppVitals2Data byte format defined in libct.h exactly.
typedef struct libct_vitals2_t  {
    /**
     * The other fields are valid when this field is non-zero (true) and invalid otherwise.
     */
    bool valid;
    /** Blood volume in mS. */
    unsigned short blood_volume;
    /** Cardiac output in L/min */
    unsigned char cardiac_output;
    /** Inter-beat Interval in mS */
    unsigned short ibi;
    /** Left ventricular ejection time */
    unsigned short lvet;
    /** P ratio */
    float p2p1;
    /** hrComp */
    float hrComp;
    /** pr */
    float pr;
    /** Reserved for future use */
    int reserved[7];
 
    /** Time stamp from the device associated with the data. */
    unsigned int timestamp;
 
} libct_vitals2_t;
/**
 * \class libct_pulse_t
 * Raw or integrated pulse data point within the libct_stream_data_t packet.
 */
// PulseDatapoint MUST match CTAppPulseDatapoint byte format defined in libct.h exactly.
typedef struct libct_pulse_t {
    /**
     * The other fields are valid when this field is non-zero (true) and invalid otherwise.
     */
    bool valid;
    /** Pulse value */
    short value;
    /** Counter value associated with the pulse value. */
    unsigned int timestamp;
} libct_pulse_t;
/**
 * \class libct_param_pulse_t
 * Parametrized pulse data within the libct_stream_data_t packet.
 * The parametrized pulse data is an aggregate of the pulse parameters and pulse snapshot waveform data.
 */
/* CAUTION:
 * The parameter fields MUST match CTAppParamPulseWaveform byte format defined in libct.h exactly,
 * but the waveform fields differ slightly. libct_param_pulse_t adds an extra waveform_len field
 * to pass the length of the waveform data to the application. So libct_param_pulse_t and CTAppParamPulseWaveform
 * are not strictly byte equivalent.
 */          
typedef struct libct_param_pulse_t {
    /** The other fields are valid when this field is non-zero (true) and invalid otherwise. */
    bool valid;
    /**  Date transfer control byte used internally to assemble the data. */
    short protocol_header;
    /** Pulse onset time (index).  */
    short t0;
    /** First pulse peak time (index). */
    short t1;
    /** Second pulse peak time (index).  */
    short t2;
    /** Third pulse peak time (index). */
    short t3;
    /** Integrated pulse onset value. */
    int p0;
    /** First integrated pulse peak value. */
    int p1;
    /** Second integrated pulse peak value. */
    int p2;
    /** Third integrated pulse peak value. */
    int p3;
    /** Inter-beat interval (1/HR) in samples @ 500Hz.  */
    short ibi;
    /** Arterial stiffness. */
    short as;
    /** Signal quality estimate. */
    short sqe;
    /** The most recent raw ADC cuff pressure. */
    short pressure;
    /** Relative system time of occurrence. */
    unsigned int time;
    /** The number of signed int8 snapshot data points. */
   int waveform_len;
    /* suppress nonstandard extension used: zero-sized array in struct/union warning below */
    #if _MSC_VER
    #pragma warning( disable : 4200 )
    #endif
    /** The pulse snapshot waveform data */
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
    char waveform[]; /* valid C99 code */
#else
    char waveform[0]; /* non-standard, but usually working code. */
#endif
} libct_param_pulse_t;
/**
 * \class libct_vitals_t
 * Vitals data point within the libct_stream_data_t packet.
 */
// MUST match CTAppVitalSignsData byte format defined in libct.h exactly.
typedef struct libct_vitals_t {
    /** The other fields are valid when this field is non-zero (true) and invalid otherwise. */
    bool valid;
    /**
     * An indicator if a valid blood pressure was found or if the algorithm failed.
     * True indicates pulse information is valid.
     */
    bool bp_status;
    /**
     * An indicator of if a valid MAP measurement has been integrated.
     */
    bool map_status;
    /**
     * An indicator if a valid HR has been determined.
     */
    bool hr_status;
    /**
     * An indicator if a valid respiration reading was found.
     */
    bool respiration_status;
    /**
     * General catchall for integration errors.
     */
    bool integration_error;
    /**
     * A discontinuity was detected in the differentiation.
     */
    bool differentiation_error;
    /**
     * Unable to locate P1 P2 within the pulse.
     */
    bool p12_finder_error;
    /**
     * Unable to locate P3 within the pulse.
     */
    bool p3_finder_eError;
    /**
     * The onset of the pulse was not found in the allowable window,
     * so the values are being discarded.
     */
    bool min_index_out_of_range;
    /**
     * The index of the minimum point in the integral was out of range.
     */
    bool max_index_out_of_range;
    /**
     * The slope correction of the integrated pulse was out of range.
     */
    bool slope_out_of_range;
    /** Systolic measurement. */
    short systolic;
    /** Diastolic measurement. */
    short diastolic;
    /** Mean arterial pressure value. */
    short map;
    /** Heart rate measurement. */
    short heart_rate;
    /** Respiration measurement. */
    short respiration;
    /** AS factor. */
    short as;
    /**
     * Signal quality estimate (sqe).
     * Values are in the range [0, 1000], so the sqe can be expressed relatively as
     * a percentage by dividing by 10, .i.e. sqe/10 %.*/
    short sqe;
    /** Time stamp from the device associated with the data. */
    unsigned int timestamp;
} libct_vitals_t;
/**
 * \class libct_cuff_pressure_t
 * Cuff pressure data point within the libct_stream_data_t packet.
 */
// MUST match CTAppCuffPressureData byte format defined in libct.h exactly.
typedef struct libct_cuff_pressure_t {
    /** The other fields are valid when this field is non-zero (true) and invalid otherwise. */
    bool valid;
    /** cuff pressure actual value. */
    int value;
    /** cuff pressure target value. */
    int target;
    /** signal to noise ratio. */
    int snr;
    /** Time stamp from the device associated with the data. */
    unsigned int timestamp;
} libct_cuff_pressure_t;
/**
 * \class libct_cal_curve_t
 * Calibration curve data point within the libct_stream_data_t packet.
 *  @note The cal curve data is for internal use or research only
 */
// MUST match CTAppCalCurveData byte format defined in libct.h exactly.
typedef struct libct_cal_curve_t {
    /** The other fields are valid when this field is non-zero (true) and invalid otherwise. */
    bool valid;
    /** Data ID. */
    int data_id;
    /** Value 1. */
    float val1;
    /** Value 2. */
    float val2;
    /** Value 3. */
    float val3;
    /** Alternate Data - does not come from device */
    char *alternateData;
} libct_cal_curve_t;
/**
 * \class libct_stream_data_t
 * This structure is used to hand-off data received from the remote device to the application.
 *
 * Data from the device is sent automatically after calling libct_start_monitoring() successfully,
 * and delivered to your application via the \ref libct_app_callbacks_t.on_data_received
 * "on_data_received()" callback function. This data structure is a container of arrays grouping one
 * or more records of the same data type at different time instances. The various array data types
 * are not produced coherently at the device so not all fields will be populated in stream data
 * packets delivered to the application. If no data is available for a given array, the array
 * data points field will be set to null and the count set to zero to signal no data.
 *
 * The stream data packets notified to the application depends on the \ref libct_monitor_flags_t
 * "monitor flags" passed to libct_start_monitoring() and whether or not libct_start_measuring() was
 * called to start taking vital sign measurements. So you can control the data reported to the
 * application by specifying only the monitoring flags corresponding to the data you care about.
 *
 * With the exception of the \ref libct_stream_data_t.device_status "device_status" and
 * \ref libct_stream_data_t.battery_info "battery_info" data members that are not array fields, the
 * following convenience macros are available to access array entries within the stream data packet.
 * More details about usage is provided in the description for each stream data member where the
 * macros apply.
 * <ul>
 * <li>libct_dp_count()
 * <li>libct_get_dp()
 * <li>libct_get_first_dp()
 * <li>libct_get_last_dp()
 * <li>for_each_dp()
 * </ul>
 */
// MUST match CTAppStreamData byte format defined in libct.h exactly
typedef struct libct_stream_data_t {
    /** Reference to the device that generated this data. */
    libct_device_t* device;
    /** Device status information. */
    libct_device_status_t device_status;
    /** Battery information. */
    libct_battery_info_t battery_info;
    /**
     * Array of vital sign data points.
     * For convenience, you can use the macros libct_get_last_dp(), libct_get_first_dp(), and
     * libct_get_dp() to extract a single vital sign data point from the stream packet like so.
     * \code
     *      libct_vitals_t* dp = libct_get_last_dp(data, vitals);
     *      if ( dp && dp->valid ) {
     *          // use most recent vitals data point
     *      }
     * \endcode
     * <p>
     * Alternatively, you could iterate over all vital sign data points with the for_each_dp() macro
     * like so.
     * \code
     *      libct_vitals_t* dp;
     *      unsigned int idx;
     *      for_each_dp(data, idx, dp, vitals) {
     *          if ( dp && dp->valid ) {
     *              // use vitals data point
     *          }
     *      }
     * \endcode
     */
    struct {
        /** Array of vital sign datapoints. */
        libct_vitals_t* datapoints;
        /** The count of datapoints. */
        unsigned int count;
    } vitals;
    /**
     * Array of cuff pressure data points.
     * For convenience, you can use the macros libct_get_last_dp(), libct_get_first_dp(), and
     * libct_get_dp() to extract a single cuff pressure data point from the stream packet like so.
     * \code
     *      libct_cuff_pressure_t* dp = libct_get_last_dp(data, cuff_pressure);
     *      if ( dp && dp->valid ) {
     *          // use most recent cuff pressure data point
     *      }
     * \endcode
     * <p>
     * Alternatively, you could iterate over all cuff pressure data points with the for_each_dp() macro
     * like so.
     * \code
     *      libct_cuff_pressure_t* dp;
     *      unsigned int idx;
     *      for_each_dp(data, idx, dp, cuff_pressure) {
     *          if ( dp && dp->valid ) {
     *              // use cuff pressure data point
     *          }
     *      }
     * \endcode
     */
    struct {
        /** Array of cuff pressure data points. */
        libct_cuff_pressure_t* datapoints;
        /** The count of data points. */
        unsigned int count;
    } cuff_pressure;
    /**
     * Array of temperature data points.
     * For convenience, you can use the macros libct_get_last_dp(), libct_get_first_dp(), and
     * libct_get_dp() to extract a single temperature data point from the stream packet like so.
     * \code
     *      libct_temperature_t* dp = libct_get_last_dp(data, temperature);
     *      if ( dp && dp->valid ) {
     *          // use most recent temperature data point
     *      }
     * \endcode
     * <p>
     * Alternatively, you could iterate over all temperature data points with the for_each_dp() macro
     * like so.
     * \code
     *      libct_temperature_t* dp;
     *      unsigned int idx;
     *      for_each_dp(data, idx, dp, temperature) {
     *          if ( dp && dp->valid ) {
     *              // use temperature data point
     *          }
     *      }
     * \endcode
     */
    struct {
        /** Array of temperature data points. */
        libct_temperature_t* datapoints;
        /** The count of data points. */
        unsigned int count;
    } temperature;
    /**
     * Array of pulse oximetry data points.
     * For convenience, you can use the macros libct_get_last_dp(), libct_get_first_dp(), and
     * libct_get_dp() to extract a single spo2 data point from the stream packet like so.
     * \code
     *      libct_pulse_ox_t* dp = libct_get_last_dp(data, pulse_ox);
     *      if ( dp && dp->valid ) {
     *          // use most recent spo2 data point
     *      }
     * \endcode
     * <p>
     * Alternatively, you could iterate over all spo2 data points with the for_each_dp()
     * macro like so.
     * \code
     *      libct_pulse_ox_t* dp;
     *      unsigned int idx;
     *      for_each_dp(data, idx, dp, pulse_ox) {
     *          if ( dp && dp->valid ) {
     *              // use spo2 data point
     *          }
     *      }
     * \endcode
     */
    struct {
        /** Array of spo2 data points. */
        libct_pulse_ox_t* datapoints;
        /** The count of data points. */
        unsigned int count;
    } pulse_ox;
    /**
     * Array of secondary vital sign data points.
     * <p>
     * @note The secondary vitals are for internal use or research only.
     * <p>
     * For convenience, you can use the macros libct_get_last_dp(), libct_get_first_dp(), and
     * libct_get_dp() to extract a single secondary vital sign data point from the stream packet.
     * \code
     *      libct_vitals2_t* dp = libct_get_last_dp(data, vitals2);
     *      if ( dp && dp->valid ) {
     *          // use most recent secondary vitals data point
     *      }
     * \endcode
     * <p>
     * Alternatively, you could iterate over all secondary vital sign data points with the for_each_dp()
     * macro like so.
     * \code
     *      libct_vitals2_t* dp;
     *      unsigned int idx;
     *      for_each_dp(data, idx, dp, vitals) {
     *          if ( dp && dp->valid ) {
     *              // use secondary vitals data point
     *          }
     *      }
     * \endcode
     */
    struct {
        /** Array of secondary vital sign data points. */
        libct_vitals2_t* datapoints;
        /** The count of data points. */
        unsigned int count;
    } vitals2;
    /**
     * Array of raw pulse (pulse rate) waveform data points.
     * For convenience, you could iterate over all raw pulse data points with the for_each_dp() macro
     * like so.
     * \code
     *      libct_pulse_t* dp;
     *      unsigned int idx;
     *      for_each_dp(data, idx, dp, raw_pulse) {
     *          if ( dp && dp->valid ) {
     *              // use pulse data point
     *          }
     *      }
     * \endcode
     */
    struct {
        /** Array of raw pulse (pulse rate) data points. */
        libct_pulse_t* datapoints;
        /** The count of data points. */
        unsigned int count;
    } raw_pulse;
    /**
     * Array of integrated pulse (pulse pressure) waveform data points.
     * For convenience, you could iterate over all integrated pulse data points with the
     * for_each_dp() macro like so.
     * \code
     *      libct_pulse_t* dp;
     *      unsigned int idx;
     *      for_each_dp(data, idx, dp, int_pulse) {
     *          if ( dp && dp->valid ) {
     *              // use pulse data point
     *          }
     *      }
     * \endcode
     */
    struct {
        /** Array of integrated pulse (pulse pressure) data points. */
        libct_pulse_t* datapoints;
        /** The count of data points. */
        unsigned int count;
    } int_pulse;
    /**
     * Array of parameterized pulse data.
     * The data is an aggregate of the pulse parameters and pulse snapshot waveform data.
     * <p>
     * For convenience, you can use the macros libct_get_last_dp(), libct_get_first_dp(), and
     * libct_get_dp() to extract a single pulse snapshot from the stream packet like so.
     * \code
     *      libct_param_pulse_t* dp = libct_get_last_dp(data, param_pulse);
     *      if ( dp && dp->valid ) {
     *          // use most recent pulse snapshot
     *      }
     * \endcode
     * <p>
     * Alternatively, you could iterate over all pulse snapshot data points with the for_each_dp()
     * macro like so.
     * \code
     *      libct_param_pulse_t* dp;
     *      unsigned int idx;
     *      for_each_dp(data, idx, dp, pulse_param) {
     *          if ( dp && dp->valid ) {
     *              // use pulse snapshot
     *          }
     *      }
     * \endcode
     */
    struct {
        /** Array of parameterize pulse (pulse snapshot) data points. */
        libct_param_pulse_t* datapoints;
        /** The count of data points. */
        unsigned int count;
    } param_pulse;
    /**
     * Array of calibration curve data points.
     * <p>
     * @note The cal curve data is for internal use or research only
     * <p>
     * For convenience, you can use the macros libct_get_last_dp(), libct_get_first_dp(), and
     * libct_get_dp() to extract a single calibration curve data point from the stream packet like so.
     * \code
     *      libct_cal_curve_t* dp = libct_get_last_dp(data, cal_curve);
     *      if ( dp && dp->valid ) {
     *          // use most recent calibration curve data point
     *      }
     * \endcode
     * <p>
     * Alternatively, you could iterate over all calibration curve data points with the for_each_dp()
     * macro like so.
     * \code
     *      libct_cal_curve_t* dp;
     *      unsigned int idx;
     *      for_each_dp(data, idx, dp, cal_curve) {
     *          if ( dp && dp->valid ) {
     *              // use calibration curve data point
     *          }
     *      }
     * \endcode
     */
    struct {
        /** Array of calibration curve data points. */
        libct_cal_curve_t* datapoints;
        /** The count of data points. */
        unsigned int count;
    } cal_curve;
    /**
     * Value of local clock (in milliseconds) when this stream packet was received.
     * This time stamp is used to measure processing latency and for history logging.
     * It differs from time stamp found in each data point generated by the remote device.
     */
    long receive_time;
} libct_stream_data_t;
/**
 * \ingroup unmanaged_secondary_api
 *
 * Returns the count of data points of the specified member array contained in \ref libct_stream_data_t
 * "stream data" received at the application.
 *
 * @param data The stream data received in your \ref libct_app_callbacks_t.on_data_received
 *             "on_data_received()" application callback.
 * @param memb The \ref libct_stream_data_t "stream data" member whose data point count is being
 *             queried.
 *
 * @return The extracted data point on success, and null on failure.
 */
#define libct_dp_count(data, memb) (data)->memb.count
/**
 * \ingroup unmanaged_secondary_api
 *
 * Extract a single data point from the specified member array contained in \ref libct_stream_data_t "stream data"
 * received at the application.
 *
 * @param data The stream data received in your \ref libct_app_callbacks_t.on_data_received
 *             "on_data_received()" application callback.
 * @param memb The \ref libct_stream_data_t "stream data" member name of the data point to
 *             extract.
 * @param pos  The position of the data point to extract.
 *
 * @return The extracted data point on success, and null on failure.
 */
#ifdef _MSC_VER
#define libct_get_dp(data, memb, pos) (((data)->memb.count && (pos) < (data)->memb.count) ? &(data)->memb.datapoints[(pos)] : NULL)
#else
#define libct_get_dp(data, memb, pos) ({\
    __typeof__((data)->memb.datapoints[0]) *dp = NULL; \
    if ( (data)->memb.count && (pos) < (data)->memb.count ) { \
        dp = &(data)->memb.datapoints[(pos)]; \
    } \
    (dp);\
})
#endif
/**
 * \ingroup unmanaged_secondary_api
 *
 * Extract the newest data point from the specified member array contained in \ref libct_stream_data_t "stream data"
 * received at the application.
 *
 * @param data The stream data received in your \ref libct_app_callbacks_t.on_data_received
 *             "on_data_received()" application callback.
 * @param memb The \ref libct_stream_data_t "stream data" member name of the data point to
 *             extract.
 *
 * @return The extracted data point on success, and null on failure.
 */
#define libct_get_last_dp(data, memb) libct_get_dp(data, memb, (data)->memb.count-1)
/**
 * \ingroup unmanaged_secondary_api
 *
 * Extract the oldest data point from the specified member array contained in \ref libct_stream_data_t "stream data"
 * received at the application.
 *
 * @param data The stream data received in your \ref libct_app_callbacks_t.on_data_received
 *             "on_data_received()" application callback.
 * @param memb The \ref libct_stream_data_t "stream data" member name of the data point to extract.
 *
 * @return The extracted data point on success, and null on failure.
 */
#define libct_get_first_dp(data, memb) libct_get_dp(data, memb, 0)
/**
 * \ingroup unmanaged_secondary_api
 *
 * Iterate over data points of the specified member array to extract from \ref libct_stream_data_t
 * "stream data" received at the application.
 *
 * @param data The stream data received in your \ref libct_app_callbacks_t.on_data_received
 *             "on_data_received()" application callback.
 * @param idx  Iterator variable of type unsigned integer.
 * @param dp   Pointer variable of type corresponding to the memb argument.
 * @param memb The \ref libct_stream_data_t "stream data" member name of the data points to extract.
 */
#define for_each_dp(data, idx, dp, memb) for(idx=0; (idx<(data)->memb.count) && (dp=((data)->memb.datapoints) ? &(data)->memb.datapoints[idx] : NULL); idx++)
/**
 * Pulse waveform data returned from the device as a result of a previous
 * read request. Note this data is returned only after an explicit read
 * of raw pulse waveform data.
 */
typedef struct libct_pulse_waveform_t {
    /**
     * Array of integrated waveform pulse data points.
     */
    struct {
        libct_pulse_t* datapoints;
        unsigned int count;
    } int_pulse;
    /**
     * Array of parameterize pulse waveform data points.
     */
    struct {
        libct_param_pulse_t* datapoints;
        unsigned int count;
    } param_pulse;
    /**
     * Value of local clock (in milliseconds) when this stream packet was received.
     * This time stamp is used to measure processing latency and for history logging.
     * It differs from time stamp found in each data point generated by the remote device.
     */
    long receive_time;
} libct_pulse_waveform_t;
/**
 * CareTaker version information
 */
typedef struct libct_version_t {
    /** major version number */
    int major;
    /** minor version number */
    int minor;
    /** revision number */
    int revision;
    /** build number */
    int build;
} libct_version_t;
/**
 * Structure used to pass calibration data to libct_start_measuring().
 */
typedef struct libct_cal_t {
    /**
     * Calibration type.
     * Set to one of the \ref libct_cal_type_t "calibration types".
     */
    int type;
    /** Calibration data. */
    union {
        /**
         * Calibration configuration when \ref libct_cal_t.type "type" is LIBCT_AUTO_CAL.
         */
        struct {
            /**
             * Patient posture.
             * Set to one of the \ref libct_posture_t "patient postures".
             */
            short posture;
        } auto_cal;
        /**
         * Calibration configuration when \ref libct_cal_t.type "type" is LIBCT_MANUAL_CAL.
         */
        struct {
            libct_bp_settings_t settings;
        } manual_cal;
    } config;
} libct_cal_t;
/**
 * Structure defining initialization data passed to libct_init().
 */
typedef struct libct_init_data_t {
    /** The \ref libct_device_class_t "device class". */
    int device_class;
} libct_init_data_t;
/**
 * Structure used to provide asynchronous notifications to the application.
 *
 * This structure is a container of function pointers to your application callback functions to
 * receive asynchronous notifications. Note you are not required to implement all callback functions.
 * Instead, initialize the libct_app_callbacks_t object to zeros and then set only the
 * function pointers to the callback functions you care about. However, you must implement at least
 * the following callbacks to connect and receive data from the device.
 *
 * <ul>
 * <li> \ref libct_app_callbacks_t.on_device_discovered "on_device_discovered()"
 * <li> \ref libct_app_callbacks_t.on_device_connected_ready "on_device_connected_ready()"
 * <li> \ref libct_app_callbacks_t.on_device_disconnected "on_device_disconnected()"
 * <li> \ref libct_app_callbacks_t.on_data_received "on_data_received()"
 * </ul>
 * <p>
 * <b>IMPORTANT:</b> When implementing a callback function, you must include <b>LIBCTAPI</b> in the function
 * signature to specify the calling convention. This ensures the application and library are using the same
 * calling convention to prevent corrupting the stack. Some platforms, such as Windows, support
 * many calling conventions and <b>LIBCTAPI</b> will be set to the default one. If you don't
 * specify <b>LIBCTAPI</b> in the callback implementation, the application source code may not
 * compile or serious failures may occur at runtime due to stack corruption. See the sample
 * implementations included with the member descriptions below for details.
 * <p>
 */
typedef struct libct_app_callbacks_t {
    /**
     * Function pointer to the application callback to receive scan notifications in response to
     * calling libct_start_discovery().
     *
     * These notifications are sent to the application during device discovery to notify
     * a discovered device when scanning for Caretaker devices.
     * <p>
     * The following example illustrates a sample implementation of the callback. Note you must
     * specify <b>LIBCTAPI</b> in the function signature.
     * <p>
     * \code
     * void LIBCTAPI on_device_discovered(libct_context_t* context, libct_device_t* device)
     * {
     *      const char* address = libct_device_get_address(device);
     *      const char* name = libct_device_get_name(device);
     *
     *     // do something
     * }
     * \endcode
     * <p>
     * And you can set the function pointer as follows.
     * \code
     *      libct_app_callbacks_t callbacks = {0};
     *      callbacks.on_device_discovered = on_device_discovered;
     * \endcode
     *
     * @param context The context returned from libct_init().
     * @param device  The discovered device.
     *                <br><br>
     *                <b>Note:</b> This device object will be recycled when the callback returns
     *                so do not save the device pointer. Instead, make a copy of the device info if
     *                needed. Also, since a connection is not established to the device at this point,
     *                device functions that require the connection to be established will not return
     *                anything useful. You can only call the following functions safely on the device
     *                object passed to this callback.
     *                <ul>
     *                <li> \ref libct_device_t.get_address()
     *                <li> \ref libct_device_t.get_name()
     *                </ul>
     *
     * @see on_discovery_timedout()
     * @see on_discovery_failed()
     */
    void (LIBCTAPI *on_device_discovered)(libct_context_t* context, libct_device_t* device);
    /**
     * Function pointer to the application callback to receive timeout notification in response
     * to calling libct_start_discovery().
     *
     * The following example illustrates a sample implementation of the callback. Note you must
     * specify <b>LIBCTAPI</b> in the function signature.
     * <p>
     * \code
     * void LIBCTAPI on_discovery_timedout(libct_context_t* context)
     * {
     *     // do something
     * }
     * \endcode
     * <p>
     * And you can set the function pointer as follows.
     * \code
     *      libct_app_callbacks_t callbacks = {0};
     *      callbacks.on_discovery_timedout = on_discovery_timedout;
     * \endcode
     *
     * @param context The context returned from libct_init().
     *
     * @see on_device_discovered()
     * @see on_discovery_failed()
     */
    void (LIBCTAPI *on_discovery_timedout)(libct_context_t* context);
    /**
     * Function pointer to the application callback to receive error notification in response
     * to calling libct_start_discovery().
     *
     * The following example illustrates a sample implementation of the callback. Note you must
     * specify <b>LIBCTAPI</b> in the function signature.
     * <p>
     * \code
     * void LIBCTAPI on_discovery_failed(libct_context_t* context, int error)
     * {
     *     // do something
     * }
     * \endcode
     * <p>
     * And you can set the function pointer as follows.
     * \code
     *      libct_app_callbacks_t callbacks = {0};
     *      callbacks.on_discovery_failed = on_discovery_failed;
     * \endcode
     *
     * @param context The context returned from libct_init().
     * @param error Generic error code describing the failure.
     *
     * @see on_device_discovered()
     * @see on_discovery_timedout()
     */
    void (LIBCTAPI *on_discovery_failed)(libct_context_t* context, int error);
    /**
     * Function pointer to the application callback to receive early connection notification in
     * response to calling libct_connect(). This is an early notification to allow the application
     * to update the device connection status, however, the device is not ready for IO at this stage
     * of the connection sequence.
     *
     * The following example illustrates a sample implementation of the callback. Note you must
     * specify <b>LIBCTAPI</b> in the function signature.
     * <p>
     * \code
     * void LIBCTAPI on_device_connected_not_ready(libct_context_t* context, libct_device_t* device)
     * {
     *     // update connection status
     * }
     * \endcode
     * <p>
     * And you can set the function pointer as follows.
     * \code
     *      libct_app_callbacks_t callbacks = {0};
     *      callbacks.on_device_connected_not_ready = on_device_connected_not_ready;
     * \endcode
     *
     * @param context The context returned from libct_init().
     * @param device The connected device.
     *               <br><br>
     *               <b>Note:</b> Since the connection is established to the device at this point there is
     *               no restriction on which device functions you can call to obtain information about the
     *               device.
     *
     * @see on_connect_error()
     * @see on_connect_timedout()
     */
    void (LIBCTAPI *on_device_connected_not_ready)(libct_context_t* context, libct_device_t* device);
    /**
     * Function pointer to the application callback to receive connection notification in response
     * to calling libct_connect(). At this stage, the device is ready for IO and the application
     * can issue requests to the device.
     *
     * The following example illustrates a sample implementation of the callback. Note you must
     * specify <b>LIBCTAPI</b> in the function signature.
     * <p>
     * \code
     * void LIBCTAPI on_device_connected_ready(libct_context_t* context, libct_device_t* device)
     * {
     *     // do something
     * }
     * \endcode
     * <p>
     * And you can set the function pointer as follows.
     * \code
     *      libct_app_callbacks_t callbacks = {0};
     *      callbacks.on_device_connected_ready = on_device_connected_ready;
     * \endcode
     *
     * @param context The context returned from libct_init().
     * @param device The connected device.
     *               <br><br>
     *               <b>Note:</b> Since the connection is established to the device at this point there is
     *               no restriction on which device functions you can call to obtain information about the
     *               device.
     *
     * @see on_connect_error()
     * @see on_connect_timedout()
     */
    void (LIBCTAPI *on_device_connected_ready)(libct_context_t* context, libct_device_t* device);
    /**
     * Function pointer to the application callback to receive error notification in response
     * to calling libct_connect().
     *
     * The following example illustrates a sample implementation of the callback. Note you must
     * specify <b>LIBCTAPI</b> in the function signature..
     * <p>
     * \code
     * void LIBCTAPI on_connect_error(libct_context_t* context, libct_device_t* device, const char* error)
     * {
     *     // do something
     * }
     * \endcode
     * <p>
     * And you can set the function pointer as follows.
     * \code
     *      libct_app_callbacks_t callbacks = {0};
     *      callbacks.on_connect_error = on_connect_error;
     * \endcode
     *
     * @param context The context returned from libct_init().
     * @param device The affected device.
     *               <br><br>
     *               <b>Note:</b> Since there is no connection to the device the device
     *               functions requiring a connection will not return anything useful.
     * @param error  String describing error.
     *
     * @see on_device_connected_not_ready()
     * @see on_device_connected_ready()
     * @see on_connect_timedout()
     */
    void (LIBCTAPI *on_connect_error)(libct_context_t* context, libct_device_t* device, const char* error);
    /**
     * Function pointer to the application callback to receive timed out notification in response
     * to calling libct_connect().
     *
     * The following example illustrates a sample implementation of the callback. Note you must
     * specify <b>LIBCTAPI</b> in the function signature.
     * <p>
     * \code
     * void LIBCTAPI on_connect_timedout(libct_context_t* context, libct_device_t* device)
     * {
     *     // do something
     * }
     * \endcode
     * <p>
     * And you can set the function pointer as follows.
     * \code
     *      libct_app_callbacks_t callbacks = {0};
     *      callbacks.on_connect_timedout = on_connect_timedout;
     * \endcode
     *
     * @param context The context returned from libct_init().
     * @param device The affected device.
     *               <br><br>
     *               <b>Note:</b> Since there is no connection to the device the device
     *               functions requiring a connection will not return anything useful.
     *
     * @see on_device_connected_not_ready()
     * @see on_device_connected_ready()
     * @see on_connect_error()
     */
    void (LIBCTAPI *on_connect_timedout)(libct_context_t* context, libct_device_t* device);
    /**
     * Function pointer to the application callback to receive disconnect notification.
     *
     * The disconnect notification is notified after the connection is established with the device
     * and the connection is lost such as when the device moves out of range and disconnects.
     *
     * The disconnect notification is also notified when the application calls libct_disconnect()
     * to disconnect explicitly. However, the notification to the application is not guaranteed
     * to occur for this scenario, which should be okay since the application initiated the disconnect. 
     *
     * <b>IMPORTANT: </b> The application must  not call libct_disconnect() or libct_deinit() from
     * within this or any library callback function. Callbacks are called from internal library threads
     * that these functions attempt to kill. As such, libct_disconnect() and libct_deinit() must only be
     * called from an application thread.
     *
     * The following example illustrates a sample implementation of the callback. Note you must
     * specify <b>LIBCTAPI</b> in the function signature.
     * <p>
     * \code
     * void LIBCTAPI on_device_disconnected(libct_context_t* context, libct_device_t* device)
     * {
     *     // do something
     * }
     * \endcode
     * <p>
     * And you can set the function pointer as follows.
     * \code
     *      libct_app_callbacks_t callbacks = {0};
     *      callbacks.on_device_disconnected = on_device_disconnected;
     * \endcode
     *
     * @param context The context returned from libct_init().
     * @param device The disconnected device.
     */
    void (LIBCTAPI *on_device_disconnected)(libct_context_t* context, libct_device_t* device);
    /**
     * Function pointer to the application callback to receive notification in response to calling
     * libct_start_monitoring().
     *
     * This notification will be sent only once (one-shot) to notify success or failure after calling
     * libct_start_monitoring(). If success, the application on_data_received() will be notified
     * repeatedly with data from the device until libct_stop_monitoring() is called subsequently
     * or the device is disconnected.
     * <p>
     * The following example illustrates a sample implementation of the callback. Note you must
     * specify <b>LIBCTAPI</b> in the function signature.
     * <p>
     * \code
     * void LIBCTAPI on_start_monitoring(libct_context_t* context, libct_device_t* device, int status)
     * {
     *     // do something
     * }
     * \endcode
     * <p>
     * And you can set the function pointer as follows.
     * \code
     *      libct_app_callbacks_t callbacks = {0};
     *      callbacks.on_start_monitoring = on_start_monitoring;
     * \endcode
     *
     * @param context The context returned from libct_init().
     * @param device  The device being monitored.
     * @param status  Status indicating success or failure: zero on success and non-zero otherwise.
     *
     * @see on_data_received()
     */
    void (LIBCTAPI *on_start_monitoring)(libct_context_t* context, libct_device_t* device, int status);
    /**
     * Function pointer to the application callback to receive notification in response to calling
     * libct_stop_monitoring().
     *
     * This notification will be sent only once (one-shot) to notify success or failure after calling
     * libct_stop_monitoring(). If success, the application on_data_received() will stop receiving
     * data notifications.
     * <p>
     * The following example illustrates a sample implementation of the callback. Note you must
     * specify <b>LIBCTAPI</b> in the function signature.
     * <p>
     * \code
     * void LIBCTAPI on_stop_monitoring(libct_context_t* context, libct_device_t* device, int status)
     * {
     *     // do something
     * }
     * \endcode
     * <p>
     * And you can set the function pointer as follows.
     * \code
     *      libct_app_callbacks_t callbacks = {0};
     *      callbacks.on_stop_monitoring = on_stop_monitoring;
     * \endcode
     *
     * @param context The context returned from libct_init().
     * @param device  The device being monitored.
     * @param status  Status indicating success or failure: zero on success and non-zero otherwise.
     */
    void (LIBCTAPI *on_stop_monitoring)(libct_context_t* context, libct_device_t* device, int status);
    /**
     * Function pointer to the application callback to receive notification in response to calling
     * libct_start_measuring().
     *
     * This notification will be sent only once (one-shot) to notify success or failure after calling
     * libct_start_measuring(). If success, the device will begin pulse decomposition analysis (PDA)
     * and vital sign measurements (blood pressure, heart rate, etc) will be notified to the
     * application on_data_received() callback.
     * <p>
     * The following example illustrates a sample implementation of the callback. Note you must
     * specify <b>LIBCTAPI</b> in the function signature.
     * <p>
     * \code
     * void LIBCTAPI on_start_measuring(libct_context_t* context, libct_device_t* device, int status)
     * {
     *     // do something
     * }
     * \endcode
     * <p>
     * And you can set the function pointer as follows.
     * \code
     *      libct_app_callbacks_t callbacks = {0};
     *      callbacks.on_start_measuring = on_start_measuring;
     * \endcode
     *
     * @param context The context returned from libct_init().
     * @param device  The device providing measurements.
     * @param status  Status indicating success or failure: zero on success and non-zero otherwise.
     */
    void (LIBCTAPI *on_start_measuring)(libct_context_t* context, libct_device_t* device, int status);
    /**
     * Function pointer to the application callback to receive notification in response to calling
     * libct_stop_measuring().
     *
     * This notification will be sent only once (one-shot) to notify success or failure after calling
     * libct_stop_measuring(). If success, the device will stop pulse decomposition analysis (PDA)
     * and vital sign measurements (blood pressure, heart rate, etc) will stop.
     * <p>
     * The following example illustrates a sample implementation of the callback. Note you must
     * specify <b>LIBCTAPI</b> in the function signature.
     * <p>
     * \code
     * void LIBCTAPI on_stop_measuring(libct_context_t* context, libct_device_t* device, int status)
     * {
     *     // do something
     * }
     * \endcode
     * <p>
     * And you can set the function pointer as follows.
     * \code
     *      libct_app_callbacks_t callbacks = {0};
     *      callbacks.on_stop_measuring = on_stop_measuring;
     * \endcode
     *
     * @param context The context returned from libct_init().
     * @param device  The device providing measurements.
     * @param status  Status indicating success or failure: zero on success and non-zero otherwise.
     */
    void (LIBCTAPI *on_stop_measuring)(libct_context_t* context, libct_device_t* device, int status);
    /**
     * Function pointer to the application callback to receive data notifications.
     *
     * These notifications are sent repeatedly to the application to hand-off data received from
     * the device some time after calling libct_start_monitoring() successfully.
     *
     * Data notified via this callback depends on the \ref libct_monitor_flags_t "monitor flags"
     * passed to libct_start_monitoring() and whether or not libct_start_measuring() was called
     * to start taking vital sign measurements. See libct_stream_data_t for data details.
     * <p>
     * The following example illustrates a sample implementation of the callback. Note you must
     * specify <b>LIBCTAPI</b> in the function signature.
     * <p>
     * \code
     * void LIBCTAPI on_data_received(libct_context_t* context, libct_device_t* device, libct_stream_data_t* data)
     * {
     *     // handle data
     * }
     * \endcode
     * <p>
     * And you can set the function pointer as follows.
     * \code
     *      libct_app_callbacks_t callbacks = {0};
     *      callbacks.on_data_received = on_data_received;
     * \endcode
     *
     * @param context The context returned from libct_init().
     * @param device The device originating the data.
     * @param data   Stream packet containing the data received from the device.
     *               <br><br>
     *               <b>NOTE:</b> The stream data packet is created with dynamic memory that will
     *               be freed after the callback returns. So you should not save
     *               pointer(s) to the data, instead copy individual fields into application
     *               memory as needed if you need to access it after on_data_received() returns.
     *               Do not copy the entire libct_stream_data_t structure as it is a structure
     *               of pointers and doing so will be saving pointers to freed memory after
     *               the callback returned.
     *
     * @see on_data_error()
     */
    void (LIBCTAPI *on_data_received)(libct_context_t* context,
                                      libct_device_t* device,
                                      libct_stream_data_t* data);
    /**
     * Function pointer to the application callback to receive data error notification.
     *
     * This notification is sent if the library encounters error receiving or processing data.
     * <p>
     * The following example illustrates a sample implementation of the callback. Note you must
     * specify <b>LIBCTAPI</b> in the function signature.
     * <p>
     * \code
     * void LIBCTAPI on_data_error(libct_context_t* context, libct_device_t* device, const char* error)
     * {
     *     // handle error
     * }
     * \endcode
     * <p>
     * And you can set the function pointer as follows.
     * \code
     *      libct_app_callbacks_t callbacks = {0};
     *      callbacks.on_data_error = on_data_error;
     * \endcode
     *
     * @param context The context returned from libct_init().
     * @param device The affected device.
     * @param error  String describing the error.
     */
    void (LIBCTAPI *on_data_error)(libct_context_t* context,
                                   libct_device_t* device,
                                   const char* error);
    /**
     * Function pointer to the application callback to receive status in response to calling
     * libct_rd_snr_min().
     *
     * This notification will be sent only once (one-shot) to notify success or failure after calling
     * libct_rd_snr_min().
     * <p>
     * The following example illustrates a sample implementation of the callback. Note you must
     * specify <b>LIBCTAPI</b> in the function signature.
     * <p>
     * \code
     * void LIBCTAPI on_rd_snr_min_rsp(libct_context_t* context, libct_device_t* device, int snr, int status)
     * {
     *     // do something
     * }
     * \endcode
     * <p>
     * And you can set the function pointer as follows.
     * \code
     *      libct_app_callbacks_t callbacks = {0};
     *      callbacks.on_rd_snr_min_rsp = on_rd_snr_min_rsp;
     * \endcode
     *
     * @param context The context returned from libct_init().
     * @param device  The device associated with the context.
     * @param snr     Minimum signal-to-noise value on success.
     * @param status  Status indicating success or failure: zero on success and non-zero otherwise.
     */
    void (LIBCTAPI *on_rd_snr_min_rsp)(libct_context_t* context,
                                       libct_device_t* device,
                                       int snr,
                                       int status);
    /**
     * Function pointer to the application callback to receive status in response to calling
     * libct_wrt_snr_min().
     *
     * This notification will be sent only once (one-shot) to notify success or failure after calling
     * libct_wrt_snr_min().
     * <p>
     * The following example illustrates a sample implementation of the callback. Note you must
     * specify <b>LIBCTAPI</b> in the function signature.
     * <p>
     * \code
     * void LIBCTAPI on_wrt_snr_min_rsp(libct_context_t* context, libct_device_t* device, int status)
     * {
     *     // do something
     * }
     * \endcode
     * <p>
     * And you can set the function pointer as follows.
     * \code
     *      libct_app_callbacks_t callbacks = {0};
     *      callbacks.on_wrt_snr_min_rsp = on_wrt_snr_min_rsp;
     * \endcode
     *
     * @param context The context returned from libct_init().
     * @param device  The device associated with the context.
     * @param status  Status indicating success or failure: zero on success and non-zero otherwise.
     */
    void (LIBCTAPI *on_wrt_snr_min_rsp)(libct_context_t* context,
                                        libct_device_t* device,
                                        int status);
    /**
     * Function pointer to the application callback to receive status in response to calling
     * libct_rd_display_state().
     *
     * This notification will be sent only once (one-shot) to notify success or failure after calling
     * libct_rd_display_state().
     * <p>
     * The following example illustrates a sample implementation of the callback. Note you must
     * specify <b>LIBCTAPI</b> in the function signature.
     * <p>
     * \code
     * void LIBCTAPI on_rd_display_state_rsp(libct_context_t* context, libct_device_t* device, unsigned char state, int status)
     * {
     *     // do something
     * }
     * \endcode
     * <p>
     * And you can set the function pointer as follows.
     * \code
     *      libct_app_callbacks_t callbacks = {0};
     *      callbacks.on_rd_display_state_rsp = on_rd_display_state_rsp;
     * \endcode
     *
     * @param context The context returned from libct_init().
     * @param device  The device associated with the context.
     * @param state   The device display state on success: 0 = off, 1 = on.
     * @param status  Status indicating success or failure: zero on success and non-zero otherwise.
     */
    void (LIBCTAPI *on_rd_display_state_rsp)(libct_context_t* context,
                                             libct_device_t* device,
                                             unsigned char state,
                                             int status);
    /**
     * Function pointer to the application callback to receive status in response to calling
     * libct_wrt_display_state().
     *
     * This notification will be sent only once (one-shot) to notify success or failure after calling
     * libct_wrt_display_state().
     * <p>
     * The following example illustrates a sample implementation of the callback. Note you must
     * specify <b>LIBCTAPI</b> in the function signature.
     * <p>
     * \code
     * void LIBCTAPI on_wrt_display_state_rsp(libct_context_t* context, libct_device_t* device, int status)
     * {
     *     // do something
     * }
     * \endcode
     * <p>
     * And you can set the function pointer as follows.
     * \code
     *      libct_app_callbacks_t callbacks = {0};
     *      callbacks.on_wrt_display_state_rsp = on_wrt_display_state_rsp;
     * \endcode
     *
     *
     * @param context The context returned from libct_init().
     * @param device  The device associated with the context.
     * @param status  Status indicating success or failure: zero on success and non-zero otherwise.
     */
    void (LIBCTAPI *on_wrt_display_state_rsp)(libct_context_t* context,
                                              libct_device_t* device,
                                              int status);
    /**
     * Function pointer to the application callback to receive status in response to calling
     * libct_rd_recal_itvl().
     *
     * This notification will be sent only once (one-shot) to notify success or failure after calling
     * libct_rd_recal_itvl().
     * <p>
     * The following example illustrates a sample implementation of the callback. Note you must
     * specify <b>LIBCTAPI</b> in the function signature.
     * <p>
     * \code
     * void LIBCTAPI on_rd_recal_itvl_rsp(libct_context_t* context, libct_device_t* device, unsigned int itvl, int status)
     * {
     *     // do something
     * }
     * \endcode
     * <p>
     * And you can set the function pointer as follows.
     * \code
     *      libct_app_callbacks_t callbacks = {0};
     *      callbacks.on_rd_recal_itvl_rsp = on_rd_recal_itvl_rsp;
     * \endcode
     *
     * @param context The context returned from libct_init().
     * @param device  The device associated with the context.
     * @param itvl    The recalibration interval in minutes on success.
     * @param status  Status indicating success or failure: zero on success and non-zero otherwise.
     */
    void (LIBCTAPI *on_rd_recal_itvl_rsp)(libct_context_t* context,
                                          libct_device_t* device,
                                          unsigned int itvl,
                                          int status);
    /**
     * Function pointer to the application callback to receive status in response to calling
     * libct_wrt_recal_itvl().
     *
     * This notification will be sent only once (one-shot) to notify success or failure after calling
     * libct_wrt_recal_itvl().
     * <p>
     * The following example illustrates a sample implementation of the callback. Note you must
     * specify <b>LIBCTAPI</b> in the function signature.
     * <p>
     * \code
     * void LIBCTAPI on_wrt_recal_itvl_rsp(libct_context_t* context, libct_device_t* device, int status)
     * {
     *     // do something
     * }
     * \endcode
     * <p>
     * And you can set the function pointer as follows.
     * \code
     *      libct_app_callbacks_t callbacks = {0};
     *      callbacks.on_wrt_recal_itvl_rsp = on_wrt_recal_itvl_rsp;
     * \endcode
     *
     * @param context The context returned from libct_init().
     * @param device  The device associated with the context.
     * @param status  Status indicating success or failure: zero on success and non-zero otherwise.
     */
    void (LIBCTAPI *on_wrt_recal_itvl_rsp)(libct_context_t* context,
                                           libct_device_t* device,
                                           int status);
        /**
        * Function pointer to the application callback to receive status in response to calling
        * libct_rd_cuff_pressure().
        *
        * This notification will be sent only once (one-shot) to notify success or failure after calling
        * libct_rd_cuff_pressure().
        * <p>
        * The following example illustrates a sample implementation of the callback. Note you must
        * specify <b>LIBCTAPI</b> in the function signature.
        * <p>
        * \code
        * void LIBCTAPI on_rd_cuff_pressure_rsp(libct_context_t* context, libct_device_t* device, int status)
        * {
        *     // do something
        * }
        * \endcode
        * <p>
        * And you can set the function pointer as follows.
        * \code
        *      libct_app_callbacks_t callbacks = {0};
        *      callbacks.on_rd_cuff_pressure_rsp = on_rd_cuff_pressure_rsp;
        * \endcode
        *
        * @param context  The context returned from libct_init().
        * @param device   The device associated with the context.
        * @param pressure The cuff pressure in mmHg on success.
        * @param status   Status indicating success or failure: zero on success and non-zero otherwise.
        */
    void (LIBCTAPI *on_rd_cuff_pressure_rsp)(libct_context_t* context,
                                             libct_device_t* device,
                                             int pressure,
                                             int status);
    /**
     * Function pointer to the application callback to receive status in response to calling
     * libct_vent_cuff().
     *
     * This notification will be sent only once (one-shot) to notify success or failure after calling
     * libct_vent_cuff().
     * <p>
     * The following example illustrates a sample implementation of the callback. Note you must
     * specify <b>LIBCTAPI</b> in the function signature.
     * <p>
     * \code
     * void LIBCTAPI on_vent_cuff_rsp(libct_context_t* context, libct_device_t* device, int status)
     * {
     *     // do something
     * }
     * \endcode
     * <p>
     * And you can set the function pointer as follows.
     * \code
     *      libct_app_callbacks_t callbacks = {0};
     *      callbacks.on_vent_cuff_rsp = on_vent_cuff_rsp;
     * \endcode
     *
     * @param context The context returned from libct_init().
     * @param device  The device associated with the context.
     * @param status  Status indicating success or failure: zero on success and non-zero otherwise.
     */
    void (LIBCTAPI *on_vent_cuff_rsp)(libct_context_t* context,
                                      libct_device_t* device,
                                      int status);
    /**
     * Function pointer to the application callback to receive status in response to calling
     * libct_clr_status().
     *
     * This notification will be sent only once (one-shot) to notify success or failure after calling
     * libct_clr_status().
     * <p>
     * The following example illustrates a sample implementation of the callback. Note you must
     * specify <b>LIBCTAPI</b> in the function signature.
     * <p>
     * \code
     * void LIBCTAPI on_clr_status_rsp(libct_context_t* context, libct_device_t* device, int status)
     * {
     *     // do something
     * }
     * \endcode
     * <p>
     * And you can set the function pointer as follows.
     * \code
     *      libct_app_callbacks_t callbacks = {0};
     *      callbacks.on_clr_status_rsp = on_clr_status_rsp;
     * \endcode
     *
     * @param context The context returned from libct_init().
     * @param device  The device associated with the context.
     * @param status  Status indicating success or failure: zero on success and non-zero otherwise.
     */
    void (LIBCTAPI *on_clr_status_rsp)(libct_context_t* context,
                                       libct_device_t* device,
                                       int status);
    /**
     * Function pointer to the application callback to receive status in response to calling
     * libct_diag_flush().
     *
     * This notification will be sent only once (one-shot) to notify success or failure after calling
     * libct_diag_flush().
     * <p>
     * The following example illustrates a sample implementation of the callback. Note you must
     * specify <b>LIBCTAPI</b> in the function signature.
     * <p>
     * \code
     * void LIBCTAPI on_diag_flush_rsp(libct_context_t* context, libct_device_t* device, int status)
     * {
     *     // do something
     * }
     * \endcode
     * <p>
     * And you can set the function pointer as follows.
     * \code
     *      libct_app_callbacks_t callbacks = {0};
     *      callbacks.on_diag_flush_rsp = on_diag_flush_rsp;
     * \endcode
     *
     * @param context The context returned from libct_init().
     * @param device  The device associated with the context.
     * @param status  Status indicating success or failure: zero on success and non-zero otherwise.
     */
    void (LIBCTAPI *on_diag_flush_rsp)(libct_context_t* context,
                                       libct_device_t* device,
                                       int status);
    /**
    * Function pointer to the application callback to receive status in response to calling
    * libct_wrt_waveform_clamping().
    *
    * This notification will be sent only once (one-shot) to notify success or failure after calling
    * libct_wrt_waveform_clamping().
    * <p>
    * The following example illustrates a sample implementation of the callback. Note you must
    * specify <b>LIBCTAPI</b> in the function signature.
    * <p>
    * \code
    * void LIBCTAPI on_wrt_waveform_clamping(libct_context_t* context,
	*                                        libct_device_t* device
	*                                        int status)
    * {
    *     // do something
    * }
    * \endcode
    * <p>
    * And you can set the function pointer as follows.
    * \code
    *      libct_app_callbacks_t callbacks = {0};
    *      callbacks.on_wrt_waveform_clamping = on_wrt_waveform_clamping;
    * \endcode
    *
    * @param context The context returned from libct_init().
    * @param device  The device associated with the context.
    * @param status  Status indicating success or failure: zero on success and non-zero otherwise.
    */
    void (LIBCTAPI *on_wrt_waveform_clamping)(libct_context_t* context,
                                              libct_device_t* device,
                                              int status);
	/**
	* Function pointer to the application callback to receive status in response to calling
	* libct_rd_waveform_clamping().
	*
	* This notification will be sent only once (one-shot) to notify success or failure after calling
	* libct_wrt_waveform_clamping().
	* <p>
	* The following example illustrates a sample implementation of the callback. Note you must
	* specify <b>LIBCTAPI</b> in the function signature.
	* <p>
	* \code
	* void LIBCTAPI on_rd_waveform_clamping(libct_context_t* context,
	*                                       libct_device_t* device,
	*                                       unsigned char value
	*                                       int status)
	* {
	*     // do something
	* }
	* \endcode
	* <p>
	* And you can set the function pointer as follows.
	* \code
	*      libct_app_callbacks_t callbacks = {0};
	*      callbacks.on_rd_waveform_clamping = on_rd_waveform_clamping;
	* \endcode
	*
	* @param context The context returned from libct_init().
	* @param device  The device associated with the context.
	* @param value   Clamp setting: 1 = ON, 0 = OFF
	* @param status  Status indicating success or failure: zero on success and non-zero otherwise.
	*/
	void (LIBCTAPI *on_rd_waveform_clamping)(libct_context_t* context,
		libct_device_t* device,
		unsigned char value,
		int status);
    /**
    * Function pointer to the application callback to receive status in response to calling
    * libct_rd_vitals_filter().
    *
    * This notification will be sent only once (one-shot) to notify success or failure after calling
    * libct_rd_vitals_filter().
    * <p>
    * The following example illustrates a sample implementation of the callback. Note you must
    * specify <b>LIBCTAPI</b> in the function signature.
    * <p>
    * \code
    * void LIBCTAPI on_rd_vitals_filter(libct_context_t* context, libct_device_t* device, int status)
    * {
    *     // do something
    * }
    * \endcode
    * <p>
    * And you can set the function pointer as follows.
    * \code
    *      libct_app_callbacks_t callbacks = {0};
    *      callbacks.on_rd_vitals_filter = on_rd_vitals_filter;
    * \endcode
    *
    * @param context The context returned from libct_init().
    * @param device  The device associated with the context.
    * @param value   The median filter value when success. 0 = Disabled, 1 = Enabled.
    * @param status  Status indicating success or failure: zero on success and nonzero otherwise.
    */
    void (LIBCTAPI *on_rd_vitals_filter)(libct_context_t* context,
                                         libct_device_t* device,
                                         int value,
                                         int status);
    /**
    * Function pointer to the application callback to receive status in response to calling
    * libct_wrt_vitals_filter().
    *
    * This notification will be sent only once (one-shot) to notify success or failure after calling
    * libct_wrt_vitals_filter().
    * <p>
    * The following example illustrates a sample implementation of the callback. Note you must
    * specify <b>LIBCTAPI</b> in the function signature.
    * <p>
    * \code
    * void LIBCTAPI on_wrt_vitals_filter(libct_context_t* context, libct_device_t* device, int status)
    * {
    *     // do something
    * }
    * \endcode
    * <p>
    * And you can set the function pointer as follows.
    * \code
    *      libct_app_callbacks_t callbacks = {0};
    *      callbacks.on_wrt_vitals_filter = on_wrt_vitals_filter;
    * \endcode
    *
    * @param context The context returned from libct_init().
    * @param device  The device associated with the context.
    * @param status  Status indicating success or failure: zero on success and non-zero otherwise.
    */
    void (LIBCTAPI *on_wrt_vitals_filter)(libct_context_t* context,
                                          libct_device_t* device,
                                          int status);
	/**
	* Function pointer to the application callback to receive status in response to calling
	* libct_rd_motion_timeout().
	*
	* This notification will be sent only once (one-shot) to notify success or failure after calling
	* libct_rd_motion_timeout().
	* <p>
	* The following example illustrates a sample implementation of the callback. Note you must
	* specify <b>LIBCTAPI</b> in the function signature.
	* <p>
	* \code
	* void LIBCTAPI on_rd_motion_timeout(libct_context_t* context, libct_device_t* device, int status, int timeout)
	* {
	*     // do something
	* }
	* \endcode
	* <p>
	* And you can set the function pointer as follows.
	* \code
	*      libct_app_callbacks_t callbacks = {0};
	*      callbacks.on_rd_motion_timeout = on_rd_motion_timeout;
	* \endcode
	*
	* @param context The context returned from libct_init().
	* @param device  The device associated with the context.
	* @param status  Status indicating success or failure: zero on success and non-zero otherwise.
	* @param timeout Motion timeout value in seconds.
	*/
	void (LIBCTAPI *on_rd_motion_timeout)(libct_context_t* context,
                                          libct_device_t* device,
		                                  int timeout,
		                                  int status);
	/**
	* Function pointer to the application callback to receive status in response to calling
	* libct_rd_persistent_log().
	*
	* This notification will be sent only once (one-shot) to notify success or failure after calling
	* libct_rd_persistent_log().
	* <p>
	* The following example illustrates a sample implementation of the callback. Note you must
	* specify <b>LIBCTAPI</b> in the function signature.
	* <p>
	* \code
	* void LIBCTAPI on_rd_persistent_log(libct_context_t* context,
	*                                    libct_device_t* device,
	*                                    const char* log,
	*                                    unsigned int len,
	*                                    int timeout)
	* {
	*     // do something
	* }
	* \endcode
	* <p>
	* And you can set the function pointer as follows.
	* \code
	*      libct_app_callbacks_t callbacks = {0};
	*      callbacks.on_rd_persistent_log = on_rd_persistent_log;
	* \endcode
	*
	* @param context The context returned from libct_init().
	* @param device  The device associated with the context.
	* @param log The device persistent log.
	* @param len Log length.
	* @param status  Status indicating success or failure: zero on success and non-zero otherwise.
	*/
	void (LIBCTAPI *on_rd_persistent_log)(libct_context_t* context,
		libct_device_t* device,
		const char* log,
		unsigned int len,
		int status);
 
} libct_app_callbacks_t;
/**
* \ingroup unmanaged_device
* \class libct_device_t
* Handle used to identify a connected device the application is monitoring.
*
* The device handle is used to identify and aggregate general information about a connected device, such as the
* device name, address, serial number, etc., that the application can query. Note each device handle is associated
* with a library context and you can retrieve it anytime with libct_get_device() passing the context as argument.
* As such, you should not hold on to device handles in your application code as they may change when the devices
* they are associated with become disconnected.
* <p>
* The device handle primary purpose is to identify data notified to your application callbacks.
*/
struct libct_device_t {
    /**
     * Return a \ref libct_device_state_t "device state" enumeration representing the current state of the library context
     * that is associated with this device.
     *
     * You would invoke the function as follows, but note a convenience macro with simpler interface than the function is available.
     *
     * \code
     * // These two calls are similar, but the second is simpler.
     * // (1) Use the device function.
     * int state = device->get_state(device);
     *
     * // (2) Or use macro with simpler interface.
     * int state = libct_device_get_state(device);
     * \endcode
     *
     * @param thiz The device instance.
     *
     * @see libct_device_get_state()
     * @see libct_device_uninitialized()
     * @see libct_device_intialized()
     * @see libct_device_discovering()
     * @see libct_device_connecting()
     * @see libct_device_connected()
     * @see libct_device_disconnecting()
     * @see libct_device_disconnected()
     * @see libct_device_monitoring()
     * @see libct_device_measuring()
     */
    int (LIBCTAPI *get_state)(struct libct_device_t* thiz);
    /**
     * \ingroup unmanaged_device
     * Convenience macro to \ref libct_device_t.get_state "device->get_state(device)"
     * @param dev Pointer to the device instance.
     */
    #define libct_device_get_state(dev) (dev)->get_state(dev)
    /**
     * \ingroup unmanaged_device
     * Returns non-zero (true) if the device is not initialized, and zero (false) otherwise.
     * @param dev Pointer to device instance.
     */
    #define libct_device_uninitialized(dev) (((dev)->get_state(dev)) & LIBCT_STATE_UNITIALIZED)
    /**
     * \ingroup unmanaged_device
     * Returns non-zero (true) if the device is initialized, and zero (false) otherwise .
     *
     */
    #define libct_device_intialized(dev) (libct_device_uninitialized(dev))
    /**
     * \ingroup unmanaged_device
     * Returns non-zero (true) if discovering the device, and zero (false) otherwise.
     * @param dev Pointer to device instance.
     */
    #define libct_device_discovering(dev) (((dev)->get_state(dev)) & LIBCT_STATE_DISCOVERING)
    /**
     * \ingroup unmanaged_device
     * Returns non-zero (true) if connecting to the device, and zero (false) otherwise.
     * @param dev Pointer to device instance.
     */
    #define libct_device_connecting(dev) (((dev)->get_state(dev)) & LIBCT_STATE_CONNECTING)
    /**
     * \ingroup unmanaged_device
     * Returns non-zero (true) if connected to the device, and zero (false) otherwise.
     * @param dev Pointer to device instance.
     */
    #define libct_device_connected(dev) (((dev)->get_state(dev)) & LIBCT_STATE_CONNECTED)
    /**
     * \ingroup unmanaged_device
     * Returns non-zero (true) if disconnecting from the device, and zero (false) otherwise.
     * @param dev Pointer to device instance.
     */
    #define libct_device_disconnecting(dev) (((dev)->get_state(dev)) & LIBCT_STATE_DISCONNECTING)
    /**
     * \ingroup unmanaged_device
     * Returns non-zero (true) if disconnected from the device, and zero (false) otherwise.
     * @param dev Pointer to device instance.
     */
    #define libct_device_disconnected(dev) (((dev)->get_state(dev)) & LIBCT_STATE_DISCONNECTED)
    /**
     * \ingroup unmanaged_device
     * Returns non-zero (true) if receiving data from the device, and zero (false) otherwise.
     * @param dev Pointer to device instance.
     */
    #define libct_device_monitoring(dev) (((dev)->get_state(dev)) & LIBCT_STATE_MONITIORING)
    /**
     * \ingroup unmanaged_device
     * Returns non-zero (true) if taking blood pressure measurements, and zero (false) otherwise.
     * @param dev Pointer to device instance.
     */
    #define libct_device_measuring(dev) (((dev)->get_state(dev)) & LIBCT_STATE_MEASURING)
    /**
     * Return the device class that was set in the initialization data passed to libct_init().
     *
     * You would invoke the function as follows, but note a convenience macro with simpler interface than the function is available.
     *
     * \code
     * // These two calls are similar, but the second is simpler.
     * // (1) Use the device function.
     * int class = device->get_class(device);
     *
     * // (2) Or use macro with simpler interface.
     * int class = libct_device_get_class(device);
     * \endcode
     *
     * @param thiz The device instance.
     *
     * @see libct_device_get_class()
     */
    int (LIBCTAPI *get_class)(struct libct_device_t* thiz);
    /**
     * \ingroup unmanaged_device
     * Convenience macro to \ref libct_device_t.get_class "device->get_class(device)"
     * @param dev Pointer to the device instance.
     */
    #define libct_device_get_class(dev) (dev)->get_class(dev)
    /**
     * Return the device manufacturer friendly name.
     *
     * You would invoke the function as follows, but note a convenience macro with simpler interface than the function is available.
     *
     * \code
     * // These two calls are similar, but the second is simpler.
     * // (1) Use the device function.
     * const char* name = device->get_name(device);
     *
     * // (2) Or use macro with simpler interface.
     * const char* name = libct_device_get_name(device);
     * \endcode
     *
     * @param thiz The device instance.
     * @see libct_device_get_name()
     */
    const char* (LIBCTAPI *get_name)(struct libct_device_t* thiz);
    /**
     * \ingroup unmanaged_device
     * Convenience macro to \ref libct_device_t.get_name "device->get_name(device)"
     * @param dev Pointer to the device instance.
     */
    #define libct_device_get_name(dev) (dev)->get_name(dev)
    /**
     * Return the device address.
     *
     * You would invoke the function as follows, but note a convenience macro with simpler interface than the function is available.
     *
     * \code
     * // These two calls are similar, but the second is simpler.
     * // (1) Use the device function.
     * const char* address = device->get_address(device);
     *
     * // (2) Or use macro with simpler interface.
     * const char* address = libct_device_get_address(device);
     * \endcode
     *
     * @param thiz The device instance.
     * @see libct_device_get_address()
     */
    const char* (LIBCTAPI *get_address)(struct libct_device_t* thiz);
    /**
     * \ingroup unmanaged_device
     * Convenience macro to \ref libct_device_t.get_address "device->get_address(device)"
     * @param dev Pointer to the device instance.
     */
    #define libct_device_get_address(dev) (dev)->get_address(dev)
    /**
     * Return the device serial number.
     *
     * You would invoke the function as follows, but note a convenience macro with simpler interface than the function is available.
     *
     * \code
     * // These two calls are similar, but the second is simpler.
     * // (1) Use the device function.
     * const char* sn = device->get_serial_number(device);
     *
     * // (2) Or use macro with simpler interface.
     * const char* sn = libct_device_get_serial_number(device);
     * \endcode
     *
     * @param thiz The device instance.
     * @see libct_device_get_serial_number()
     */
    const char* (LIBCTAPI *get_serial_number)(struct libct_device_t* thiz);
    /**
     * \ingroup unmanaged_device
     * Convenience macro to \ref libct_device_t.get_serial_number "device->get_serial_number(device)"
     * @param dev Pointer to the device instance.
     */
    #define libct_device_get_serial_number(dev) (dev)->get_serial_number(dev)
    /**
     * Return the device hardware version.
     *
     * You would invoke the function as follows, but note a convenience macro with simpler interface than the function is available.
     *
     * \code
     * // These two calls are similar, but the second is simpler.
     * // (1) Use the device function.
     * const libct_version_t* version = device->get_hw_version(device);
     *
     * // (2) Or use macro with simpler interface.
     * const libct_version_t* version = libct_device_get_hw_version(device);
     * \endcode
     *
     * @param thiz The device instance.
     * @see libct_device_get_hw_version()
     */
    const libct_version_t* (LIBCTAPI *get_hw_version)(struct libct_device_t* thiz);
    /**
     * \ingroup unmanaged_device
     * Convenience macro to \ref libct_device_t.get_hw_version "device->get_hw_version(device)"
     * @param dev Pointer to the device instance.
     */
    #define libct_device_get_hw_version(dev) (dev)->get_hw_version(dev)
    /**
     * Return the device firmware version.
     *
     * You would invoke the function as follows, but note a convenience macro with simpler interface than the function is available.
     *
     * \code
     * // These two calls are similar, but the second is simpler.
     * // (1) Use the device function.
     * const libct_version_t* version = device->get_fw_version(device);
     *
     * // (2) Or use macro with simpler interface.
     * const libct_version_t* version = libct_device_get_fw_version(device);
     * \endcode
     *
     * @param thiz The device instance.
     * @see libct_device_get_fw_version()
     */
    const libct_version_t* (LIBCTAPI *get_fw_version)(struct libct_device_t* thiz);
    /**
     * \ingroup unmanaged_device
     * Convenience macro to \ref libct_device_t.get_fw_version "device->get_fw_version(device)"
     * @param dev Pointer to the device instance.
     */
    #define libct_device_get_fw_version(dev) (dev)->get_fw_version(dev)
    /**
     * Return the library context bound to this device.
     *
     * You would invoke the function as follows, but note a convenience macro with simpler interface than the function is available.
     *
     * \code
     * // These two calls are similar, but the second is simpler.
     * // (1) Use the device function.
     * libct_context_t* context = device->get_context(device);
     *
     * // (2) Or use macro with simpler interface.
     * libct_context_t* context  = libct_device_get_context(device);
     * \endcode
     *
     * @param thiz The device instance.
     */
    libct_context_t* (LIBCTAPI *get_context)(struct libct_device_t* thiz);
    /**
     * \ingroup unmanaged_device
     * Convenience macro to \ref libct_device_t.get_context "device->get_context(device)"
     * @param dev Pointer to the device instance.
     */
    #define libct_device_get_context(dev) (dev)->get_context(dev)
};
/*-------------------------------------------------------------------------------------------------
 * Primary APIs
 */
/**
 * \ingroup unmanaged_primary_api
 *
 * \brief Initializes device context.
 *
 * Call this function to initialize a device context before calling any other library
 * functions with the said context.
 *
 * @note You can initialize multiple contexts if you wish to connect to multiple devices
 * simultaneously, but you must call libct_deinit() from the same thread to de-initialize
 * each context when it is no longer needed.
 *
 * @param context   Address to store the created context.
 *                  <br><br>
 *                  <b>IMPORTANT:</b> Initialize the context pointer to
 *                  null before passing it. The internal library code depends on this
 *                  to ensure the context is initialized only once.
 * @param data      Data to initialize the context.
 * @param callbacks The application callback functions to receive asynchronous notifications.
 *                  This pointer must not be null, or else your application will not receive
 *                  notifications notifying connection and data events. However, you can set
 *                  function pointers within this structure that you don't care about to null.
 *                  <br><br>
 *                  <b>NOTE:</b> You can set application specific data to use inside your callbacks
 *                  with libct_set_app_specific_data() after initialization, and later retrieve it
 *                  with libct_get_app_specific_data() to get the application instance data to act
 *                  upon inside the callbacks.
 *
 * @return An appropriate \ref libct_status_t "status code" indicating success or error.
 */
LIBCTEXPORT int LIBCTAPI libct_init(libct_context_t** context,
                               libct_init_data_t* data,
                               libct_app_callbacks_t* callbacks);
/**
 * \ingroup unmanaged_primary_api
 *
 * \brief De-initializes the context.
 *
 * Call this function to release resources when you no longer need the context.
 *
 * <b>IMPORTANT:</b> The application must call dibct_deinit() some time after calling libct_init()
 * to prevent resource leaks. libct_deinit() must not be called from any library callback function.
 * Library callbacks are called from internal library threads that this function attempts
 * to kill. As such, it must only be called from an application thread.
 *
 *  @param context The context returned from libct_init().
 */
LIBCTEXPORT void LIBCTAPI libct_deinit(libct_context_t* context);
/**
 * \ingroup unmanaged_primary_api
 *
 * \brief Discover the device.
 *
 * This function start scanning for devices specify by the \ref libct_device_class_t "device class"
 * in the initialization data passed earlier to libct_init(). Scan results will be notified
 * asynchronously via the \ref libct_app_callbacks_t "application callbacks" passed to
 * libct_init(); specifically, these discovery callback functions will be invoked some time later
 * with the results when devices are discovered or if scanning timed out or failed.
 * <ul>
 * <li>\ref libct_app_callbacks_t.on_device_discovered() "on_device_discovered()"
 * <li>\ref libct_app_callbacks_t.on_discovery_timedout() "on_discovery_timedout()"
 * <li>\ref libct_app_callbacks_t.on_discovery_failed() "on_discovery_failed()"
 * </ul>
 *
 * @note Devices must be advertising and be within range for this method to discover them.
 * Press the button on the caretaker to start advertising. Note the caretaker only advertises for
 * 20 seconds after pressing the button and then stops.
 *
 * @param context The context returned from libct_init().
 * @param timeout Scanning will be canceled after the number of milliseconds specified by this
 *                timeout and the application discovery timeout callback will be invoked.
 *
 * @return An appropriate \ref libct_status_t "status code" indicating success or error.
 */
LIBCTEXPORT int LIBCTAPI libct_start_discovery(libct_context_t* context, unsigned long timeout);
/**
 * \ingroup unmanaged_primary_api
 *
 * \brief Stop device discovery.
 *
 * Call this function to stop device discovery previously started with libct_start_discovery().
 *
 * @param context The context returned from libct_init().
 *
 * @return An appropriate \ref libct_status_t "status code" indicating success or error.
 */
LIBCTEXPORT int LIBCTAPI libct_stop_discovery(libct_context_t* context);
/**
 * \ingroup unmanaged_primary_api
 *
 * \brief Connect to a device.
 *
 * Call this method after device discovery to establish connection to the device.
 * The results will be notified asynchronously via the \ref libct_app_callbacks_t "application callbacks"
 * passed to libct_init(); specifically, one or more of the following callback functions will be invoked
 * some time later with the results if the connection is established, timed out or failed.
 * <ul>
 * <li>\ref libct_app_callbacks_t.on_device_connected_not_ready() "on_device_connected_not_ready()"
* <li>\ref libct_app_callbacks_t.on_device_connected_ready() "on_device_connected_ready()"
 * <li>\ref libct_app_callbacks_t.on_connect_error() "on_connect_error()"
 * <li>\ref libct_app_callbacks_t.on_connect_timedout() "on_connect_timedout()"
 * </ul>
 *
 * @param context The context returned from libct_init().
 * @param device  A discovered device notified with your application callback
 *                \ref libct_app_callbacks_t.on_device_discovered "on_device_discovered()".
 *
 * @return An appropriate \ref libct_status_t "status code" indicating success or error.
 */
LIBCTEXPORT int LIBCTAPI libct_connect(libct_context_t* context, libct_device_t* device);
/**
 * \ingroup unmanaged_primary_api
 *
 * \brief Disconnect from a device.
 *
 * Call this method after calling libct_connect() to clean up resources that were allocated
 * by the connect call.
 *
 * <b>IMPORTANT:</b> The application must call libct_disconnect() to release connection resources
 * before calling libct_connect() subsequently on the same context. Otherwise, the subsequent connect
 * calls may fail. Also, the application must not call libct_disconnect() from any library callback
 * function. Library callbacks are called from internal library threads and this function attempts
 * to kill. As such, it must only be called from an application thread.
 *
 * @param context The context returned from libct_init().
 *
 * @return An appropriate \ref libct_status_t "status code" indicating success or error.
 */
LIBCTEXPORT int LIBCTAPI libct_disconnect(libct_context_t* context);
/**
 * \ingroup unmanaged_primary_api
 *
 * \brief Start monitoring data at the remote caretaker device.
 *
 * Call this function after the connection is established with the device to start monitoring data
 * or to change the data being monitored.
 * <p>
 * Calling this function will trigger your application's \ref libct_app_callbacks_t.on_start_monitoring()
 * "on_start_monitoring()" to be invoked some time later with results. Also, if monitoring was started
 * successfully, data from the device will be notified to your application's
 * \ref libct_app_callbacks_t.on_data_received() "on_data_received()" continuously until stopped
 * explicitly by calling to libct_stop_monitoring() or if the device becomes disconnected.
 *
 * @param context The context returned from libct_init().
 * @param flags   Bitwise OR of \ref libct_monitor_flags_t "monitor flags" specifying the data
 *                to monitor.
 *                <br><br>
 *                <b>Note:</b> The stream data packets notified to the application depends on these flags.
 *                So you can control the amount of data reported to the application by specifying only the
 *                monitoring flags corresponding to the data you care about.
 *
 * @return An appropriate \ref libct_status_t "status code" indicating success or error.
 */
LIBCTEXPORT int LIBCTAPI libct_start_monitoring(libct_context_t* context, int flags);
/**
 * \ingroup unmanaged_primary_api
 *
 * \brief Stops monitoring.
 *
 * Call this method to stop monitoring data after calling libct_start_monitoring() successfully.
 * <p>
 * Calling this function will trigger \ref libct_app_callbacks_t.on_stop_monitoring
 * "on_stop_monitoring()" to be invoked sometime later with results.
 *
 * @param context The context returned from libct_init().
 *
 * @return An appropriate \ref libct_status_t "status code" indicating success or error.
 */
LIBCTEXPORT int LIBCTAPI libct_stop_monitoring(libct_context_t* context);
/**
 * \ingroup unmanaged_primary_api
 *
 * \brief Start taking measurement.
 *
 * If monitoring was started successfully, call this function to initialize (calibrate) blood
 * pressure settings with either auto or manual calibration then start taking vital sign measurements.
 * <p>
 * Calling this function will trigger \ref libct_app_callbacks_t.on_start_measuring
 * "on_start_measuring()" to be invoked sometime later with results.
 *
 * @param context The context returned from libct_init().
 * @param cal     Auto or manual calibration settings.
 *
 * @return An appropriate \ref libct_status_t "status code" indicating success or error
 */
LIBCTEXPORT int LIBCTAPI libct_start_measuring(libct_context_t* context, libct_cal_t* cal);
/**
 * \ingroup unmanaged_primary_api
 *
 * \brief Stops measuring.
 *
 * Call this method to stop measuring data after calling libct_start_measuring() successfully.
 * <p>
 * Calling this function will trigger \ref libct_app_callbacks_t.on_stop_measuring
 * "on_stop_measuring()" to be invoked sometime later with results.
 *
 * @param context The context returned from libct_init().
 *
 * @return An appropriate \ref libct_status_t "status code" indicating success or error.
 */
LIBCTEXPORT int LIBCTAPI libct_stop_measuring(libct_context_t* context);
/*-------------------------------------------------------------------------------------------------
 * Secondary APIs
 */
/**
 * \ingroup unmanaged_secondary_api
 *
 * \brief Returns the device handle
 *
 * Call this function to get a pointer to the device instance associated with the context.
 *
 * @param context The context returned from libct_init().
 *
 * @return Device object or NULL;
 *
 * @see \ref device "The device APIs"
 */
LIBCTEXPORT libct_device_t* LIBCTAPI libct_get_device(libct_context_t* context);
/**
 * \ingroup unmanaged_secondary_api
 *
 * \brief Sets application specific data that can be retrieved and used later in the callbacks.
 *
 * Basically, this function provides the means to bind your callback application code with the
 * library context. For example, set application instance data after initializing the library,
 * and then retrieve the instance data using libct_get_app_specific_data() inside the \ref libct_app_callbacks_t
 * "callbacks".
 * \code
 * // QT main window initialization
 * MainWindow::MainWindow(QWidget *parent) :
 *    QMainWindow(parent),
 *    ui(new Ui::MainWindow)
 * {
 *      ui->setupUi(this);
 *      setWindowTitle(QString("SampleApp"));
 *
 *      // initialize ui (code not shown)
 *
 *      // initialize the library
 *      int status = libct_init(&context, &init_data, &callbacks);
 *      if ( LIBCT_FAILED(status) ) {
 *          // handle error
 *      }
 *
 *      // set main window instance to act upon in the callbacks
 *      libct_set_app_specific_data(context, this);
 * }
 *
 * // libcaretaker callback - called to notify data from the device
 * void on_data_received_cb(libct_context_t* context, libct_device_t* device, libct_stream_data_t* data) {
 *      MainWindow* window = (MainWindow*) libct_get_app_specific_data(context);
 *
 *      // display the most recent vitals
 *      libct_vitals_t* vitals = libct_get_last_dp(data, vitals);
 *      if ( vitals && vitals->valid ) {
 *          window->setHr(vitals->heart_rate);
 *          window->setRes(vitals->respiration);
 *          window->setMap(vitals->map);
 *          window->setBp(vitals->systolic, vitals->diastolic);
 *      }
 * }
 * \endcode
 *
 * @param context The context returned from libct_init().
 * @param data Generic pointer to the application specific data, or null to clear the existing pointer.
 *
 */
LIBCTEXPORT void LIBCTAPI libct_set_app_specific_data(libct_context_t* context, void* data);
/**
 * \ingroup unmanaged_secondary_api
 *
 * \brief Retrieve application specific data.
 *
 * Retrieves application specific data last set with libct_set_app_specific_data().
 *
 * @param context The context returned from libct_init().
 */
LIBCTEXPORT void* LIBCTAPI libct_get_app_specific_data(libct_context_t* context);
/**
 * \ingroup unmanaged_secondary_api
 *
 * \brief Get the library version info.
 *
 * Call this function to get the library version.
 *
 * @return The library version string.
 */
LIBCTEXPORT const char* LIBCTAPI libct_get_version_string(void);
/**
* \ingroup unmanaged_secondary_api
*
* \brief Get the library build date and time string.
*
* Call this function to get the library build date and time.
*
* @return The library build date string.
*/
LIBCTEXPORT const char* LIBCTAPI libct_get_build_date_string(void);
/**
 * \ingroup unmanaged_secondary_api
 *
 * \brief Sets the library log level.
 *
 * Call this function to set the log level to increase or decrease log messages verbosity.
 *
 * @param level One of the following log levels.
 * <ul>
 * <li> 0 - shows all logs, most verbose</li>
 * <li> 1 - shows only info, warning, and error logs</li>
 * <li> 2 - shows only warning and error logs</li>
 * <li> 3 - shows only error logs, least verbose</li>
 * </ul>
 */
LIBCTEXPORT void LIBCTAPI libct_set_log_level(int level);
/**
 * \ingroup unmanaged_secondary_api
 *
 * \brief Re-calibrates the device.
 *
 * Call this function sometime later after calling libct_start_measuring() to force vital
 * signs re-calibration at the device while taking measurements.
 *
 * @param context The context returned from libct_init().
 *
 * @return Success if a request was queued to be sent to the device, and error otherwise.
 */
LIBCTEXPORT int LIBCTAPI libct_recalibrate(libct_context_t* context);
/**
 * \ingroup unmanaged_secondary_api
 *
 * \brief Adjusts the cuff pressure in 10 mmHg increment/decrement.
 *
 * @note The macros libct_inc_cuff_pressure() and libct_dec_cuff_pressure() simplify this
 *       function so you should use them instead.
 *
 * @param context     The context returned from libct_init().
 * @param direction   Zero - Decrement pressure.
 *                    Nonzero - Increment pressures
 *
 * @see libct_inc_cuff_pressure()
 * @see libct_dec_cuff_pressure()
 *
 * @return Success if a request was queued to be sent to the device, and error otherwise.
 */
LIBCTEXPORT int LIBCTAPI libct_adjust_cuff_pressure(libct_context_t* context, int direction);
/**
 * \ingroup unmanaged_secondary_api
 *
 * Increments the cuff pressure in 10 mmHg increment.
 *
 * @param context     The context returned from libct_init().
 *
 * @return Success if a request was queued to be sent to the device, and error otherwise.
 */
#define libct_inc_cuff_pressure(context) libct_adjust_cuff_pressure((context), 1)
/**
 * \ingroup unmanaged_secondary_api
 *
 * Decrements the cuff pressure in 10 mmHg increment.
 *
 * @param context     The context returned from libct_init().
 *
 * @return Success if a request was queued to be sent to the device, and error otherwise.
 */
#define libct_dec_cuff_pressure(context) libct_adjust_cuff_pressure((context), 0)
 /**
 * \ingroup unmanaged_secondary_api
 *
 * Reads the cuff pressure.
 * Results will be notified later with \ref libct_app_callbacks_t.on_rd_cuff_pressure_rsp "on_rd_cuff_pressure_rsp()".
 *
 * @param context  The context returned from libct_init().
 *
 * @return Success if a request was queued to be sent to the device, and error otherwise.
 */
LIBCTEXPORT int LIBCTAPI libct_rd_cuff_pressure(libct_context_t* context);
/**
 * \ingroup unmanaged_secondary_api
 *
 * \brief Deflates the cuff pressure.
 * Results will be notified later with \ref libct_app_callbacks_t.on_vent_cuff_rsp "on_vent_cuff_rsp()".
 *
 * @param context     The context returned from libct_init().
 *
 * @return Success if a request was queued to be sent to the device, and error otherwise.
 */
LIBCTEXPORT int LIBCTAPI libct_vent_cuff(libct_context_t* context);
/**
 * \ingroup unmanaged_secondary_api
 *
 * \brief Clears the device status.
 * Results will be notified later with \ref libct_app_callbacks_t.on_clr_status_rsp "on_clr_status_rsp()".
 *
 * @param context     The context returned from libct_init().
 *
 * @return Success if a request was queued to be sent to the device, and error otherwise.
 */
LIBCTEXPORT int LIBCTAPI libct_clr_status(libct_context_t* context);
/**
 * \ingroup unmanaged_secondary_api
 *
 * Invoke the device diagnostic plumbing tree flush.
 * Results will be notified later with \ref libct_app_callbacks_t.on_diag_flush_rsp "on_diag_flush_rsp()".
 *
 * @param context   The context returned from libct_init().
 *
 * @return Success if a request was queued to be sent to the device, and error otherwise.
 */
LIBCTEXPORT int LIBCTAPI libct_diag_flush(libct_context_t* context);
/**
 * \ingroup unmanaged_secondary_api
 *
 * Writes the device noise filter parameter.
 * Results will be notified later with \ref libct_app_callbacks_t.on_wrt_snr_min_rsp "on_wrt_snr_min_rsp()".
 *
 * @param context The context returned from libct_init().
 * @param snr     The minimum signal-to-noise value. Valid range [0, 100].
 *
 * @return Success if a request was queued to be sent to the device, and error otherwise.
 */
LIBCTEXPORT int LIBCTAPI libct_wrt_snr_min(libct_context_t* context, int snr);
/**
 * \ingroup unmanaged_secondary_api
 *
 * Reads the device noise filter parameter.
 * Results will be notified later with \ref libct_app_callbacks_t.on_rd_snr_min_rsp "on_rd_snr_min_rsp()".
 *
 * @param context   The context returned from libct_init().
 *
 * @return Success if a request was queued to be sent to the device, and error otherwise.
 */
LIBCTEXPORT int LIBCTAPI libct_rd_snr_min(libct_context_t* context);
/**
 * \ingroup unmanaged_secondary_api
 *
 * Turns the device display on/off.
 * Results will be notified later with \ref libct_app_callbacks_t.on_wrt_display_state_rsp "on_rd_snr_min_rsp()".
 *
 * @param context   The context returned from libct_init().
 * @param state     Display state to write: 0 = off, 1 = on.
 *
 * @return Success if a request was queued to be sent to the device, and error otherwise.
 */
LIBCTEXPORT int LIBCTAPI libct_wrt_display_state(libct_context_t* context, unsigned char state);
/**
 * \ingroup unmanaged_secondary_api
 *
 * Reads the device display state.
 * Results will be notified later with \ref libct_app_callbacks_t.on_rd_display_state_rsp "on_rd_snr_min_rsp()".
 *
 * @param context   The context returned from libct_init().
 *
 * @return Success if a request was queued to be sent to the device, and error otherwise.
 */
LIBCTEXPORT int LIBCTAPI libct_rd_display_state(libct_context_t* context);
/**
 * \ingroup unmanaged_secondary_api
 *
 * Writes the recalibration interval.
 * Results will be notified later with \ref libct_app_callbacks_t.on_wrt_recal_itvl_rsp "on_wrt_recal_itvl_rsp()".
 *
 * @param context  The context returned from libct_init().
 * @param itvl     The recalibration interval in minutes. The acceptable range is [30, 240].
 *
 * @return Success if a request was queued to be sent to the device, and error otherwise.
 */
LIBCTEXPORT int LIBCTAPI libct_wrt_recal_itvl(libct_context_t* context, unsigned int itvl);
/**
 * \ingroup unmanaged_secondary_api
 *
 * Reads the recalibration interval.
 * Results will be notified later with \ref libct_app_callbacks_t.on_rd_recal_itvl_rsp "on_rd_recal_itvl_rsp()".
 *
 * @param context   The context returned from libct_init().
 *
 * @return Success if a request was queued to be sent to the device, and error otherwise.
 */
LIBCTEXPORT int LIBCTAPI libct_rd_recal_itvl(libct_context_t* context);
/**
 * \ingroup unmanaged_secondary_api
 *
 * Writes the device waveform clamping setting.
 * Status will be notified later with \ref libct_app_callbacks_t.on_wrt_waveform_clamping "on_wrt_waveform_clamping()".
 *
 * @param context  The context returned from libct_init().
 * @param value    Clamp setting: 1 = ON, 0 = OFF.
 *
 * @return Success if a request was queued to be sent to the device, and error otherwise.
 */
LIBCTEXPORT int LIBCTAPI libct_wrt_waveform_clamping(libct_context_t* context, unsigned char value);
/**
 * \ingroup unmanaged_secondary_api
 *
 * Reads the device waveform clamping setting.
 * Status will be notified later with \ref libct_app_callbacks_t.on_rd_waveform_clamping "on_rd_waveform_clamping()".
 *
 * @param context  The context returned from libct_init().
 *
 * @return Success if a request was queued to be sent to the device, and error otherwise.
 */
LIBCTEXPORT int LIBCTAPI libct_rd_waveform_clamping(libct_context_t* context);
/**
* \ingroup unmanaged_secondary_api
*
* Reads the current filter settings to enable or disable filtering outlier vitals measurements.
* Status will be notified later with \ref libct_app_callbacks_t.on_rd_vitals_filter "on_rd_vitals_filter()".
*
* @param context  The context returned from libct_init().
*
* @return Success if a request was queued to be sent to the device, and error otherwise.
*/
LIBCTEXPORT int LIBCTAPI libct_rd_vitals_filter(libct_context_t* context);
/**
 * \ingroup unmanaged_secondary_api
 *
 * Writes the filter settings to enable or disable filtering outlier vitals measurements.
 * Status will be notified later with \ref libct_app_callbacks_t.on_wrt_vitals_filter "on_wrt_vitals_filter()".
 *
 * @param context  The context returned from libct_init().
 * @param value   Filter setting value: 1 = Enable, 0 = Disable.
 *
 * @return Success if a request was queued to be sent to the device, and error otherwise.
 */
LIBCTEXPORT int LIBCTAPI libct_wrt_vitals_filter(libct_context_t* context, unsigned char value);
/**
 * \ingroup unmanaged_secondary_api
 *
 * Writes the device simulation mode.
 *
 * @note The device does not provide real-time data when simulation mode is enabled.
 *       Hard-coded numeric and waveform data (i.e., synthetic data) is provided.
 *       As such, simulation mode should be enabled for demonstration and test purposes only.
 *
 * @param context  The context returned from libct_init().
 * @param mode    Simulation mode: 1 = Enable, 0 = Disable.
 *
 * @return Success if a request was queued to be sent to the device, and error otherwise.
 */
LIBCTEXPORT int LIBCTAPI libct_wrt_simulation_mode(libct_context_t* context, unsigned char mode);
/**
 * \ingroup unmanaged_secondary_api
 *
 * Writes the motion tolerance timeout parameter.
 *
 * @param context  The context returned from libct_init().
 * @param timeout  Time out in seconds. Acceptable range [0, 30]
 *
 * @return Success if a request was queued to be sent to the device, and error otherwise.
 */
LIBCTEXPORT int LIBCTAPI libct_wrt_motion_timeout(libct_context_t* context, int timeout);
/**
 * \ingroup unmanaged_secondary_api
 *
 * Reads the motion tolerance timeout parameter.
 * Status will be notified later with \ref libct_app_callbacks_t.on_rd_motion_timeout "on_rd_motion_timeout()".
 *
 * @param context  The context returned from libct_init().
 *
 * @return Success if a request was queued to be sent to the device, and error otherwise.
 */
LIBCTEXPORT int LIBCTAPI libct_rd_motion_timeout(libct_context_t* context);
/**
* \ingroup unmanaged_secondary_api
*
* Reads the device log messages.
* Status will be notified later with \ref libct_app_callbacks_t.on_rd_persistent_log "on_rd_persistent_log()".
*
* @note Reading the device log is a slow request so the results will be delayed by many seconds.
*
* @param context  The context returned from libct_init().
*
* @return Success if a request was queued to be sent to the device, and error otherwise.
*/
LIBCTEXPORT int LIBCTAPI libct_rd_persistent_log(libct_context_t* context);
#ifdef __cplusplus
}
#endif
#endif /* LIBCT_CARETAKER_H */

