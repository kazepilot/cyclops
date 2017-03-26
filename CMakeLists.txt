cmake_minimum_required(VERSION 2.8)
project (cyclops)

find_package(Tcmalloc)
find_package(Threads)
find_package(OpenSSL)
find_package(OpenCV REQUIRED)
find_package(CUDA REQUIRED)

if(OPENSSL_FOUND)
  include_directories(${OPENSSL_INCLUDE_DIR})
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

if (MSVC)
    set(Boost_USE_STATIC_LIBS "On")
    find_package( Boost 1.52 COMPONENTS system thread regex REQUIRED )
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++1y -pedantic -Wextra")
    find_package( Boost 1.52 COMPONENTS system thread REQUIRED )
endif()

set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

set(SRC_DIR src)
FILE(GLOB SRC_FILES_CPP ${SRC_DIR}/*.cpp)

include_directories("${PROJECT_INCLUDE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}")
include_directories( ${Boost_INCLUDE_DIR} )

include_directories(/usr/local/include)
link_directories(/usr/local/lib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -fpermissive -DGPU -L/usr/local/cuda/lib/ -lcurand -L/usr/lib -I/usr/include")

set(CMAKE_BUILD_TYPE "Release")

if(CUDA_FOUND)
  INCLUDE_DIRECTORIES(${CUDA_INCLUDE_DIRS})
  set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS}; -O3 -DGPU -std=c++11 -gencode arch=compute_52,code=sm_52 -D_FORCE_INLINES)
  add_definitions(-DGPU_OPENCV_ENABLE)

  SET(CUDA_LIB_DEPS
    ${CUDA_LIBRARIES}
    cyclops
  )
endif()

add_executable(cyclops ${SRC_FILES_CPP})

target_link_libraries(
    cyclops
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
    ${OpenCV_LIBRARIES}
    darknet
    pthread
    stdc++
    cuda
    cudart
    cublas
    curand
    cudnn
)