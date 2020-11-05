//
// Created by Amalric Lombard de Buffières on 10/23/20.
//

#ifndef CREATURES_FARMCONTROL_H
#define CREATURES_FARMCONTROL_H
#include <nlohmann/json.hpp>


class FarmControl {
private:
    bool running;
    bool paused;
    int waitingTimeMs;

    std::function<void()> triggerUpdate;

public:
    FarmControl();


    bool isRunning() const;

    void setRunning(bool running);

    bool isPaused() const;

    void setPaused(bool paused);

    void setTriggerUpdate(const std::function<void()> &triggerUpdate);

    int getWaitingTimeMs() const;

    void setWaitingTimeMs(int waitingTimeMs);

    nlohmann::json asJSON();
};


#endif //CREATURES_FARMCONTROL_H
