//
// Created by Amalric Lombard de Buffières on 7/31/20.
//

#include <iostream>
#include "EntityUI.h"
#include "colors/ColorUtils.h"

EntityUI::EntityUI(Entity *entity, int vertexCount, sf::PrimitiveType type) : entity(entity) {
    this->vertexArray = sf::VertexArray(sf::Quads, vertexCount);

    RGBColor color = hslToRgb(entity->getColor(), 1.f, 0.5f);
    this->color = sf::Color(color.getRed(), color.getGreen(), color.getBlue());
}

