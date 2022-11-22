cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(example)

# Set source and include folder ###########################################
set(SRC_FOLDER "${CMAKE_CURRENT_LIST_DIR}/src")

# Include universal (posits) ##############################################
include_directories("~/Documents/libs/universal/include")

## Implement Parallelism
#find_package(OpenMP)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

# Sources
add_executable(gen_lut ${SRC_FOLDER}/gen_lut.cpp)
set_property(TARGET gen_lut PROPERTY CXX_STANDARD 17)

add_executable(gen_tb ${SRC_FOLDER}/gen_tb.cpp)
set_property(TARGET gen_tb PROPERTY CXX_STANDARD 17)

add_executable(gen_rand_tb ${SRC_FOLDER}/gen_rand_tb.cpp)
set_property(TARGET gen_rand_tb PROPERTY CXX_STANDARD 17)

add_executable(test_acc ${SRC_FOLDER}/test_acc.cpp)
set_property(TARGET test_acc PROPERTY CXX_STANDARD 17)

add_executable(test_acc_old ${SRC_FOLDER}/test_acc_old.cpp)
set_property(TARGET test_acc_old PROPERTY CXX_STANDARD 17)
