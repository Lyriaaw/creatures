//
// Created by Amalric Lombard de Buffières on 8/8/20.
//

#include <iostream>
#include "DataItem.h"

const std::vector<double> &DataItem::getValues() const {
    return values;
}

void DataItem::setValues(const std::vector<double> &values) {
    DataItem::values = values;
}

void DataItem::addRawValue(double value) {
    values.push_back(value);

    if (value > max) {
        max = value;
    }

    if (value < min) {
        min = value;
    }


}
void DataItem::addValue(double value) {
    addRawValue(value);

    if (!isAveraged) {
        return;
    }

    int averagedOn = 100;
    int currentCount = getCount();

    if (currentCount <= averagedOn) {
        averagedOn = currentCount;
    }

    double totalValue = 0.0;
    for (int it = currentCount - averagedOn; it < currentCount; it++) {
        totalValue += values.at(it);
    }
    double average = totalValue / double(averagedOn);

    averaged->addRawValue(average);
}

double DataItem::getLastValue() {
    if (values.size() == 0) {
        return 0;
    }

    return values.at(values.size() - 1);
}

double DataItem::getAveragedLastValue() {
    if (!isAveraged) {
        return 0;
    }

    return averaged->getLastValue();
}



double DataItem::getValueForTick(int tick) {
    if (values.size() - 1 < tick) {
        tick = values.size() - 1;
    }

    return values.at(tick);
}

double DataItem::getAveragedValueForTick(int tick) {
    if (!isAveraged) {
        return 0;
    }

    return averaged->getValueForTick(tick);
}



DataItem::DataItem(std::string name, bool isAveraged): isAveraged(isAveraged), name(name) {
    min = 10000000000;
    max = -10000000;

    if (isAveraged) {
        averaged = new DataItem(name + " averaged", false);
    }
}

double DataItem::getMin() const {
    return min;
}

double DataItem::getMax() const {
    return max;
}

int DataItem::getCount() {
    return this->values.size();
}

DataItem *DataItem::getAveraged() const {
    return averaged;
}

const std::string &DataItem::getName() const {
    return name;
}
