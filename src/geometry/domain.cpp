#include "../../include/geometry/domain.h"

domain::domain(double Re, double ext_f, double x, double y, int nx, int ny): Re(Re), ext_force(ext_f), x_size(x), y_size(y) {
    //p_mesh = std::make_shared(Re, ext_f, x, y);

    p_mesh = std::make_shared<mesh>();

    p_mesh->nx = nx;
    p_mesh->ny = ny;

    p_mesh->x_axis_grid = xt::linspace<double>(0,x_size,nx);
    p_mesh->y_axis_grid = xt::linspace<double>(0,y_size,ny);
    
    p_mesh->f_distr = xt::zeros<double>({nx,ny,9});
    p_mesh->f_distr_next = xt::zeros<double>({nx,ny,9});
    p_mesh->rho_mesh = xt::ones<double>({nx,ny});
    p_mesh->vel_mesh = xt::zeros<double>({nx,ny,2});
    p_mesh->is_solid = xt::zeros<bool>({nx,ny});

    // make horizontal pipe
    xt::view(p_mesh->is_solid, xt::all(), 0).fill(true);
    xt::view(p_mesh->is_solid, xt::all(), ny-1).fill(true);
    
    
    /*for (int i = 0; i < nx; i++){
        p_mesh->border_node_indecies.push_back({i,0});
        p_mesh->border_node_indecies.push_back({i,ny-1});
    }*/

    mark_border_nodes();
    init_phys_field();

    /*p_mesh->add_vel = xt::zeros<double>({nx,ny,2});
    xt::view(p_mesh->add_vel, xt::all(), xt::all(), 0).fill(ext_f);

    //eliminate add_vel and rho in solid nodes
    for (int i = 0; i < nx; ++i)
        for (int j = 0; j < ny; ++j)
            if (p_mesh->is_solid(i,j)){
                p_mesh->add_vel(i,j,0) = 0;
                p_mesh->add_vel(i,j,1) = 0;
                p_mesh->rho_mesh(i,j) = 0;
            }*/

    for (int j = 1; j < ny-1; ++j){
        p_mesh->in_out_node_indecies.push_back({0,j,4}); // west nodes
        p_mesh->in_out_node_indecies.push_back({nx-1,j,2}); // east nodes
    }
};

domain::~domain(){};

void domain::mark_border_nodes(){
    auto& is_solid = p_mesh->is_solid;
    int& nx = p_mesh->nx;
    int& ny = p_mesh->ny;
    // inner nodes
    for (int i = 1; i < nx-1; ++i)
        for (int j = 1; j < ny-1; ++j)
            if (is_solid(i,j))
                if (!(is_solid(i+1,j) && is_solid(i,j+1) && is_solid(i-1,j) && is_solid(i,j-1)
                    && is_solid(i+1,j+1) && is_solid(i-1,j+1) && is_solid(i+1,j-1) && is_solid(i-1,j-1)))
                    p_mesh->border_node_indecies.push_back({i,j});

    // south border
    for (int i = 1; i < nx-1; ++i)
        if (is_solid(i,0))
            if (!(is_solid(i+1,0) && is_solid(i,1) && is_solid(i-1,0)
                && is_solid(i+1,1) && is_solid(i-1,1)))
                p_mesh->border_node_indecies.push_back({i,0});
    
    // south-west corner
    if (is_solid(0,0) && !(is_solid(0,1) && is_solid(1,0) && is_solid(1,1)))
        p_mesh->border_node_indecies.push_back({0,0});

    // south-east corner
    if (is_solid(nx-1,0) && !(is_solid(nx-1,1) && is_solid(nx-2,0) && is_solid(nx-2,1)))
        p_mesh->border_node_indecies.push_back({nx-1,0});

    // north border
    for (int i = 1; i < nx-1; ++i)
        if (is_solid(i,ny-1))
            if (!(is_solid(i+1,ny-1) && is_solid(i,ny-2) && is_solid(i-1,ny-1)
                && is_solid(i+1,ny-2) && is_solid(i-1,ny-2)))
                p_mesh->border_node_indecies.push_back({i,ny-1});
    
    // north-west corner
    if (is_solid(0,ny-1) && !(is_solid(0,ny-2) && is_solid(1,ny-1) && is_solid(1,ny-2)))
        p_mesh->border_node_indecies.push_back({0,ny-1});

    // north-east corner
    if (is_solid(nx-1,ny-1) && !(is_solid(nx-1,ny-2) && is_solid(nx-2,ny-1) && is_solid(nx-2,ny-2)))
        p_mesh->border_node_indecies.push_back({nx-1,ny-1});

};

void domain::init_phys_field(){
    int& nx = p_mesh->nx;
    int& ny = p_mesh->ny;
    p_mesh->add_vel = xt::zeros<double>({nx,ny,2});
    xt::view(p_mesh->add_vel, xt::all(), xt::all(), 0).fill(ext_force);

    //eliminate add_vel and rho in solid nodes
    for (int i = 0; i < nx; ++i)
        for (int j = 0; j < ny; ++j)
            if (p_mesh->is_solid(i,j)){
                p_mesh->add_vel(i,j,0) = 0;
                p_mesh->add_vel(i,j,1) = 0;
                p_mesh->rho_mesh(i,j) = 0;
            }
}

void domain::insert_figure(std::shared_ptr<figure> fig){
    fig->insert_into_domain(shared_from_this());
    mark_border_nodes();
    init_phys_field();
};