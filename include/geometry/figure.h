#pragma once
#include <memory>

class domain;

class figure {
public:

    virtual void insert_into_domain(std::shared_ptr<domain> p_domain) = 0;
    //virtual ~figure() {};

};