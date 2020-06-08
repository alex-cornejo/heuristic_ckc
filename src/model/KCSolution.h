//
// Created by alex on 03/06/20.
//

#ifndef CKC_CPP_KCSOLUTION_H
#define CKC_CPP_KCSOLUTION_H


#include "string"
#include "list"
#include "Center.h"
#include "nlohmann/json.hpp"

class KCSolution {
private:
    std::string instance;
    std::vector<int> outliers;
    std::vector<Center> centers;
public:
    void addCenter(Center &center);

    const std::string &getInstance() const;

    void setInstance(const std::string &instance);

    const std::vector<int> &getOutliers() const;

    void setOutliers(const std::vector<int> &outliers);

    const std::vector<Center> &getCenters() const;

    void setCenters(const std::vector<Center> &centers);

    std::string toJson();
};


#endif //CKC_CPP_KCSOLUTION_H
