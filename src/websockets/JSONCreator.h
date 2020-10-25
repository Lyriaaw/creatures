//
// Created by Amalric Lombard de Buffières on 10/25/20.
//

#ifndef CREATURES_JSONCREATOR_H
#define CREATURES_JSONCREATOR_H


#include <string>

class JSONCreator {
private:
    std::string result;
public:
    JSONCreator();

    void addStringKeyValue(std::string key, std::string value);
    void addJSONKeyValue(std::string key, std::string value);
    void addIntKeyValue(std::string key, int value);

    std::string getResult();

    void addDoubleKeyValue(std::string key, double value);
};


#endif //CREATURES_JSONCREATOR_H
