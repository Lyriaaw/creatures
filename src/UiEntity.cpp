//
// Created by Amalric Lombard de Buffières on 7/31/20.
//

#include <iostream>
#include <math.h>
#include "Entity.h"
#include "Point.h"
#include "ui/Camera.h"
#include "colors/RGBColor.h"
#include "Utils.h"


UiEntity::UiEntity() {
    this->body = sf::VertexArray(sf::Quads, 4);
}

void UiEntity::initialize(float colorHue) {
    RGBColor rgbColor = hslToRgb(colorHue, 1.f, 0.5f);

    color = sf::Color(rgbColor.getRed(), rgbColor.getGreen(), rgbColor.getBlue(), 255);
}


void UiEntity::draw(sf::RenderWindow *window, Camera *camera, Point position, float size, float rotation, float hue) {

    Point screenPoint = camera->getScreenCoordinates(position);

    float screenSize = size * camera->getZoom();



    for (int it = 0; it < 4; it++) {
        float angle = ((2.f * float(M_PI)) * (float(it) / 4.0f)) - (0.25f * M_PI) + rotation;

        float currentX = screenPoint.getX() + (screenSize * cos(angle));
        float currentY = screenPoint.getY() + (screenSize * sin(angle));
        body[it] = sf::Vector2f(currentX, currentY);
        body[it].color = color;
    }

    window->draw(body);

}


