/******************************************************************************
* @file libcaretaker_dynamic.hpp
* Header file defining program constructs to load libcaretaker_dynamic.dll and
* setup the library function pointers.
*******************************************************************************/
#ifndef LIBCARETAKER_DYNAMIC_HPP
#define LIBCARETAKER_DYNAMIC_HPP

#include "caretaker_dynamic.h"

/******************************************************************************
* libcaretaker_dynamic.dll function types
*/
typedef int (LIBCTAPI *LP_LIBCT_INIT)(libct_context_t**, libct_init_data_t*, libct_app_callbacks_t*);
typedef void (LIBCTAPI *LP_LIBCT_DEINIT)(libct_context_t*);
typedef int (LIBCTAPI *LP_LIBCT_START_DISCOVERY)(libct_context_t* context, unsigned long timeout);
typedef int (LIBCTAPI *LP_LIBCT_STOP_DISCOVERY)(libct_context_t* context);
typedef int (LIBCTAPI *LP_LIBCT_CONNECT)(libct_context_t* context, libct_device_t* device);
typedef int (LIBCTAPI *LP_LIBCT_DISCONNECT)(libct_context_t* context);
typedef int (LIBCTAPI *LP_LIBCT_START_MONITORING)(libct_context_t* context, int flags);
typedef int (LIBCTAPI *LP_LIBCT_STOP_MONITORING)(libct_context_t* context);
typedef int (LIBCTAPI *LP_LIBCT_START_MEASURING)(libct_context_t* context, libct_cal_t* cal);
typedef int (LIBCTAPI *LP_LIBCT_STOP_MEASURING)(libct_context_t* context);
typedef libct_device_t* (LIBCTAPI *LP_LIBCT_GET_DEVICE)(libct_context_t* context);
typedef void (LIBCTAPI *LP_LIBCT_SET_APP_SPECIFIC_DATA)(libct_context_t* context, void* data);
typedef void* (LIBCTAPI *LP_LIBCT_GET_APP_SPECIFIC_DATA)(libct_context_t* context);
typedef const char* (LIBCTAPI *LP_LIBCT_GET_VERSION_STRING)(void);
typedef const char* (LIBCTAPI *LP_LIBCT_GET_BUILD_DATE_STRING)(void);
typedef void (LIBCTAPI *LP_LIBCT_SET_LOG_LEVEL)(int level);
typedef int (LIBCTAPI *LP_LIBCT_RECALIBRATE)(libct_context_t* context);
typedef int (LIBCTAPI *LP_LIBCT_ADJUST_CUFF_PRESSURE)(libct_context_t* context, int direction);
typedef int (LIBCTAPI *LP_LIBCT_READ_CUFF_PRESSURE)(libct_context_t* context);
typedef int (LIBCTAPI *LP_LIBCT_VENT_CUFF)(libct_context_t* context);
typedef int (LIBCTAPI *LP_LIBCT_CLEAR_STATUS)(libct_context_t* context);
typedef int (LIBCTAPI *LP_LIBCT_DIAGNOSTIC_FLUSH)(libct_context_t* context);
typedef int (LIBCTAPI *LP_LIBCT_WRITE_SNR_MINIMUM)(libct_context_t* context, int snr);
typedef int (LIBCTAPI *LP_LIBCT_READ_SNR_MINIMUM)(libct_context_t* context);
typedef int (LIBCTAPI *LP_LIBCT_WRITE_DISPLAY_STATE)(libct_context_t* context, unsigned char state);
typedef int (LIBCTAPI *LP_LIBCT_READ_DISPLAY_STATE)(libct_context_t* context);
typedef int (LIBCTAPI *LP_LIBCT_WRITE_RECAL_INTERVAL)(libct_context_t* context, unsigned int itvl);
typedef int (LIBCTAPI *LP_LIBCT_READ_RECAL_INTERVAL)(libct_context_t* context);
typedef int (LIBCTAPI *LP_LIBCT_WRITE_WAVEFORM_CLAMPING)(libct_context_t* context, unsigned char value);
typedef int (LIBCTAPI *LP_LIBCT_READ_WAVEFORM_CLAMPING)(libct_context_t* context);
typedef int (LIBCTAPI *LP_LIBCT_WRITE_MEDIAN_FILTER)(libct_context_t* context, unsigned char value);
typedef int (LIBCTAPI *LP_LIBCT_READ_MEDIAN_FILTER)(libct_context_t* context);
typedef int (LIBCTAPI *LP_LIBCT_WRITE_AMBULATORY_FILTER)(libct_context_t* context, unsigned char value);
typedef int (LIBCTAPI *LP_LIBCT_READ_AMBULATORY_FILTER)(libct_context_t* context);
typedef int (LIBCTAPI *LP_LIBCT_WRITE_SIMULATION_MODE)(libct_context_t* context, unsigned char mode);
typedef int (LIBCTAPI *LP_LIBCT_WRITE_MOTION_TIMEOUT)(libct_context_t* context, int timeout);
typedef int (LIBCTAPI *LP_LIBCT_READ_MOTION_TIMEOUT)(libct_context_t* context);
typedef int (LIBCTAPI *LP_LIBCT_READ_PERSISTENT_LOG)(libct_context_t* context);

/******************************************************************************
* Structure of libcaretaker_dynamic.dll function pointers
*/
static struct {
	HINSTANCE Handle;
	LP_LIBCT_INIT Init;
	LP_LIBCT_DEINIT Deinit;
	LP_LIBCT_START_DISCOVERY StartDiscovery;
	LP_LIBCT_STOP_DISCOVERY StopDiscovery;
	LP_LIBCT_CONNECT Connect;
	LP_LIBCT_DISCONNECT Disconnect;
	LP_LIBCT_START_MONITORING StartMonitoring;
	LP_LIBCT_STOP_MONITORING StopMonitoring;
	LP_LIBCT_START_MEASURING StartMeasuring;
	LP_LIBCT_STOP_MEASURING StopMeasuring;
	LP_LIBCT_GET_DEVICE GetDevice;
	LP_LIBCT_SET_APP_SPECIFIC_DATA SetAppSpecificData;
	LP_LIBCT_GET_APP_SPECIFIC_DATA GetAppSpecificData;
	LP_LIBCT_GET_VERSION_STRING GetVersionString;
	LP_LIBCT_GET_BUILD_DATE_STRING GetBuildDateString;
	LP_LIBCT_SET_LOG_LEVEL SetLogLevel;
	LP_LIBCT_RECALIBRATE Recalibrate;
	LP_LIBCT_ADJUST_CUFF_PRESSURE AdjustCuffPressure;
	LP_LIBCT_READ_CUFF_PRESSURE ReadCuffPressure;
	LP_LIBCT_VENT_CUFF VentCuff;
	LP_LIBCT_CLEAR_STATUS ClearStatus;
	LP_LIBCT_DIAGNOSTIC_FLUSH Flush;
	LP_LIBCT_WRITE_SNR_MINIMUM WriteSnrMinimum;
	LP_LIBCT_READ_SNR_MINIMUM ReadSnrMinimum;
	LP_LIBCT_WRITE_DISPLAY_STATE WriteDisplayState;
	LP_LIBCT_READ_DISPLAY_STATE ReadDisplayState;
	LP_LIBCT_WRITE_RECAL_INTERVAL WriteRecalInterval;
	LP_LIBCT_READ_RECAL_INTERVAL ReadRecalInterval;
	LP_LIBCT_WRITE_WAVEFORM_CLAMPING WriteWaveformClamping;
	LP_LIBCT_READ_WAVEFORM_CLAMPING ReadWaveformClamping;
	LP_LIBCT_WRITE_MEDIAN_FILTER WriteMedianFilter;
	LP_LIBCT_READ_MEDIAN_FILTER ReadMedianFilter;
    LP_LIBCT_WRITE_AMBULATORY_FILTER WriteAmbulatoryFilter;
    LP_LIBCT_READ_AMBULATORY_FILTER ReadAmbulatoryFilter;
	LP_LIBCT_WRITE_SIMULATION_MODE WriteSimulationMode;
	LP_LIBCT_WRITE_MOTION_TIMEOUT WriteMotionTimeout;
	LP_LIBCT_READ_MOTION_TIMEOUT ReadMotionTimeout;
	LP_LIBCT_READ_PERSISTENT_LOG ReadPersistentLog;
} LIBCT_DLL = { 0 };

/******************************************************************************
* Macros mapping libcaretaker_dynamic.dll function pointers to the actual
* function names exported in caretaker_dynamic.h.
*/

// Redefine library APIs to DLL functions
#define libct_init LIBCT_DLL.Init
#define libct_deinit LIBCT_DLL.Deinit
#define libct_start_discovery LIBCT_DLL.StartDiscovery
#define libct_stop_discovery LIBCT_DLL.StopDiscovery
#define libct_connect LIBCT_DLL.Connect
#define libct_disconnect LIBCT_DLL.Disconnect
#define libct_start_monitoring LIBCT_DLL.StartMonitoring
#define libct_stop_monitoring LIBCT_DLL.StopMonitoring
#define libct_start_measuring LIBCT_DLL.StartMeasuring
#define libct_stop_measuring LIBCT_DLL.StopMeasuring
#define libct_get_device LIBCT_DLL.GetDevice
#define libct_set_app_specific_data LIBCT_DLL.SetAppSpecificData
#define libct_get_app_specific_data LIBCT_DLL.GetAppSpecificData
#define libct_get_version_string LIBCT_DLL.GetVersionString
#define libct_get_build_date_string LIBCT_DLL.GetBuildDateString
#define libct_set_log_level LIBCT_DLL.SetLogLevel
#define libct_recalibrate LIBCT_DLL.Recalibrate
#define libct_adjust_cuff_pressure LIBCT_DLL.AdjustCuffPressure
#define libct_rd_cuff_pressure LIBCT_DLL.ReadCuffPressure
#define libct_vent_cuff LIBCT_DLL.VentCuff
#define libct_clr_status LIBCT_DLL.ClearStatus
#define libct_diag_flush LIBCT_DLL.Flush
#define libct_wrt_snr_min LIBCT_DLL.WriteSnrMinimum
#define libct_rd_snr_min LIBCT_DLL.ReadSnrMinimum
#define libct_wrt_display_state LIBCT_DLL.WriteDisplayState
#define libct_rd_display_state LIBCT_DLL.ReadDisplayState
#define libct_wrt_recal_itvl LIBCT_DLL.WriteRecalInterval
#define libct_rd_recal_itvl LIBCT_DLL.ReadRecalInterval
#define libct_wrt_waveform_clamping LIBCT_DLL.WriteWaveformClamping
#define libct_rd_waveform_clamping LIBCT_DLL.ReadWaveformClamping
#define libct_wrt_median_filter LIBCT_DLL.WriteMedianFilter
#define libct_rd_median_filter LIBCT_DLL.ReadMedianFilter
#define libct_rd_ambulatory_filter  LIBCT_DLL.ReadAmbulatoryFilter
#define libct_wrt_ambulatory_filter LIBCT_DLL.WriteAmbulatoryFilter
#define libct_wrt_simulation_mode LIBCT_DLL.WriteSimulationMode
#define libct_wrt_motion_timeout LIBCT_DLL.WriteMotionTimeout
#define libct_rd_motion_timeout LIBCT_DLL.ReadMotionTimeout
#define libct_rd_persistent_log LIBCT_DLL.ReadPersistentLog

/******************************************************************************
* libcaretaker_dynamic.dll loader function
*/
static bool load_libcaretaker_dll() {

	// Load DLL
	LIBCT_DLL.Handle = LoadLibrary(L"libcaretaker_dynamic.dll");
	if (!LIBCT_DLL.Handle) {
		return false;
	}

	// Note x86 and x64 DLL name decorations are different.
	// For _stdcall convention, the x86 names have format _NAME@N, where NAME is the function
	// name and N is the size of the function arguments. For example, libct_init is decorated
	// as _libct_init@12.
	// These decoration differences were handled by use of libcaretaker_dynamic.def built
    // with the libcaretaker_dynamic project.

	LIBCT_DLL.Init = (LP_LIBCT_INIT)GetProcAddress(LIBCT_DLL.Handle, "libct_init");
	LIBCT_DLL.Deinit = (LP_LIBCT_DEINIT)GetProcAddress(LIBCT_DLL.Handle, "libct_deinit");
	LIBCT_DLL.StartDiscovery = (LP_LIBCT_START_DISCOVERY)GetProcAddress(LIBCT_DLL.Handle, "libct_start_discovery");
	LIBCT_DLL.StopDiscovery = (LP_LIBCT_STOP_DISCOVERY)GetProcAddress(LIBCT_DLL.Handle, "libct_stop_discovery");
	LIBCT_DLL.Connect = (LP_LIBCT_CONNECT)GetProcAddress(LIBCT_DLL.Handle, "libct_connect");
	LIBCT_DLL.Disconnect = (LP_LIBCT_DISCONNECT)GetProcAddress(LIBCT_DLL.Handle, "libct_disconnect");
	LIBCT_DLL.StartMonitoring = (LP_LIBCT_START_MONITORING)GetProcAddress(LIBCT_DLL.Handle, "libct_start_monitoring");
	LIBCT_DLL.StopMonitoring = (LP_LIBCT_STOP_MONITORING)GetProcAddress(LIBCT_DLL.Handle, "libct_stop_monitoring");
	LIBCT_DLL.StartMeasuring = (LP_LIBCT_START_MEASURING)GetProcAddress(LIBCT_DLL.Handle, "libct_start_measuring");
	LIBCT_DLL.StopMeasuring = (LP_LIBCT_STOP_MEASURING)GetProcAddress(LIBCT_DLL.Handle, "libct_stop_measuring");
	LIBCT_DLL.GetDevice = (LP_LIBCT_GET_DEVICE)GetProcAddress(LIBCT_DLL.Handle, "libct_get_device");
	LIBCT_DLL.SetAppSpecificData = (LP_LIBCT_SET_APP_SPECIFIC_DATA)GetProcAddress(LIBCT_DLL.Handle, "libct_set_app_specific_data");
	LIBCT_DLL.GetAppSpecificData = (LP_LIBCT_GET_APP_SPECIFIC_DATA)GetProcAddress(LIBCT_DLL.Handle, "libct_get_app_specific_data");
	LIBCT_DLL.GetVersionString = (LP_LIBCT_GET_VERSION_STRING)GetProcAddress(LIBCT_DLL.Handle, "libct_get_version_string");
	LIBCT_DLL.GetBuildDateString = (LP_LIBCT_GET_BUILD_DATE_STRING)GetProcAddress(LIBCT_DLL.Handle, "libct_get_build_date_string");
	LIBCT_DLL.SetLogLevel = (LP_LIBCT_SET_LOG_LEVEL)GetProcAddress(LIBCT_DLL.Handle, "libct_set_log_level");
	LIBCT_DLL.Recalibrate = (LP_LIBCT_RECALIBRATE)GetProcAddress(LIBCT_DLL.Handle, "libct_recalibrate");
	LIBCT_DLL.AdjustCuffPressure = (LP_LIBCT_ADJUST_CUFF_PRESSURE)GetProcAddress(LIBCT_DLL.Handle, "libct_adjust_cuff_pressure");
	LIBCT_DLL.ReadCuffPressure = (LP_LIBCT_READ_CUFF_PRESSURE)GetProcAddress(LIBCT_DLL.Handle, "libct_rd_cuff_pressure");
	LIBCT_DLL.VentCuff = (LP_LIBCT_VENT_CUFF)GetProcAddress(LIBCT_DLL.Handle, "libct_vent_cuff");
	LIBCT_DLL.ClearStatus = (LP_LIBCT_CLEAR_STATUS)GetProcAddress(LIBCT_DLL.Handle, "libct_clr_status");
	LIBCT_DLL.Flush = (LP_LIBCT_DIAGNOSTIC_FLUSH)GetProcAddress(LIBCT_DLL.Handle, "libct_diag_flush");
	LIBCT_DLL.WriteSnrMinimum = (LP_LIBCT_WRITE_SNR_MINIMUM)GetProcAddress(LIBCT_DLL.Handle, "libct_wrt_snr_min");
	LIBCT_DLL.ReadSnrMinimum = (LP_LIBCT_READ_SNR_MINIMUM)GetProcAddress(LIBCT_DLL.Handle, "libct_rd_snr_min");
	LIBCT_DLL.WriteDisplayState = (LP_LIBCT_WRITE_DISPLAY_STATE)GetProcAddress(LIBCT_DLL.Handle, "libct_wrt_display_state");
	LIBCT_DLL.ReadDisplayState = (LP_LIBCT_READ_DISPLAY_STATE)GetProcAddress(LIBCT_DLL.Handle, "libct_rd_display_state");
	LIBCT_DLL.WriteRecalInterval = (LP_LIBCT_WRITE_RECAL_INTERVAL)GetProcAddress(LIBCT_DLL.Handle, "libct_wrt_recal_itvl");
	LIBCT_DLL.ReadRecalInterval = (LP_LIBCT_READ_RECAL_INTERVAL)GetProcAddress(LIBCT_DLL.Handle, "libct_rd_recal_itvl");
	LIBCT_DLL.WriteWaveformClamping = (LP_LIBCT_WRITE_WAVEFORM_CLAMPING)GetProcAddress(LIBCT_DLL.Handle, "libct_wrt_waveform_clamping");
	LIBCT_DLL.ReadWaveformClamping = (LP_LIBCT_READ_WAVEFORM_CLAMPING)GetProcAddress(LIBCT_DLL.Handle, "libct_rd_waveform_clamping");
	LIBCT_DLL.WriteMedianFilter = (LP_LIBCT_WRITE_MEDIAN_FILTER)GetProcAddress(LIBCT_DLL.Handle, "libct_wrt_median_filter");
	LIBCT_DLL.ReadMedianFilter = (LP_LIBCT_READ_MEDIAN_FILTER)GetProcAddress(LIBCT_DLL.Handle, "libct_rd_median_filter");
    LIBCT_DLL.WriteAmbulatoryFilter = (LP_LIBCT_WRITE_MEDIAN_FILTER)GetProcAddress(LIBCT_DLL.Handle, "libct_wrt_ambulatory_filter");
    LIBCT_DLL.ReadAmbulatoryFilter = (LP_LIBCT_READ_MEDIAN_FILTER)GetProcAddress(LIBCT_DLL.Handle, "libct_rd_ambulatory_filter");
	LIBCT_DLL.WriteSimulationMode = (LP_LIBCT_WRITE_SIMULATION_MODE)GetProcAddress(LIBCT_DLL.Handle, "libct_wrt_simulation_mode");
	LIBCT_DLL.WriteMotionTimeout = (LP_LIBCT_WRITE_MOTION_TIMEOUT)GetProcAddress(LIBCT_DLL.Handle, "libct_wrt_motion_timeout");
	LIBCT_DLL.ReadMotionTimeout = (LP_LIBCT_READ_MOTION_TIMEOUT)GetProcAddress(LIBCT_DLL.Handle, "libct_rd_motion_timeout");
	LIBCT_DLL.ReadPersistentLog = (LP_LIBCT_READ_PERSISTENT_LOG)GetProcAddress(LIBCT_DLL.Handle, "libct_rd_persistent_log");

	return true;
}

/******************************************************************************
* libcaretaker_dynamic.dll loader function
*/
static bool unload_libcaretaker_dll() {
	FreeLibrary(LIBCT_DLL.Handle);
	memset(&LIBCT_DLL, 0, sizeof(LIBCT_DLL));
	return true;
}

/******************************************************************************
* libcaretaker_dynamic.dll loader macros
*/
#undef LOAD_LIBCARETAKER_DLL
#undef UNLOAD_LIBCARETAKER_DLL
#define LOAD_LIBCARETAKER_DLL  load_libcaretaker_dll
#define UNLOAD_LIBCARETAKER_DLL  unload_libcaretaker_dll

#endif /* LIBCARETAKER_DYNAMIC_HPP */


