//
// Created by Amalric Lombard de Buffières on 7/16/20.
//

#include "Entity.h"
#include <random>
#include <iostream>
#include <math.h>

#include "World.h"
#include "ui/Camera.h"
#include "colors/RGBColor.h"

#include "Utils.h"

Entity::Entity(Point point, float size): point(point) {
    this->size = size;
    this->vertexArray = sf::VertexArray(sf::Quads, 4);


    float hue = (rand() % 1000) / 1000.f;
    std::cout << "Color : " << hue << std::endl;
    RGBColor color = hslToRgb(hue, 1.f, 0.5f);

    this->color = sf::Color(color.getRed(), color.getGreen(), color.getBlue());
}

void Entity::draw(sf::RenderWindow *window, Camera *camera) {

    Point screenPoint = camera->getScreenCoordinates(this->point);

    float screenSize = this->size * camera->getZoom();

    for (int it = 0; it < 4; it++) {
        double angle = ((2 * M_PI) * (it / 4.0)) - (0.25 * M_PI) + this->rotation;

        float currentX = screenPoint.getX() + (screenSize * cos(angle));
        float currentY = screenPoint.getY() + (screenSize * sin(angle));
        vertexArray[it] = sf::Vector2f(currentX, currentY);
        vertexArray[it].color = this->color;
    }

    window->draw(vertexArray);

}



void Entity::move() {


//    bool x = dist(mt) < 0;
    bool x = true;
    float addedSpeed = (float(rand() % 21) - 10.f) / 10.f;
    float addedRotation = (float(rand() % 21) - 10.f) / 100.f;

    this->rotation += addedRotation;
    this->speed += addedSpeed;

    if (this->speed > 5) {
        this->speed = 5;
    }
    if (this->speed < -2.5f) {
        this->speed = -2.5f;
    }



    this->point.setX(this->point.getX() + (this->speed * cos(this->rotation)));
    this->point.setY(this->point.getY() + (this->speed * sin(this->rotation)));

    if (this->point.getX() < 0) {
        this->point.setX(FARM_WIDTH);
    }
    if (this->point.getX() > FARM_WIDTH) {
        this->point.setX(0);
    }

    if (this->point.getY() < 0) {
        this->point.setY(FARM_HEIGHT);
    }
    if (this->point.getY() > FARM_HEIGHT) {
        this->point.setY(0);
    }
}