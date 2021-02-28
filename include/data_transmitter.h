#pragma once
#include <boost/asio.hpp>
#include <memory>
#include "geometry/domain.h"

using boost::asio::ip::tcp;

class data_transmitter {
    
    boost::asio::io_context io_context;
    tcp::acceptor acceptor;
    tcp::socket socket;
    std::shared_ptr<domain> dom;
    std::vector<uint32_t> shape;

    uint64_t n_of_cycles;

public:

    data_transmitter(int socket_port): acceptor(io_context, tcp::endpoint(tcp::v4(), socket_port)), 
                                       socket(io_context) {};
    ~data_transmitter() {};

    void set_n_of_cycles(uint64_t n) { n_of_cycles = n; };

    void attatch_domain(std::shared_ptr<domain> dom);
    void accept_connection();
    void send_data();
    void close_connection();
};