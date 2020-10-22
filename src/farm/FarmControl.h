//
// Created by Amalric Lombard de Buffières on 10/23/20.
//

#ifndef CREATURES_FARMCONTROL_H
#define CREATURES_FARMCONTROL_H


class FarmControl {
private:
    bool running;
    bool paused;

public:
    FarmControl();


    bool isRunning() const;

    void setRunning(bool running);

    bool isPaused() const;

    void setPaused(bool paused);
};


#endif //CREATURES_FARMCONTROL_H
