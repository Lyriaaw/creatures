//
// Created by Amalric Lombard de Buffières on 10/17/20.
//

#ifndef CREATURES_EVOLUTIONSCREEN_H
#define CREATURES_EVOLUTIONSCREEN_H


#include "Screen.h"
#include "../components/Button.h"

class EvolutionScreen: public Screen {
private:
    int windowWidth, windowHeight;

    std::vector<Button *> graphButtons;


public:
    EvolutionScreen(Farm *farm, sf::Font * font);

    int getId() override;

    void init() override;

    Camera *open() override;

    void draw(sf::RenderWindow *window) override;

    void updateSelectedCreature(Life *connector) override;

    void onWindowResize(int width, int height) override;

    void mouseMoved(int x, int y) override;

    void loadGraphs();
    void loadButtons(sf::Font *font);

    void mouseClicked(int x, int y) override;

    void clickedOnButton(int id);

};


#endif //CREATURES_EVOLUTIONSCREEN_H
