//
// Created by alex on 26/05/20.
//

#include "CkcSolver.h"
#include <algorithm>
#include <iostream>
#include <mpi.h>
#include <queue>
#include <limits>

using namespace std;

CkcSolver::CkcSolver(int k, int L, const vector<vector<int>> &G, int rep) :
        k(k), L(L), G(G), rep(rep) {
    n = G.size();
    loadEdges();
    loadRefMatrix();

    //init assigned vertices
    assigned.resize(n);

    //init capacities
    capacities.resize(n);

    //reset distances for all v to C
    distances.resize(n);

    //reset scores
    scores.resize(n);

    // init parallel measures variables
    timeDBS = 0;
    callsDBSCount = 0;
    sumClosedN = 0;

    reset();
}

void CkcSolver::reset() {
    unassignedCount = n;

    //reset assigned vertices
    fill(assigned.begin(), assigned.end(), false);

    //reset capacities
    fill(capacities.begin(), capacities.end(), L);

    //reset distances for all v to C
    fill(distances.begin(), distances.end(), numeric_limits<int>::max());
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

//void CkcSolver::computeScore(int r) {
//
//    //reset scores
//    fill(scores.begin(), scores.end(), 0);
//
//    // start time
//    clock_t begin = clock();
//
//    for (int i = 0; i < n; i++) {
//        for (int j = 0; j < n; j++) {
//            if (i != j && G[i][j] <= r)
//                scores[i]++;
//        }
//    }
//
//    clock_t end = clock();
//    double time_spent = (double) (end - begin) / CLOCKS_PER_SEC;
//    timeDBS += time_spent;
//}

void CkcSolver::computeScore(int r) {

    // start time
    clock_t begin = clock();

    int batch_size = n / mpi_size;
    int mpi_mod = n % mpi_size;
    int mpi_low_idx = mpi_rank * batch_size + (mpi_rank <= mpi_mod ? mpi_rank : mpi_mod);
    int mpi_high_idx = mpi_low_idx + batch_size + (mpi_rank < mpi_mod ? 1 : 0);

    vector<int> score_rank(mpi_high_idx - mpi_low_idx);

    for (int i = mpi_low_idx; i < mpi_high_idx; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j && G[i][j] <= r)
                score_rank[i - mpi_low_idx]++;
        }
    }

    int recvcounts[mpi_size];
    int displs[mpi_size];

    for (int tmp_rank = 0; tmp_rank < mpi_size; ++tmp_rank) {
        int tmp_low_idx = tmp_rank * batch_size + (tmp_rank <= mpi_mod ? tmp_rank : mpi_mod);
        int tmp_high_idx = tmp_low_idx + batch_size + (tmp_rank < mpi_mod ? 1 : 0);
        recvcounts[tmp_rank] = (tmp_high_idx - tmp_low_idx);
        displs[tmp_rank] = tmp_rank == 0 ? 0 : (displs[tmp_rank - 1] + recvcounts[tmp_rank - 1]);
    }

    MPI_Allgatherv(score_rank.data(), score_rank.size(), MPI_INT, scores.data(), recvcounts, displs,
                   MPI_INT,
                   MPI_COMM_WORLD);

    clock_t end = clock();
    double time_spent = (double) (end - begin) / CLOCKS_PER_SEC;
    timeDBS += time_spent;
}

//void CkcSolver::updateScore(pair<int, vector<int>> &ca, int r) {
////    // start time
////    clock_t begin = clock();
//    for (int v: ca.second) {
//        for (int j = 0; j < n; j++) {
//            if (v != j && G[v][j] <= r) {
//                scores[j]--;
//            }
//        }
//    }
//    int v = ca.first;
//    for (int j = 0; j < n; j++) {
//        if (v != j && G[v][j] <= r)
//            scores[j]--;
//    }
//
////    clock_t end = clock();
////    double time_spent = (double) (end - begin) / CLOCKS_PER_SEC;
////    timeDBS += time_spent;
//}

void CkcSolver::updateScore(pair<int, vector<int>> &ca, int r) {
    // start time
    clock_t begin = clock();

    if (true) {
        int batch_size = n / mpi_size;
        int mpi_mod = n % mpi_size;
        int mpi_low_idx = mpi_rank * batch_size + (mpi_rank <= mpi_mod ? mpi_rank : mpi_mod);
        int mpi_high_idx = mpi_low_idx + batch_size + (mpi_rank < mpi_mod ? 1 : 0);

        vector<int> score_subtraction(mpi_high_idx - mpi_low_idx);
        for (int v: ca.second) {
            assigned[v] = true; // vertices covered by center are checked as assigned
            for (int j = mpi_low_idx; j < mpi_high_idx; j++) {
                if (v != j && G[v][j] <= r) {
                    score_subtraction[j - mpi_low_idx]--;
                }
            }
        }

        int recvcunt = n;
        int all_data[recvcunt];
        int recvcounts[mpi_size];
        int displs[mpi_size];

        for (int tmp_rank = 0; tmp_rank < mpi_size; ++tmp_rank) {
            int tmp_low_idx = tmp_rank * batch_size + (tmp_rank <= mpi_mod ? tmp_rank : mpi_mod);
            int tmp_high_idx = tmp_low_idx + batch_size + (tmp_rank < mpi_mod ? 1 : 0);
            recvcounts[tmp_rank] = (tmp_high_idx - tmp_low_idx);
            displs[tmp_rank] = tmp_rank == 0 ? 0 : (displs[tmp_rank - 1] + recvcounts[tmp_rank - 1]);
        }

        MPI_Allgatherv(score_subtraction.data(), score_subtraction.size(), MPI_INT, all_data, recvcounts, displs,
                       MPI_INT,
                       MPI_COMM_WORLD);

        int v = ca.first;
        for (int i = 0; i < recvcunt; ++i) {
            // 1st: if i is neighbor of v, then it must be subtracted
            // 2nd: only subtraction of clients is performed
            scores[i] += (v != i && G[v][i] <= r ? all_data[i] - 1 : all_data[i]);
        }

    } else {
        for (int v: ca.second) {
            assigned[v] = true; // vertices covered by center are checked as assigned
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

    clock_t end = clock();
    double time_spent = (double) (end - begin) / CLOCKS_PER_SEC;
    timeDBS += time_spent;
}

void CkcSolver::updateDistances(int c) {

    for (int i = 0; i < n; i++) {
        distances[i] = min(G[c][i], distances[i]);
    }
}

void CkcSolver::loadRefMatrix() {
    refMatrix.clear();
    refMatrix.resize(n);
    vector<vector<int>> vertexReferences(n, vector<int>(2));
    for (int i = 0; i < n; i++) {
        int ir = 0;
        for (int j = n - 1; j >= 0; j--, ir++) {
            vertexReferences[ir] = {j, G[i][j]};
        }
        sort(vertexReferences.begin(), vertexReferences.end(),
             [](const vector<int> &v1, const vector<int> &v2) { return v1[1] > v2[1]; });
        vector<int> references(n);
        for (int j = 0; j < n; j++) {
            references[j] = vertexReferences[j][0];
        }
        refMatrix[i] = references;
    }
}

int CkcSolver::getFVertex(int idxK, int iter) {
    int f = 0;
    if (idxK > 0) {

        // get farthest vertex
        int maxDist = -1;
        for (int i = 0; i < n; i++) {
            int dist = distances[i];
            if (maxDist < dist && !assigned[i]) {
                maxDist = dist;
                f = i;
            }
        }
    } else {
        f = (rep == n) ? iter : rand() % n;
    }
    return f;
}

void CkcSolver::assignMissingVertices(vector<int> &C, vector<vector<int>> &A) {

    for (int j = 0; j < n; j++) {
        if (!assigned[j]) {
            int minDist = numeric_limits<int>::max();
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

pair<int, vector<int>> CkcSolver::distanceBasedSelection(vector<int> &NgL, int r) {

    callsDBSCount += 1;
    sumClosedN += NgL.size();

    // start time
    clock_t begin = clock();

    pair<int, vector<int>> ca;
//    if (NgL.size() >= mpi_size * 2) {
    if (true) {
        int batch_size = NgL.size() / mpi_size;
        int mpi_mod = NgL.size() % mpi_size;
        int mpi_low_idx = mpi_rank * batch_size + (mpi_rank <= mpi_mod ? mpi_rank : mpi_mod);
        int mpi_high_idx = mpi_low_idx + batch_size + (mpi_rank < mpi_mod ? 1 : 0);

        int d = numeric_limits<int>::max();
        for (int i = mpi_low_idx; i < mpi_high_idx; ++i) {
            int v = NgL[i];
            // get v_ref vertex
            int vref = -1;
            int maxDist = -1;
            for (int j = 0; j < n; j++) {
                int dist = min(distances[j], G[v][j]);
                if (maxDist < dist && !assigned[j]) {
                    maxDist = dist;
                    vref = j;
                }
            }

            // get unassigned neighbors of v
            vector<int> vertices;
            vertices.reserve(L + 2);
            vertices.resize(L);
            int dv = 0;
            int iu = 0;
            for (int u: refMatrix[vref]) {
                // 1st: avoid v to be assigned to itself
                // 2nd: Pruned graph
                // 3rd: u is not assigned
                if (u != v && G[v][u] <= r && !assigned[u]) {
                    if (iu == L) {
                        dv = G[vref][u];
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
        if (d != numeric_limits<int>::max()) {
            ca.second.push_back(d);
            ca.second.push_back(ca.first);
        }
        int process_used = mpi_size <= NgL.size() ? mpi_size : mpi_mod;
        int recvcounts[mpi_size];
        int displs[mpi_size];

        int mpi_recvcount = L + 2;
        for (int i = 0; i < mpi_size; ++i) {
            if (i < process_used) {
                recvcounts[i] = L + 2;
                displs[i] = i * (L + 2);
            } else {
                recvcounts[i] = 0;
                displs[i] = 0;
            }
        }

        int all_data[process_used * mpi_recvcount];
        MPI_Allgatherv(ca.second.data(), ca.second.size(), MPI_INT, all_data, recvcounts, displs, MPI_INT,
                       MPI_COMM_WORLD);

        vector<int> vertices;
        vertices.reserve(L);
        int prevD = numeric_limits<int>::max();
        int v;
        int posV = -1;
        for (int i = L + 1; i < process_used * mpi_recvcount; i += (L + 2)) {
            if (all_data[i - 1] < prevD) {
                prevD = all_data[i - 1];
                v = all_data[i];
                posV = i;
            }
        }
        for (int i = posV - L - 1; i < posV - 1; ++i) {
            vertices.push_back(all_data[i]);
        }
        ca.first = v;
        ca.second = vertices;

    } else {
        int d = numeric_limits<int>::max();
        for (int v: NgL) {

            // get f_ref vertex
            int fref = 0;
            int maxDist = -1;
            for (int j = 0; j < n; j++) {

                int dist = min(distances[j], G[j][v]);
                if (maxDist < dist) {
                    maxDist = dist;
                    fref = j;
                }
            }

            // get unassigned neighbors of v
            vector<int> vertices(L);
            int dv = 0;
            int iu = 0;
            for (int u: refMatrix[fref]) {
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
    }
    clock_t end = clock();
    double time_spent = (double) (end - begin) / CLOCKS_PER_SEC;
    timeDBS += time_spent;
    return ca;
}

int CkcSolver::getRadio(pair<vector<int>, vector<vector<int>>> &A) {
    int maxDist = -1;
    for (int idxC = 0; idxC < A.first.size(); ++idxC) {
        int c = A.first[idxC];
        for (int i: A.second[idxC]) {
            if (G[i][c] > maxDist)
                maxDist = G[i][c];
        }
    }
    return maxDist;
}

pair<vector<int>, vector<vector<int>>> CkcSolver::getFeasibleSolution(int r, int iter) {
    computeScore(r);

    vector<int> C;
    C.reserve(k);
    vector<vector<int>> A;
    A.reserve(k);

    for (int idxK = 0; idxK < k && unassignedCount > 0; idxK++) {

        pair<int, vector<int>> ca;

        int f = getFVertex(idxK, iter);

        // get N(f)U{f} with score > L ... candidates to become centers
        // if not vertex with score > L is found, the algorithm continues
        // by processing vertex with biggest score
        vector<int> NgL;
        NgL.reserve(unassignedCount);
        int maxScore = -1;
        int vMaxScore = -1;
        for (int v = 0; v < n; ++v) {
            // 1st: v is not assigned
            // 2nd: Pruned graph
            if (!assigned[v] && G[f][v] <= r) {
                // looking for vertices with Score > L
                if (scores[v] > L) {
                    NgL.push_back(v);
                }
                // looking for vertex with max Score
                if (maxScore < scores[v]) {
                    maxScore = scores[v];
                    vMaxScore = v;
                }
            }
        }

        if (!NgL.empty()) {
            ca = distanceBasedSelection(NgL, r);
        } else {
            // first center is taken randomly
            if (idxK == 0) {
                vMaxScore = f;
                maxScore = scores[f];
            }

            vector<int> vertices;
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

        //center is checked as assigned to avoid to be considered in future
        // vertices covered by center are checked as assigned in the updateScore function
        assigned[ca.first] = true;

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

tuple<pair<vector<int>, vector<vector<int>>>, int> CkcSolver::solve() {

    int high = w.size() - 1;
    int low = 0;

    pair<vector<int>, vector<vector<int>>> CA;
    int real_r = numeric_limits<int>::max();

    while (low <= high) {

        int mid = (high + low) / 2;
        int r = w[mid];
        pair<vector<int>, vector<vector<int>>> CA_tmp;
        for (int iter = 0; iter < rep; iter++) {
//            std::cout<<seed<<std::endl;
            srand(seed++);

            reset();
            CA_tmp = getFeasibleSolution(r, iter);
            int real_r_tmp = getRadio(CA_tmp);

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

//int CkcSolver::addMissingCenters(pair<vector<int>, vector<vector<int>>> &CA, int r) {
//
//    int q = k - CA.first.size();
//    vector<vector<int>> dV2C;
//    dV2C.reserve(n - k);
//    for (int idxC = 0; idxC < CA.first.size(); ++idxC) {
//        int c = CA.first[idxC];
//        for (int i: CA.second[idxC]) {
//            // idxCenter; vertex; distance(vertex, center);
//            dV2C.push_back({idxC, i, G[c][i]});
//        }
//    }
//
//    sort(dV2C.begin(), dV2C.end(),
//         [](auto &v1, auto &v2) { return v1[2] > v2[2]; });
//    r = dV2C[q][2];
//    for (int i = 0; i < q; ++i) {
//        int v = dV2C[i][1];
//        int idxC = dV2C[i][0];
//        CA.first.push_back(v);   //convert v to center
//        CA.second.emplace_back();
//
//        // remove assignment of v
//        CA.second[idxC].erase(remove(CA.second[idxC].begin(), CA.second[idxC].end(), v), CA.second[idxC].end());
//    }
//    return r;
//}

typedef tuple<int, int, int> pi;

class PQI : public priority_queue<pi, vector<pi>, greater<pi> > {
public:
    vector<pi> &impl() { return c; }
};

int CkcSolver::addMissingCenters(pair<vector<int>, vector<vector<int>>> &CA, int r) {

    PQI pq;
    int q = k - CA.first.size();

    for (int idxC = 0; idxC < CA.first.size(); ++idxC) {
        int c = CA.first[idxC];
        int idx = 0;
        for (int i: CA.second[idxC]) {

            if (pq.size() < q) {
                pq.push(make_tuple(G[c][i], idx, idxC));
            } else {
                int tmp_d, tmp_idx, tmp_idxC;
                tie(tmp_d, tmp_idx, tmp_idxC) = pq.top();

                if (tmp_d < G[c][i]) {
                    pq.pop();
                    pq.push(make_tuple(G[c][i], idx, idxC));
                }
            }
            idx++;
        }
    }

    for (auto it = pq.impl().begin(); it != pq.impl().end(); ++it) {
        int tmp_d, tmp_idx, tmp_idxC;
        tie(tmp_d, tmp_idx, tmp_idxC) = *it;

        CA.first.push_back(CA.second[tmp_idxC][tmp_idx]);   //convert v to center
        CA.second.emplace_back();

        // remove assignment of v
        CA.second[tmp_idxC][tmp_idx] = CA.second[tmp_idxC].back();
        CA.second[tmp_idxC].pop_back();
    }
    return r;
}

int CkcSolver::farthest_distance(int c, int v, vector<int> &X) {
    int farthest = G[c][v];
    for (int i: X) {
        if (G[v][i] > farthest) {
            farthest = G[v][i];
        }
    }
    return farthest;
}

int CkcSolver::alternate_heuristic(pair<vector<int>, vector<vector<int>>> &CA) {
    int max_r = -1;
    for (int idxC = 0; idxC < CA.first.size(); ++idxC) {
        int c = CA.first[idxC];
        int new_c = c;
        int idx_new_c = -1;
        int min_distance = farthest_distance(c, c, CA.second[idxC]);
        int idx = 0;
        for (int i: CA.second[idxC]) {
            int tmp_distance = farthest_distance(c, i, CA.second[idxC]);
            if (tmp_distance < min_distance) {
                new_c = i;
                min_distance = tmp_distance;
                idx_new_c = idx;
            }
            idx++;
        }
        if (c != new_c) {
            CA.first[idxC] = new_c;
            CA.second[idxC][idx_new_c] = c;
        }
        if (max_r < min_distance) max_r = min_distance;
    }
    return max_r;
}

long CkcSolver::averageCardClosedN() {
    return sumClosedN / callsDBSCount;
}

double CkcSolver::getTimeDBS() const {
    return timeDBS;
}

void CkcSolver::resetParallelMeasure() {
    timeDBS = 0;
}

void CkcSolver::setSeed(int seed) {
    this->seed = seed;
}

int CkcSolver::getSeed() {
    return seed;
}
