cmake_minimum_required(VERSION 3.0.0)
project(GPIO VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB SRC
    "src/*.h"
    "src/*.cpp"
)

include(CTest)
enable_testing()

add_executable(GPIO main.cpp ${SRC})

find_package(Threads)

target_link_libraries(
    GPIO
    ${CMAKE_THREAD_LIBS_INIT}
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
