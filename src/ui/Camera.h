//
// Created by Amalric Lombard de Buffières on 7/27/20.
//

#ifndef CREATURES_CAMERA_H
#define CREATURES_CAMERA_H


#include "../Point.h"

class Camera {
    // Center of the camera in the world
    Point center;

    // Top left of the image on the screen
    Point topLeft;

    // Width and height, in pixel, on the screen
    int width;
    int height;

    float zoom;

public:
    Camera(Point center, Point topLeft);

    int getWidth() const;

    void setWidth(int width);

    int getHeight() const;

    void setHeight(int height);

    const Point &getCenter() const;

    void setCenter(const Point &center);

    const Point &getTopLeft() const;

    void setTopLeft(const Point &topLeft);

    float getZoom() const;

    void setZoom(float zoom);


    void changeZoom(float ratio);

    void move(float x, float y);




    Point getWorldCoordinates(Point point);
};


#endif //CREATURES_CAMERA_H
