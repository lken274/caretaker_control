using System;
//using System.Drawing;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Shapes;
using System.Windows.Forms.DataVisualization.Charting;

namespace CaretakerCSharpSampleApp
{
    /// <summary>
    /// Defines how new data is added to the waveform
    /// </summary>
    public enum WaveformDirection
    {
        /// <summary>
        /// New data is added to the right so the waveform updates from right to left
        /// </summary>
        RIGHT_SHFIT,
        /// <summary>
        /// New data is added to the left so the waveform updates from left to right
        /// </summary>
        LEFT_SHFIT,
    }

    /// <summary>
    /// Displays Caretaker waveform data
    /// </summary>
    public class Waveform
    {
        // The waveform data rate in Hz. Approximately 31 Hz.
        uint mDataPointsRate = 31;

        // The waveform length in seconds
        uint mDuration = 6;

        // The maximum number of points to display on the waveform
        // MaxPoints = Duration * DataRate
        int mMaxDisplayedPoints;

        // Waveform backing buffer to hold Caretaker data to display
        Point[] mDataPoints;
        Point mDataPointsMax = new Point();
        Point mDataPointsMin = new Point();
        Point mDataPointsRange = new Point();
        int mDataPointsCount = 0;
        WaveformDirection mDirection = WaveformDirection.LEFT_SHFIT;

        // Data points converted to xy canvas coordinates
        Point[] mCoordinates;

        // Canvas to draw waveform
        Canvas mCanvas;

        // Data mutex
        System.Threading.Mutex mMutex = new System.Threading.Mutex();

        // Plot grid where the waveform is drawn
        double mPlotWidth;
        double mPlotHeight;
        Margins mPlotMargins;
        Brush mPlotStroke;
        double mPlotStrokeThickness;

        // Range and text labels dimensions
        int mScaleMarkerHeight = 0; // set to font size
        int mScaleMarkerWidth = 0; // set to approx. 6 units per character

        // Range of waveform values to display on plot
        int[] mRange = null;

        // Scales to to show on the waveform. Each scale must be within Range.
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
        /// Waveform window length in seconds
        /// </summary>
        public uint Duration
        {
            set
            {
                mDuration = value;
                Reset();
            }
            get
            {
                return mDuration;
            }
        }

        /// <summary>
        /// Waveform data rate Hertz (Hz)
        /// </summary>
        public uint DataRate
        {
            get
            {
                return mDataPointsRate;
            }
            set
            {
                mDataPointsRate = value;
                Reset();
            }
        }

        /// <summary>
        /// Waveform direction
        /// </summary>
        public WaveformDirection Direction
        {
            set
            {
                mDirection = value;
                Reset();
            }
            get
            {
                return mDirection;
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

            // calculate range and scale marker label dimensions
            if (mRange != null && mRange.GetLength(0) > 1)
            {
                int max = Math.Max(mRange[0], mRange[1]);
                String str = "" + max;
                mScaleMarkerWidth = 6 * str.Length;
                mScaleMarkerHeight = 12;
            }

            // set plot margins to account for range and scale markers
            //int top = mScaleMarkerHeight;
            //int bottom = 
            mPlotMargins = new Margins(mScaleMarkerHeight, 0, mScaleMarkerWidth, 0);
            mPlotWidth = mCanvas.Width - mPlotMargins.Left - mPlotMargins.Right ;
            mPlotHeight = mCanvas.Height - mPlotMargins.Top - mPlotMargins.Bottom;

            mMaxDisplayedPoints = (int)mDuration * (int)mDataPointsRate;
            mDataPointsCount = 0;

            mDataPoints = new Point[mMaxDisplayedPoints];
            mCoordinates = new Point[mMaxDisplayedPoints];
           
            for (int i = 0; i < mMaxDisplayedPoints; i++)
            {
               mDataPoints[i].X = 0;
                mDataPoints[i].Y = 0;
                mCoordinates[i].X = 0;
                mCoordinates[i].Y = 0;
            }

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

            mCanvas.Children.Clear();
            mDataPointsCount = 0;
            for (int i = 0; i < mMaxDisplayedPoints; i++)
            {
                mDataPoints[i].X = 0;
                mDataPoints[i].Y = 0;
                mCoordinates[i].X = 0;
                mCoordinates[i].Y = 0;
            }

            ShowScales();

            UnLockMutex();
        }

        /** 
         * <summary>
         * Method to call from library thread to add new points.
         * </summary>
         */
        public void AddPoints(Int32[] values, Int32[] timestamps)
        {
            LockMutex();

            int maxDataPoints = mDataPoints.GetLength(0);
            int count = Math.Min(values.GetLength(0), timestamps.GetLength(0));
            int newDataStart = 0;
            int newDataEnd = count;

            // truncate new points to fit within the grid as needed.
            // this effectively throws away the oldest points.
            if (count > maxDataPoints)
            {
                newDataStart = count- maxDataPoints;
                count = maxDataPoints;
            }

            // shift then add new points
            for (int i = count, j = 0; i < maxDataPoints; i++, j++)
            {
                mDataPoints[j] = mDataPoints[i];
            }

            for (int i = (maxDataPoints - count), j = newDataStart; i < maxDataPoints; i++, j++)
            {
                mDataPoints[i].X = timestamps[j];
                mDataPoints[i].Y = values[j];
            }

            mDataPointsCount += count;
            if (mDataPointsCount > maxDataPoints)
            {
                mDataPointsCount = maxDataPoints;
            }

            // calculate pixels per unit (ppu) resolution to map data to screen space
            double xRes, yRes = xRes = 1.0f;
            if (mDirection == WaveformDirection.RIGHT_SHFIT)
            {
                mDataPointsMin.Y = mDataPointsMax.Y = mDataPoints[maxDataPoints - 1].Y;
                mDataPointsMin.X = mDataPointsMax.X = mDataPoints[maxDataPoints - 1].X;
                int end = maxDataPoints - mDataPointsCount;
                for (int i = maxDataPoints - 1; i >= end; i--)
                {
                    mDataPointsMin.X = Math.Min(mDataPoints[i].X, mDataPointsMin.X);
                    mDataPointsMin.Y = Math.Min(mDataPoints[i].Y, mDataPointsMin.Y);
                    mDataPointsMax.X = Math.Max(mDataPoints[i].X, mDataPointsMax.X);
                    mDataPointsMax.Y = Math.Max(mDataPoints[i].Y, mDataPointsMax.Y);
                }
            } else
            {
                mDataPointsMin.Y = mDataPointsMax.Y = mDataPoints[0].Y;
                mDataPointsMin.X = mDataPointsMax.X = mDataPoints[0].X;
                for (int i = 0; i < mDataPointsCount; i++)
                {
                    mDataPointsMin.X = Math.Min(mDataPoints[i].X, mDataPointsMin.X);
                    mDataPointsMin.Y = Math.Min(mDataPoints[i].Y, mDataPointsMin.Y);
                    mDataPointsMax.X = Math.Max(mDataPoints[i].X, mDataPointsMax.X);
                    mDataPointsMax.Y = Math.Max(mDataPoints[i].Y, mDataPointsMax.Y);
                }
            }

            mDataPointsRange.X = mDataPointsMax.X - mDataPointsMin.X;
            if (mDataPointsRange.X > 0)
            {
                xRes = mPlotWidth / mDataPointsRange.X; 
            }

            mDataPointsRange.Y = mDataPointsMax.Y - mDataPointsMin.Y;
            if (mDataPointsRange.Y > 0)
            {
                yRes = mPlotHeight / mDataPointsRange.Y;
            }

            // convert values to pixels in screen coordinates.
            int maxCoordinates = mCoordinates.GetLength(0);
            int copyCount = Math.Min(maxCoordinates, mDataPointsCount);
            int xOffset = mPlotMargins.Left;
            int yOffset = mPlotMargins.Top;
            for (int i = 0; i < copyCount; i++)
            {
                mCoordinates[i].X = xOffset + (mDataPoints[i].X - mDataPointsMin.X) * xRes;
                mCoordinates[i].Y = yOffset + mPlotHeight - (mDataPoints[i].Y - mDataPointsMin.Y) * yRes;
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
            mCanvas.Children.Add(upperLine);

            Line lowerLine = new Line();
            lowerLine.Stroke = Brushes.Gray;
            lowerLine.StrokeThickness = 2;
            lowerLine.X1 = mPlotMargins.Left;
            lowerLine.Y1 = mPlotMargins.Top + mPlotHeight;
            lowerLine.X2 = mPlotWidth;
            lowerLine.Y2 = mPlotMargins.Top + mPlotHeight;
            mCanvas.Children.Add(lowerLine);
        }

        /** 
         * <summary>
         * Method to call from UI thread after adding points to update the plot.
         * </summary>
         */
        public void Update()
        {
            LockMutex();

            mCanvas.Children.Clear();

            ShowScales();

            // draw waveform
            int maxCoordinates = mCoordinates.GetLength(0);
            int count = Math.Min(maxCoordinates, mDataPointsCount);
            int stop = maxCoordinates - count;
            for (int i = count - 1; i > stop; i--)
            {
                Line line = new Line();
                line.Stroke = PlotStroke;
                line.StrokeThickness = PlotStrokeThickness;
                line.X1 = mCoordinates[i].X;
                line.Y1 = mCoordinates[i].Y;
                line.X2 = mCoordinates[i - 1].X;
                line.Y2 = mCoordinates[i - 1].Y;
                mCanvas.Children.Add(line);
            }

            UnLockMutex();
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
