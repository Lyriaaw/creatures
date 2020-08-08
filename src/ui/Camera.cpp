//
// Created by Amalric Lombard de Buffières on 7/27/20.
//

#include "Camera.h"


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
    center.setX(center.getX() + (x));
    center.setY(center.getY() + (y));
}

Point Camera::getScreenCoordinates(Point point) {
    float x = (topLeft.getX() + (float(width ) / 2.0f)) + ((point.getX() -  center.getX()) * zoom);
    float y = (topLeft.getY() + (float(height) / 2.0f)) + ((point.getY() - center.getY()) * zoom);

    return {x, y};
}

Point Camera::getWorldCoordinates(Point result) {

    float pointX = center.getX() - (((topLeft.getX() + (float(width ) / 2.0f)) - result.getX()) / zoom);
    float pointY = center.getY() - (((topLeft.getY() + (float(height) / 2.0f)) - result.getY()) / zoom);

    return {pointX, pointY};
}



void Camera::switchGrid() {
    showGrid = !showGrid;
}



Camera::Camera(Point center, Point topLeft) : center(center), topLeft(topLeft), showGrid(false), width(0), height(0) {
}

bool Camera::isShowGrid() const {
    return showGrid;
}

void Camera::setShowGrid(bool showGrid) {
    Camera::showGrid = showGrid;
}
