#pragma once
#include "geometry/domain.h"
#include "geometry/cylinder.h"
#include "geometry/rectangle.h"
#include <memory>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

class domain_builder {
public:
    std::shared_ptr<domain> build_domain(std::string config_file_name);
};