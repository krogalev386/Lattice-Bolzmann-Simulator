#include "../include/data_transmitter.h"
#include <xtensor/xadapt.hpp>

namespace net = boost::asio;

template<class Vec>
static auto to_buf(const Vec &data){
    return net::buffer(data.data(), sizeof(typename Vec::value_type)*data.size());
};

template<class Sock, class Vec>
static auto send(Sock &socket, const Vec &data){
    auto len = (uint64_t)sizeof(typename Vec::value_type)*data.size();
    std::vector<uint64_t> len_vec{ std::move(len) };
    net::write(socket, to_buf(len_vec), net::transfer_all());
    return net::write(socket, to_buf(data), net::transfer_all());
};

void data_transmitter::attatch_domain(std::shared_ptr<domain> dom){
    this->dom = dom;
    shape.emplace_back(dom->get_mesh().rho_mesh.shape().at(0));
    shape.emplace_back(dom->get_mesh().rho_mesh.shape().at(1));
};

void data_transmitter::accept_connection(){
    acceptor.accept(socket);
};

void data_transmitter::send_data(){

    // Init socket
    boost::system::error_code ignored_error;

    // Assign references
    xt::xarray<double>& vel_mesh = dom->get_mesh().vel_mesh;
    xt::xarray<double>& rho_mesh = dom->get_mesh().rho_mesh;
    xt::xarray<bool>& solid_map = dom->get_mesh().is_solid;

    char checker;
    // Send info about arrays
    std::size_t sh_error = send(socket, shape);
    net::read(socket, net::buffer(&checker,sizeof(char)), net::transfer_all());
    // Send arrays
    std::size_t rho_error = send(socket, rho_mesh);
    net::read(socket, net::buffer(&checker,sizeof(char)), net::transfer_all());
    std::size_t vel_error = send(socket, vel_mesh);
    net::read(socket, net::buffer(&checker,sizeof(char)), net::transfer_all());
    std::size_t sol_error = send(socket, solid_map);
    net::read(socket, net::buffer(&checker,sizeof(char)), net::transfer_all());

    std::cout << "Shape: " << sh_error << "; Rho: " << rho_error << "; Vel: " << vel_error << "; Sol: " << sol_error << std::endl;

};