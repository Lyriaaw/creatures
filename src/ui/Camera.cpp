//
// Created by Amalric Lombard de Buffières on 7/27/20.
//

#include <cmath>
#include <algorithm>
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

float Camera::fixPointCoordinate(float value, float maxValue, float totalValue) {
    if (value < 0) {
        value += totalValue * zoom;
    }

    if (value > maxValue) {
        value -= totalValue * zoom;
    }

    return value;
}

Point Camera::getScreenCoordinates(Point point) {
    Point flatEarthPoint = getFlatEarthScreenCoordinates(point);

    if (shouldDisplayPoint(flatEarthPoint)) {
        return flatEarthPoint;
    }

    float bottomRightX = (topLeft.getX() + float(width ));
    float bottomRightY = (topLeft.getY() + float(height));

    float x = fixPointCoordinate(flatEarthPoint.getX(), bottomRightX, FARM_WIDTH);
    float y = fixPointCoordinate(flatEarthPoint.getY(), bottomRightY, FARM_HEIGHT);

    Point newPoint = Point(x, y);

    if (shouldDisplayPoint(newPoint)) {
        return newPoint;
    }

    float distanceNewPointTopLeftX = abs(topLeft.getX() - newPoint.getX());
    float distanceNewPointTopLeftY = abs(topLeft.getY() - newPoint.getY());

    float distanceNewPointBottomRightX = abs(bottomRightX - newPoint.getX());
    float distanceNewPointBottomRightY = abs(bottomRightY - newPoint.getY());

    float smallestNewX = std::min(distanceNewPointTopLeftX, distanceNewPointBottomRightX);
    float smallestNewY = std::min(distanceNewPointTopLeftY, distanceNewPointBottomRightY);


    float distanceFlatEarthPointTopLeftX = abs(topLeft.getX() - flatEarthPoint.getX());
    float distanceFlatEarthPointTopLeftY = abs(topLeft.getY() - flatEarthPoint.getY());

    float distanceFlatEarthPointBottomRightX = abs(bottomRightX - flatEarthPoint.getX());
    float distanceFlatEarthPointBottomRightY = abs(bottomRightY - flatEarthPoint.getY());

    float smallestFlatEarthX = std::min(distanceFlatEarthPointTopLeftX, distanceFlatEarthPointBottomRightX);
    float smallestFlatEarthY = std::min(distanceFlatEarthPointTopLeftY, distanceFlatEarthPointBottomRightY);


    float selectedX = 0;

    if (smallestFlatEarthX < smallestNewX) {
        selectedX = flatEarthPoint.getX();
    } else {
        selectedX = newPoint.getX();
    }

    float selectedY = 0;

    if (smallestFlatEarthY < smallestNewY) {
        selectedY = flatEarthPoint.getY();
    } else {
        selectedY = newPoint.getY();
    }

    return Point(selectedX, selectedY);

}

Point Camera::getFlatEarthScreenCoordinates(Point point) {
    float cameraScreenCenterX = (topLeft.getX() + (float(width ) / 2.0f));
    float cameraScreenCenterY = (topLeft.getY() + (float(height) / 2.0f));

    float worldDistanceBetweenCameraCenterAndPointX = point.getX() -  center.getX();
    float worldDistanceBetweenCameraCenterAndPointY = point.getY() -  center.getY();

    float x = cameraScreenCenterX + (worldDistanceBetweenCameraCenterAndPointX * zoom);
    float y = cameraScreenCenterY + (worldDistanceBetweenCameraCenterAndPointY * zoom);

    return {x, y};
}

Point Camera::getWorldCoordinates(Point result) {

    float pointX = center.getX() - (((topLeft.getX() + (float(width ) / 2.0f)) - result.getX()) / zoom);
    float pointY = center.getY() - (((topLeft.getY() + (float(height) / 2.0f)) - result.getY()) / zoom);

    return {pointX, pointY};
}

bool Camera::shouldDisplayPoint(Point point) {
    float bottomRightX = topLeft.getX() + width;
    float bottomRightY = topLeft.getY() + height;


    if (point.getX() < topLeft.getX() || point.getX() > bottomRightX) {
        return false;
    }

    if (point.getY() < topLeft.getY() || point.getY() > bottomRightY) {
        return false;
    }

    return true;
}



void Camera::switchGrid() {
    showGrid = !showGrid;
}



Camera::Camera(Point center, Point topLeft) : center(center), topLeft(topLeft), showGrid(false) {
}

bool Camera::isShowGrid() const {
    return showGrid;
}

void Camera::setShowGrid(bool showGrid) {
    Camera::showGrid = showGrid;
}
