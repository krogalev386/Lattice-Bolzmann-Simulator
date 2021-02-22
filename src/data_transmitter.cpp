#include "../include/data_transmitter.h"
#include <xtensor/xadapt.hpp>

namespace net = boost::asio;

template<class Vec>
static auto to_buf(const Vec &data, uint64_t buff_size, int offset){
    return net::buffer((data.data()+offset), buff_size);
}

template<class Sock, class Vec>
static auto send(Sock &socket, const Vec &data){
    uint64_t size_of_type = (uint64_t)sizeof(typename Vec::value_type); 
    auto len = size_of_type*data.size();
    std::vector<uint64_t> len_vec;

    uint64_t buff_size = 64*1024; // set buffer size
    uint64_t n_of_cycles = len / buff_size; // number of transfer iterations
    uint64_t n_of_rest_bytes = len % buff_size; // number of rest bytes to transfer after main cycle

    int step_offset = buff_size / size_of_type; // offset in data structure per cycle
    int data_rest = n_of_rest_bytes / size_of_type; // number of rest elements to transfer after cycle

    len_vec.push_back(std::move(buff_size));
    len_vec.push_back(std::move(n_of_cycles));
    len_vec.push_back(std::move(n_of_rest_bytes));

    net::write(socket, to_buf(len_vec, (uint64_t)sizeof(len_vec), 0), net::transfer_all());
    for (int i = 0; i < n_of_cycles; ++i){
        net::write(socket, to_buf(data, buff_size, i*step_offset), net::transfer_all());
    }
    net::write(socket, to_buf(data, n_of_rest_bytes, n_of_cycles*step_offset), net::transfer_all());
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
    send(socket, shape);
    // Send arrays
    send(socket, rho_mesh);
    send(socket, vel_mesh);
    send(socket, solid_map);
};