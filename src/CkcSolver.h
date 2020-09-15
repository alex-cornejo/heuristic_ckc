//
// Created by alex on 26/05/20.
//

#ifndef CKC_CPP_CKCSOLVER_H
#define CKC_CPP_CKCSOLVER_H


#include <vector>
#include <map>
#include <tuple>

class CkcSolver {
public:
    CkcSolver(int k, int L, const std::vector<std::vector<float>> &G, int numRepetitions);

private:
    int k;
    int L;
    std::vector<std::vector<float>> G;
    int n;
    std::vector<float> w;
    std::vector<int> scores;
    std::vector<bool> assigned;
    std::vector<bool> centers;
    int numRepetitions;
    std::vector<int> capacities;
    int unassignedCount; // tracks the unassigned vertices
    int seed = 0;
    std::vector<float> distances;
    std::vector<std::vector<int>> refMatrix;

    float addMissingCenters(std::map<int, std::vector<int>> &A, float r);

public:
    void loadEdges();

    void computeScore(float r);

    void updateDistances(int c);

    void init();

    void loadRefMatrix();

    int getFVertex(std::vector<int> &C, int iter);

    void assignMissingVertices(std::vector<int> &C, std::vector<std::vector<int>> &A);

    std::pair<int, std::vector<int>> distanceBasedSelectionConstant(std::vector<int> &NgL, float r);

    float getRadio(std::map<int, std::vector<int>> &A);

    std::map<int, std::vector<int>> getFeasibleSolution(float r, int iter);

    std::tuple<std::map<int, std::vector<int>>, float> solve();

    void updateScore(std::pair<int, std::vector<int>> &ca, float r);
};


#endif //CKC_CPP_CKCSOLVER_H
