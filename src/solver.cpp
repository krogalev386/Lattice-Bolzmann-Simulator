#include "../include/solver.h"
#include <tuple>

// Additional functions

std::tuple<double, double, double> f_eq_params(int index, double c){
    double e_x = 0; double e_y = 0; double w = 0;
    if (index == 1) {e_x = c; w = 1./9;}
    else if (index == 2) {e_y = c; w = 1./9;}
    else if (index == 3) {e_x = -c; w = 1./9;}
    else if (index == 4) {e_y = -c; w = 1./9;}
    else if (index == 5) {e_x = c; e_y = c; w = 1./36;}
    else if (index == 6) {e_x = -c; e_y = c; w = 1./36;}
    else if (index == 7) {e_x = -c; e_y = -c; w = 1./36;}
    else if (index == 8) {e_x = c; e_y = -c; w = 1./36;}
    else {w = 4./9;}

    return std::tuple<double,double,double>({w,e_x,e_y});
};

int anti_k(int k){
    if (k==1) return 3;
    if (k==2) return 4;
    if (k==3) return 1;
    if (k==4) return 2;
    if (k==5) return 7;
    if (k==6) return 8;
    if (k==7) return 5;
    if (k==8) return 6;
    if (k==9) return 9;
    else throw std::runtime_error("unknown k!");
};

// Class methods

int solver::next_i(int i, int k){
    int new_i = i;
    if ((k==5)||(k==1)||(k==8)) new_i++;
    if ((k==6)||(k==3)||(k==7)) new_i--;
    
    if (new_i < 0) return (dom->get_mesh().nx - 1); // periodic boundaries
    if (new_i > (dom->get_mesh().nx - 1)) return 0;
    
    return new_i;
};

int solver::next_j(int j, int k){
    int new_j = j;
    if ((k==6)||(k==2)||(k==5)) new_j++;
    if ((k==7)||(k==4)||(k==8)) new_j--;
    
    if (new_j < 0) return (dom->get_mesh().ny - 1); // periodic boundaries
    if (new_j > (dom->get_mesh().ny - 1)) return 0;
    
    return new_j;
};

void solver::init_f_distr(){
    //std::cout << "Pre-init f_distr" << std::endl;
    //std::cout << dom->get_mesh().f_distr << std::endl;
    auto& rho_m = dom->get_mesh().rho_mesh;
    for (uint k = 1; k < 10; ++k){
        std::tuple<double,double,double> params = f_eq_params(k,c);
        double w = std::get<0>(params);
        double e_x = std::get<1>(params);
        double e_y = std::get<2>(params);
        xt::view(f_eq, xt::all(), xt::all(), k-1) = w*rho_m;
    }
    dom->get_mesh().f_distr = f_eq;
    //std::cout << "Post-init f_distr" << std::endl;
    //std::cout << dom->get_mesh().f_distr << std::endl;
};

void solver::f_eq_prepare(){

    //double& add_vel = dom->get_force();
    auto& rho_m = dom->get_mesh().rho_mesh;
    auto& vel_m = dom->get_mesh().vel_mesh;
    auto& add_vel = dom->get_mesh().add_vel;
    auto vel_m_x = xt::view(vel_m+add_vel, xt::all(), xt::all(), 0);
    auto vel_m_y = xt::view(vel_m+add_vel, xt::all(), xt::all(), 1);

    for (uint k = 0; k < 9; ++k){
        std::tuple<double,double,double> params = f_eq_params(k+1,1);
        double w = std::get<0>(params);
        double e_x = std::get<1>(params);
        double e_y = std::get<2>(params);

        xt::view(f_eq, xt::all(), xt::all(), k) = w*rho_m*(
            xt::ones<double>({f_eq.shape()[0],f_eq.shape()[1]})
            + 3*(e_x*vel_m_x + e_y*vel_m_y)
            + (9./2)*((e_x*vel_m_x + e_y*vel_m_y)*(e_x*vel_m_x + e_y*vel_m_y))
            - (3./2)*(vel_m_x*vel_m_x + vel_m_y*vel_m_y)
        );
        //std::cout << xt::view(f_eq, xt::all(), xt::all(), k) << std::endl;
    }
};

void solver::_bounce_back(){
    auto& border_nodes = dom->get_mesh().border_node_indecies;
    auto& is_solid_nodes = dom->get_mesh().is_solid;
    auto& f_distr = dom->get_mesh().f_distr;
    for (auto iter = border_nodes.begin(); iter!= border_nodes.end(); ++iter){
        for (uint k = 1; k < 10; ++k){
            int i = iter->at(0);
            int j = iter->at(1);
            f_distr(next_i(i,anti_k(k)),next_j(j,anti_k(k)),anti_k(k)-1) += f_distr(i,j,k-1);
            f_distr(i,j,k-1) = 0;
        }
    }
};

void solver::_transport_to_neighbours(){
    auto& f_distr = dom->get_mesh().f_distr;
    auto& f_distr_next = dom->get_mesh().f_distr_next;
    auto& is_solid = dom->get_mesh().is_solid;
    auto& vel_mesh = dom->get_mesh().vel_mesh;
    
    //xt::xarray<double> f_distr_masked = xt::view(f_distr,)
    int nx = dom->get_mesh().nx;
    int ny = dom->get_mesh().ny;
    
    //f_distr_next = f_distr;
    for (int k = 1; k < 10; ++k){
        // all nodes except inlet-outlet nodes
        xt::view(f_distr_next, xt::range(next_i(1,k), next_i((nx-2),k)+1),
                                xt::range(next_j(1,k), next_j((ny-2),k)+1),
                                k-1) = xt::view(f_distr, xt::range(1, nx-1), xt::range(1,ny-1), k-1);
        // inlet-outlet nodes
        xt::view(f_distr_next, next_i(0,k),
                                xt::range(next_j(1,k), next_j((ny-2),k)+1),
                                k-1) = xt::view(f_distr, 0, xt::range(1,ny-1), k-1);
        xt::view(f_distr_next, next_i(nx-1,k),
                                xt::range(next_j(1,k), next_j((ny-2),k)+1),
                                k-1) = xt::view(f_distr, nx-1, xt::range(1,ny-1), k-1);        
    }
};

void solver::_apply_next_step_f_distr(){
    auto& f_distr = dom->get_mesh().f_distr;
    auto& f_distr_next = dom->get_mesh().f_distr_next;    
    f_distr = f_distr_next;

}

void solver::_dense_eval(){
    auto& rho_mesh = dom->get_mesh().rho_mesh;
    auto& f_distr = dom->get_mesh().f_distr;
    rho_mesh = xt::sum(f_distr, 2);
};

void solver::_vel_eval(){
    auto& vel_mesh = dom->get_mesh().vel_mesh;
    auto& f_distr = dom->get_mesh().f_distr;

    xt::view(vel_mesh, xt::all(), xt::all(), 0)
        = (xt::view(f_distr, xt::all(), xt::all(), 4)
        + xt::view(f_distr, xt::all(), xt::all(), 0)
        + xt::view(f_distr, xt::all(), xt::all(), 7)
        - xt::view(f_distr, xt::all(), xt::all(), 5)
        - xt::view(f_distr, xt::all(), xt::all(), 2)
        - xt::view(f_distr, xt::all(), xt::all(), 6));

    xt::view(vel_mesh, xt::all(), xt::all(), 1)
        = (xt::view(f_distr, xt::all(), xt::all(), 5)
        + xt::view(f_distr, xt::all(), xt::all(), 1)
        + xt::view(f_distr, xt::all(), xt::all(), 4)
        - xt::view(f_distr, xt::all(), xt::all(), 7)
        - xt::view(f_distr, xt::all(), xt::all(), 3)
        - xt::view(f_distr, xt::all(), xt::all(), 6));
};

void solver::_collision_step(){
    f_eq_prepare();
    dom->get_mesh().f_distr = f_eq;
};

void solver::_streaming_step(){

    // streaming
    _bounce_back();
    _transport_to_neighbours();
    _apply_next_step_f_distr();
    _bounce_back();
    // density and velocity evaluation
    _dense_eval();
    _vel_eval();
};

void solver::attatch_transmitter(std::shared_ptr<data_transmitter> transmitter){
    this->transmitter = transmitter;
    transmitter->attatch_domain(dom);
    transmitter->set_n_of_cycles(uint64_t(timesteps / check_period));
    transmitter->accept_connection();
};

void solver::solve() {
    dom->mark_border_nodes();
    dom->init_phys_field();
    init_f_distr();
    for (uint ts = 0; ts < timesteps; ++ts){
        if (ts % check_period == 0){
            std::cout << "Timestep " << ts << std::endl;
            if (transmitter)
                transmitter->send_data();
        }
        _streaming_step();
        _collision_step();
    }
    if (transmitter)
        transmitter->close_connection();
};