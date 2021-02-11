#include "../../include/geometry/domain.h"

domain::domain(double Re, double ext_f, double x, double y): Re(Re), ext_force(ext_f), x_size(x), y_size(y) {
    //p_mesh = std::make_shared(Re, ext_f, x, y);

    p_mesh = std::make_shared<mesh>();

    p_mesh->nx = x;
    p_mesh->ny = y;

    int& nx = p_mesh->nx;
    int& ny = p_mesh->ny;

    p_mesh->f_distr = xt::zeros<double>({nx,ny,9});
    p_mesh->f_distr_next = xt::zeros<double>({nx,ny,9});
    p_mesh->rho_mesh = xt::ones<double>({nx,ny});
    p_mesh->vel_mesh = xt::zeros<double>({nx,ny,2});
    p_mesh->is_solid = xt::zeros<bool>({nx,ny});

    // make horizontal pipe
    xt::view(p_mesh->is_solid, xt::all(), 0).fill(true);
    xt::view(p_mesh->is_solid, xt::all(), ny-1).fill(true);
    for (int i = 0; i < nx; i++){
        p_mesh->border_node_indecies.push_back({i,0});
        p_mesh->border_node_indecies.push_back({i,ny-1});
    }

    p_mesh->add_vel = xt::zeros<double>({nx,ny,2});
    xt::view(p_mesh->add_vel, xt::all(), xt::all(), 0).fill(ext_f);

    //eliminate add_vel and rho in solid nodes
    for (int i = 0; i < nx; ++i)
        for (int j = 0; j < ny; ++j)
            if (p_mesh->is_solid(i,j)){
                p_mesh->add_vel(i,j,0) = 0;
                p_mesh->add_vel(i,j,1) = 0;
                p_mesh->rho_mesh(i,j) = 0;
            }

        

    for (int j = 1; j < ny-1; ++j){
        p_mesh->in_out_node_indecies.push_back({0,j,4}); // west nodes
        p_mesh->in_out_node_indecies.push_back({nx-1,j,2}); // east nodes
    }
};

domain::~domain(){};

//void domain::insert_figure(std::shared_ptr<figure> fig){
 //   fig->instert_into_domain(std::shared_from_this());
//}