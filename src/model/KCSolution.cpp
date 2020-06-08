//
// Created by alex on 03/06/20.
//

#include "KCSolution.h"
#include <boost/property_tree/json_parser.hpp>

// for convenience
using json = nlohmann::json;

using boost::property_tree::ptree;

const std::string &KCSolution::getInstance() const {
    return instance;
}

void KCSolution::setInstance(const std::string &instance) {
    KCSolution::instance = instance;
}

const std::vector<int> &KCSolution::getOutliers() const {
    return outliers;
}

void KCSolution::setOutliers(const std::vector<int> &outliers) {
    KCSolution::outliers = outliers;
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
                     {"outliers", getOutliers()}};

    json centersJson;
    for (auto &center : getCenters()) {
        centersJson.push_back({{"center", center.getCenter()},
                                    {"nodes",  center.getNodes()}});
    }
    bodyJson["centers"] = centersJson;
    return bodyJson.dump();
}