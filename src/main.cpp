#include "../include/geometry/domain.h"
#include "../include/config_reader.h"
#include "../include/solver.h"
#include <xtensor/xnpy.hpp>

int main(int argc, char** argv) {
    //std::shared_ptr<domain> dom = std::make_shared<domain>(0,1.104e-4,10,5,40,23);
    //std::shared_ptr<figure> cylind = std::make_shared<cylinder>(5,2.5,1);
    //std::cout << "Chech" << std::endl;
    config_reader conf_read;
    std::string conf_file = argv[1];
    std::shared_ptr<domain> dom = conf_read.build_domain(conf_file);
    //dom->insert_figure(cylind);
    solver solver_obj = solver(dom, conf_read.get_timesteps(conf_file));
    solver_obj.solve();
    xt::xarray<double> vel_m = solver_obj.get_velocities();
    xt::xarray<double> rho_m = solver_obj.get_density_map();
    xt::xarray<double> f_distr = solver_obj.get_f_distr();
    xt::xarray<double> f_distr_next = solver_obj.get_f_distr_next();
    
    //std::cout << rho_m << std::endl;
    xt::dump_npy("rho_m.npy", rho_m);
    xt::dump_npy("vel_m.npy", vel_m);
    xt::dump_npy("obstacle_map.npy", dom->get_mesh().is_solid);
    //std::cout << xt::view(vel_m, xt::all(), xt::all(), 0) << std::endl;
};