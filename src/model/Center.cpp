//
// Created by alex on 03/06/20.
//

#include "Center.h"

int Center::getCenter() const {
    return center;
}

void Center::setCenter(int center) {
    Center::center = center;
}

const std::vector<int> &Center::getNodes() const {
    return nodes;
}

void Center::setNodes(const std::vector<int> &nodes) {
    Center::nodes = nodes;
}

void Center::addNode(int node) {
    Center::nodes.push_back(node);
}
