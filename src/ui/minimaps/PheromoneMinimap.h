//
// Created by Amalric Lombard de Buffières on 10/20/20.
//

#ifndef CREATURES_PHEROMONEMINIMAP_H
#define CREATURES_PHEROMONEMINIMAP_H


#include "Minimap.h"

class PheromoneMinimap: public Minimap {
public:

    PheromoneMinimap();

    PheromoneMinimap(double pixelSize, double positionX, double positionY);
    std::string getName() override;

    void setPixelColor(int tileX, int tileY, Farm * farm) override;
    void draw(sf::RenderWindow *window) override;

    void generateValues(Farm *farm) override;

};


#endif //CREATURES_PHEROMONEMINIMAP_H
