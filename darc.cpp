#include "darc.h"
#include <cmath>
#include "utils.h"

dArc::dArc(dPoint *_i, double _k, double _L) : i(_i), k(_k), L(_L)
{
    f = circLine(_L, _i, _k);
}

dPoint *dArc::circLine(double s, dPoint *p, double k)
{
    double x = p->x + s * Utils::sinc(k * s / 2.0) * cos(p->t + k * s / 2);
    double y = p->y + s * Utils::sinc(k * s / 2.0) * sin(p->t + k * s / 2);
    double theta = Utils::mod2pi(p->t + k * s);
    return new dPoint(x, y, theta);
}

void dArc::plotArc(cv::Mat *image, int _index)
{
    int n_points = 1000;
    std::vector<cv::Point> contours;

    for (int index = 0; index < n_points; index++)
    {
        double s = L / n_points * index;
        dPoint *d3 = circLine(s, i, k);
        contours.push_back(cv::Point(d3->x * 100 + 200, d3->y * 100 + 200));
    }

    cv::Scalar *color = nullptr;
    switch (_index)
    {
    case 0:
        color = new cv::Scalar(0, 255, 0);
        break;
    case 1:
        color = new cv::Scalar(255, 0, 0);
        break;
    case 2:
        color = new cv::Scalar(0, 0, 255);
        break;
    default:
        color = new cv::Scalar(255, 255, 255);
        break;
    }

    for (int i = 1; i < n_points; i++)
    {
        cv::line(*image, contours[i], contours[i - 1], *color, 1);
    }
}
