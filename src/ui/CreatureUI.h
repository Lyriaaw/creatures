//
// Created by Amalric Lombard de Buffières on 8/1/20.
//

#ifndef CREATURES_CREATUREUI_H
#define CREATURES_CREATUREUI_H


#include "EntityUI.h"
#include "../farm/Creature.h"

class CreatureUI: public EntityUI {
private:
    sf::VertexArray sensors;
    Creature * creature;

public:
    CreatureUI(Creature *entity);

    void draw(sf::RenderWindow *window, Camera *camera, Entity * selectedEntity);
};


#endif //CREATURES_CREATUREUI_H
