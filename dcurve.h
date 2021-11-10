#ifndef __DCURVE_H__
#define __DCURVE_H__

#include "darc.h"
#include "dpoint.h"

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

class dCurve
{
public:
    dArc *a1, *a2, *a3;
    double L;

    dCurve(dPoint *d1, double s1, double s2, double s3, double k1, double k2, double k3);

    void plot(cv::Mat *image);
};

#endif
