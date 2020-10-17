//
// Created by Amalric Lombard de Buffières on 10/14/20.
//

#include "ColorsGraph.h"
#include "../colors/RGBColor.h"
#include <cmath>
#include <iostream>


void ColorsGraph::getMinAndMaxValues() {
    min = 0.0;
    max = dataAnalyser->getMaxValueCount();


    widthRatio = width / double(dataAnalyser->getTickCount());

    heightRatio = 1;

    heightRatio = 0;

    if (max - min != 0) {
        heightRatio = height / abs(max - min);
    }
}

ColorsGraph::ColorsGraph(const std::string &name, float xRatio, float yRatio, float widthRatio, float heightRatio, GeneGraphAnalyser *dataAnalyser) : Graph(name, xRatio, yRatio, widthRatio, heightRatio),dataAnalyser(dataAnalyser), mode("POPULATION") {
    texture = sf::Texture();
    texture.create(0, 0);

    sprite = sf::Sprite(texture);
    sprite.setPosition(x, y);

}

void ColorsGraph::draw(sf::RenderWindow *window) {
    if (dataAnalyser->getTickCount() < 10) {
        return;
    }
    getMinAndMaxValues();

//    window->draw(background);

    int W = width;
    int H = height;

    sf::Uint8 * uintPixels = new sf::Uint8[W*H*4];

    int ticks = dataAnalyser->getTickCount();

    if (ticks == 0) {
        return;
    }


    std::vector<std::vector<double>> values = dataAnalyser->getValues();



    double currentHeightRatio;
    for (int it = 0; it < W; it++) {
        int currentTick = it / widthRatio;

        if (mode == "RATIO") {
            currentHeightRatio = height / double(values.at(currentTick).size());
        }

        for (int jt = 0; jt < H; jt++) {
            int currentHeight = 0;
            if (mode == "POPULATION") {
                currentHeight = max - (jt / heightRatio);

            }
            if (mode == "RATIO") {
                currentHeight = values.at(currentTick).size() - (jt / currentHeightRatio);
            }

            int index = ((jt * W) + it) * 4;

            if (currentHeight >= values.at(currentTick).size()) {
                uintPixels[index ] = 100; // obviously, assign the values you need here to form your color
                uintPixels[index + 1] = 100;
                uintPixels[index + 2] = 100;
                uintPixels[index + 3] = 255;
                continue;
            }

            RGBColor outlineColor = RGBColor(values.at(currentTick).at(currentHeight), 1.f, 0.5);

            uintPixels[index ] = outlineColor.getRed(); // obviously, assign the values you need here to form your color
            uintPixels[index +1] = outlineColor.getGreen();
            uintPixels[index +2] = outlineColor.getBlue();
            uintPixels[index +3] = 255;



        }
    }


    texture.update(uintPixels);


    window->draw(sprite);

}

void ColorsGraph::windowResized(float windowWidth, float windowHeight) {
    Graph::windowResized(windowWidth, windowHeight);

    texture = sf::Texture();
    texture.create(int(width), int(height));

    sprite = sf::Sprite(texture);
    sprite.setPosition(x, y);
}


void ColorsGraph::switchMode() {
    if (mode == "POPULATION"){
        mode = "RATIO";
    } else if (mode == "RATIO") {
        mode = "POPULATION";
    }
}