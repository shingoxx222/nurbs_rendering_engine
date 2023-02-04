cmake_minimum_required(VERSION 3.0.0)
project(nurbs_rendering_engine VERSION 0.1.0)

find_package(CUDA)

set(OPTIX_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/ext/NVIDIA-OptiX-SDK-7.6.0-linux64-x86_64/include)

include(CTest)
enable_testing()

add_executable(${PROJECT_NAME} 
               src/main.cpp
               src/CIntersectionList.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/inc ${OPTIX_INCLUDE_DIR})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
