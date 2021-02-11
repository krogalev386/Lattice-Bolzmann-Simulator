#pragma once
#include "figure.h"

class cylinder: public figure {
    double x, y, radius;

public:
    cylinder(double x, double, y, double radius): x(x), y(y), radius(radius) {};
    ~cylinder(){};
    void insert_into_domain(std::shared_ptr<domain> p_domain);
}