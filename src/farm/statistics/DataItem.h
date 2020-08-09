//
// Created by Amalric Lombard de Buffières on 8/8/20.
//

#ifndef CREATURES_DATAITEM_H
#define CREATURES_DATAITEM_H

#include <string>
#include <shared_mutex>

#include <vector>

class DataItem {
private:
    std::vector<double> values;

    double min;
    double max;

    bool isAveraged;
    DataItem * averaged;

    std::string name;

public:
    DataItem(std::string name, bool averaged);

    const std::vector<double> &getValues() const;
    void setValues(const std::vector<double> &values);

    void addValue(double value);
    void addRawValue(double value);

    double getValueForTick(int tick);
    double getAveragedValueForTick(int tick);

    double getLastValue();
    double getAveragedLastValue();


    double getMin() const;
    double getMax() const;

    DataItem *getAveraged() const;

    int getCount();

    const std::string &getName() const;

};


#endif //CREATURES_DATAITEM_H
