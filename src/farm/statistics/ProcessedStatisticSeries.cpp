//
// Created by Amalric Lombard de Buffieres on 09/07/2021.
//

#include "ProcessedStatisticSeries.h"

ProcessedStatisticSeries::ProcessedStatisticSeries(int farmId, const std::string &dataName, std::vector<double> data) : farmId(farmId),
                                                                                              dataName(dataName), data(data), mongoClient(new MongoClient()) {

}


void ProcessedStatisticSeries::process() {
    int tickCount = data.size();

    for (int it = 0; it < tickCount; it++) {
        int startingTick = std::max(0, it - 99);
        int foundTickCount = it - startingTick + 1;

        double total = 0;
        for (int jt = startingTick; jt <= it; jt++){
            total += data.at(jt);
        }

        double value = total /  (double) foundTickCount;

        averagedData.emplace_back(value);
    }
}


void ProcessedStatisticSeries::saveToMongo() {
    auto jsonArray = bsoncxx::builder::basic::array{};
    auto averagedJsonArray = bsoncxx::builder::basic::array{};

    int tickCount = data.size();

    for (int it = 0; it < tickCount; it++) {
        jsonArray.append(data.at(it));
        averagedJsonArray.append(averagedData.at(it));
    }


    auto docValue = bsoncxx::builder::stream::document{} << "type" << dataName << "farm_id" << farmId << "values" << jsonArray << "averaged_values" << averagedJsonArray << finalize;

    mongoClient->saveStatisticsSeries(docValue.view());
}


