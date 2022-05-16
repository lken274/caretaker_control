#include "waveformviewer.h"
#include "pulsewaveform.h"
#include "qpainter.h"
#include "qmutex.h"
#include <string.h>
#include <cfloat>
#include <stdio.h>

waveformViewer::waveformViewer(const QRect& rect,
                               int marginLeft,
                               int marginTop,
                               int marginRight,
                               int marginBottom,
                               float xres)
{
    mRect = rect;
    mMarginLeft= marginLeft;
    mMarginTop = marginTop;
    mMarginRight = marginRight;
    mMarginBottom = marginBottom;
    mNumPoints = 0;
    mMaxPoints = mRect.width() * xres;
    mPoints = new Point[mMaxPoints];
    mPixelMap = new QPixmap(mRect.width(), mRect.height());
}

waveformViewer::~waveformViewer()
{
    delete [] mPoints;
    delete mPixelMap;
}

void waveformViewer::clear(void)
{
    mNumPoints = 0;
    for (int i = 0; i < mMaxPoints; i++) {
        mPoints[i].x = mPoints->y = 0;
    }
}

void waveformViewer::add(const Point* newPoints, int count)
{
    // truncate new points to fit within window as needed
    // this effectively throws away oldest points in newPoints.
    if ( count > mMaxPoints) {
        newPoints += (count - mMaxPoints);
        count = mMaxPoints;
    }

    // sign extend window left on first call to avoid elongated waveform initially
    if (mNumPoints == 0) {
        float x = newPoints[0].x - 1, y = newPoints[0].y;
        for (int i=0; i < mMaxPoints; i++) {
            mPoints[i].x = x--;
            mPoints[i].y = y;
        }
        mNumPoints = mMaxPoints;
    }

    // shift the points in window left by count
    for (int i=count; i < mMaxPoints; i++) {
        mPoints[i-count] = mPoints[i];
    }

    // add new points
    for(int i=mMaxPoints-count, j=0; i<mMaxPoints && j<count; i++, j++) {
        mPoints[i] = newPoints[j];
    }

    // track the number of points in the window
    mNumPoints += count;
    if ( mNumPoints > mMaxPoints ) {
        mNumPoints = mMaxPoints;
    }

    // calculate pixel per unit (ppu) resolutions to map data to screen space
    float ymax, ymin = ymax = mPoints[0].y;
    float xmax, xmin = xmax = mPoints[0].x;
    for (int i = 1; i < mNumPoints; i++) {
        if (mPoints[i].x < xmin) xmin = mPoints[i].x;
        if (mPoints[i].x > xmax) xmax = mPoints[i].x;
        if (mPoints[i].y < ymin) ymin = mPoints[i].y;
        if (mPoints[i].y > ymax) ymax = mPoints[i].y;
    }

    int w = mRect.width() - mMarginLeft - mMarginRight;
    int h = mRect.height() - mMarginTop - mMarginBottom;

    float yrange = ymax - ymin;
    float yres = h / yrange; // in ppu

    float xrange = xmax - xmin;
    float xres = w / xrange; // in ppu

    // do drawing in the forground, fill with our pixmap on the paint thread
    // first draw black background
    mMutex.lock();
    QPainter painter(mPixelMap);
    QBrush blackbrush(Qt::black, Qt::SolidPattern);
    painter.fillRect(0,0,mRect.width(),mRect.height(), blackbrush);

    // next put in the waveform in green
    painter.setPen(QPen(Qt::green));

    int end = mNumPoints-1;
    for (int i=0; i<end; i++) {
        // convert points to pixels in screen coordinates
        int x1 = (mPoints[i].x - xmin) * xres;
        int x2 = (mPoints[i+1].x - xmin) * xres;
        int y1 = (mPoints[i].y - ymin) * yres;
        int y2 = (mPoints[i+1].y - ymin) * yres;

        // screen origin is located at top-left corner of the view, the following
        // transpose the pixel coordinates to xy coordinates with origin at the bottom-left corner.
        x1 = x1 + mMarginLeft;
        x2 = x2 + mMarginLeft;
        y1 = h - y1 + mMarginTop;
        y2 = h - y2 + mMarginTop;

        painter.drawLine(x1,y1,x2,y2);
    }
    mMutex.unlock();
}

void waveformViewer::draw(QPainter* painter)
{
        // do double buffered frame animation
        mMutex.lock();
        painter->drawPixmap(mRect,*mPixelMap);
        mMutex.unlock();
}

