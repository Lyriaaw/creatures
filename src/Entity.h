//
// Created by Amalric Lombard de Buffières on 7/16/20.
//

#ifndef CREATURES_ENTITY_H
#define CREATURES_ENTITY_H


#include "Point.h"
#include <SFML/Graphics.hpp>
#include <random>


class Entity {
public:
    Entity(Point point, float size);

    void draw(sf::RenderWindow *window);

private:
    Point point;
    float size;

    float rotation;
    float speed;

    sf::VertexArray vertexArray;
    sf::Color color;
public:
    void move();
};


#endif //CREATURES_ENTITY_H
