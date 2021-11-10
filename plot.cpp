#include "plot.h"

void Plot::create_image()
{
    image = new cv::Mat(768, 1024, CV_8UC3, cv::Scalar(0, 0, 0));
}

void Plot::plot_curve(dCurve &curve)
{
    curve.plot(image);
}

void Plot::show_image()
{
    cv::flip(*image, *image, 0);
    cv::imshow("Cirruzzo o' Gabbiano", *image);
    cv::waitKey(0);
}
