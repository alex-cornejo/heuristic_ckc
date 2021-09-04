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
    CkcSolver(int k, int L, const std::vector<std::vector<int>> &G, int numRepetitions);

private:
    int k;
    int L;
    std::vector<std::vector<int>> G;
    int n;
    std::vector<int> w;
    std::vector<int> scores;
    std::vector<bool> assigned;
    std::vector<bool> centers;
    int numRepetitions;
    std::vector<int> capacities;
    int unassignedCount; // tracks the amount of unassigned vertices
    int seed = 0;
    std::vector<int> distances;
    std::vector<std::vector<int>> refMatrix;


public:
    int mpi_size;
    int mpi_rank;

    void loadEdges();

    void computeScore(int r);

    void updateDistances(int c);

    void reset();

    void loadRefMatrix();

    int getFVertex(int idxK, int iter);

    void assignMissingVertices(std::vector<int> &C, std::vector<std::vector<int>> &A);

    std::pair<int, std::vector<int>> distanceBasedSelection(std::vector<int> &NgL, int r);

    void updateScore(std::pair<int, std::vector<int>> &ca, int r);

    int getRadio(std::pair<std::vector<int>, std::vector<std::vector<int>>> &A);

    std::pair<std::vector<int>, std::vector<std::vector<int>>> getFeasibleSolution(int r, int iter);

    int addMissingCenters(std::pair<std::vector<int>, std::vector<std::vector<int>>> &CA, int r);

    std::tuple<std::pair<std::vector<int>, std::vector<std::vector<int>>>, int> solve();

    int farthest_distance(int c, int v, std::vector<int> &X);

    int alternate_heuristic(std::pair<std::vector<int>, std::vector<std::vector<int>>> &CA);
};


#endif //CKC_CPP_CKCSOLVER_H
