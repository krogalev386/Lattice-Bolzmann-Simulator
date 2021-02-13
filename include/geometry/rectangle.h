#pragma once
#include "figure.h"

class rectangle: public figure {
    double x, y, dx, dy;

public:
    rectangle(double x, double y, double dx, double dy): x(x), y(y), dx(dx), dy(dy) {};
    ~rectangle(){};
    void insert_into_domain(std::shared_ptr<domain> p_domain) override ;
};