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

    std::vector<Button *> evolutionButtons;

    sf::Text * currentGenerationNumberText;

    int lastEvolutionsCount;

    std::map<int, Evolution *> allEvolutions;


public:
    EvolutionScreen(Farm *farm, sf::Font * font);

    int getId() override;

    void init() override;

    Camera *open() override;

    void draw(sf::RenderWindow *window);

    void updateSelectedCreature(Life *connector) override;

    void onWindowResize(int width, int height);

    void mouseMoved(int x, int y);

    void loadTexts();
    void loadButtons(sf::Font *font);

    void mouseClicked(int x, int y);

    void clickedOnButton(int id);

    void placeElements();

    void updateTexts();

    void updateEvolutions(std::map<int, Evolution *> allEvolutions);
};


#endif //CREATURES_EVOLUTIONSCREEN_H
