//
// Created by alex on 26/05/20.
//

#include "CkcSolver.h"
#include <algorithm>
#include <cmath>
#include <iostream>

CkcSolver::CkcSolver(int k, int L, const std::vector<std::vector<float>> &G, int numRepetitions) :
        k(k), L(L), G(G), numRepetitions(numRepetitions) {
    n = G.size();
    loadEdges();
    loadRefMatrix();
    init();
}

void CkcSolver::init() {
    unassignedCount = n;

    //reset assigned vertices
    assigned.clear();
    assigned.resize(n);

    //reset assigned centers
    centers.clear();
    centers.resize(n);

    //reset capacities
    capacities.clear();
    capacities.resize(k);
    std::fill(capacities.begin(), capacities.end(), L);

    //reset distances for all v to C
    distances.clear();
    distances.resize(n);
    std::fill(distances.begin(), distances.end(), std::numeric_limits<float>::infinity());
}

void CkcSolver::loadEdges() {
    w.reserve((n * (n - 1)) / 2);
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            w.push_back(G[i][j]);
        }
    }
    sort(w.begin(), w.end());
}

void CkcSolver::computeScore(float r) {

    //reset scores
    scores.clear();
    scores.resize(n);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (G[i][j] <= r) scores[i]++;
        }
    }
}

void CkcSolver::updateDistances(int c) {
    for (int i = 0; i < n; i++) {
        if (G[i][c] < distances[i]) {
            distances[i] = G[i][c];
        }
    }
}

void CkcSolver::loadRefMatrix() {
    refMatrix.clear();
    refMatrix.resize(n);
    std::vector<std::vector<float>> vertexReferences(n, std::vector<float>(2));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            vertexReferences[j] = {(float) j, G[i][j]};
        }
        std::sort(vertexReferences.begin(), vertexReferences.end(), [](auto &v1, auto &v2) { return v1[1] > v2[1]; });
        std::vector<int> references(n);
        for (int j = 0; j < n; j++) {
            references[j] = (int) vertexReferences[j][0];
        }
        refMatrix[i] = references;
    }
}

int CkcSolver::getFVertex(std::vector<int> &C, int iter) {
    int f = 0;
    if (!C.empty()) {

        // get farthest vertex
        float maxDist = 0;
        for (int i = 0; i < n; i++) {
            float dist = distances[i];
            if (maxDist < dist) {
                maxDist = dist;
                f = i;
            }
        }
    } else {
        f = (numRepetitions == n) ? iter : rand() % n;
    }
    return f;
}

void CkcSolver::assignMissingVertices(std::vector<int> &C, std::vector<std::vector<int>> &A) {

    for (int j = 0; j < n; j++) {
        if (!assigned[j]) {
            float minDist = +INFINITY;
            int iClosestCenter = 0;
            int ic = 0;
            for (int c : C) {
                if (capacities[ic] > 0 && G[c][j] < minDist) {
                    minDist = G[c][j];
                    iClosestCenter = ic;
                }
                ic++;
            }
            capacities[iClosestCenter]--;
            unassignedCount--;
            assigned[j] = true;
            A[iClosestCenter].push_back(j);
        }
    }
}

std::pair<int, std::vector<int>> CkcSolver::distanceBasedSelectionConstant(std::vector<int> &NgL, float r) {

    std::pair<int, std::vector<int>> assignment;
    float d = +INFINITY;

    for (int v : NgL) {

        // get f_ref vertex
        int fref = 0;
        float maxDist = 0;
        for (int j = 0; j < n; j++) {

            float dist = std::min(distances[j], G[j][v]);
            if (maxDist < dist) {
                maxDist = dist;
                fref = j;
            }
        }

        // get unassigned neighbors of v
        std::vector<int> vertices(L);
        float dv = 0;
        int iu = 0;
        for (int u : refMatrix[fref]) {

            if (G[v][u] <= r && !assigned[u]) {
                if (iu == L) {
                    dv = G[fref][u];
                    break;
                }
                vertices[iu] = u;
                iu++;
            }
        }

        if (dv < d) {
            d = dv;
            assignment.first = v;
            assignment.second = vertices;
        }
    }
    return assignment;
}

float CkcSolver::getRadio(std::map<int, std::vector<int>> &A) {
    float maxDist = -1;
    for (auto &c : A) {
        for (int v : c.second) {
            int z = c.first;
            if (G[z][v] > maxDist) {
                maxDist = G[c.first][v];
            }
        }
    }
    return maxDist;
}

std::map<int, std::vector<int>> CkcSolver::getFeasibleSolution(float r, int iter) {
    computeScore(r);
    std::map<int, std::vector<int>> A;
    std::vector<int> C;
    std::vector<std::vector<int>> assignments;
    for (int i = 0; i < k; i++) {

        std::pair<int, std::vector<int>> assignment;

        int f = getFVertex(C, iter);

        // get N(f)U{f} with score > L ... candidates to become centers
        std::vector<int> NgL;
        NgL.reserve(n);
        for (int v = 0; v < n; ++v) {
            if (G[f][v] <= r && scores[v] > L && !centers[v]) {
                NgL.push_back(v);
            }
        }

        if (!NgL.empty() > 0) {
            assignment = distanceBasedSelectionConstant(NgL, r);
        } else {
            float maxScore = -1;
            int vMaxScore = 0;
            if (i > 0) {
                for (int j = 0; j < n; j++) {
                    if (!centers[j] && G[f][j] <= r) {
                        if (maxScore < scores[j]) {
                            maxScore = scores[j];
                            vMaxScore = j;
                        }
                    }
                }
            } else {
                vMaxScore = f;
            }

            std::vector<int> vertices;
            vertices.reserve(maxScore);
            for (int j = 0; j < n; j++) {
                if (!assigned[j] && G[vMaxScore][j] <= r) {
                    vertices.push_back(j);
                }
            }
            assignment.first = vMaxScore;
            assignment.second = vertices;
        }

        // check center and vertices
        centers[assignment.first] = true;
        for (int v : assignment.second) {
            assigned[v] = true;
        }
        capacities[i] -= assignment.second.size();
        unassignedCount -= assignment.second.size();

        // update scores
        for (int v : assignment.second) {
            for (int j = 0; j < n; j++) {
                if (G[v][j] <= r) {
                    scores[j]--;
                }
            }
        }

        C.push_back(assignment.first);
        assignments.push_back(assignment.second);
        updateDistances(assignment.first);
    }

    // put unassigned vertices
    assignMissingVertices(C, assignments);
    for (int l = 0; l < k; ++l) {
        A.insert({C[l], assignments[l]});
    }
    return A;
}

std::map<int, std::vector<int>> CkcSolver::solve() {

    int high = w.size();
    int low = -1;

    std::map<int, std::vector<int>> A;
    float coverRadius = +INFINITY;

    while (high - low > 1) {

        int mid = (high + low) / 2;
        float r = w[mid];
        std::map<int, std::vector<int>> APrime;
        for (int iter = 0; iter < numRepetitions; iter++) {
            srand(seed++);

            init();
            APrime = getFeasibleSolution(r, iter);
            float coverRadiusTmp = getRadio(APrime);

            if (coverRadiusTmp <= coverRadius) {
                coverRadius = coverRadiusTmp;
                A = APrime;
            }
        }

        if (coverRadius <= w[mid]) {
            high = mid;
        } else {
            low = mid;
        }
    }

    return A;
}