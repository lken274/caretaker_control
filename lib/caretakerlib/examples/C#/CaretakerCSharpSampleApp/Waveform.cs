using System;
//using System.Drawing;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Shapes;
using System.Windows.Forms.DataVisualization.Charting;
using System.Collections.Generic;

namespace CaretakerCSharpSampleApp
{
    /// <summary>
    /// Displays Caretaker waveform data
    /// </summary>
    public class Waveform
    {
        // Device data rate in Hz.
        // Default settings for Caretker5
         double mInputRate = 500;   // 500 Hz data

        // Scale to down-sample input rate
        int mInputRateDivider = 10;

        // Rate at which the waveform staging buffer fills
        double mBufferedRate;

        // The displayed waveform refresh rate (Hz)
        double mRefreshRate = 10;

        // The waveform window length
        const uint mWindowDuration = 6; // 6 seconds

        // The desired latency of points displayed on the waveform.
        // The value set here is not guaranteed.
        const double mDisplayLatency = 2; // 2 seconds

        // Staging buffer to hold data points to display
        List<Int32> mDataBuffer = new List<Int32>();

        // Display window
        Point[] mWindow;
        int mWindowSize;
        int mWindowIndex;
        Boolean mFirstWindow;

        // Canvas to draw waveform
        Canvas mCanvas;
        System.Threading.Mutex mMutex = new System.Threading.Mutex();

        // Plot grid where the waveform is drawn
        double mPlotWidth;
        double mPlotHeight;
        Margins mPlotMargins;
        Brush mPlotStroke;
        double mPlotStrokeThickness;
        System.Windows.Shapes.Rectangle mCursor = new System.Windows.Shapes.Rectangle();

        // Range and text labels dimensions
        int mScaleMarkerHeight = 0; // set to font size
        int mScaleMarkerWidth = 0; // set to approx. 6 units per character

        // Range of waveform values to display on plot
        int[] mRange = null;

        // Scales to show on the waveform. Each scale must be within Range.
        int[] mScales = null;

        /// <summary>
        /// Upper and lower range of waveform values.
        /// </summary>
        public int[] Range
        {
            set
            {
                mRange = value;
                Reset();
            }
            get
            {
                return mRange;
            }
        }

        /// <summary>
        /// Scales to show on the waveform. The scales must be subset of Range.
        /// </summary>
        public int[] Scales
        {
            set
            {
                mScales = value;
            }
            get
            {
                return mScales;
            }
        }

        /// <summary>
        /// Displayed waveform refresh rate Hertz (Hz)
        /// </summary>
        public double RefreshRate
        {
            get
            {
                return mRefreshRate;
            }
            set
            {
                mRefreshRate = value;
                Reset();
            }
        }

        /// <summary>
        /// Set/Get device data rate Hertz (Hz)
        /// For Caretaker4, data rate = 31.25Hz
        /// For Caretaker5, data rate = 500 Hz
        /// </summary>
        public double InputRate
        {
            get
            {
                return mInputRate;
            }
            set
            {
                mInputRate = value;
                Reset();
            }
        }

        /// <summary>
        /// Set/Get device input data rate divider
        /// Use it to scale 500Hz input to reducing buffer
        /// </summary>
        public int InputRateDivder
        {
            get
            {
                return mInputRateDivider;
            }
            set
            {
                mInputRateDivider = value;
                Reset();
            }
        }

        /// <summary>
        /// The plot stroke
        /// </summary>
        public Brush PlotStroke
        {
            set
            {
                mPlotStroke = value;
            }
            get
            {
                return mPlotStroke;
            }
        }

        /** 
         * <summary>
         * The plot stroke thickness
         * </summary>
         */
        public double PlotStrokeThickness
        {
            set
            {
                mPlotStrokeThickness = value;
            }
            get
            {
                return mPlotStrokeThickness;
            }
        }

        /** 
         * <summary>
         * Construct a new waveform on the given canvas
         * </summary>
         */
        public Waveform(Canvas canvas)
        {
            mCanvas = canvas;
            PlotStroke = Brushes.Green;
            PlotStrokeThickness = 2;

            Reset();
            ShowScales();
        }

        //Reset the number of points displayed on the waveform.
        private void Reset()
        {
            LockMutex();

            mBufferedRate = mInputRate / mInputRateDivider;
            mWindowSize = (int)(mWindowDuration * mBufferedRate);
            mWindow = new Point[mWindowSize];

            // calculate range and scale marker label dimensions
            if (mRange != null && mRange.GetLength(0) > 1)
            {
                int max = Math.Max(mRange[0], mRange[1]);
                String str = "" + max;
                mScaleMarkerWidth = 6 * str.Length;
                mScaleMarkerHeight = 12;
            }

            // Set plot margins to account for range and scale markers
            mPlotMargins = new Margins(mScaleMarkerHeight, 0, mScaleMarkerWidth, 0);
            mPlotWidth = mCanvas.Width - mPlotMargins.Left - mPlotMargins.Right ;
            mPlotHeight = mCanvas.Height - mPlotMargins.Top - mPlotMargins.Bottom;

            mCursor.Stroke = new SolidColorBrush(Color.FromRgb(15, 15, 15));
            mCursor.Fill = mCursor.Stroke;
            mCursor.HorizontalAlignment = HorizontalAlignment.Left;
            mCursor.VerticalAlignment = VerticalAlignment.Center;
            mCursor.Height = mPlotHeight;

            Clear();

            UnLockMutex();
        }

        /**
         * <summary>
         * Method to call from UI thread to clear the waveform. 
         * </summary>
         */
        public void Clear()
        {
            LockMutex();

            mDataBuffer.Clear();
            mCanvas.Children.Clear();

            mFirstWindow = true;
            mWindowIndex = 0;
  
            for (int i = 0; i < mWindowSize; i++)
            {
                mWindow[i].X = i;
                mWindow[i].Y = 0;
            }

            ShowScales();

            UnLockMutex();
        }

        /** 
         * <summary>
         * Method to call from library thread to add new points.
         * </summary>
         */
        public void AddPoints(Int32[] values, Int64[] timestamps)
        {
            LockMutex();

            // down-sample data if needed
            int stride = mInputRateDivider;
            int count = values.GetLength(0);
            for (int i = 0; i < count; i+=stride)
            {
                mDataBuffer.Add(values[i]);
            }

            UnLockMutex();
            
        }

        private void ShowScales()
        {
            if (mRange != null && mRange.GetLength(0) > 1)
            {
                int lower = Math.Min(mRange[0], mRange[1]);
                int upper = Math.Max(mRange[0], mRange[1]);
                int range = upper - lower;
                if (range != 0)
                {
                    Label upperLabel = new Label();
                    upperLabel.Content = "" + upper;
                    upperLabel.Background = mCanvas.Background;
                    upperLabel.Foreground = Brushes.Gray;
                    upperLabel.FontSize = mScaleMarkerHeight;
                    upperLabel.Margin = new Thickness(0, 0, 0, 0);
                    mCanvas.Children.Add(upperLabel);

                    Label lowerLabel = new Label();
                    lowerLabel.Content = "" + lower;
                    lowerLabel.Background = mCanvas.Background;
                    lowerLabel.Foreground = Brushes.Gray;
                    lowerLabel.FontSize = mScaleMarkerHeight;
                    lowerLabel.Margin = new Thickness(0, mPlotHeight, 0, 0);
                    mCanvas.Children.Add(lowerLabel);

                    if (mScales != null)
                    {
                        foreach (int scale in mScales)
                        {
                            if (scale > lower && scale < upper)
                            {
                                double t = mPlotHeight - ((double)scale / range) * mPlotHeight;
                                Label label = new Label();
                                label.Content = "" + scale;
                                label.Background = mCanvas.Background;
                                label.Foreground = Brushes.Gray;
                                label.FontSize = mScaleMarkerHeight;
                                label.Margin = new Thickness(0, t, 0, 0);
                                mCanvas.Children.Add(label);
                            }
                        }
                    }
                }
            }


            Line upperLine = new Line();
            upperLine.Stroke = Brushes.Gray;
            upperLine.StrokeThickness = 2;
            upperLine.X1 = mPlotMargins.Left;
            upperLine.Y1 = mPlotMargins.Top;
            upperLine.X2 = mPlotWidth;
            upperLine.Y2 = mPlotMargins.Top;
            //mCanvas.Children.Add(upperLine);

            Line lowerLine = new Line();
            lowerLine.Stroke = Brushes.Gray;
            lowerLine.StrokeThickness = 2;
            lowerLine.X1 = mPlotMargins.Left;
            lowerLine.Y1 = mPlotMargins.Top + mPlotHeight;
            lowerLine.X2 = mPlotWidth;
            lowerLine.Y2 = mPlotMargins.Top + mPlotHeight;
            //mCanvas.Children.Add(lowerLine);
        }

        /** 
         * <summary>
         * Method to call from UI thread after adding points to update the plot.
         * </summary>
         */
        public void Update()
        {
            LockMutex();

            // Calculate the number of points to remove
            int desiredMaxThreshold = (int) (mDisplayLatency * mBufferedRate + 0.5);
            int desiredMinThreshold = (int) (mDisplayLatency * 0.75 * mBufferedRate + 0.5);
            int removeCount = (int)(mBufferedRate / mRefreshRate + 0.5);
            if (removeCount < 1)
                removeCount = 1;

            // Adjust remove count up/down as needed to control display latency.
            if (mDataBuffer.Count > desiredMaxThreshold)
            {
                removeCount++;
                Console.WriteLine("Max buffer threshold exceeded: Count={0}, Desired={1}, RemoveCount={2}",
                    mDataBuffer.Count, desiredMinThreshold, removeCount);
            }
            else if (mDataBuffer.Count < desiredMinThreshold)
            {
                removeCount--;
            }

            // Add new points to Window
            if (mDataBuffer.Count > removeCount)
            {
                for(int i=0; i<removeCount; i++)
                {
                    mWindow[mWindowIndex].Y = mDataBuffer[0];
                    mDataBuffer.RemoveAt(0);
                    if ( ++mWindowIndex >= mWindowSize )
                    {
                        mWindowIndex = 0;
                        mFirstWindow = false;
                    }
                }
            }

            UnLockMutex();

            //Console.WriteLine("Displayed {0} points, BufferCount={1}", removeCount, mDataBuffer.Count);

            // Calculate pixels per unit resolution to map data to screen space
            double yMin, yMax;
            yMin = yMax =mWindow[0].Y;
            for (int i = 0; i < mWindowSize; i++) {
                yMin = Math.Min(mWindow[i].Y, yMin);
                yMax = Math.Max(mWindow[i].Y, yMax);
            }

            double yRes = 1.0;
            double yRange = yMax - yMin;
            if (yRange > 0)
                yRes = mPlotHeight / yRange;
            else
                yRes = 1;

            double xRes = mPlotWidth / mWindowSize; 

            // Convert values to pixels in screen coordinates.
            int xOffset = mPlotMargins.Left;
            int yOffset = mPlotMargins.Top;
            Point[] window = new Point[mWindowSize];
            for (int i = 0; i < mWindowSize; i++)
            {
                window[i].X = xOffset + i*xRes;
                window[i].Y = yOffset + mPlotHeight - (mWindow[i].Y - yMin) * yRes;
            }

            // Draw waveform
            mCanvas.Children.Clear();
            ShowScales();
            int count = (mFirstWindow) ? mWindowIndex : mWindowSize;
            for (int i = 1; i < count; i++)
            {
                Line line = new Line();
                line.Stroke = PlotStroke;
                line.StrokeThickness = PlotStrokeThickness;
                line.X1 = window[i].X;
                line.Y1 = window[i].Y;
                line.X2 = window[i - 1].X;
                line.Y2 = window[i - 1].Y;
                mCanvas.Children.Add(line);

            }

            mCursor.Width = 10 * xRes;
            mCanvas.Children.Add(mCursor);
            Canvas.SetLeft(mCursor, xOffset + mWindowIndex * xRes);
        }

        private void LockMutex()
        {
            mMutex.WaitOne();
        }

        private void UnLockMutex()
        {
            mMutex.ReleaseMutex();
        }
    }
}
