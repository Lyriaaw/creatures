//
// Created by Amalric Lombard de Buffières on 10/20/20.
//

#ifndef CREATURES_GRAPHCONTROLCENTER_H
#define CREATURES_GRAPHCONTROLCENTER_H


#include <string>
#include <mutex>

class GraphControlCenter {
public:
    GraphControlCenter();


    std::string &getMode();

    int getLastTickFrame();

    int getFromTick();

    int getToTick();

    void setLastTimeFrame(int count);

    void setAll();

    void setTimeFrame(int from, int to);

private:
    std::string mode;

    int lastTickFrame;

    int fromTick, toTick;

    std::mutex update_mutex;

};


#endif //CREATURES_GRAPHCONTROLCENTER_H
