#include <iostream>
#include <math.h>
#include "constants.hpp"

using namespace std;

namespace LGR {
  struct Ball {
    Axis X = Axis("X");
    Axis Y = Axis("Y");
    Axis Z = Axis("Z");

    TickMeter t;

    int consecFramesFound = 0;

    void nextCoords(double newX, double newY, double newZ);
    void calcFinalPos(double dt, Robot* r);

    void print();
    void reset();
  };

  void Ball::calcFinalPos(double dt, Robot* r) {
    if (consecFramesFound < 2) {
        return;
    }
    if (consecFramesFound == 2) {
      X.calcVel(dt);
      Y.calcVel(dt);
      Z.calcVel(dt);

      double finaldt = abs((Z.p2)/Z.vel);

      X.pf = (X.vel * finaldt) * (1 - EXP_AVG_WT) + X.pf * EXP_AVG_WT;
      Y.pf = (0.5 * GRAVITY * (finaldt*finaldt) + Y.vel * finaldt + Y.p1)  * (1 - EXP_AVG_WT) + Y.pf * EXP_AVG_WT;
    }
    else if (consecFramesFound > 2) {
        Z.calcVel(dt);
        X.pf = ((Z.p2 * Z.p3 * (Z.p2 - Z.p3) * X.p1 + Z.p3 * Z.p1 * (Z.p3 - Z.p1) * X.p2 + Z.p1 * Z.p2 * (Z.p1 - Z.p2) * X.p3) / ((Z.p1 - Z.p2) * (Z.p1 - Z.p3) * (Z.p2 - Z.p3)))  * (1 - EXP_AVG_WT) + X.pf * EXP_AVG_WT;
        Y.pf = ((Z.p2 * Z.p3 * (Z.p2 - Z.p3) * Y.p1 + Z.p3 * Z.p1 * (Z.p3 - Z.p1) * Y.p2 + Z.p1 * Z.p2 * (Z.p1 - Z.p2) * Y.p3) / ((Z.p1 - Z.p2) * (Z.p1 - Z.p3) * (Z.p2 - Z.p3)))  * (1 - EXP_AVG_WT) + Y.pf * EXP_AVG_WT;
    }

    if (Y.pf < r->Camera_dy_Ground) {
        Y.pf = - r->Camera_dy_Ground - (Y.pf + r->Camera_dy_Ground);
    }

    r->DestX = X.pf;
    r->DestY = Y.pf;


    if (Z.vel < SLOW_TOL) {
        r->DestX = X.p1;
        r->DestY = Y.p1;
    }
  }

  void Ball::nextCoords(double newX, double newY, double newZ) {
    X.stepTime(newX);
    Y.stepTime(newY);
    Z.stepTime(newZ);

    consecFramesFound++;
  }

  void Ball::reset() {
    X.reset();
    Y.reset();
    Z.reset();

    consecFramesFound = 0;
  }

  void Ball::print() {
    X.print();
    Y.print();
    Z.print();
  }
}