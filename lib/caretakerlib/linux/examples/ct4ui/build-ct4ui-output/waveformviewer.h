#ifndef WAVEFORMVIEWER_H
#define WAVEFORMVIEWER_H
#include "qpainter.h"
#include "qmutex.h"
#include <stdint.h>

class waveformViewer
{
public:
    struct Point {
        float x;
        float y;

        Point() : x(0), y(0) {}

        Point(float x, float y) {
            this->x = x;
            this->y = y;
        }

        Point(const Point& p) {
            *this = p;
        }

        Point& operator= (const Point& rhs) {
            if (this != &rhs) {
                x = rhs.x;
                y = rhs.y;
            }
            return *this;
        }
    };

    waveformViewer(const QRect& rect,
                   int marginLeft,
                   int marginTop,
                   int marginRight,
                   int marginBottom,
                   float xres = 1.0);

    ~waveformViewer();

    void draw(QPainter* painter);

    void add(float v) {
        add(v,v);
    }

    void add(float x, float y) {
        Point p(x,y);
        add(&p, 1);
    }

    void add(const Point& p) {
        add(&p, 1);
    }

    void add(const Point* newPoints, int count);

    void clear(void);

private:
    QRect mRect;
    QMutex mMutex;
    QPixmap *mPixelMap;
    int mMarginTop;
    int mMarginBottom;
    int mMarginLeft;
    int mMarginRight;
    Point *mPoints;
    int mMaxPoints;
    int mNumPoints;
};

#endif // WAVEFORMVIEWER_H
