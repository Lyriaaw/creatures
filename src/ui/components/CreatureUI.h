//
// Created by Amalric Lombard de Buffières on 8/1/20.
//

#ifndef CREATURES_CREATUREUI_H
#define CREATURES_CREATUREUI_H


#include "EntityUI.h"
#include "../../farm/entities/Creature.h"

class CreatureUI: public EntityUI {
private:
    sf::VertexArray sensors;
    sf::VertexArray energy;
    Creature * creature;

    sf::Text energyLabel;

public:
    CreatureUI(Creature *entity, sf::Font *font);

    void draw(sf::RenderWindow *window, Camera *camera, Entity * selectedEntity);
};


#endif //CREATURES_CREATUREUI_H
