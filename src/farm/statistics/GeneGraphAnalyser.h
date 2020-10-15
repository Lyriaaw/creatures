//
// Created by Amalric Lombard de Buffières on 10/14/20.
//

#ifndef CREATURES_GENEGRAPHANALYSER_H
#define CREATURES_GENEGRAPHANALYSER_H


#include <vector>
#include <string>

class GeneGraphAnalyser {
private:
    std::vector<std::vector<double>> values;

    double maxValueCount;

    std::string name;
public:
    GeneGraphAnalyser(const std::string &name);

    void addTick(const std::vector<double> tickValues);

    const std::vector<double> getTickAt(int tick);

    double getMaxValueCount() const;

    int getTickCount() const;

    const std::vector<std::vector<double>> &getValues() const;
};


#endif //CREATURES_GENEGRAPHANALYSER_H
