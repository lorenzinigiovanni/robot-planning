#include "dcurve.h"

dCurve::dCurve(dPoint *d1, double s1, double s2, double s3, double k1, double k2, double k3)
{
    a1 = new dArc(d1, k1, s1);
    a2 = new dArc(a1->f, k2, s2);
    a3 = new dArc(a2->f, k3, s3);
    L = a1->L + a2->L + a3->L;
}

void dCurve::plot(cv::Mat *image)
{
    a1->plotArc(image, 0);
    a2->plotArc(image, 1);
    a3->plotArc(image, 2);
}
