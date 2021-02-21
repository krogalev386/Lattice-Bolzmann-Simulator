#include "../include/data_transmitter.h"

void data_transmitter::attatch_domain(std::shared_ptr<domain> dom){
    this->dom = dom;
    shape.push_back(dom->get_mesh().rho_mesh.shape()[0]);
    shape.push_back(dom->get_mesh().rho_mesh.shape()[1]);
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
    //std::vector<int> shape;
    //shape.push_back(rho_mesh.shape()[0]);
    //shape.push_back(rho_mesh.shape()[1]);
    boost::asio::write(socket, boost::asio::buffer(shape.data(),2) );//,boost::asio::transfer_all(), ignored_error); 
    std::cout << "Size info transmitted" << std::endl;

    // Send arrays
    boost::asio::write(socket, boost::asio::buffer(rho_mesh.data(), rho_mesh.size()));
    //                    boost::asio::transfer_all(), ignored_error); 
    boost::asio::write(socket, boost::asio::buffer(vel_mesh.data(), vel_mesh.size()));
    //                    boost::asio::transfer_all(), ignored_error);
    boost::asio::write(socket, boost::asio::buffer(solid_map.data(), solid_map.size()));
    //                    boost::asio::transfer_all(), ignored_error); 
    std::cout << "Mesh content transmitted" << std::endl;

};