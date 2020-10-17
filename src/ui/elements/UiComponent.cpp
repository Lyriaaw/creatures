//
// Created by Amalric Lombard de Buffières on 10/17/20.
//

#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>
#include "UiComponent.h"


UiComponent::UiComponent(double xRatio, double yRatio, double widthRatio, double heightRatio) {
    this->xRatio = xRatio;
    this->yRatio = yRatio;
    this->widthRatio = widthRatio;
    this->heightRatio = heightRatio;
}

void UiComponent::setFixedSizeAndPosition(double x, double y, double width, double height) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
}

void UiComponent::onWindowResized(double windowWidth, double windowHeight) {
    if (xRatio != 0) {
        x = xRatio * windowWidth;
    }
    if (yRatio != 0) {
        y = yRatio * windowHeight;
    }

    if (widthRatio != 0) {
        width = widthRatio * windowWidth;
    }
    if (heightRatio != 0) {
        height = heightRatio * windowHeight;
    }
    resized();
}

void UiComponent::mouseMoved(int mouseX, int mouseY, int previousX, int previousY) {
    bool currentInside = false;
    bool previousInside = false;

    if (mouseX > x && mouseY > y && mouseX < x + width && mouseY < y + height) {
        currentInside = true;
    }
    if (previousX > x && previousY > y && previousX < x + width && previousY < y + height) {
        previousInside = true;
    }

    if (currentInside && !previousInside) {
        mouseEnter();
    }
    if (previousInside && !currentInside) {
        mouseLeave();
    }


    if (currentInside) {
        mouseHovering(mouseX, mouseY);
    }
}


void UiComponent::mouseClicked(int mouseX, int mouseY) {
    if (mouseX < x || mouseY < y || mouseX > x + width || mouseY > y + height) {
        return;
    }
    mouseClickedInside(mouseX, mouseY);
}




//
//void UiComponent::resized() {
//}
//
//void UiComponent::mouseClickedInside(int mouseX, int mouseY) {
//
//}
//void UiComponent::mouseHovering(int mouseX, int mouseY) {
//
//}
//
//void UiComponent::mouseEnter() {
//}
//void UiComponent::mouseLeave() {
//
//}
//
//void UiComponent::draw(sf::RenderWindow *window) {
//}