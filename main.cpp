#include <iostream>
#include <string.h>
#include <cmath>
#include <vector>
#include "dcurve.h"
#include "darc.h"
#include "dpoint.h"
#include "dubins.h"
#include "plot.h"

int main(int argc, char *argv[])
{
    double Kmax = 8.0;

    Plot plot;
    plot.create_image();

    dPoint points[] = {
        dPoint(0.4, 0.4, M_PI),
        dPoint(0.2, 0.6, M_PI/2),
        dPoint(0.4, 0.8, 0),
        
        dPoint(0.8, 1, 0),
        
        dPoint(1.2, 0.8, 0),
        dPoint(1.4, 0.6, M_PI/2),
        dPoint(1.2, 0.4, M_PI),
    };

    std::vector<Dubins::Solution> solutions;

    for (int i = 0; i < sizeof(points) / sizeof(points[0]) - 1; i++)
    {
        solutions.push_back(Dubins::solve(points[i], points[i + 1], Kmax));
        if (solutions[i].pidx >= 0)
        {
            cout << "Success to find a solution\n";

            cout << "a1: " << solutions[i].c.a1 << endl;
            cout << "a2: " << solutions[i].c.a2 << endl;
            cout << "a3: " << solutions[i].c.a3 << endl;
            cout << "L: " << solutions[i].c.L << endl;

            plot.plot_curve(solutions[i].c);
        }
        else
        {
            cout << "Failed to find a solution\n";
        }
    }

    plot.show_image();
}
