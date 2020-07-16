cmake_minimum_required(VERSION 3.16)
project(Creatures)

set(CMAKE_CXX_STANDARD 14)

include_directories(/Library/Frameworks/SDL2.framework/Headers)
include_directories(/Library/Frameworks/SDL2.framework/)

link_directories(/usr/local/lib)
include_directories(/usr/local/include)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_DIRS}/include)


add_executable(Creatures main.cpp src/Entity.cpp src/Entity.h src/Point.cpp src/Point.h)
target_link_libraries(Creatures ${SDL2_LIBRARIES})