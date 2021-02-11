#include "../../include/geometry/domain.h"
#include "../../include/geometry/cylinder.h"

void cylinder::insert_into_domain(std::shared_ptr<domain> p_domain){

    mesh& geo_mesh = p_domain->get_mesh();
    auto& x_lin = geo_mesh.x_axis_grid;
    auto& y_lin = geo_mesh.y_axis_grid;

    for (int i = 0; i < geo_mesh.nx; ++i)
        for (int j = 0; j < geo_mesh.ny; ++j)
            // mark solid nodes
            if ((x_lin(i) - x)*(x_lin(i) - x) 
                + (y_lin(j) - y)*(y_lin(j) - y) < radius*radius)
                geo_mesh.is_solid(i,j) = true;
}