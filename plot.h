#ifndef __PLOT_H__
#define __PLOT_H__

#include <iostream>
#include <opencv2/highgui/highgui.hpp>

#include "dcurve.h"

class Plot
{
    cv::Mat *image;

public:
    void create_image();
    void plot_curve(dCurve &curve);
    void show_image();
};

#endif
