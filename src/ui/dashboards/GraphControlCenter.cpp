//
// Created by Amalric Lombard de Buffières on 10/20/20.
//

#include "GraphControlCenter.h"

GraphControlCenter::GraphControlCenter() : mode("all"), lastTickFrame(0), fromTick(0), toTick(0) {}

void GraphControlCenter::setAll() {
    std::lock_guard<std::mutex> guard(update_mutex);

    this->mode = "all";
}

void GraphControlCenter::setLastTimeFrame(int count) {
    std::lock_guard<std::mutex> guard(update_mutex);
    this->mode = "last";
    this->lastTickFrame = count;
}

void GraphControlCenter::setTimeFrame(int from, int to) {
    std::lock_guard<std::mutex> guard(update_mutex);
    this->mode = "fixed";
    this->fromTick = from;
    this->toTick = to;
}

std::string &GraphControlCenter::getMode() {
    std::lock_guard<std::mutex> guard(update_mutex);
    return mode;
}

int GraphControlCenter::getLastTickFrame() {
    std::lock_guard<std::mutex> guard(update_mutex);
    return lastTickFrame;
}

int GraphControlCenter::getFromTick() {
    std::lock_guard<std::mutex> guard(update_mutex);
    return fromTick;
}

int GraphControlCenter::getToTick() {
    std::lock_guard<std::mutex> guard(update_mutex);
    return toTick;
}
