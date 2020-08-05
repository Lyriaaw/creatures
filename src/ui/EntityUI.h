//
// Created by Amalric Lombard de Buffières on 7/31/20.
//

#ifndef CREATURES_ENTITYUI_H
#define CREATURES_ENTITYUI_H


#include "../farm/Entity.h"
#include <SFML/Graphics.hpp>



class EntityUI {
protected:
    Entity *entity;

    sf::VertexArray vertexArray;
    sf::Color color;


public:
    EntityUI(Entity *entity, int vertexCount, sf::PrimitiveType type);

    ~ EntityUI();

    virtual void draw(sf::RenderWindow *window, Camera *camera, Entity * selectedEntity) = 0;

    Entity *getEntity() const;


};


#endif //CREATURES_ENTITYUI_H
