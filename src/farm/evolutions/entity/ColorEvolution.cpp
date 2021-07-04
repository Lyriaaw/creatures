//
// Created by Amalric Lombard de Buffières on 8/3/20.
//

#include "ColorEvolution.h"

ColorEvolution::ColorEvolution(): Evolution() {

}

void ColorEvolution::generateFromRandom(Life * life) {
    this->hue = (rand() % 1000) / 1000.f;
}

void ColorEvolution::perform(Life * life) {
    life->getEntity()->setColor(this->hue);
}

std::string ColorEvolution::describe() {
    return std::to_string(this->generationNumber) + " => Color => " + std::to_string(this->hue);
}

Evolution * ColorEvolution::generateWithMate(Evolution * mate) {
    ColorEvolution * mateColorEvolution(nullptr);

    if (mate == nullptr) {
        std::cout << "NULL POINTER OF FATHER COLOR" << std::endl;
    } else {
        mateColorEvolution = dynamic_cast<ColorEvolution *>(mate);
        if (mateColorEvolution == nullptr) {
            std::cout << "UNABLE TO CAST FATHER COLOR EVOLUTION" << std::endl;
        }
    }

    return generateFromCastedMate(mateColorEvolution);
}

Evolution * ColorEvolution::generateFromCastedMate(ColorEvolution * mate) {
    ColorEvolution * childColorEvolution = new ColorEvolution();
    childColorEvolution->setGenerationNumber(getGenerationNumber());

    if (mate == nullptr) {
        childColorEvolution->setHue(this->hue);
        return childColorEvolution;
    }

    // 3 chances out of 4 to simply select one of the parent's sensor
    if (rand() % MUTATION_RATIO != 0) {
        int geneExpression = rand() % 3;


        if (rand() % 2 == 0) {
            childColorEvolution->hue = this->hue;
        } else {
            childColorEvolution->hue = mate->hue;
        }

        return childColorEvolution;
    }

    float newHue;

    float smallestHue = this->hue;
    if (mate->hue < smallestHue) {
        smallestHue = mate->hue;
    }

    float rawDistance =  abs(this->hue - mate->hue);
    float hueDistances = std::min(rawDistance, 1.f - rawDistance);

    if (rawDistance < 1 - rawDistance) {
        newHue = smallestHue + (hueDistances / 2.f);
    } else {
        newHue = 1 + (smallestHue - (hueDistances / 2.f));
    }

    childColorEvolution->setHue(newHue);


    return childColorEvolution;
}

void ColorEvolution::setHue(float hue) {
    ColorEvolution::hue = hue;
}

std::string ColorEvolution::getName() {
    return "Color";
}


void ColorEvolution::saveInMongo(MongoClient *client, int farmId) {
    auto builder = bsoncxx::builder::stream::document{};

    builder
            << "farmId" << farmId
            << "evolution_number" << this->generationNumber
            << "type" << "color"
            << "variables"
            << bsoncxx::builder::stream::open_document
            << "hue" << this->hue
            << bsoncxx::builder::stream::close_document;

    bsoncxx::document::value doc_value = builder << bsoncxx::builder::stream::finalize;


    try {
        client->saveEvolution(doc_value.view());
    } catch (const std::exception& e) {
        std::cout << "Error while saving mouth evolution: " << " in thread " << std::this_thread::get_id() << " -> " << e.what() <<  std::endl;
    }
}

bsoncxx::builder::stream::document ColorEvolution::generateMongoVariables() {
    auto builder = bsoncxx::builder::stream::document{};

    builder
            << "evolution_number" << this->generationNumber
            << "type" << "color"
            << "variables"
            << bsoncxx::builder::stream::open_document
            << "hue" << this->hue
            << bsoncxx::builder::stream::close_document;

    return builder;
}
