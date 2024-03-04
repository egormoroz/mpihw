cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 17)

project(mpi)

add_executable(1 1.cpp)
add_executable(2 2.cpp)
add_executable(3 3.cpp)
add_executable(4 4.cpp)
add_executable(5 5.cpp)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(1 PRIVATE Threads::Threads)

