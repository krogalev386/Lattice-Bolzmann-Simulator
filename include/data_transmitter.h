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
    std::vector<int> shape;

public:

    data_transmitter(int socket_port): acceptor(io_context, tcp::endpoint(tcp::v4(), socket_port)), 
                                       socket(io_context) {};
    ~data_transmitter() {};

    void attatch_domain(std::shared_ptr<domain> dom);
    void accept_connection();
    void send_data();
};