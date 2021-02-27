#pragma once
#include <xtensor/xarray.hpp>
#include <xtensor/xio.hpp>
#include <xtensor/xview.hpp>
#include <memory>
#include "geometry/domain.h"
#include "data_transmitter.h"

class solver {
    std::shared_ptr<domain> dom;
    std::shared_ptr<data_transmitter> transmitter;
    int c;
    int timesteps;
    
    // internal functions
    void _bounce_back();
    void _streaming_step();
    void _collision_step();

    void _transport_to_neighbours();
    void _dense_eval();
    void _vel_eval();
    void _apply_next_step_f_distr();

    void init_f_distr();
    void f_eq_prepare();

    void _apply_left_vel_border_condition(double vel);
    void _apply_right_vel_border_condition(double vel);
    void _apply_vel_border_condition(double vel);
    
    int next_i(int i, int k);
    int next_j(int j, int k);

public:

    xt::xarray<double> f_eq;

    xt::xarray<double> get_velocities() { return dom->get_mesh().vel_mesh; }
    xt::xarray<double> get_density_map() { return dom->get_mesh().rho_mesh; }
    xt::xarray<double> get_f_distr() { return dom->get_mesh().f_distr; }
    xt::xarray<double> get_f_distr_next() { return dom->get_mesh().f_distr_next; }


    solver(std::shared_ptr<domain> dom, int timesteps): dom(dom), timesteps(timesteps) {
        int nx = dom->get_mesh().rho_mesh.shape()[0];
        int ny = dom->get_mesh().rho_mesh.shape()[1];
        f_eq = xt::zeros<double>({nx,ny,9});
        c = 1;
    };
    
    ~solver() {};
    
    void attatch_transmitter(std::shared_ptr<data_transmitter> transmitter);

    void solve();

};