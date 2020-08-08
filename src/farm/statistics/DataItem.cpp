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

void DataItem::addValue(double value) {
    values.push_back(value);

    if (value > max) {
        max = value;
    }

    if (value < min) {
        min = value;
    }
}

double DataItem::getLastValue() {
    if (values.size() == 0) {
        return 0;
    }

    return values.at(values.size() - 1);
}

double DataItem::getSecondToLastValue() {
    return values.at(values.size() - 2);
}


double DataItem::getValueForTick(int tick) {
    return values.at(tick);
}

DataItem::DataItem() {
    min = 10000000000;
    max = -10000000;
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
