# Lattice-Bolzmann-Simulator
2D fluid simulator based on Lattice Bolzmann Method with matplotlib visualization

# Installation
## Pre-requirements

You should have g++, python3 and CMake 3.13.0 installed

## Pre-build

Install conan:
```
pip3 install conan
conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan
conan profile new default --detect
conan profile update settings.compiler.libcxx=libstdc++11 default
conan profile update settings.cppstd=17 default
```

## Build

```
mkdir build && cd build
conan install ../recipies/connector --build=missing
conan create ../recipies/connector local/stable

conan install .. --build=missing
cmake .. 
cmake --build . --config Release
```

# User guide

Simulation configuration is set by configuration file ```config.txt```.
Configuration format is following:
```
DOMAIN: <x_size> <y_size> <nodes_along_x_axis> <nodes_along_y_axis> <external_force_accelleration>
[CYLINDER: <x_center_coord> <y_center_coord> <radius>]
[RECTANCLE: <x_left_top_corner> <y_left_top_corner> <width> <height>]
TIMESTEPS: <number_of_algorithm_steps>
CHECK_STEP: <number_of_steps_per_animation_frame>
```
Strings in [] can appear any number of time.

To start simulation enter ```python3 simulation.py```.
