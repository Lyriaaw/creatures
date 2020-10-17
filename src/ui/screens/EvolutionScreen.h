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
    EvolutionScreen(FarmUI * farmUi);

    int getId() override;

    void init() override;

    void draw(sf::RenderWindow *window);

    void updateSelectedCreature() override;

    void onWindowResize(int width, int height);


    void loadTexts();
    void loadButtons(sf::Font *font);


    void clickedOnButton(int id);

    void placeElements();

    void updateTexts();

    void updateEvolutions(std::map<int, Evolution *> allEvolutions);
};


#endif //CREATURES_EVOLUTIONSCREEN_H
