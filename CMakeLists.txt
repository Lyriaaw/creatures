cmake_minimum_required(VERSION 3.16)
project(Creatures)

set(CMAKE_CXX_STANDARD 14)

include_directories(/Library/Frameworks/SDL2.framework/Headers)
include_directories(/Library/Frameworks/SDL2.framework/)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_DIRS}/include)


add_executable(Creatures main.cpp)
target_link_libraries(Creatures ${SDL2_LIBRARIES})