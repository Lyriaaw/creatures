//
// Created by Amalric Lombard de Buffières on 10/10/20.
//

#ifndef CREATURES_LIFESCREEN_H
#define CREATURES_LIFESCREEN_H


#include "Screen.h"
#include "../components/BrainUI.h"
#include "../components/Button.h"
#include "../elements/UiLabel.h"

class LifeScreen: public Screen {
private:
    BrainUI * brainUi;

    int windowWidth, windowHeight;

    Camera *camera;

    sf::RectangleShape mapBackground;

    std::vector<Button> buttons;

    std::string informationToShow;


    std::vector<sf::Text> genomeTexts;

    UiLabel * creatureAgeText;

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
    LifeScreen(FarmUI * farmUi);

    int getId() override;

    void init() override;

    void draw(sf::RenderWindow *window);

    void updateSelectedCreature();

    void onWindowResize(int width, int height);

    void mouseMoved(int x, int y);

    void mouseClicked(int x, int y);

    void handleButtonClick(int id);

    void drawGenome(sf::RenderWindow *window);

    void loadSelectedGenome();

    void loadSelectedGraphs();

    void updateGraphs();

    void drawGraphs(sf::RenderWindow *pWindow);

    void mouseScrolled(float delta, int mouseX, int mouseY);

    void mouseMoved(int x, int y, int previousX, int previousY) override;
};


#endif //CREATURES_LIFESCREEN_H