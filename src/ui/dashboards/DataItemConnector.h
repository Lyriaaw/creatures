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

    // 0 : Do not show
    // 1 : Show normal
    // 2 : Show averaged
    int displayMode;

public:
    DataItemConnector(DataItem *item, int displayMode, int red, int green, int blue);

    int getRed() const;

    int getGreen() const;

    int getBlue() const;

    DataItem *getItem() const;

    int getDisplayMode() const;

    void setDisplayMode(int displayMode);

    double getValueForTick(int tick);
    double getMin();
    double getMax();
};


#endif //CREATURES_DATAITEMCONNECTOR_H
