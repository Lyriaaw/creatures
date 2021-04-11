//
// Created by Amalric Lombard de Buffières on 10/23/20.
//

#include "FarmControl.h"

FarmControl::FarmControl(): running(true), paused(true), waitingTimeMs(10) {}

bool FarmControl::isRunning() const {
    return running;
}

void FarmControl::setRunning(bool running) {
    FarmControl::running = running;
    triggerUpdate();
}

bool FarmControl::isPaused() const {
    return paused;
}

void FarmControl::setPaused(bool paused) {
    FarmControl::paused = paused;
    triggerUpdate();
}


nlohmann::json FarmControl::asJSON() {
    nlohmann::json result;
    result["running"] = running;
    result["paused"] = paused;
    result["waitingTimeMs"] = waitingTimeMs;


    return {{"farmControl", result}};
}

void FarmControl::setTriggerUpdate(const std::function<void()> &triggerUpdate) {
    FarmControl::triggerUpdate = triggerUpdate;
}

int FarmControl::getWaitingTimeMs() const {
    return waitingTimeMs;
}

void FarmControl::setWaitingTimeMs(int waitingTimeMs) {
    FarmControl::waitingTimeMs = waitingTimeMs;
}

