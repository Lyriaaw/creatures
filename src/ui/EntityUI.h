//
// Created by Amalric Lombard de Buffières on 7/31/20.
//

#ifndef CREATURES_ENTITYUI_H
#define CREATURES_ENTITYUI_H


#include "../farm/Entity.h"
#include <SFML/Graphics.hpp>


class EntityUI {
private:
    Entity *entity;

    sf::VertexArray vertexArray;
    sf::Color color;


public:
    EntityUI(Entity *entity);

    void draw(sf::RenderWindow *window, Camera *camera);


};


#endif //CREATURES_ENTITYUI_H
