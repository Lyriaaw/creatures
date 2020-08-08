//
// Created by Amalric Lombard de Buffières on 8/8/20.
//

#include "DataItem.h"

const std::vector<double> &DataItem::getValues() const {
    return values;
}

void DataItem::setValues(const std::vector<double> &values) {
    DataItem::values = values;
}

void DataItem::addValue(double value) {
    values.push_back(value);
}

double DataItem::getLastValue() {
    return values.at(values.size() - 1);
}

double DataItem::getValueForTick(int tick) {
    return values.at(tick);
}

