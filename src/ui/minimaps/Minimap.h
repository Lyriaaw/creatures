//
// Created by Amalric Lombard de Buffières on 8/8/20.
//

#ifndef CREATURES_MINIMAP_H
#define CREATURES_MINIMAP_H


#include <string>
#include <SFML/Graphics.hpp>
#include "../../farm/Farm.h"

class Minimap {
protected:
    double positionX, positionY, width, height;

    std::vector<std::vector<sf::RectangleShape>> tiles;
    double pixelSize;


public:

    Minimap(double pixelSize, double positionX, double positionY);
    void load();

    void move(double positionX, double positionY, double width, double height);
    void changeSize(double width, double height);

    virtual void draw(int tileX, int tileY, Farm * farm, sf::RenderWindow *window) = 0;

    virtual std::string getName() = 0;
};


class WorldMinimap: public Minimap {
public:
    WorldMinimap(double pixelSize, double positionX, double positionY);
    std::string getName() override;

    void draw(int tileX, int tileY, Farm * farm, sf::RenderWindow *window);

};




class CreatureCountMinimap: public Minimap {
public:
    CreatureCountMinimap(double pixelSize, double positionX, double positionY);
    std::string getName() override;

    void draw(int tileX, int tileY, Farm * farm, sf::RenderWindow *window);

};





#endif //CREATURES_MINIMAP_H
