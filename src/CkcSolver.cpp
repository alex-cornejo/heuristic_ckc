//
// Created by alex on 26/05/20.
//

#include "CkcSolver.h"
#include <algorithm>
#include <cmath>

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
            if (i != j) {
                if (G[i][j] <= r) scores[i]++;
            }
        }
    }
}

void CkcSolver::updateScore(std::pair<int, std::vector<int>> &ca, float r) {

    for (int v : ca.second) {
        for (int j = 0; j < n; j++) {
            if (v != j && G[v][j] <= r) {
                scores[j]--;
            }
        }
    }
    int v = ca.first;
    for (int j = 0; j < n; j++) {
        if (v != j && G[v][j] <= r) {
            scores[j]--;
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
        int ir = 0;
        for (int j = n - 1; j >= 0; j--, ir++) {
            vertexReferences[ir] = {(float) j, G[i][j]};
        }
        std::sort(vertexReferences.begin(), vertexReferences.end(),
                  [](auto &v1, auto &v2) { return v1[1] > v2[1]; });
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
        float maxDist = -1;
        for (int i = 0; i < n; i++) {
            float dist = distances[i];
            if (maxDist < dist && !assigned[i]) {
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

    std::pair<int, std::vector<int>> ca;
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
            // 1st: avoid v to be assigned to itself
            // 2nd: Pruned graph
            // 3rd: u is not assigned
            if (u != v && G[v][u] <= r && !assigned[u]) {
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
            ca.first = v;
            ca.second = vertices;
        }
    }
    return ca;
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
    std::map<int, std::vector<int>> CA;
    std::vector<int> C;
    std::vector<std::vector<int>> A;
    for (int i = 0; i < k && unassignedCount > 0; i++) {

        std::pair<int, std::vector<int>> ca;

        int f = getFVertex(C, iter);

        // get N(f)U{f} with score > L ... candidates to become centers
        std::vector<int> NgL;
        NgL.reserve(n);
        for (int v = 0; v < n; ++v) {
            // 1st: v is not assigned
            // 2nd: Pruned graph
            // 3rd: Score > L
            // 4th: v is not a center
            if (!assigned[v] && G[f][v] <= r && scores[v] > L && !centers[v]) {
                NgL.push_back(v);
            }
        }

        if (!NgL.empty()) {
            ca = distanceBasedSelectionConstant(NgL, r);
        } else {
            float maxScore = -1;
            int vMaxScore = -1;
            if (i > 0) {
                for (int j = 0; j < n; j++) {
                    // 1st: j is not assigned
                    // 2nd: j is not a center
                    // 3rd: Pruned graph
                    if (!assigned[j] && !centers[j] && G[f][j] <= r) {
                        if (maxScore < scores[j]) {
                            maxScore = scores[j];
                            vMaxScore = j;
                        }
                    }
                }
            } else {
                vMaxScore = f;
                maxScore = 0;
            }

            std::vector<int> vertices;
            vertices.reserve(maxScore);
            for (int j = 0; j < n; j++) {
                // 1st: j is not the center
                // 2nd: j is not assigned
                // 3rd: Pruned graph
                if (j != vMaxScore && !assigned[j] && G[vMaxScore][j] <= r) {
                    vertices.push_back(j);
                }
            }
            ca.first = vMaxScore;
            ca.second = vertices;
        }

        centers[ca.first] = true; // center is checked as center
        assigned[ca.first] = true; //center is checked as assigned

        // vertices covered by center are checked as assigned
        for (int v : ca.second) {
            assigned[v] = true;
        }
        capacities[i] -= ca.second.size(); // capacity of center i-th is reduced
        unassignedCount -= ca.second.size() + 1; // +1 center is not considered

        updateScore(ca, r);

        C.push_back(ca.first);
        A.push_back(ca.second);
        updateDistances(ca.first);
    }

    // put unassigned vertices
    if (unassignedCount > 0) {
        assignMissingVertices(C, A);
    }
    for (int l = 0; l < C.size(); ++l) {
        CA.insert({C[l], A[l]});
    }
    return CA;
}

std::tuple<std::map<int, std::vector<int>>, float> CkcSolver::solve() {

    int high = w.size() - 1;
    int low = 0;

    std::map<int, std::vector<int>> A;
    float coverRadius = +INFINITY;

    while (low <= high) {

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
        if (coverRadius <= r) high = mid - 1;
        else low = mid + 1;
    }

    if (A.size() < k)
        coverRadius = addMissingCenters(A, coverRadius);

    return std::make_tuple(A, coverRadius);
}

float CkcSolver::addMissingCenters(std::map<int, std::vector<int>> &A, float r) {

    int missingCenters = k - A.size();
    std::vector<std::vector<float>> dV2C;
    dV2C.reserve(n);
    for (auto &a : A) {
        for (int v : a.second) {
            dV2C.push_back({(float) a.first, (float) v, G[a.first][v]});
            // center; vertex; distance(vertex, center)
        }
    }
    std::sort(dV2C.begin(), dV2C.end(),
              [](auto &v1, auto &v2) { return v1[2] > v2[2]; });
    if (dV2C[missingCenters][2] < r) {
        r = dV2C[missingCenters][2];
        for (int i = 0; i < missingCenters; ++i) {
            int v = dV2C[i][1];
            int c = dV2C[i][0];

            // convert v in center
            A.insert({v, {}});

            // remove v from assignment
            A[c].erase(std::remove(A[c].begin(), A[c].end(), v), A[c].end());
        }
    }
    return r;
}