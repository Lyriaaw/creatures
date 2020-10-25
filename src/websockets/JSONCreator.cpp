//
// Created by Amalric Lombard de Buffières on 10/25/20.
//

#include <sstream>
#include "JSONCreator.h"

JSONCreator::JSONCreator(): result("") {}

void JSONCreator::addStringKeyValue(std::string key, std::string value) {
    std::stringstream newResult;

    newResult << result;

    if (result != "") {
        newResult << ", ";
    }
    newResult << "\"" << key << "\": \"" << value << "\"";

    result = newResult.str();
}
void JSONCreator::addJSONKeyValue(std::string key, std::string value) {
    std::stringstream newResult;

    newResult << result;

    if (result != "") {
        newResult << ", ";
    }
    newResult << "\"" << key << "\": " << value << "";

    result = newResult.str();
}

void JSONCreator::addIntKeyValue(std::string key, int value) {
    std::stringstream newResult;

    newResult << result;

    if (result != "") {
        newResult << ", ";
    }
    newResult << "\"" << key << "\": " << value << "";

    result = newResult.str();
}

void JSONCreator::addDoubleKeyValue(std::string key, double value) {
    std::stringstream newResult;

    newResult << result;

    if (result != "") {
        newResult << ", ";
    }
    newResult << "\"" << key << "\": " << value << "";

    result = newResult.str();
}






std::string JSONCreator::getResult() {
    return "{" + result + "}";
}
