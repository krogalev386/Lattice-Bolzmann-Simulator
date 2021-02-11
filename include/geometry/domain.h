#pragma once 
#include <memory>
//#include "mesh.h"
//#include "figure.h"

#include <xtensor/xarray.hpp>
#include <xtensor/xio.hpp>
#include <xtensor/xview.hpp>

struct mesh {
    int nx, ny;

    xt::xarray<double> f_distr;
    xt::xarray<double> f_distr_next;
    xt::xarray<double> rho_mesh;
    xt::xarray<double> vel_mesh;

    xt::xarray<bool> is_solid;

    xt::xarray<double> add_vel;

    std::vector<std::array<int,3>> in_out_node_indecies;
    std::vector<std::array<int,2>> border_node_indecies;
};


class domain: public std::enable_shared_from_this<domain> {
    double Re;
    double ext_force;
    double x_size, y_size;
    std::shared_ptr<mesh> p_mesh;

public:
    domain(double Re, double ext_f, double x, double y);
    ~domain();

    mesh& get_mesh() { return *p_mesh; };
    double get_force() { return ext_force; };

    //void insert_figure(std::shared_ptr<figure> fig);
};