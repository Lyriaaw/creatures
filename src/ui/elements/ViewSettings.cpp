//
// Created by Amalric Lombard de Buffières on 10/18/20.
//

#include "ViewSettings.h"
#include "UiBackground.h"
#include "UiButton.h"
#include "../minimaps/ActionsMinimap.h"
#include "../minimaps/CreatureTileCountMinimap.h"
#include "../minimaps/HeatEnergyMinimap.h"
#include "../minimaps/FoodTileCountMinimap.h"
#include "../minimaps/GroundEnergyMinimap.h"

ViewSettings::ViewSettings(double xRatio, double yRatio, double widthRatio, double heightRatio) : UiComponent(xRatio,yRatio,widthRatio,heightRatio) {
    load();
}

void ViewSettings::load() {
    UiBackground * background = new UiBackground(xRatio, yRatio, widthRatio, heightRatio);
    background->setFillColor(sf::Color(64, 64, 64, 255));
    uiComponents.emplace_back(background);


    double buttonsX = xRatio + 0.005;
    double buttonsY = yRatio + 0.005;

    double buttonWidth = (widthRatio - 0.01) / 2.0;
    double buttonHeight = 0.05;


    UiButton * worldMinimapButton = new UiButton(1, "World", buttonsX + (0 * (buttonWidth + 0.001)), buttonsY + (0 * buttonHeight), buttonWidth - 0.002, buttonHeight - 0.001);
    worldMinimapButton->setOnClick([&](int it){
       this->changeCurrentMinimap(new WorldMinimap());
    });
    uiComponents.emplace_back(worldMinimapButton);

    UiButton * actionsMinimapButton = new UiButton(1, "Actions", buttonsX + (1 * (buttonWidth + 0.001)), buttonsY + (0 * buttonHeight), buttonWidth - 0.002, buttonHeight - 0.001);
    actionsMinimapButton->setOnClick([&](int it){
       this->changeCurrentMinimap(new ActionsMinimap());
    });
    uiComponents.emplace_back(actionsMinimapButton);

    UiButton * creatureColorMinimapButton = new UiButton(1, "Creatures", buttonsX + (0 * (buttonWidth + 0.001)), buttonsY + (1 * buttonHeight), buttonWidth - 0.002, buttonHeight - 0.001);
    creatureColorMinimapButton->setOnClick([&](int it){
       this->changeCurrentMinimap(new CreatureTileCountMinimap());
    });
    uiComponents.emplace_back(creatureColorMinimapButton);

    UiButton * heatMinimapButton = new UiButton(1, "Heat", buttonsX + (1 * (buttonWidth + 0.001)), buttonsY + (1 * buttonHeight), buttonWidth - 0.002, buttonHeight - 0.001);
    heatMinimapButton->setOnClick([&](int it){
       this->changeCurrentMinimap(new HeatEnergyMinimap());
    });
    uiComponents.emplace_back(heatMinimapButton);

    UiButton * entitiesColorMinimapButton = new UiButton(1, "Entities", buttonsX + (0 * (buttonWidth + 0.001)), buttonsY + (2 * buttonHeight), buttonWidth - 0.002, buttonHeight - 0.001);
    entitiesColorMinimapButton->setOnClick([&](int it){
       this->changeCurrentMinimap(new FoodTileCountMinimap());
    });
    uiComponents.emplace_back(entitiesColorMinimapButton);

    UiButton * groundEnergyMinimapButton = new UiButton(1, "Ground", buttonsX + (1 * (buttonWidth + 0.001)), buttonsY + (2 * buttonHeight), buttonWidth - 0.002, buttonHeight - 0.001);
    groundEnergyMinimapButton->setOnClick([&](int it){
       this->changeCurrentMinimap(new GroundEnergyMinimap());
    });
    uiComponents.emplace_back(groundEnergyMinimapButton);







}

void ViewSettings::onWindowResized(double windowWidth, double windowHeight) {
    UiComponent::onWindowResized(windowWidth, windowHeight);

    for (int it = 0; it < uiComponents.size(); it++) {
        uiComponents.at(it)->onWindowResized(windowWidth, windowHeight);
    }
}

void ViewSettings::mouseMoved(int mouseX, int mouseY, int previousX, int previousY) {
    UiComponent::mouseMoved(mouseX, mouseY, previousX, previousY);

    for (int it = 0; it < uiComponents.size(); it++) {
        uiComponents.at(it)->mouseMoved(mouseX, mouseY, previousX, previousY);
    }
}

void ViewSettings::mouseClicked(int mouseX, int mouseY) {
    UiComponent::mouseClicked(mouseX, mouseY);

    for (int it = 0; it < uiComponents.size(); it++) {
        uiComponents.at(it)->mouseClicked(mouseX, mouseY);
    }
}

void ViewSettings::mouseHovering(int mouseX, int mouseY) {

}

void ViewSettings::mouseClickedInside(int mouseX, int mouseY) {

}

void ViewSettings::resized() {

}

void ViewSettings::mouseEnter() {
    mouseInside = true;
}

void ViewSettings::mouseLeave() {
    mouseInside = false;

}

void ViewSettings::draw(sf::RenderWindow *window) {
    if (!mouseInside) {
        return;
    }

    for (int it = 0; it < uiComponents.size(); it++) {
        uiComponents.at(it)->draw(window);
    }
}

void ViewSettings::setChangeCurrentMinimap(const std::function<void(Minimap *)> &changeCurrentMinimap) {
    ViewSettings::changeCurrentMinimap = changeCurrentMinimap;
}
