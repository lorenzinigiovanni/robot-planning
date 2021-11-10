#ifndef __UTILS_H__
#define __UTILS_H__

class Utils
{
public:
    double static sinc(double t);
    double static mod2pi(double ang);
    double static rangeSymm(double ang);
    bool static check(double s1, double k0, double s2, double k1, double s3, double k2, double th0, double thf);

private:
    Utils();
};

#endif