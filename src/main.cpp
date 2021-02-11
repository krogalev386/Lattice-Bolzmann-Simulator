#include "../include/geometry/domain.h"
#include "../include/solver.h"

int main() {
    std::shared_ptr<domain> dom = std::make_shared<domain>(0,1.104e-3,10,5,20,13);
    std::shared_ptr<figure> cylind = std::make_shared<cylinder>(5,2.5,1);
    std::cout << "Chech" << std::endl;
    dom->insert_figure(cylind);
    solver solver_obj = solver(dom, 1000);
    solver_obj.solve();
    xt::xarray<double> vel_m = solver_obj.get_velocities();
    xt::xarray<double> rho_m = solver_obj.get_density_map();
    xt::xarray<double> f_distr = solver_obj.get_f_distr();
    xt::xarray<double> f_distr_next = solver_obj.get_f_distr_next();
    
    std::cout << rho_m << std::endl;
    //std::cout << xt::view(vel_m, xt::all(), xt::all(), 0) << std::endl;
};