//
// Created by Amalric Lombard de Buffières on 8/8/20.
//

#ifndef CREATURES_DATAITEM_H
#define CREATURES_DATAITEM_H


#include <vector>

class DataItem {
private:
    std::vector<double> values;

public:

    const std::vector<double> &getValues() const;
    void setValues(const std::vector<double> &values);

    void addValue(double value);
    double getLastValue();
    double getValueForTick(int tick);

};


#endif //CREATURES_DATAITEM_H
