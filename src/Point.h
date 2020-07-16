//
// Created by Amalric Lombard de Buffières on 7/16/20.
//

#ifndef CREATURES_POINT_H
#define CREATURES_POINT_H



class Point {
public:
    Point(int x, int y);


    int getX() const;

    void setX(int x);

    int getY() const;

    void setY(int y);


private:
    int x;
    int y;
};


#endif //CREATURES_POINT_H
