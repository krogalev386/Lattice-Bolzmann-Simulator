#include "../include/data_transmitter.h"
#include <xtensor/xadapt.hpp>

namespace net = boost::asio;

template<class Vec>
static auto to_buf(const Vec &data){
    return net::buffer(data.data(), sizeof(typename Vec::value_type)*data.size());
}

template<class Sock, class Vec>
static auto send(Sock &socket, const Vec &data){
    auto len = (uint64_t)sizeof(typename Vec::value_type)*data.size();
    std::vector<uint64_t> len_vec{ std::move(len) };
    net::write(socket, to_buf(len_vec), net::transfer_all());
    net::write(socket, to_buf(data), net::transfer_all());
}

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

    // Send info about arrays
    net::write(socket, to_buf(shape), net::transfer_all());
    std::cout << "Size info transmitted" << std::endl;
    std::cout << "shape:" << xt::adapt(shape) << std::endl;

    // Send arrays
    send(socket, rho_mesh);
    std::cout << "rho shape:" << xt::adapt(rho_mesh.shape()) << std::endl;
    send(socket, vel_mesh);
    std::cout << "vel shape:" << xt::adapt(vel_mesh.shape()) << std::endl;
    send(socket, solid_map);
    std::cout << "solid shape:" << xt::adapt(solid_map.shape()) << std::endl;
    std::cout << "Mesh content transmitted" << std::endl;
};