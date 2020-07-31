cmake_minimum_required(VERSION 3.16)
project(Creatures)

set(CMAKE_CXX_STANDARD 14)

set(SFML_INCLUDE_DIR "./SFML/include")
set(SFML_LIBRARY_DIR "./SFML/build/lib")


link_directories(SFML_LIBRARY_DIR)
include_directories(SFML_INCLUDE_DIR)

find_package(SFML 2.5.1 COMPONENTS system window graphics network audio)

add_executable(Creatures main.cpp src/farm/Entity.cpp src/farm/Entity.h src/utils/Point.cpp src/utils/Point.h src/farm/Farm.cpp src/farm/Farm.h src/farm/Map.cpp src/farm/Map.h src/ui/MainWindow.cpp src/ui/MainWindow.h src/ui/FarmUI.cpp src/ui/FarmUI.h src/World.h src/ui/Camera.cpp src/ui/Camera.h src/farm/perlin/PerlinNoise.cpp src/farm/perlin/PerlinNoise.h src/ui/colors/ColorUtils.h src/ui/colors/RGBColor.cpp src/ui/colors/RGBColor.h src/ui/EntityUI.cpp src/ui/EntityUI.h src/farm/Creature.cpp src/farm/Creature.h src/ui/CreatureUI.cpp src/ui/CreatureUI.h)

target_link_libraries (Creatures sfml-graphics sfml-window sfml-system)



# https://medium.com/@remy.villulles/compile-and-install-sfml-2-5-1-on-macos-with-clion-and-cmake-b4e52199db1c