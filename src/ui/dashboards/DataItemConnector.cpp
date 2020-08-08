//
// Created by Amalric Lombard de Buffières on 8/8/20.
//

#include "DataItemConnector.h"

DataItemConnector::DataItemConnector(DataItem *item, int red, int green, int blue) : red(red), green(green), blue(blue),
                                                                                     item(item) {}

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
