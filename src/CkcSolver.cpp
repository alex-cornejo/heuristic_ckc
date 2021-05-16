//
// Created by alex on 26/05/20.
//

#include "CkcSolver.h"
#include <algorithm>
#include <cmath>
#include <iostream>

using namespace std;

CkcSolver::CkcSolver(int k, int L, const std::vector<std::vector<float>> &G, int numRepetitions) :
        k(k), L(L), G(G), numRepetitions(numRepetitions) {
    n = G.size();
    loadEdges();
    loadRefMatrix();

    //init assigned vertices
    assigned.resize(n);

    //init assigned centers
    centers.resize(n);

    //init capacities
    capacities.resize(n);

    //reset distances for all v to C
    distances.resize(n);

    //reset scores
    scores.resize(n);

    reset();
}

void CkcSolver::reset() {
    unassignedCount = n;

    //reset assigned vertices
    std::fill(assigned.begin(), assigned.end(), false);

    //reset assigned centers
    std::fill(centers.begin(), centers.end(), false);

    //reset capacities
    std::fill(capacities.begin(), capacities.end(), L);

    //reset distances for all v to C
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
    std::fill(scores.begin(), scores.end(), 0);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j && G[i][j] <= r)
                scores[i]++;
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
        if (v != j && G[v][j] <= r)
            scores[j]--;
    }
}

void CkcSolver::updateDistances(int c) {
    for (int i = 0; i < n; i++) {
        if (G[i][c] < distances[i])
            distances[i] = G[i][c];
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

int CkcSolver::getFVertex(int idxK, int iter) {
    int f = 0;
    if (idxK > 0) {

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

void CkcSolver::assignMissingVertices(vector<int> &C, vector<vector<int>> &A) {

    for (int j = 0; j < n; j++) {
        if (!assigned[j]) {
            float minDist = +INFINITY;
            int closestC = -1;
            int idxC = -1;
            for (int i = 0; i < C.size(); ++i) {
                int c = C[i];
                if (capacities[c] > 0 && G[c][j] < minDist) {
                    minDist = G[c][j];
                    closestC = c;
                    idxC = i;
                }
            }
            capacities[closestC]--;
            unassignedCount--;
            assigned[j] = true;
            A[idxC].push_back(j);
        }
    }
}

std::pair<int, std::vector<int>> CkcSolver::distanceBasedSelection(std::vector<int> &NgL, float r) {

    std::pair<int, std::vector<int>> ca;
    float d = +INFINITY;

    for (int v : NgL) {

        // get f_ref vertex
        int fref = 0;
        float maxDist = -1;
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

float CkcSolver::getRadio(pair<vector<int>, vector<vector<int>>> &A) {
    float maxDist = -1;
    for (int idxC = 0; idxC < A.first.size(); ++idxC) {
        int c = A.first[idxC];
        for (int i : A.second[idxC]) {
            if (G[i][c] > maxDist)
                maxDist = G[i][c];
        }
    }
    return maxDist;
}

pair<vector<int>, vector<vector<int>>> CkcSolver::getFeasibleSolution(float r, int iter) {
    computeScore(r);

    std::vector<int> C;
    C.reserve(k);
    std::vector<std::vector<int>> A;
    A.reserve(k);

    for (int idxK = 0; idxK < k && unassignedCount > 0; idxK++) {

        std::pair<int, std::vector<int>> ca;

        int f = getFVertex(idxK, iter);

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
            ca = distanceBasedSelection(NgL, r);
        } else {
            float maxScore = -1;
            int vMaxScore = -1;
            if (idxK > 0) {
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
        assigned[ca.first] = true; //center is checked as assigned to avoid to be considered in future

        // vertices covered by center are checked as assigned
        for (int v : ca.second) {
            assigned[v] = true;
        }

        capacities[ca.first] -= ca.second.size(); // capacity of center idxK-th is reduced
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
    return make_pair(C, A);
}

tuple<pair<std::vector<int>, vector<vector<int>>>, float> CkcSolver::solve() {

    int high = w.size() - 1;
    int low = 0;

    pair<vector<int>, vector<vector<int>>> CA;
    float real_r = +INFINITY;

    while (low <= high) {

        int mid = (high + low) / 2;
        float r = w[mid];
        pair<vector<int>, vector<vector<int>>> CA_tmp;
        for (int iter = 0; iter < numRepetitions; iter++) {
//            std::cout<<seed<<std::endl;
            srand(seed++);

            reset();
            CA_tmp = getFeasibleSolution(r, iter);
            float real_r_tmp = getRadio(CA_tmp);

            if (real_r_tmp <= real_r) {
                real_r = real_r_tmp;
                CA = CA_tmp;
            }
        }
        if (real_r <= r) high = mid - 1;
        else low = mid + 1;
    }

    if (CA.first.size() < k)
        real_r = addMissingCenters(CA, real_r);

    real_r = alternate_heuristic(CA);
    return make_tuple(CA, real_r);
}

float CkcSolver::addMissingCenters(pair<vector<int>, vector<vector<int>>> &A, float r) {

    int q = k - A.first.size();
    std::vector<std::vector<float>> dV2C;
    dV2C.reserve(n - k);
    for (int idxC = 0; idxC < A.first.size(); ++idxC) {
        int c = A.first[idxC];
        for (int i : A.second[idxC]) {
            // center; vertex; distance(vertex, center); idxCenter
            dV2C.push_back({(float) idxC, (float) i, G[c][i]});
        }
    }

    std::sort(dV2C.begin(), dV2C.end(),
              [](auto &v1, auto &v2) { return v1[2] > v2[2]; });
    r = dV2C[q][2];
    for (int i = 0; i < q; ++i) {
        int v = dV2C[i][1];
        int idxC = dV2C[i][0];
        A.first.push_back(v);   //convert v to center
        A.second.emplace_back();

        // remove assignment of v
        A.second[idxC].erase(remove(A.second[idxC].begin(), A.second[idxC].end(), v), A.second[idxC].end());

    }
    return r;
}

float CkcSolver::farthest_distance(int c, int v, vector<int> &X) {
    float farthest = G[c][v];
    for (int i: X) {
        if (G[v][i] > farthest) {
            farthest = G[v][i];
        }
    }
    return farthest;
}

float CkcSolver::alternate_heuristic(pair<vector<int>, vector<vector<int>>> &CA) {
    float max_r = -1;
    for (int idxC = 0; idxC < CA.first.size(); ++idxC) {
        int c = CA.first[idxC];
        int new_c = c;
        float min_distance = farthest_distance(c, c, CA.second[idxC]);
        for (int i : CA.second[idxC]) {
            float tmp_distance = farthest_distance(c, i, CA.second[idxC]);
            if (tmp_distance < min_distance) {
                new_c = i;
                min_distance = tmp_distance;
            }
        }
        if (c != new_c) {
            CA.second[idxC].erase(remove(CA.second[idxC].begin(), CA.second[idxC].end(), new_c), CA.second[idxC].end());
            CA.second[idxC].push_back(c);
            CA.first[idxC] = new_c;
        }
        if (max_r < min_distance) max_r = min_distance;
    }
    return max_r;
}