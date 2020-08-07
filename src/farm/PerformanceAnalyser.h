//
// Created by Amalric Lombard de Buffières on 8/7/20.
//

#ifndef CREATURES_PERFORMANCEANALYSER_H
#define CREATURES_PERFORMANCEANALYSER_H


#include <vector>
#include <chrono>

class PerformanceAnalyser {
private:
    std::vector<double> tickTimes;
    std::vector<double> betweenTicksTimes;

    std::vector<double> entityGrids;
    std::vector<double> brainOutputs;
    std::vector<double> moveCreatures;
    std::vector<double> prepareActions;
    std::vector<double> executeActions;
    std::vector<double> populations;
    std::vector<double> vegetalisations;
    std::vector<double> brainProcessing;


    std::chrono::system_clock::time_point tickStart;
    std::chrono::system_clock::time_point tickEnd;



public:
    PerformanceAnalyser();

    void recordTickStart();
    void recordTickEnd();



    const std::vector<double> &getTickTimes() const;
    const std::vector<double> &getBetweenTicksTimes() const;

    double getLastTickTimes() const;
    double getLastBetweenTicksTimes() const;

    const std::vector<double> &getBrainOutputs() const;
    const std::vector<double> &getMoveCreatures() const;
    const std::vector<double> &getPrepareActions() const;
    const std::vector<double> &getExecuteActions() const;
    const std::vector<double> &getPopulations() const;
    const std::vector<double> &getVegetalisations() const;
    const std::vector<double> &getBrainProcessing() const;

    double getLastBEntityGrids() const;
    double getLastBrainOutputs() const;
    double getLastMoveCreatures() const;
    double getLastPrepareActions() const;
    double getLastExecuteActions() const;
    double getLastPopulations() const;
    double getLastVegetalisations() const;
    double getLastBrainProcessing() const;



    void addEntityGrids(double value);
    void addBrainOutputs(double value);
    void addMoveCreatures(double value);
    void addPrepareActions(double value);
    void addExecuteActions(double value);
    void addPopulations(double value);
    void addVegetalisations(double value);
    void addBrainProcessing(double value);

    std::string detailsString() const;
};


#endif //CREATURES_PERFORMANCEANALYSER_H
