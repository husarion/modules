get_filename_component(CURRENT_LIB_DIR2 ${CMAKE_CURRENT_LIST_FILE} PATH) # for cmake before 2.8.3

list(APPEND srcs "${CURRENT_LIB_DIR2}/MPU9250.cpp")
list(APPEND srcs "${CURRENT_LIB_DIR2}/MPU9250/mpudmp.cpp")
list(APPEND srcs "${CURRENT_LIB_DIR2}/MPU9250/mpudmp_6axis.cpp")

add_definitions(-DMPU9250_MULTIPLE_INSTANCES)
add_definitions(-DMPU9250_CLASSNAME=MPU9250Class)
