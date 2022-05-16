/**************************************************************************************************************
 * Sample C# application illustrating use of libcaretaker_clr to monitor the Caretaker device.
 * This application illustrates using the Caretaker.DeviceObserver asynchronous interface for monitoring.
 */
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.ComponentModel;

namespace CaretakerCSharpSampleApp
{
    /// <summary>
    /// Caretaker device observer implementation to receive real-time data
    /// </summary>
    public class CaretakerObserver : Caretaker.DeviceObserver
    {
        MainWindow window = null;

        /**************************************************************************************************************
         */
        public CaretakerObserver(MainWindow window)
        {
            this.window = window;
        }

        /**************************************************************************************************************
         */
        public override void OnConnectionStatus(Caretaker.Device device, Caretaker.ConnectionStatus status)
        {
            // Do nothing if the application exited
            if (Application.Current == null)
            {
                return;
            }

            /* Note the calls to Application.Current.Dispatcher.BeginInvoke() below execute asynchronous
             * processing on the main thread to update the UI. You should not do heavy processing or
             * perform blocking operations, such as writing to file, in the Action passed as argument
             * to prevent freezing the application.
             * 
             * Also, this method is called by the low-level stack thread that must not be blocked or else
             * data or events may be lost, so do not call Application.Current.Dispatcher.Invoke() or
             * perform any blocking operation here.
             */
            switch (status)
            {
                case Caretaker.ConnectionStatus.SCAN_ERROR:
                    window.Connecting = false;
                    if (!window.Aborting)
                    {
                        Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            window.Status.Content = "Scan error. Plug or unplug then replug BLE dongle then retry.";
                            window.Status.Background = window.mErrorStatusBackground;
                            window.Status.Foreground = window.mErrorStatusForeground;
                            window.SerialNumberTextBox.IsEnabled = true;
                            window.ConnectButton.Content = "Connect";
                            foreach (Button b in window.OperationButons)
                            {
                                b.IsEnabled = false;
                            }
                        }));
                    }
                    break;

                case Caretaker.ConnectionStatus.SCAN_TIMEOUT:
                    window.Connecting = false;
                    if (!window.Aborting)
                    {
                        Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            window.Status.Content = "Scan timeout. Device not found.";
                            window.Status.Background = window.mErrorStatusBackground;
                            window.Status.Foreground = window.mErrorStatusForeground;
                            window.SerialNumberTextBox.IsEnabled = true;
                            window.ConnectButton.Content = "Connect";
                            foreach (Button b in window.OperationButons)
                            {
                                b.IsEnabled = false;
                            }
                        }));
                    }
                    break;

                case Caretaker.ConnectionStatus.CONNECT_ERROR:
                    window.Connecting = false;
                    if (!window.Aborting)
                    {
                        Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            window.Status.Content = "Connect error.";
                            window.Status.Background = window.mErrorStatusBackground;
                            window.Status.Foreground = window.mErrorStatusForeground;
                            window.SerialNumberTextBox.IsEnabled = true;
                            window.ConnectButton.Content = "Connect";
                            foreach (Button b in window.OperationButons)
                            {
                                b.IsEnabled = false;
                            }
                        }));
                    }
                    break;

                case Caretaker.ConnectionStatus.CONNECT_TIMEOUT:
                    window.Connecting = false;
                    if (!window.Aborting)
                    {
                        Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            window.Status.Content = "Connect timeout.";
                            window.Status.Background = window.mErrorStatusBackground;
                            window.Status.Foreground = window.mErrorStatusForeground;
                            window.SerialNumberTextBox.IsEnabled = true;
                            window.ConnectButton.Content = "Connect";
                            foreach (Button b in window.OperationButons)
                            {
                                b.IsEnabled = false;
                            }
                        }));
                    }
                    break;

                case Caretaker.ConnectionStatus.CONNECTED:
                    window.Connecting = false;
                    Application.Current.Dispatcher.BeginInvoke(new Action(() => {
                        window.Status.Content = "Connected. Press start to calibrate.";
                        window.Status.Background = window.mConnectedStatusBackground;
                        window.Status.Foreground = window.mConnectedStatusForeground;
                        window.SerialNumberTextBox.IsEnabled = false;
                        window.ConnectButton.Content = "Disconnect";
                        foreach(Button b in window.OperationButons)
                        {
                            b.IsEnabled = true;
                        }
                        window.PulsePressureWaveform.Clear();
                        window.PulseRateWaveform.Clear();
                    }));
                    break;

                case Caretaker.ConnectionStatus.DISCONNECTED:
                    window.Disconnecting = false;
                    window.Aborting = false;

                    // Note this delay is not required, but added here to allow Disconnecting... status to
                    // show on the UI.
                    System.Threading.Thread.Sleep(1000);

                    Application.Current.Dispatcher.BeginInvoke(new Action(() => {
                        window.HeartRate.Foreground = Brushes.Gray;
                        window.Systolic.Foreground = Brushes.Gray;
                        window.Diastolic.Foreground = Brushes.Gray;
                        window.MAP.Foreground = Brushes.Gray;
                        window.BpDividerLine.Stroke = Brushes.Gray;
                        window.Respiration.Foreground = Brushes.Gray;
                        window.Status.Content = "Enter Caretaker serial number then press connect to begin.";
                        window.Status.Background = window.mDefaultStatusBackground;
                        window.Status.Foreground = window.mDefaultStatusForeground;
                        window.SerialNumberTextBox.IsEnabled = true;
                        window.ConnectButton.Content = "Connect";
                        foreach (Button b in window.OperationButons)
                        {
                            b.IsEnabled = false;
                        }
                    }));
                    break;

                case Caretaker.ConnectionStatus.CONNECTION_LOST:
                    Application.Current.Dispatcher.BeginInvoke(new Action(() => {
                        window.HeartRate.Foreground = Brushes.Gray;
                        window.Systolic.Foreground = Brushes.Gray;
                        window.Diastolic.Foreground = Brushes.Gray;
                        window.MAP.Foreground = Brushes.Gray;
                        window.BpDividerLine.Stroke = Brushes.Gray;
                        window.Respiration.Foreground = Brushes.Gray;
                        window.Status.Content = "Connect lost. Reconnecting...";
                        window.Status.Background = window.mErrorStatusBackground;
                        window.Status.Foreground = window.mErrorStatusForeground;
                        window.SerialNumberTextBox.IsEnabled = true;
                        window.ConnectButton.Content = "Disconnect";
                        foreach (Button b in window.OperationButons)
                        {
                            b.IsEnabled = false;
                        }
                    }));
                    break;
            }
        }

        /**************************************************************************************************************
         */
        public override void OnStartStatus(Caretaker.Device device, Caretaker.StartStatus status)
        {
            // Do nothing if the application exited
            if (Application.Current == null)
            {
                return;
            }

            window.Starting = false;

            /* Note the call to Application.Current.Dispatcher.BeginInvoke() below executes asynchronous
             * processing on the main thread to update the UI. You should not do heavy processing or
             * perform blocking operations, such as writing to file, in the Action passed as argument
             * to prevent freezing the application.
             */
            if (status == Caretaker.StartStatus.START_ERROR)
            {
                Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                {
                    window.Status.Content = "Start error. Press start to retry.";
                    window.Status.Background = window.mErrorStatusBackground;
                    window.Status.Foreground = window.mErrorStatusForeground;
                }));
            }
        }

        /**************************************************************************************************************
         */
        public override void OnStopStatus(Caretaker.Device device, Caretaker.StopStatus status)
        {
            // Do nothing if the application exited
            if (Application.Current == null)
            {
                return;
            }

            /* Note the call to Application.Current.Dispatcher.BeginInvoke() below executes asynchronous
             * processing on the main thread to update the UI. You should not do heavy processing or
             * perform blocking operations, such as writing to file, in the Action passed as argument
             * to prevent freezing the application.
             */

            if (!window.Starting)
            {
                switch (status)
                {
                    case Caretaker.StopStatus.STOPPED:
                        Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            window.Status.Content = "Stopped. Press start to continue.";
                        }));
                        break;

                    default:
                        Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            window.Status.Content = "Stop error.";
                        }));
                        break;
                }
            }
        }

        /**************************************************************************************************************
         */
        public override void OnBatteryStatus(Caretaker.Device device, Caretaker.BatteryStatus status)
        {
            // Do nothing if the application exited
            if (Application.Current == null)
            {
                return;
            }

            /* Note the call to Application.Current.Dispatcher.BeginInvoke() below executes asynchronous
             * processing on the main thread to update the UI. You should not do heavy processing or
             * perform blocking operations, such as writing to file, in the Action passed as argument
             * to prevent freezing the application.
             */

            Application.Current.Dispatcher.BeginInvoke(new Action(() => {
                StringBuilder sb = new StringBuilder();
                sb.AppendFormat(null, "{0:F1} V", status.voltage / 1000.0);
                window.BatteryVoltage.Content = sb.ToString();
            }));

        }

        /**************************************************************************************************************
         */
        public override void OnCuffStatus(Caretaker.Device device, Caretaker.CuffStatus status)
        {
            // Do nothing if the application exited
            if (Application.Current == null)
            {
                return;
            }

            /* Note the call to Application.Current.Dispatcher.BeginInvoke() below executes asynchronous
             * processing on the main thread to update the UI. You should not do heavy processing or
             * perform blocking operations, such as writing to file, in the Action passed as argument
             * to prevent freezing the application.
             */

            Application.Current.Dispatcher.BeginInvoke(new Action(() => {
                window.TargetPressure.Content = "" + status.targetPressure;
                window.ActualPressure.Content = "" + status.actualPressure + " mmHg";
                int snr = Math.Min(100, status.signalToNoise / 10);
                window.SignalNoiseRatio.Content = "" + Math.Max(0,snr) + "%";
            }));

        }

        /**************************************************************************************************************
         */
        public override void OnDeviceStatus(Caretaker.Device device, Caretaker.DeviceStatus status)
        {
            // Do nothing if the application exited
            if (Application.Current == null)
            {
                return;
            }

            /* Note calls to Application.Current.Dispatcher.BeginInvoke() below execute asynchronous
             * processing on the main thread to update the UI. You should not do heavy processing or
             * perform blocking operations, such as writing to file, in the Action passed as argument
             * to prevent freezing the application.
             */
            if ( device.Calibrating() )
            {
                Application.Current.Dispatcher.BeginInvoke(new Action(() => {
                    window.Status.Background = window.mConnectedStatusBackground;
                    window.Status.Foreground = window.mConnectedStatusForeground;
                    window.Status.Content = "Calibrating... " + status.calibrationPercentage + "%";
                }));
            }

            else if (device.Calibrated())
            {
                Application.Current.Dispatcher.BeginInvoke(new Action(() => {
                    window.Status.Background = window.mConnectedStatusBackground;
                    window.Status.Foreground = window.mConnectedStatusForeground;
                    window.Status.Content = "Calibrated";
                }));
            }

            else if (status.invalidDataEntry)
            {
                String reason = "Invalid input";
                Application.Current.Dispatcher.BeginInvoke(new Action(() => {
                    window.Status.Content = reason;
                    window.Status.Background = window.mAlertStatusBackground;
                    window.Status.Foreground = window.mAlertStatusForeground;
                }));
            }

            else if (device.CalibrationFailed())
            {
                // Note one or more of these events during calibration will cause it fail.
                String reason = "Calibration failed";
                if (status.poorSignal) reason += ": Poor pulse signal";
                if (status.noPulseTimeout) reason += ": No pulse timeout";
                if (status.weakSignal) reason += ": Weak signal";
                if (status.inflateFailed) reason += ": Failed to inflate cuff";
                if (status.cuffTooTight) reason += ": Cuff too tight. Please adjust cuff.";
                if (status.cuffTooLoose) reason += ": Cuff too loose. Please adjust cuff";
                if (status.motionEvent) reason += ": Motion detected. Please keep still.";
                if (status.badCuff) reason += ": Bad cuff. Please replace cuff.";

                Application.Current.Dispatcher.BeginInvoke(new Action(() => {
                    window.Status.Content = reason;
                    window.Status.Background = window.mAlertStatusBackground;
                    window.Status.Foreground = window.mAlertStatusForeground;
                }));
            }

            if (status.poorSignal)
            {
                Application.Current.Dispatcher.BeginInvoke(new Action(() => {
                    window.Status.Content = "Poor pulse signal";
                    window.Status.Background = window.mAlertStatusBackground;
                    window.Status.Foreground = window.mAlertStatusForeground;
                }));
            }

            if (status.noPulseTimeout)
            {
                Application.Current.Dispatcher.BeginInvoke(new Action(() => {
                    window.Status.Content = "No pulse timeout";
                    window.Status.Background = window.mAlertStatusBackground;
                    window.Status.Foreground = window.mAlertStatusForeground;
                }));
            }

            if (status.weakSignal)
            {
                Application.Current.Dispatcher.BeginInvoke(new Action(() => {
                    window.Status.Content = "Weak signal";
                    window.Status.Background = window.mAlertStatusBackground;
                    window.Status.Foreground = window.mAlertStatusForeground;
                }));
            }

            if (status.inflateFailed)
            {
                Application.Current.Dispatcher.BeginInvoke(new Action(() => {
                    window.Status.Content = "Failed to inflate cuff";
                    window.Status.Background = window.mAlertStatusBackground;
                    window.Status.Foreground = window.mAlertStatusForeground;
                }));
            }

            if (status.cuffTooTight)
            {
                Application.Current.Dispatcher.BeginInvoke(new Action(() => {
                    window.Status.Content = "Cuff too tight. Please adjust cuff.";
                    window.Status.Background = window.mAlertStatusBackground;
                    window.Status.Foreground = window.mAlertStatusForeground;
                }));
            }

            if (status.cuffTooLoose)
            {
                Application.Current.Dispatcher.BeginInvoke(new Action(() => {
                    window.Status.Content = "Cuff too loose. Please adjust cuff.";
                    window.Status.Background = window.mAlertStatusBackground;
                    window.Status.Foreground = window.mAlertStatusForeground;
                }));
            }

            if (status.motionEvent)
            {
                Application.Current.Dispatcher.BeginInvoke(new Action(() => {
                    window.Status.Content = "Motion detected. Please keep still.";
                    window.Status.Background = window.mAlertStatusBackground;
                    window.Status.Foreground = window.mAlertStatusForeground;
                }));
            }

            if (status.badCuff)
            {
                Application.Current.Dispatcher.BeginInvoke(new Action(() => {
                    window.Status.Content = "Bad cuff. Please replace cuff.";
                    window.Status.Background = window.mAlertStatusBackground;
                    window.Status.Foreground = window.mAlertStatusForeground;
                }));
            }

            // mark measurements invalid
            if (!status.dataValid)
            {
                Application.Current.Dispatcher.BeginInvoke(new Action(() => {
                    window.HeartRate.Foreground = Brushes.Gray;
                    window.Systolic.Foreground = Brushes.Gray;
                    window.Diastolic.Foreground = Brushes.Gray;
                    window.MAP.Foreground = Brushes.Gray;
                    window.BpDividerLine.Stroke = Brushes.Gray;
                    window.Respiration.Foreground = Brushes.Gray;
                }));
            }
        }

        /**************************************************************************************************************
         */
        public override void OnPulseRateWaveformDataPoints(Caretaker.Device device, Caretaker.WaveformDataPoints datapoints)
        {
            // Do nothing if the application exited
            if (Application.Current == null)
            {
                return;
            }

            window.PulseRateWaveform.AddPoints(datapoints.values, datapoints.timestamps);
        }

        /**************************************************************************************************************
         */
        public override void OnPulsePressureWaveformDataPoints(Caretaker.Device device, Caretaker.WaveformDataPoints datapoints)
        {
            // Do nothing if the application exited
            if (Application.Current == null)
            {
                return;
            }

            window.PulsePressureWaveform.AddPoints(datapoints.values, datapoints.timestamps);
        }

        /**************************************************************************************************************
         */
        public override void OnParameterizedPulse(
            Caretaker.Device device,
             Caretaker.WaveformDataPoints datapoints,
            Int16 t0,
            Int16 t1,
            Int16 t2,
            Int16 t3,
            Int32 p0,
            Int32 p1,
            Int32 p2,
            Int32 p3
        )
        {
            // Do nothing if the application exited
            if (Application.Current == null)
            {
                return;
            }

            Console.WriteLine("*** Pulse Parameters ***");
            Console.WriteLine("T0, P0: {0}, {1}", t0, p0);
            Console.WriteLine("T1, P1: {0}, {1}", t1, p1);
            Console.WriteLine("T2, P2: {0}, {1}", t2, p2);
            Console.WriteLine("T3, P3: {0}, {1}", t3, p3);
        }


        /**************************************************************************************************************
         */
        public override void OnPrimaryVitals(Caretaker.Device device, Caretaker.PrimaryVitals[] vitalsArray)
        {
            // Do nothing if the application exited
            if (Application.Current == null)
            {
                return;
            }

            /* Note Application.Current.Dispatcher.BeginInvoke() executes asynchronous processing on the main thread
             * to update the UI, but the synchronous Application.Current.Dispatcher.Invoke() is called here instead
             * to throttle library calls since OnPrimaryVitals() maybe called very regularly (once or multiple times
             * per second) to report vitals to the application depending on the Caretaker's update interval as well
             * as queuing in the library. If Dispatcher.BeginInvoke() is called instead, the application must be
             * capable of handling  high library call volumes to prevent application freeze.
             * 
             * Also, you should not perform heavy processing or perform blocking operations, such as writing to
             * file, in the Action passed as argument to Dispatcher.Invoke() to prevent application freeze.
             */
            Application.Current.Dispatcher.Invoke(new Action(() => {
                // display most recent vitals
                Caretaker.PrimaryVitals vitals = vitalsArray[vitalsArray.Length - 1];
                if (vitals.heartRate > 0)
                    window.HeartRate.Content = "" + vitals.heartRate;

                window.Systolic.Content = "" + vitals.systolic;
                window.Diastolic.Content = "" + vitals.diastolic;

                window.MAP.Content = "(" + vitals.map + ")";
                if (vitals.respiration > 0 )
                    window.Respiration.Content = "" + vitals.respiration;

                int sqe = Math.Min(100, (int)vitals.signalQualityEstimate/100);
                window.SignalQaulity.Content = "" + Math.Max(0,sqe) + "%";

                window.HeartRate.Foreground = Brushes.Green;
                window.Systolic.Foreground = Brushes.Red;
                window.Diastolic.Foreground = Brushes.Red;
                window.MAP.Foreground = Brushes.Red;
                window.BpDividerLine.Stroke = Brushes.Red;
                window.Respiration.Foreground = Brushes.Yellow;
            }));
        }

        public override void OnSecondaryVitals(Caretaker.Device device, Caretaker.SecondaryVitals[] vitalsArray)
        {
            // Do nothing if the application exited
            if (Application.Current == null)
            {
                return;
            }

            /* Note Application.Current.Dispatcher.BeginInvoke() executes asynchronous processing on the main thread
             * to update the UI, if needed, call the synchronous Application.Current.Dispatcher.Invoke() instead
             * to throttle library calls since OnSecondaryVitals() maybe called very regularly (once or multiple times
             * per second) to report vitals to the application depending on the Caretaker's update interval as well
             * as queuing in the library. If Dispatcher.BeginInvoke() is called instead, the application must be
             * capable of handling  high library call volumes to prevent application freeze.
             * 
             * Also, you should not perform heavy processing or perform blocking operations, such as writing to
             * file, in the Action passed as argument to Dispatcher.Invoke() to prevent application freeze.
             */
            Caretaker.SecondaryVitals vitals = vitalsArray[vitalsArray.Length - 1];
            Console.WriteLine("*** SECONDARY VITALS ***");
            Console.WriteLine("Blood Volume: {0}", vitals.bloodVolume);
            Console.WriteLine("Cardiac Output: {0}", vitals.cardiacOutput);
            Console.WriteLine("HR Comp: {0}", vitals.hrComp);
            Console.WriteLine("IBI: {0}", vitals.interbeatInterval);
            Console.WriteLine("LVET: {0}", vitals.lvet);
            Console.WriteLine("P2P1 Ratio: {0}", vitals.p2p1Ratio);
            Console.WriteLine("PR: {0}", vitals.pr);
        }

        public override void OnReadDisplayState(Caretaker.Device device, Boolean status, Boolean state)
        {
            // Do nothing if the application exited
            if (Application.Current == null)
            {
                return;
            }

            if (status)
            {
                Console.WriteLine("** DISPLAY STATE *** ");
                Console.WriteLine("Display: {0}", (state)?"ON":"OFF");
            }
        }

        public override void OnReadSnrMinimum(Caretaker.Device device, Boolean status, Int32 snr)
        {
            // Do nothing if the application exited
            if (Application.Current == null)
            {
                return;
            }

            if (status)
            {
                Console.WriteLine("**SNR MINIMUM *** ");
                Console.WriteLine("SNR: {0}", snr);
            }
        }

        public override void OnReadRecalibrationInterval(Caretaker.Device device, Boolean status, UInt32 interval)
        {
            // Do nothing if the application exited
            if (Application.Current == null)
            {
                return;
            }

            if (status)
            {
                Console.WriteLine("** RECALIBRATION INTERVAL *** ");
                Console.WriteLine("Interval: {0}", interval);
            }
        }

        public override void OnReadVitalsFilterSetting(Caretaker.Device device, Boolean status, Byte value)
        {
            // Do nothing if the application exited
            if (Application.Current == null)
            {
                return;
            }

            if (status)
            {
                Console.WriteLine("** VITALS FILTER *** ");
                Console.WriteLine("Value: {0}", value);
            }
        }

        public override void OnReadMotionTolerance(Caretaker.Device device, Boolean status, int timeout)
        {
            // Do nothing if the application exited
            if (Application.Current == null)
            {
                return;
            }

            if (status)
            {
                Console.WriteLine("** VITALS FILTER *** ");
                Console.WriteLine("Timeout: {0}", timeout);
            }
        }

        public override void OnReadPersistentLog(Caretaker.Device device, bool status, string log)
        {
            // Do nothing if the application exited
            if (Application.Current == null)
            {
                return;
            }

            if (status)
            {
                Console.WriteLine("** DEVICE LOGS ***\n" + log);
            }
        }

        public override void OnReadWaveformClampSetting(Caretaker.Device device, Boolean status, Byte value)
        {
            // Do nothing if the application exited
            if (Application.Current == null)
            {
                return;
            }

            if (status)
            {
                Console.WriteLine("** WAVEFORM CLAMPING *** ");
                Console.WriteLine("Value: {0}", value);
            }
        }
    }

    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        // Caretaker device proxy
        CaretakerObserver mCaretakerObserver;
        Caretaker.Device mCaretaker;
        String mSerialNumber = "";

        // Caretaker data
        volatile Waveform mPulsePressureWaveform;
        volatile Waveform mPulseRateWaveform;

        // Timers
        System.Threading.Timer mUpdateTimer;
        System.Threading.Timer mTimeoutTimer;

        /* status indicators */
        public SolidColorBrush mDefaultStatusBackground = Brushes.DarkGray;
        public SolidColorBrush mDefaultStatusForeground = Brushes.Black;
        public SolidColorBrush mConnectedStatusBackground;
        public SolidColorBrush mConnectedStatusForeground;
        public SolidColorBrush mAlertStatusBackground;
        public SolidColorBrush mAlertStatusForeground;
        public SolidColorBrush mErrorStatusBackground = Brushes.Crimson;
        public SolidColorBrush mErrorStatusForeground = Brushes.White;

        // Calibration menu indexes
        public const int MANUAL_CAL = 0;
        public const int AUTO_CAL = 1;

        // Posture menu indexes
        public const int POSTURE_SITTING = 0;
        public const int POSTURE_STANDING = 1;

        // True if starting a calibration and false otherwise
        public bool Starting = false;

        // True if connecting and false otherwise
        public bool Connecting = false;

        // True if disconnecting and false otherwise
        public bool Disconnecting = false;

        // True if aborting connecting and false otherwise
        public bool Aborting = false;

        public Label Status
        {
            get
            {
                return mStatus;
            }
        }

        public TextBox SerialNumberTextBox
        {
            get
            {
                return mSerialNumberTextBox;
            }
        }

        public Button ConnectButton
        {
            get
            {
                return mConnectButton;
            }
        }

        public Button[] OperationButons
        {
            get
            {
                return new Button[] {
                    mStartButton,
                    mStopButton,
                    mRealibrateButton,
                    mDeviceInfoButton,
                    mDecrementTargetPressure,
                    mIncrementTargetPressure
                };
            }
        }

        public Waveform PulsePressureWaveform
        {
            get
            {
                return mPulsePressureWaveform;
            }
        }

        public Waveform PulseRateWaveform
        {
            get
            {
                return mPulseRateWaveform;
            }
        }

        /**************************************************************************************************************
         */
        public MainWindow()
        {
            InitializeComponent();

            try
            {
                // Redirect library logging
                Caretaker.Device.SetLibraryLogLevel(0);
                Caretaker.Device.RedirectLibraryLogs();

                // Create device instance passing Observer to receive notifications
                mCaretakerObserver = new CaretakerObserver(this);
                mCaretaker = new Caretaker.Device(mCaretakerObserver, true);

            } catch(Exception e)
            {
                Console.WriteLine("Failed to create Caretaker.Device instance!\nException: {0}", e.ToString());
                return;
            }

            // use white text on blue background for connected status
            mConnectedStatusBackground = new SolidColorBrush();
            mConnectedStatusBackground.Color = Color.FromRgb(0, 112, 192);
            mConnectedStatusForeground = new SolidColorBrush();
            mConnectedStatusForeground.Color = Color.FromRgb(255, 255, 255);

            // use white on orange background for alert status
            mAlertStatusBackground = new SolidColorBrush();
            mAlertStatusBackground.Color = Color.FromRgb(212, 97, 18);
            mAlertStatusForeground = mConnectedStatusForeground;

            // set default status
            mStatus.Content = "Enter Caretaker serial number then press connect to begin.";
            mStatus.Background = mDefaultStatusBackground;
            mStatus.Foreground = mDefaultStatusForeground;

            mPulsePressureWaveform = new Waveform(mPulsePressureCanvas);
            PulsePressureWaveform.PlotStroke = Brushes.Red;

            mPulseRateWaveform = new Waveform(mPulseRateCanvas);
            PulseRateWaveform.PlotStroke = Brushes.Green;

            // disable operational buttons initially
            foreach(Button b in this.OperationButons)
            {
                b.IsEnabled = false;
            }
            ConnectButton.IsEnabled = false;

            // default settings
            CalibrationModeComboBox.SelectedIndex = MANUAL_CAL;
            PostureComboBox.SelectedIndex = POSTURE_SITTING;
        }

        /**************************************************************************************************************
         */
        void StartTimeout(System.Threading.TimerCallback callback, int timeout)
        {
            try
            {
                if (mTimeoutTimer != null) {
                    mTimeoutTimer.Dispose();
                }
            }
            catch (Exception exp)
            {
                Console.WriteLine("Timer dispose exception! {0}", exp.ToString());
            }

            mTimeoutTimer = new System.Threading.Timer(callback, null, timeout, 0);
        }

        /**************************************************************************************************************
        */
        private void OnUpdateWaveforms(Object stateInfo)
        {
            if (Application.Current == null)
            {
                return;
            }

            Application.Current.Dispatcher.BeginInvoke(new Action(() =>
            {
                PulseRateWaveform.Update();
                PulsePressureWaveform.Update();
            }));
        }

        /**************************************************************************************************************
         */
        private void OnEnableConnectButton(Object stateInfo)
        {
            if (Application.Current == null)
            {
                return;
            }

            Application.Current.Dispatcher.BeginInvoke(new Action(() =>
            {
               ConnectButton.IsEnabled = true;
            }));
        }

        /**************************************************************************************************************
        */
        protected override void OnClosing(CancelEventArgs e)
        {
            try
            {
                if (mTimeoutTimer != null)
                {
                    mTimeoutTimer.Dispose();
                }

                if (mUpdateTimer != null)
                {
                    mUpdateTimer.Dispose();
                }
            }
            catch (Exception exp)
            {
                Console.WriteLine("Timer dispose exception! {0}", exp.ToString());
            }

            // IMPORTANT: Destroy resources allocated for the device instance in Create()
            mStatus.Content = "Closing...";
            mCaretaker.ReleaseResources();
            mCaretakerObserver = null;
            mCaretaker = null;

            base.OnClosing(e);
        }

        /**************************************************************************************************************
         */
        private void Start_Click(object sender, RoutedEventArgs e)
        {
            PulsePressureWaveform.Clear();
            mPulseRateWaveform.Clear();

            Status.Background = mConnectedStatusBackground;
            Status.Foreground = mConnectedStatusForeground;

            /* start manual or auto calibration according to UI settings */
            if (CalibrationModeComboBox.SelectedIndex == MANUAL_CAL)
            {
                int systolic = 0, diastolic = 0;
                if (
                    Int32.TryParse(SystolicSetting.Text, out systolic) &&
                    Int32.TryParse(DiastolicSetting.Text, out diastolic)
                    )
                {
                    mStatus.Content = "Starting manual calibration...";
                    Starting = mCaretaker.StartManualCal(systolic, diastolic);
                }
            }
            else
            {
                Caretaker.PatientPosture posture;
                switch (PostureComboBox.SelectedIndex)
                {
                    case POSTURE_SITTING:
                        posture = Caretaker.PatientPosture.SITTING;
                        break;

                    default:
                        posture = Caretaker.PatientPosture.SUPINE;
                        break;
                }
                mStatus.Content = "Starting auto calibration...";
                Starting = mCaretaker.StartAutoCal(posture);
            }

            // Update the waveforms 10 times per second
            if (Starting)
            {
                mUpdateTimer = new System.Threading.Timer(OnUpdateWaveforms, null, 1000, 100);
            }
        }

        /**************************************************************************************************************
         */
        private void Stop_Click(object sender, RoutedEventArgs e)
        {
            mStatus.Content = "Stopping...";
            mCaretaker.Stop();

            try
            {
                mUpdateTimer.Dispose();
            }
            catch (Exception exp)
            {
                Console.WriteLine("Timer dispose exception! {0}", exp.ToString());
            }
        }

        /**************************************************************************************************************
        */
        private void Connect_Click(object sender, RoutedEventArgs e)
        {
            
            if (ConnectButton.Content.Equals("Connect"))
            {
                Connecting = true;
                mStatus.Content = "Connecting to " + mSerialNumber + "... Press button on device to connect.";
                mStatus.Background = mDefaultStatusBackground;
                mStatus.Foreground = mDefaultStatusForeground;

                if ( mCaretaker.ConnectToSerialNumber(mSerialNumber, 60000))
                {
                    ConnectButton.IsEnabled = false;
                    ConnectButton.Content = "Abort";
                    SerialNumberTextBox.IsEnabled = false;
                    // re-enable button 1 second later to filter multiple clicks
                    StartTimeout(OnEnableConnectButton, 1000);
                }
                else
                {
                    Connecting = false;
                    mStatus.Content = "Connect error.";
                    Status.Background = mErrorStatusBackground;
                    Status.Foreground = mErrorStatusForeground;
                }
            }
            else if (ConnectButton.Content.Equals("Abort"))
            {
                Aborting = true;
                mStatus.Content = "Aborting...";
                SerialNumberTextBox.IsEnabled = true;
                ConnectButton.IsEnabled = false;
                // re-enable button 1 second later to filter multiple clicks
                StartTimeout(OnEnableConnectButton, 1000);
                mCaretaker.Disconnect();
            }
            else
            {
                Disconnecting = true;
                ConnectButton.IsEnabled = false;
                mStatus.Content = "Disconnecting...";
                // re-enable button 2 seconds later to filter multiple clicks
                StartTimeout(OnEnableConnectButton, 2000);
                mCaretaker.Disconnect();
            }
        }

        /**************************************************************************************************************
         */
        private void Recalibrate_Click(object sender, RoutedEventArgs e)
        {
            mCaretaker.Recalibrate();
        }

        /**************************************************************************************************************
        */
        private void DecrementTargetPressure_Click(object sender, RoutedEventArgs e)
        {
            int p = 0;
            if (Int32.TryParse(TargetPressure.Content.ToString(), out p))
            {
                if (p>0)
                {
                    mCaretaker.DecrementCuffPressure();
                }
            }
        }

        /**************************************************************************************************************
         */
        private void IncrementTargetPressure_Click(object sender, RoutedEventArgs e)
        {
            int p = 0;
            if (Int32.TryParse(TargetPressure.Content.ToString(), out p))
            {
                if (p < 100)
                {
                    p += 10;
                    mCaretaker.IncrementCuffPressure();
                    TargetPressure.Content = "" + p;
                }
            }
        }

        /**************************************************************************************************************
        */
        private void SerialNumber_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (mSerialNumberTextBox.Text.Equals(""))
            {
                if (ConnectButton != null)
                {
                    ConnectButton.IsEnabled = false;
                }
                return;
            }

            mSerialNumber = mSerialNumberTextBox.Text;
            bool serilaNumberValid = !mSerialNumber.Equals("Enter serial number");
            if (serilaNumberValid && ConnectButton != null)
            {
                ConnectButton.IsEnabled = true;
            }
        
        }

        /**************************************************************************************************************
        */
        private void SerialNumber_TextInput(object sender, TextCompositionEventArgs e)
        {
        }

        /**************************************************************************************************************
        */
        private void SerialNumber_MouseEnter(object sender, MouseEventArgs e)
        {
            
            mSerialNumberTextBox.Foreground = Brushes.Black;
            if (mSerialNumber.Equals("Enter serial number"))
            {
                mSerialNumberTextBox.Clear();
            }

        }

        /**************************************************************************************************************
         */
        private void SerialNumber_MouseLeave(object sender, MouseEventArgs e)
        {
           //mSerialNumberTextBox.Foreground = Brushes.LightGray;
        }

        private void SerialNumberTextBox_MouseDown(object sender, MouseButtonEventArgs e)
        {
        }

        private void CalibrationMode_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            switch (CalibrationModeComboBox.SelectedIndex)
            {
                // manual cal selected
                case MANUAL_CAL:
                    {
                        SystolicSettingGird.Visibility = Visibility.Visible;
                        DiastolicSettingGrid.Visibility = Visibility.Visible;
                        PostureSettingsGrid.Visibility = Visibility.Hidden;
                    }
                    break;

                // auto cal selected
                default:
                    {
                        SystolicSettingGird.Visibility = Visibility.Hidden;
                        DiastolicSettingGrid.Visibility = Visibility.Hidden;
                        PostureSettingsGrid.Visibility = Visibility.Visible;
                    }
                    break;
            }
        }

        private void DeviceInfoButton_Click(object sender, RoutedEventArgs e)
        {
            if(mCaretaker.IsConnected())
            {
                System.Text.StringBuilder messageBoxCS = new System.Text.StringBuilder();
                messageBoxCS.AppendFormat("Device Name:        {0}\n", mCaretaker.GetName());
                messageBoxCS.AppendLine();
                messageBoxCS.AppendFormat("Serial Number:      {0}\n", mCaretaker.GetSerialNumber());
                messageBoxCS.AppendLine();
                messageBoxCS.AppendFormat("MAC Address:        {0}\n", mCaretaker.GetMacAddress());
                messageBoxCS.AppendLine();
                messageBoxCS.AppendFormat("Hardware Version:   {0}\n", mCaretaker.GetHardwareVersion());
                messageBoxCS.AppendLine();
                messageBoxCS.AppendFormat("Firmware Version:   {0}\n", mCaretaker.GetFirmwareVersion());
                messageBoxCS.AppendLine();
                messageBoxCS.AppendFormat("Library Version:    {0}\n", mCaretaker.GetLibraryVersion());
                messageBoxCS.AppendLine();
                messageBoxCS.AppendFormat("Library Build Date: {0}\n", mCaretaker.GetLibraryBuildDate());
                messageBoxCS.AppendLine();

                MessageBox.Show(messageBoxCS.ToString(), "Device Info");
            }
        }
    }
}
