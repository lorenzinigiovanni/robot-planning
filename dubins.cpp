#include "dubins.h"
#include "utils.h"
#include <limits>
#include <cmath>
#include <cassert>

int Dubins::kSigns[6][3] = {
    {1, 0, 1},   //LSL
    {-1, 0, -1}, //RSR
    {1, 0, -1},  //LSR
    {-1, 0, 1},  //RSL
    {-1, 1, -1}, //RLR
    {1, -1, 1},  //LRL
};

Dubins::StandardForm Dubins::scaleToStandard(dPoint &pi, dPoint &pf, double Kmax)
{
    double dx = pf.x - pi.x;
    double dy = pf.y - pi.y;
    double phi = atan2(dy, dx);
    double lambda = hypot(dx, dy) / 2;

    // scale and normalize angles and curvature
    double sc_th0 = Utils::mod2pi(pi.t - phi);
    double sc_thf = Utils::mod2pi(pf.t - phi);
    double sc_Kmax = Kmax * lambda;

    return Dubins::StandardForm(sc_th0, sc_thf, sc_Kmax, lambda);
}

Dubins::NotStandardForm Dubins::scaleFromStandard(Dubins::StandardForm &sf)
{
    double s1 = sf.sc_th0 * sf.lambda;
    double s2 = sf.sc_thf * sf.lambda;
    double s3 = sf.sc_Kmax * sf.lambda;
    return Dubins::NotStandardForm(s1, s2, s3);
}

Dubins::Solution Dubins::solve(dPoint &p1, dPoint &p2, double Kmax)
{
    Dubins::StandardForm sf = Dubins::scaleToStandard(p1, p2, Kmax);

    int pidx = -1;
    double L = std::numeric_limits<double>::max(); // infinito
    Dubins::StandardForm tmp_sf = StandardForm(0, 0, 0, 0);

    for (int i = 0; i <= 5; i++)
    {
        Ok_Pieces op = (Dubins::primitives[i])(sf);
        double Lcur = op.nsf.s1 + op.nsf.s2 + op.nsf.s3;

        if (op.check && (Lcur < L))
        {
            L = Lcur;
            tmp_sf.sc_th0 = op.nsf.s1;
            tmp_sf.sc_thf = op.nsf.s2;
            tmp_sf.sc_Kmax = op.nsf.s3;
            pidx = i;
        }
    }

    if (pidx >= 0)
    {
        tmp_sf.lambda = sf.lambda;
        Dubins::NotStandardForm nsf = scaleFromStandard(tmp_sf);

        double k1 = Dubins::kSigns[pidx][0] * Kmax;
        double k2 = Dubins::kSigns[pidx][1] * Kmax;
        double k3 = Dubins::kSigns[pidx][2] * Kmax;
        dCurve curve = dCurve(&p1, nsf.s1, nsf.s2, nsf.s3, k1, k2, k3);

        // assert(Utils::check(
        //     tmp_sf.sc_th0,
        //     Dubins::kSigns[pidx][0] * sf.sc_Kmax,
        //     tmp_sf.sc_thf,
        //     Dubins::kSigns[pidx][1] * sf.sc_Kmax,
        //     tmp_sf.sc_Kmax,
        //     Dubins::kSigns[pidx][2] * sf.sc_Kmax,
        //     sf.sc_th0,
        //     sf.sc_thf));

        if (!Utils::check(
                tmp_sf.sc_th0,
                Dubins::kSigns[pidx][0] * sf.sc_Kmax,
                tmp_sf.sc_thf,
                Dubins::kSigns[pidx][1] * sf.sc_Kmax,
                tmp_sf.sc_Kmax,
                Dubins::kSigns[pidx][2] * sf.sc_Kmax,
                sf.sc_th0,
                sf.sc_thf))
        {
            cout << "ERROR, dubins.cpp riga 92" << endl;
        }

        return Dubins::Solution(pidx, curve);
    }

    return Dubins::Solution(pidx, dCurve(new dPoint(0, 0, 0), 0, 0, 0, 0, 0, 0));
}

Dubins::Ok_Pieces Dubins::LSL(Dubins::StandardForm &sf)
{
    double sc_s1;
    double sc_s2;
    double sc_s3;

    double invK = 1 / sf.sc_Kmax;
    double C = cos(sf.sc_thf) - cos(sf.sc_th0);
    double S = 2 * sf.sc_Kmax + sin(sf.sc_th0) - sin(sf.sc_thf);
    double temp1 = atan2(C, S);

    sc_s1 = invK * Utils::mod2pi(temp1 - sf.sc_th0);
    double delta = 2 + 4 * pow(sf.sc_Kmax, 2) - 2 * cos(sf.sc_th0 - sf.sc_thf) + 4 * sf.sc_Kmax * (sin(sf.sc_th0) - sin(sf.sc_thf));

    if (delta < 0)
    {
        return Dubins::Ok_Pieces(false, Dubins::NotStandardForm(0, 0, 0));
    }

    sc_s2 = invK * sqrt(delta);
    sc_s3 = invK * Utils::mod2pi(sf.sc_thf - temp1);

    return Dubins::Ok_Pieces(true, Dubins::NotStandardForm(sc_s1, sc_s2, sc_s3));
}

Dubins::Ok_Pieces Dubins::RSR(Dubins::StandardForm &sf)
{
    double sc_s1;
    double sc_s2;
    double sc_s3;

    double invK = 1 / sf.sc_Kmax;
    double C = cos(sf.sc_th0) - cos(sf.sc_thf);
    double S = 2 * sf.sc_Kmax - sin(sf.sc_th0) + sin(sf.sc_thf);
    double temp1 = atan2(C, S);
    sc_s1 = invK * Utils::mod2pi(sf.sc_th0 - temp1);
    double delta = 2 + 4 * pow(sf.sc_Kmax, 2) - 2 * cos(sf.sc_th0 - sf.sc_thf) - 4 * sf.sc_Kmax * (sin(sf.sc_th0) - sin(sf.sc_thf));

    if (delta < 0)
    {
        return Dubins::Ok_Pieces(false, Dubins::NotStandardForm(0, 0, 0));
    }

    sc_s2 = invK * sqrt(delta);
    sc_s3 = invK * Utils::mod2pi(temp1 - sf.sc_thf);

    return Dubins::Ok_Pieces(true, Dubins::NotStandardForm(sc_s1, sc_s2, sc_s3));
}

Dubins::Ok_Pieces Dubins::LSR(Dubins::StandardForm &sf)
{
    double sc_s1;
    double sc_s2;
    double sc_s3;

    double invK = 1 / sf.sc_Kmax;
    double C = cos(sf.sc_th0) + cos(sf.sc_thf);
    double S = 2 * sf.sc_Kmax + sin(sf.sc_th0) + sin(sf.sc_thf);
    double delta = -2 + 4 * pow(sf.sc_Kmax, 2) + 2 * cos(sf.sc_th0 - sf.sc_thf) + 4 * sf.sc_Kmax * (sin(sf.sc_th0) + sin(sf.sc_thf));
    if (delta < 0)
    {
        return Dubins::Ok_Pieces(false, Dubins::NotStandardForm(0, 0, 0));
    }

    sc_s2 = invK * sqrt(delta);
    sc_s1 = invK * Utils::mod2pi(atan2(-C, S) - atan2(-2, sf.sc_Kmax * sc_s2) - sf.sc_th0);
    sc_s3 = invK * Utils::mod2pi(atan2(-C, S) - atan2(-2, sf.sc_Kmax * sc_s2) - sf.sc_thf);

    return Dubins::Ok_Pieces(true, Dubins::NotStandardForm(sc_s1, sc_s2, sc_s3));
}

Dubins::Ok_Pieces Dubins::RSL(Dubins::StandardForm &sf)
{
    double sc_s1;
    double sc_s2;
    double sc_s3;

    double invK = 1 / sf.sc_Kmax;
    double C = cos(sf.sc_th0) + cos(sf.sc_thf);
    double S = 2 * sf.sc_Kmax - sin(sf.sc_th0) - sin(sf.sc_thf);

    double delta = -2 + 4 * pow(sf.sc_Kmax, 2) + 2 * cos(sf.sc_th0 - sf.sc_thf) - 4 * sf.sc_Kmax * (sin(sf.sc_th0) + sin(sf.sc_thf));
    if (delta < 0)
    {
        return Dubins::Ok_Pieces(false, Dubins::NotStandardForm(0, 0, 0));
    }

    sc_s2 = invK * sqrt(delta);
    sc_s1 = invK * Utils::mod2pi(sf.sc_th0 - atan2(C, S) + atan2(2, sf.sc_Kmax * sc_s2));
    sc_s3 = invK * Utils::mod2pi(sf.sc_thf - atan2(C, S) + atan2(2, sf.sc_Kmax * sc_s2));

    return Dubins::Ok_Pieces(true, Dubins::NotStandardForm(sc_s1, sc_s2, sc_s3));
}

Dubins::Ok_Pieces Dubins::RLR(Dubins::StandardForm &sf)
{
    double sc_s1;
    double sc_s2;
    double sc_s3;

    double invK = 1 / sf.sc_Kmax;
    double C = cos(sf.sc_th0) - cos(sf.sc_thf);
    double S = 2 * sf.sc_Kmax - sin(sf.sc_th0) + sin(sf.sc_thf);

    sc_s2 = invK * Utils::mod2pi(2 * M_PI - acos(0.125 * (6 - 4 * pow(sf.sc_Kmax, 2) + 2 * cos(sf.sc_th0 - sf.sc_thf) + 4 * sf.sc_Kmax * (sin(sf.sc_th0) - sin(sf.sc_thf)))));
    sc_s1 = invK * Utils::mod2pi(sf.sc_th0 - atan2(C, S) + 0.5 * sf.sc_Kmax * sc_s2);
    sc_s3 = invK * Utils::mod2pi(sf.sc_th0 - sf.sc_thf + sf.sc_Kmax * (sc_s2 - sc_s1));

    return Dubins::Ok_Pieces(true, Dubins::NotStandardForm(sc_s1, sc_s2, sc_s3));
}

Dubins::Ok_Pieces Dubins::LRL(Dubins::StandardForm &sf)
{
    double sc_s1;
    double sc_s2;
    double sc_s3;

    double invK = 1 / sf.sc_Kmax;
    double C = cos(sf.sc_thf) - cos(sf.sc_th0);
    double S = 2 * sf.sc_Kmax + sin(sf.sc_th0) - sin(sf.sc_thf);

    sc_s2 = invK * Utils::mod2pi(2 * M_PI - acos(0.125 * (6 - 4 * pow(sf.sc_Kmax, 2) + 2 * cos(sf.sc_th0 - sf.sc_thf) - 4 * sf.sc_Kmax * (sin(sf.sc_th0) - sin(sf.sc_thf)))));
    sc_s1 = invK * Utils::mod2pi(-sf.sc_th0 + atan2(C, S) + 0.5 * sf.sc_Kmax * sc_s2);
    sc_s3 = invK * Utils::mod2pi(sf.sc_thf - sf.sc_th0 + sf.sc_Kmax * (sc_s2 - sc_s1));

    return Dubins::Ok_Pieces(true, Dubins::NotStandardForm(sc_s1, sc_s2, sc_s3));
}

Dubins::Ok_Pieces (*Dubins::primitives[6])(Dubins::StandardForm &sf) = {
    Dubins::LSL,
    Dubins::RSR,
    Dubins::LSR,
    Dubins::RSL,
    Dubins::RLR,
    Dubins::LRL,
};
