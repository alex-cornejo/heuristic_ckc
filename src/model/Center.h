//
// Created by alex on 03/06/20.
//

#ifndef CKC_CPP_CENTER_H
#define CKC_CPP_CENTER_H

#include "vector"
#include "list"
#include "iostream"

class Center {
private:
    int center;
    std::vector<int> nodes;
public:
    int getCenter() const;

    void setCenter(int center);

    const std::vector<int> &getNodes() const;

    void setNodes(const std::vector<int> &nodes);

    void addNode(int node);
};


#endif //CKC_CPP_CENTER_H
