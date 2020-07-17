#include <iostream>


#include <tiff.h>
#include <random>
#include <array>
#include <ctime>

#include "src/Entity.h"
#include "src/ui/MainWindow.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace std::chrono;
using namespace sf;







int main(int argc, char **argv)
{
    srand((unsigned) time(0));

    MainWindow window = MainWindow();
    window.start();

    return 0;
}
