#include <iostream>
#include <chrono>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>
#include "util/Utils.h"
#include "CkcSolver.h"
#include "model/KCSolution.h"
#include <mpi.h>
#include <queue>

using namespace std;
int mpi_size;
int mpi_rank;

KCSolution toKCModel(pair<vector<int>, vector<vector<int>>> &A) {
    KCSolution kcSolution;

    for (int idxC = 0; idxC < A.first.size(); ++idxC) {
        int c = A.first[idxC];
        Center center(c, A.second[idxC]);
        kcSolution.addCenter(center);
    }
    return kcSolution;
}

void validateSolution(KCSolution &kcSolution, int n, int L) {
    bool valid = true;
    vector<int> repetitions(n);
    for (auto &c: kcSolution.getCenters()) {
        if (c.getNodes().size() > L)
            cerr << "Center " << c.getCenter() << " has " << c.getNodes().size() << " nodes assigned !!!"
                 << endl;
        repetitions[c.getCenter()]++;
        for (int i: c.getNodes()) {
            repetitions[i]++;
        }
    }
    for (int i = 0; i < n; i++) {
        if (repetitions[i] != 1) {
            valid = false;
            cout << "vertex " << i << " is assigned " << repetitions[i] << " times." << endl;
        }
    }
    if (!valid) {
        cout << "Solution is INVALID " << endl;
    }
}

void execute(string &instancePath, int n, int k, int L,
             int maxIter, int numRepetitions, bool printable, string &instanceFormat) {

    vector<vector<int>> G;
    if (instanceFormat == "tsplib") {
        G = Utils::loadGEucSpace(instancePath);
    } else if (instanceFormat == "orlib") {
//        G = Utils::loadGMetricSpace(n, instancePath);
    } else {
        cerr << "instance format " << instanceFormat << " is not supported!!!" << endl;
        return;
    }

    float solutionSizeSum = 0;
    float totalTime = 0;

    vector<float> solutionSizeArr(maxIter);
    CkcSolver solver(k, L, G, numRepetitions);
    solver.mpi_rank = mpi_rank;
    solver.mpi_size = mpi_size;

    pair<vector<int>, vector<vector<int>>> bestAssignment;
    float bestFitness = +INFINITY;
    for (int i = 0; i < maxIter; i++) {

        // start time
        clock_t begin = clock();

        pair<vector<int>, vector<vector<int>>> A;
        float solutionSizeTmp;
        tie(A, solutionSizeTmp) = solver.solve();

        // finish time
        clock_t end = clock();
        double time_spent = (double) (end - begin) / CLOCKS_PER_SEC;

        totalTime += time_spent;
        solutionSizeSum += solutionSizeTmp;
        solutionSizeArr[i] = solutionSizeTmp;

        if (solutionSizeTmp < bestFitness) {
            bestFitness = solutionSizeTmp;
            bestAssignment = A;
        }
    }

    if (mpi_rank == 0) {
        auto average = solutionSizeSum / maxIter;
        cout << bestFitness << "," << average << "," << Utils::stdDev(solutionSizeArr, average) << ","
             << (totalTime / maxIter) << endl;

        KCSolution kcSolution = toKCModel(bestAssignment);
        kcSolution.setInstance(instancePath);
        validateSolution(kcSolution, n, L);

        if (printable) {
            cout << kcSolution.toJson() << endl;
        }
    }
}

int main(int argc, char **argv) {

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

    string instancePath = argv[1];
    int n = atoi(argv[2]);
    int k = atoi(argv[3]);
    int L = atoi(argv[4]);
    int maxIter = atoi(argv[5]);
    int numRepetitions = atoi(argv[6]);
    bool printable = strcmp(argv[7], "true") == 0;
    string instanceFormat = argv[8];

    execute(instancePath, n, k, L, maxIter, numRepetitions, printable, instanceFormat);

    MPI_Finalize();

    return 0;
}