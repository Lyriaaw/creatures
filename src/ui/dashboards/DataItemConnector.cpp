//
// Created by Amalric Lombard de Buffières on 8/8/20.
//

#include "DataItemConnector.h"

DataItemConnector::DataItemConnector(DataItem *item, int displayMode, int red, int green, int blue) : red(red), green(green), blue(blue),
                                                                                     item(item), displayMode(displayMode) {}

int DataItemConnector::getRed() const {
    return red;
}

int DataItemConnector::getGreen() const {
    return green;
}

int DataItemConnector::getBlue() const {
    return blue;
}

DataItem *DataItemConnector::getItem() const {
    return item;
}

int DataItemConnector::getDisplayMode() const {
    return displayMode;
}

void DataItemConnector::setDisplayMode(int displayMode) {
    DataItemConnector::displayMode = displayMode;
}

double DataItemConnector::getValueForTick(int tick) {
    if (displayMode == 2) {
        return item->getAveragedValueForTick(tick);
    }

    return item->getValueForTick(tick);
}

double DataItemConnector::getMin() {
    if (displayMode == 2) {
        return item->getAveraged()->getMin();
    }

    return item->getMin();
}

double DataItemConnector::getMax() {
    if (displayMode == 2) {
        return item->getAveraged()->getMax();
    }

    return item->getMax();
}

