//
// Created by Amalric Lombard de Buffières on 10/18/20.
//

#ifndef CREATURES_VIEWSETTINGS_H
#define CREATURES_VIEWSETTINGS_H


#include "UiComponent.h"
#include "../../../../../../../../usr/local/Cellar/sfml/2.5.1/include/SFML/Graphics/RenderWindow.hpp"
#include "../minimaps/Minimap.h"

class ViewSettings: public UiComponent {
private:
    std::vector<UiComponent *> uiComponents;

    bool mouseInside;

    std::function<void(Minimap * minimap)> changeCurrentMinimap;
public:
    ViewSettings(double xRatio, double yRatio, double widthRatio, double heightRatio);

    void mouseHovering(int mouseX, int mouseY) override;

    void mouseClickedInside(int mouseX, int mouseY) override;

    void resized() override;

    void mouseEnter() override;

    void mouseLeave() override;

    void draw(sf::RenderWindow *window) override;

    void load();

    void onWindowResized(double windowWidth, double windowHeight) override;

    void mouseMoved(int mouseX, int mouseY, int previousX, int previousY) override;

    void setChangeCurrentMinimap(const std::function<void(Minimap *)> &changeCurrentMinimap);

    virtual void mouseClicked(int mouseX, int mouseY);
};


#endif //CREATURES_VIEWSETTINGS_H
