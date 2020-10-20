//
// Created by Amalric Lombard de Buffières on 8/8/20.
//

#ifndef CREATURES_WORLDSCREEN_H
#define CREATURES_WORLDSCREEN_H


#include "Screen.h"
#include "../elements/UiBackground.h"

class WorldScreen: public Screen {
private:
    Camera *camera;
public:
    WorldScreen(FarmUI *farmUi);

    int getId() override;
    void init() override;

    void updateSelectedCreature() override;

    void loadCamera();

    void onWindowResize(int width, int height) override;

    void draw(sf::RenderWindow *window) override;

    void mouseMoved(int x, int y, int previousX, int previousY) override;

    void mouseClicked(int x, int y) override;

    void mouseScrolled(float delta, int mouseX, int mouseY) override;

    void handleKeyboardEvents(sf::Event::KeyEvent event) override;

};


#endif //CREATURES_WORLDSCREEN_H
