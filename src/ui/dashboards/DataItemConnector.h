//
// Created by Amalric Lombard de Buffières on 8/8/20.
//

#ifndef CREATURES_DATAITEMCONNECTOR_H
#define CREATURES_DATAITEMCONNECTOR_H


#include "../../farm/statistics/DataItem.h"

class DataItemConnector {
private:
    int red;
    int green;
    int blue;

    DataItem * item;

public:
    DataItemConnector(DataItem *item, int red, int green, int blue);

    int getRed() const;

    int getGreen() const;

    int getBlue() const;

    DataItem *getItem() const;
};


#endif //CREATURES_DATAITEMCONNECTOR_H
