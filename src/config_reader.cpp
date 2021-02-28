#include "../include/config_reader.h"

std::shared_ptr<domain> config_reader::build_domain(std::string config_file_name){
    
    int x, y, nx, ny, timesteps;
    double add_vel;

    std::vector<figure*> figures;

    std::fstream config_file;
    std::string curr_str, tmp;
    std::stringstream ss;

    config_file.open(config_file_name, std::ios::in);
    while(std::getline(config_file, curr_str)){
        ss = std::stringstream(curr_str);
        ss >> tmp;
        if (tmp == "DOMAIN:"){
            ss >> tmp; x = std::stod(tmp);
            ss >> tmp; y = std::stod(tmp);
            ss >> tmp; nx = std::stoi(tmp);
            ss >> tmp; ny = std::stoi(tmp);
            ss >> tmp; add_vel = std::stod(tmp);
        } else if (tmp == "CYLINDER:"){
            std::cout << "Cylind Check" << std::endl;
            ss >> tmp; double cyl_x = std::stod(tmp);
            ss >> tmp; double cyl_y = std::stod(tmp);
            ss >> tmp; double cyl_r = std::stod(tmp);
            figures.push_back(new cylinder(cyl_x,cyl_y,cyl_r));
        } else if (tmp == "RECTANGLE:"){
            ss >> tmp; double rec_x = std::stod(tmp);
            ss >> tmp; double rec_y = std::stod(tmp);
            ss >> tmp; double rec_dx = std::stod(tmp);
            ss >> tmp; double rec_dy = std::stod(tmp);
            figures.push_back(new rectangle(rec_x,rec_y,rec_dx,rec_dy));
        }
    }
    config_file.close();

    std::shared_ptr<domain> dom = std::make_shared<domain>(0,add_vel,x,y,nx,ny);
    for (auto fig: figures){
        fig->insert_into_domain(dom);
    }

    return dom;
};

int config_reader::get_timesteps(std::string config_file_name){
    int timesteps;

    std::fstream config_file;
    std::string curr_str, tmp;
    std::stringstream ss;

    config_file.open(config_file_name, std::ios::in);
    while(std::getline(config_file, curr_str)){
        ss = std::stringstream(curr_str);
        ss >> tmp;
        if (tmp == "TIMESTEPS:"){
            ss >> tmp; timesteps = std::stoi(tmp);
        }
    }
    return timesteps;
}

int config_reader::get_check_step(std::string config_file_name){
    int check_step;

    std::fstream config_file;
    std::string curr_str, tmp;
    std::stringstream ss;

    config_file.open(config_file_name, std::ios::in);
    while(std::getline(config_file, curr_str)){
        ss = std::stringstream(curr_str);
        ss >> tmp;
        if (tmp == "CHECK_STEP:"){
            ss >> tmp; check_step = std::stoi(tmp);
        }
    }
    return check_step;
}

