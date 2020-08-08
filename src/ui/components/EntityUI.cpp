//
// Created by Amalric Lombard de Buffières on 7/31/20.
//

#include "EntityUI.h"
#include "../colors/RGBColor.h"
#include "../colors/ColorUtils.h"

EntityUI::~EntityUI() {

}

EntityUI::EntityUI(Entity *entity, int vertexCount, sf::PrimitiveType type) : entity(entity) {
    this->vertexArray = sf::VertexArray(type, vertexCount);

    RGBColor color = RGBColor(entity->getColor(), 1.f, entity->getBrightness());
    this->color = sf::Color(color.getRed(), color.getGreen(), color.getBlue());
}

Entity *EntityUI::getEntity() const {
    return entity;
}

