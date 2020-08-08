cmake_minimum_required(VERSION 3.16)
project(Creatures)

set(CMAKE_CXX_STANDARD 14)

set(SFML_INCLUDE_DIR "./SFML/include")
set(SFML_LIBRARY_DIR "./SFML/build/lib")


link_directories(SFML_LIBRARY_DIR)
include_directories(SFML_INCLUDE_DIR)

find_package(SFML 2.5.1 COMPONENTS system window graphics network audio)

add_executable(Creatures main.cpp src/farm/Entity.cpp src/farm/Entity.h src/utils/Point.cpp src/utils/Point.h src/farm/Farm.cpp src/farm/Farm.h src/farm/Map.cpp src/farm/Map.h src/ui/MainWindow.cpp src/ui/MainWindow.h src/ui/FarmUI.cpp src/ui/FarmUI.h src/World.h src/ui/Camera.cpp src/ui/Camera.h src/farm/perlin/PerlinNoise.cpp src/farm/perlin/PerlinNoise.h src/ui/colors/ColorUtils.h src/ui/colors/RGBColor.cpp src/ui/colors/RGBColor.h src/ui/EntityUI.cpp src/ui/EntityUI.h src/farm/Creature.cpp src/farm/Creature.h src/ui/CreatureUI.cpp src/ui/CreatureUI.h src/farm/Food.cpp src/farm/Food.h src/ui/FoodUI.cpp src/ui/FoodUI.h src/farm/brain/Brain.cpp src/farm/brain/Brain.h src/farm/brain/neurons/Neuron.cpp src/farm/brain/neurons/Neuron.h src/farm/brain/neurons/InputNeuron.cpp src/farm/brain/neurons/InputNeuron.h src/farm/brain/CreatureNursery.cpp src/farm/brain/CreatureNursery.h src/farm/brain/evolutions/Evolution.cpp src/farm/brain/evolutions/Evolution.h src/farm/brain/evolutions/SensorEvolution.cpp src/farm/brain/evolutions/SensorEvolution.h src/farm/brain/evolutions/EvolutionLibrary.cpp src/farm/brain/evolutions/EvolutionLibrary.h src/farm/brain/neurons/SensorNeuron.cpp src/farm/brain/neurons/SensorNeuron.h src/farm/brain/BrainConnector.cpp src/farm/brain/BrainConnector.h src/ui/BrainUI.cpp src/ui/BrainUI.h src/farm/brain/neurons/OutputNeuron.cpp src/farm/brain/neurons/OutputNeuron.h src/farm/brain/Link.cpp src/farm/brain/Link.h src/farm/brain/neurons/BiasNeuron.cpp src/farm/brain/neurons/BiasNeuron.h src/farm/brain/evolutions/BiasInputEvolution.cpp src/farm/brain/evolutions/BiasInputEvolution.h src/farm/brain/evolutions/SpeedEvolution.cpp src/farm/brain/evolutions/SpeedEvolution.h src/farm/brain/evolutions/RotationEvolution.cpp src/farm/brain/evolutions/RotationEvolution.h src/farm/brain/evolutions/LinkEvolution.cpp src/farm/brain/evolutions/LinkEvolution.h src/farm/brain/evolutions/ColorEvolution.cpp src/farm/brain/evolutions/ColorEvolution.h src/farm/brain/evolutions/SizeEvolution.cpp src/farm/brain/evolutions/SizeEvolution.h src/farm/brain/evolutions/MouthEvolution.cpp src/farm/brain/evolutions/MouthEvolution.h src/farm/brain/evolutions/GenitalsEvolution.cpp src/farm/brain/evolutions/GenitalsEvolution.h src/farm/brain/muscles/Muscle.cpp src/farm/brain/muscles/Muscle.h src/farm/Action.cpp src/farm/Action.h src/farm/brain/ActionDTO.cpp src/farm/brain/ActionDTO.h src/farm/brain/evolutions/EnergySensorEvolution.cpp src/farm/brain/evolutions/EnergySensorEvolution.h src/farm/brain/neurons/EnergySensorNeuron.cpp src/farm/brain/neurons/EnergySensorNeuron.h src/farm/statistics/DataAnalyser.cpp src/farm/statistics/DataAnalyser.h src/farm/statistics/DataItem.cpp src/farm/statistics/DataItem.h src/ui/dashboards/Graph.cpp src/ui/dashboards/Graph.h src/ui/dashboards/DataItemConnector.cpp src/ui/dashboards/DataItemConnector.h src/ui/views/Screen.cpp src/ui/views/Screen.h src/ui/views/WorldScreen.cpp src/ui/views/WorldScreen.h src/ui/Button.cpp src/ui/Button.h src/ui/views/StatisticsScreen.cpp src/ui/views/StatisticsScreen.h src/ui/views/MinimapsScreen.cpp src/ui/views/MinimapsScreen.h src/ui/minimaps/Minimap.cpp src/ui/minimaps/Minimap.h src/ui/minimaps/CreatureTileCountMinimap.cpp src/ui/minimaps/CreatureTileCountMinimap.h src/ui/minimaps/FoodTileCountMinimap.cpp src/ui/minimaps/FoodTileCountMinimap.h)

target_link_libraries (Creatures sfml-graphics sfml-window sfml-system)



# https://medium.com/@remy.villulles/compile-and-install-sfml-2-5-1-on-macos-with-clion-and-cmake-b4e52199db1c