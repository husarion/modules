set(CMAKE_SYSTEM_NAME "Generic")
get_filename_component(CURRENT_LIB_DIR ${CMAKE_CURRENT_LIST_FILE} PATH) # for cmake before 2.8.3

include_directories("${CURRENT_LIB_DIR}/MPU9250/api/include/")
include_directories("${CURRENT_LIB_DIR}/DistanceSensor/api/include/")

# MPU9250
include("${CURRENT_LIB_DIR}/MPU9250/api/src/library.cmake")

FILE(GLOB tmp "${CURRENT_LIB_DIR}/DistanceSensor/api/src/*.cpp")
list(APPEND srcs ${tmp})
set(MODULES_SOURCES "${srcs}")
