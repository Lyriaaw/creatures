//
// Created by Amalric Lombard de Buffières on 10/10/20.
//

#ifndef CREATURES_LIFESCREEN_H
#define CREATURES_LIFESCREEN_H


#include "Screen.h"
#include "../components/BrainUI.h"
#include "../components/Button.h"

class LifeScreen: public Screen {
private:
    BrainUI * brainUi;

    int windowWidth, windowHeight;

    Camera *camera;

    sf::RectangleShape mapBackground;

    std::vector<Button> buttons;

    std::string informationToShow;


    std::vector<sf::Text> genomeTexts;

    sf::Text creatureAgeText;

    DataItem * grad0;
    DataItem * grad1;
    DataItem * gradm1;

    Graph * inputNeuronsGraph;

    Graph * outputNeuronsGraph;
    DataItem * movement;
    DataItem * rotation;
    DataItem * mouth;
    DataItem * genitals;

    Graph * bioGraph;
    DataItem * energy;
    DataItem * mass;
    DataItem * wasted;
    DataItem * maxEnergy;





public:
    LifeScreen(Farm *farm, sf::Font *font);

    int getId() override;

    void init() override;

    Camera *open() override;

    void draw(sf::RenderWindow *window);

    void updateSelectedCreature(Life * life);

    void onWindowResize(int width, int height);

    void mouseMoved(int x, int y);

    void mouseClicked(int x, int y);

    void handleButtonClick(int id);

    void drawGenome(sf::RenderWindow *window);

    void loadSelectedGenome();

    void loadSelectedGraphs();

    void updateGraphs();

    void drawGraphs(sf::RenderWindow *pWindow);
};


#endif //CREATURES_LIFESCREEN_H