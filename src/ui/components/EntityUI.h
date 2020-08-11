//
// Created by Amalric Lombard de Buffières on 7/31/20.
//

#ifndef CREATURES_ENTITYUI_H
#define CREATURES_ENTITYUI_H


#include "../../farm/entities/Entity.h"
#include <SFML/Graphics.hpp>



class EntityUI {
protected:
    Entity *entity;

    sf::Font * font;

    sf::Color color;
    sf::VertexArray vertexArray;


public:
    EntityUI(Entity *entity, sf::Font * font);

    void draw(sf::RenderWindow *window, Camera *camera, Entity * selectedEntity);

    Entity *getEntity() const;


};


#endif //CREATURES_ENTITYUI_H
