#include "../include/geometry/domain.h"
#include "../include/config_reader.h"
#include "../include/solver.h"
#include <xtensor/xnpy.hpp>

int main(int argc, char** argv) {
 
    config_reader conf_read;
    std::string conf_file = argv[1];
    std::shared_ptr<domain> dom = conf_read.build_domain(conf_file);
 
    std::shared_ptr<data_transmitter> transmitter = std::make_shared<data_transmitter>(1337);
 
    solver solver_obj = solver(dom, conf_read.get_timesteps(conf_file), 100);
    solver_obj.attatch_transmitter(transmitter);

    solver_obj.solve();

    xt::xarray<double> vel_m = solver_obj.get_velocities();
    xt::xarray<double> rho_m = solver_obj.get_density_map();
    xt::xarray<double> f_distr = solver_obj.get_f_distr();
    xt::xarray<double> f_distr_next = solver_obj.get_f_distr_next();
    
    xt::dump_npy("rho_m.npy", rho_m);
    xt::dump_npy("vel_m.npy", vel_m);
    xt::dump_npy("obstacle_map.npy", dom->get_mesh().is_solid);
};