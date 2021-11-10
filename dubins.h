#ifndef __DUBINS_H__
#define __DUBINS_H__

#include <iostream>

#include "dpoint.h"
#include "dcurve.h"

using namespace std;

class Dubins
{
public:
    static int kSigns[6][3];

    struct Solution
    {
        // tipo di soluzione, curva trovata
        int pidx;
        dCurve c;
        Solution(int _pidx, dCurve _c) : pidx(_pidx), c(_c) {} // probabilmente ce va un puntatore, cosi me sa che è per copia
    };

    struct StandardForm
    {
        double sc_th0;
        double sc_thf;
        double sc_Kmax;
        double lambda;
        StandardForm(double _sc_th0, double _sc_thf, double _sc_Kmax, double _lambda) : sc_th0(_sc_th0), sc_thf(_sc_thf), sc_Kmax(_sc_Kmax), lambda(_lambda) {}
    };

    struct NotStandardForm
    {
        double s1, s2, s3;
        NotStandardForm(double _s1, double _s2, double _s3) : s1(_s1), s2(_s2), s3(_s3) {}
    };

    struct Ok_Pieces
    {
        bool check;
        NotStandardForm nsf;
        Ok_Pieces(bool _check, NotStandardForm _nsf) : check(_check), nsf(_nsf) {}
    };

    static Solution solve(dPoint &p1, dPoint &p2, double Kmax);
    static StandardForm scaleToStandard(dPoint &pi, dPoint &pf, double Kmax);
    static NotStandardForm scaleFromStandard(StandardForm &sf);

    static Ok_Pieces LSL(StandardForm &sf);
    static Ok_Pieces RSR(StandardForm &sf);
    static Ok_Pieces LSR(StandardForm &sf);
    static Ok_Pieces RSL(StandardForm &sf);
    static Ok_Pieces RLR(StandardForm &sf);
    static Ok_Pieces LRL(StandardForm &sf);

    static Ok_Pieces (*primitives[6])(StandardForm &sf);

private:
    Dubins();
};

#endif
