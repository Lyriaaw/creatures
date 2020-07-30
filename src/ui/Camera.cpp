//
// Created by Amalric Lombard de Buffières on 7/27/20.
//

#include <iostream>
#include "Camera.h"
#include "../World.h"


const Point &Camera::getCenter() const {
    return center;
}

void Camera::setCenter(const Point &center) {
    Camera::center = center;
}

const Point &Camera::getTopLeft() const {
    return topLeft;
}

void Camera::setTopLeft(const Point &topLeft) {
    Camera::topLeft = topLeft;
}

float Camera::getZoom() const {
    return zoom;
}

void Camera::setZoom(float zoom) {
    Camera::zoom = zoom;
}

int Camera::getWidth() const {
    return width;
}

void Camera::setWidth(int width) {
    Camera::width = width;
}

int Camera::getHeight() const {
    return height;
}

void Camera::setHeight(int height) {
    Camera::height = height;
}

void Camera::changeZoom(float ratio) {
    zoom *= ratio;
}

void Camera::move(float x, float y) {
    center.setX(center.getX() + (x / zoom));
    center.setY(center.getY() + (y / zoom));
}

Point Camera::getWorldCoordinates(Point point) {
    float x = (topLeft.getX() + (float(width) / 2.0f)) + ((point.getX() - center.getX()) * zoom);
    float y = (topLeft.getY() + (float(height) / 2.0f)) + ((point.getY() - center.getY()) * zoom);


    std::cout << "Inputs => X:" << point.getX() << " Y: " << point.getY() << " | Outputs => X:" << x << " Y: " << y << std::endl;

    return {x, y};
}



Camera::Camera(Point center, Point topLeft) : center(center), topLeft(topLeft) {
}
