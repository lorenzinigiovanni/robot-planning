#ifndef __DARC_H__
#define __DARC_H__

#include "dpoint.h"

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

class dArc
{
public:
    dPoint *i;
    dPoint *f;
    double k, L;

    dArc(dPoint *_i, double _k, double _L);
    dPoint *circLine(double s, dPoint *p, double k);
    void plotArc(cv::Mat *image, int index);
};

#endif
