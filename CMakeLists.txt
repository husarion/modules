cmake_minimum_required(VERSION 2.6)
project(hModules NONE)

include(${HFRAMEWORK_PATH}/hFramework.cmake)

include_directories("include")

add_library(hModules
  src/DistanceSensor.cpp
  src/MPU9250/mpudmp.cpp
  src/MPU9250/mpudmp_6axis.cpp
  # src/MPU9250/mpudmp_9axis.cpp
  src/MPU9250.cpp)
