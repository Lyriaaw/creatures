//
// Created by Amalric Lombard de Buffières on 8/7/20.
//

#include <string>
#include "PerformanceAnalyser.h"

PerformanceAnalyser::PerformanceAnalyser() {
    tickEnd = std::chrono::system_clock::now();
}


void PerformanceAnalyser::recordTickStart() {
    tickStart = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed_time = tickStart - tickEnd;
    betweenTicksTimes.emplace_back(elapsed_time.count());
}

void PerformanceAnalyser::recordTickEnd() {
    tickEnd = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed_time = tickEnd - tickStart;
    tickTimes.emplace_back(elapsed_time.count());
}




std::string PerformanceAnalyser::detailsString() const {
    std::string details = "";


//    details = details + " Tick Time: " + std::to_string(getLastTickTimes());
//    details = details + " Between Ticks: " + std::to_string(getLastBetweenTicksTimes());
//    details = details + " Entity Grid: " + std::to_string(getLastBEntityGrids());
//    details = details + " Brain Output: " + std::to_string(getLastBrainOutputs());
//    details = details + " Move Creature: " + std::to_string(getLastMoveCreatures());
//    details = details + " Handle Actions: " + std::to_string(getLastHandleActions());
//    details = details + " Population Control: " + std::to_string(getLastPopulations());
//    details = details + " Vegetalisation: " + std::to_string(getLastVegetalisations());
//    details = details + " Brain Processing: " + std::to_string(getLastBrainProcessing());



    details = details + "T: " + std::to_string(getLastTickTimes());
    details = details + " BT: " + std::to_string(getLastBetweenTicksTimes());
    details = details + " EG: " + std::to_string(getLastBEntityGrids());
    details = details + " BO: " + std::to_string(getLastBrainOutputs());
    details = details + " MC: " + std::to_string(getLastMoveCreatures());
    details = details + " PA: " + std::to_string(getLastPrepareActions());
    details = details + " EA: " + std::to_string(getLastExecuteActions());
    details = details + " PC: " + std::to_string(getLastPopulations());
    details = details + " V: " + std::to_string(getLastVegetalisations());
    details = details + " BP: " + std::to_string(getLastBrainProcessing());





    return details;
}






const std::vector<double> &PerformanceAnalyser::getTickTimes() const {
    return tickTimes;
}

const std::vector<double> &PerformanceAnalyser::getBetweenTicksTimes() const {
    return betweenTicksTimes;
}

double PerformanceAnalyser::getLastTickTimes() const {
    return tickTimes.at(tickTimes.size() - 1);
}

double PerformanceAnalyser::getLastBetweenTicksTimes() const {
    return betweenTicksTimes.at(betweenTicksTimes.size() - 1);
}

const std::vector<double> &PerformanceAnalyser::getBrainOutputs() const {
    return brainOutputs;
}

const std::vector<double> &PerformanceAnalyser::getMoveCreatures() const {
    return moveCreatures;
}

const std::vector<double> &PerformanceAnalyser::getPrepareActions() const {
    return prepareActions;
}
const std::vector<double> &PerformanceAnalyser::getExecuteActions() const {
    return executeActions;
}

const std::vector<double> &PerformanceAnalyser::getPopulations() const {
    return populations;
}

const std::vector<double> &PerformanceAnalyser::getVegetalisations() const {
    return vegetalisations;
}

const std::vector<double> &PerformanceAnalyser::getBrainProcessing() const {
    return brainProcessing;
}



double PerformanceAnalyser::getLastBrainOutputs() const {
    if (brainOutputs.empty()) return 0;

    return brainOutputs.at(brainOutputs.size() - 1);
}

double PerformanceAnalyser::getLastBEntityGrids() const {
    if (entityGrids.empty()) return 0;

    return entityGrids.at(entityGrids.size() - 1);
}

double PerformanceAnalyser::getLastMoveCreatures() const {
    if (moveCreatures.empty()) return 0;

    return moveCreatures.at(moveCreatures.size() - 1);
}

double PerformanceAnalyser::getLastPrepareActions() const {
    if (prepareActions.empty()) return 0;

    return prepareActions.at(prepareActions.size() - 1);
}
double PerformanceAnalyser::getLastExecuteActions() const {
    if (executeActions.empty()) return 0;

    return executeActions.at(executeActions.size() - 1);
}

double PerformanceAnalyser::getLastPopulations() const {
    if (populations.empty()) return 0;

    return populations.at(populations.size() - 1);
}

double PerformanceAnalyser::getLastVegetalisations() const {
    if (vegetalisations.empty()) return 0;

    return vegetalisations.at(vegetalisations.size() - 1);
}

double PerformanceAnalyser::getLastBrainProcessing() const {
    if (brainProcessing.empty()) return 0;

    return brainProcessing.at(brainProcessing.size() - 1);
}





void PerformanceAnalyser::addEntityGrids(double value){
    entityGrids.emplace_back(value);
}

void PerformanceAnalyser::addBrainOutputs(double value){
    brainOutputs.emplace_back(value);
}

void PerformanceAnalyser::addMoveCreatures(double value){
    moveCreatures.emplace_back(value);
}

void PerformanceAnalyser::addPrepareActions(double value){
    prepareActions.emplace_back(value);
}

void PerformanceAnalyser::addExecuteActions(double value){
    prepareActions.emplace_back(value);
}

void PerformanceAnalyser::addPopulations(double value){
    populations.emplace_back(value);
}

void PerformanceAnalyser::addVegetalisations(double value){
    vegetalisations.emplace_back(value);
}

void PerformanceAnalyser::addBrainProcessing(double value){
    brainProcessing.emplace_back(value);
}

