//
// Created by Amalric Lombard de Buffières on 8/8/20.
//

#ifndef CREATURES_WORLDSCREEN_H
#define CREATURES_WORLDSCREEN_H


#include "Screen.h"

class WorldScreen: public Screen {
private:
    Camera *camera;
public:
    WorldScreen(FarmUI *farmUi);

    int getId() override;
    void init() override;

    void updateSelectedCreature(Life *connector) override;

    void loadCamera();

    void onWindowResize(int width, int height) override;

    void draw(sf::RenderWindow *window) override;

};


#endif //CREATURES_WORLDSCREEN_H
