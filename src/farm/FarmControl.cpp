//
// Created by Amalric Lombard de Buffières on 10/23/20.
//

#include "FarmControl.h"

FarmControl::FarmControl(): running(false), paused(true) {}

bool FarmControl::isRunning() const {
    return running;
}

void FarmControl::setRunning(bool running) {
    FarmControl::running = running;
}

bool FarmControl::isPaused() const {
    return paused;
}

void FarmControl::setPaused(bool paused) {
    FarmControl::paused = paused;
}
