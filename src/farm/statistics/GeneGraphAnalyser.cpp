//
// Created by Amalric Lombard de Buffières on 10/14/20.
//

#include "GeneGraphAnalyser.h"

GeneGraphAnalyser::GeneGraphAnalyser(const std::string &name) : name(name), maxValueCount(0) {}

void GeneGraphAnalyser::addTick(const std::vector<double> tickValues) {
    values.emplace_back(tickValues);

    if (tickValues.size() > maxValueCount) {
        maxValueCount = tickValues.size();
    }
}

const std::vector<double> GeneGraphAnalyser::getTickAt(int tick) {
    if (tick >= values.size()) {
        return std::vector<double>();
    }

    return values.at(tick);
}

double GeneGraphAnalyser::getMaxValueCount() const {
    return maxValueCount;
}

int GeneGraphAnalyser::getTickCount() const {
    return values.size();
}

const std::vector<std::vector<double>> &GeneGraphAnalyser::getValues() const {
    return values;
}
