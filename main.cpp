#include <iostream>


#include <tiff.h>
#include <random>
#include <array>
#include <ctime>

#include "src/Entity.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace std::chrono;
using namespace sf;

int WINDOW_WIDTH = 1920;
int WINDOW_HEIGHT = 1080;





void runLoop(RenderWindow *window) {
    bool done(false);

    random_device rd;
    mt19937 mt(rd());
    uniform_real_distribution<double> distWidth(0, WINDOW_WIDTH);
    uniform_real_distribution<double> distHeight(0, WINDOW_HEIGHT);

    std::uniform_real_distribution<float> distMovement(-1, 1);

    auto start = chrono::system_clock::now();
    auto end = chrono::system_clock::now();

    vector<Entity> entities;


    for (int it = 0; it < 100; it++) {
        int x = distWidth(mt);
        int y = distHeight(mt);

        Point point(x, y);

        Entity entity(point, 20);
        entities.push_back(entity);

    }



    while (!done) {
        // check all the window's events that were triggered since the last iteration of the loop
        Event event;
        while (window->pollEvent(event))
        {
            switch (event.type) {
                case Event::Closed:
                    done = true;
                    break;
                case Event::Resized:
                    cout << "New Width: " << event.size.width << " height: " << event.size.height << endl;
                    break;
            }
        }

        for (int it = 0; it < entities.size(); it++) {
            entities.at(it).move();
        }


        window->clear(sf::Color::Black);


        for (int it = 0; it < entities.size(); it++) {
            entities.at(it).draw(window);
        }

        window->display();

        end = chrono::system_clock::now();
        chrono::duration<double> elapsed_time = end - start;
//        cout << "Time : " << elapsed_time.count() << "s" << endl;


        start = chrono::system_clock::now();
    }

    window->close();
}


int main(int argc, char **argv)
{
    srand((unsigned) time(0));
    RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Creatures");
    window.setVerticalSyncEnabled(true);

    runLoop(&window);
    return 0;
}
