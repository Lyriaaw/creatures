//
// Created by Amalric Lombard de Buffières on 10/17/20.
//

#ifndef CREATURES_TESTSCREEN_H
#define CREATURES_TESTSCREEN_H


#include "Screen.h"

class TestScreen: public Screen {
private:
public:
    TestScreen(FarmUI * farmUi);

    int getId() override;

    void init() override;

    void updateSelectedCreature(Life *connector) override;
};


#endif //CREATURES_TESTSCREEN_H
