cmake_minimum_required(VERSION 3.16)

# Set the extra component directory to share components from the tracker project
set(EXTRA_COMPONENT_DIRS "../components")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(ground_station)
