//
// Created by Amalric Lombard de Buffières on 7/16/20.
//

#ifndef CREATURES_POINT_H
#define CREATURES_POINT_H



class Point {
public:
    Point(float x, float y);


    float getX() const;

    void setX(float x);

    float getY() const;

    void setY(float y);


private:
    float x;
    float y;
};


#endif //CREATURES_POINT_H
