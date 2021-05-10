//
// Created by alex on 03/06/20.
//

#include "KCSolution.h"

// for convenience
using json = nlohmann::json;

const std::string &KCSolution::getInstance() const {
    return instance;
}

void KCSolution::setInstance(const std::string &instance) {
    KCSolution::instance = instance;
}

const std::vector<Center> &KCSolution::getCenters() const {
    return centers;
}

void KCSolution::setCenters(const std::vector<Center> &centers) {
    KCSolution::centers = centers;
}

void KCSolution::addCenter(Center &center) {
    KCSolution::centers.push_back(center);
}

std::string KCSolution::toJson() {
    json bodyJson = {{"instance", getInstance()},
                     {"type",     type}};

    json centersJson;
    for (auto &center : getCenters()) {
        centersJson.push_back({{"center", center.getCenter()},
                               {"nodes",  center.getNodes()}});
    }
    bodyJson["centers"] = centersJson;
    return bodyJson.dump();
}

KCSolution::KCSolution() {
    type = "cvkp";
}
