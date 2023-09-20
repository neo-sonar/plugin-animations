cmake_minimum_required(VERSION 3.23)
project(sml)

add_library(sml INTERFACE)
add_library(boost::sml ALIAS sml)
target_include_directories(sml INTERFACE ${PROJECT_SOURCE_DIR})
