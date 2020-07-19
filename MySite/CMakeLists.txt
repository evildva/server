cmake_minimum_required(VERSION 3.10)

# set the project name
project(Site)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# get all source files as src in .
aux_source_directory(. src)

add_compile_options(-g)

# print to terminate
message(${src})

# add the executable
add_executable(site ${src})

# add library
target_link_libraries(site pthread)
