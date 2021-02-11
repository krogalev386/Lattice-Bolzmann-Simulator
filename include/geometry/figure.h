#pragma once
#include <memory>

class domain;

class figure {
public:

    virtual instert_into_domain(std::shared_ptr<domain> p_domain);
    virtual ~figure() = 0;

}