#include <iostream>
#include <chrono>
#include "util/Utils.h"
#include "CkcSolver.h"
#include "model/KCSolution.h"

KCSolution toKCModel(std::pair<std::vector<int>, std::vector<int>> &A) {
    KCSolution kcSolution;

    for (int c : A.first) {
        Center center;
        center.setCenter(c);
        for (int i = 0; i < A.second.size(); ++i) {
            if (A.second[i] == c)
                center.addNode(i);
        }
        kcSolution.addCenter(center);
    }
    return kcSolution;
}

void validateSolution(KCSolution &kcSolution, int n, int L) {
    bool valid = true;
    std::vector<int> repetitions(n);
    for (auto &c : kcSolution.getCenters()) {
        if (c.getNodes().size() > L)
            std::cerr << "Center " << c.getCenter() << " has " << c.getNodes().size() << " nodes assigned !!!"
                      << std::endl;
        repetitions[c.getCenter()]++;
        for (int i : c.getNodes()) {
            repetitions[i]++;
        }
    }
    for (int i = 0; i < n; i++) {
        if (repetitions[i] != 1) {
            valid = false;
            std::cout << "vertex " << i << " is assigned " << repetitions[i] << " times." << std::endl;
        }
    }
    if (!valid) {
        std::cout << "Solution is INVALID " << std::endl;
    }
}

void execute(std::string &instancePath, int n, int k, int L,
             int maxIter, int numRepetitions, bool printable, std::string &instanceFormat) {

    std::vector<std::vector<float>> G;
    if (instanceFormat == "tsplib") {
        G = Utils::loadGEucSpace(instancePath);
    } else if (instanceFormat == "orlib") {
        G = Utils::loadGMetricSpace(n, instancePath);
    } else {
        std::cerr << "instance format " << instanceFormat << " is not supported!!!" << std::endl;
        return;
    }

    float solutionSizeSum = 0;
    float totalTime = 0;

    std::vector<float> solutionSizeArr(maxIter);
    CkcSolver solver(k, L, G, numRepetitions);
    std::pair<std::vector<int>, std::vector<int>> bestAssignment;
    float bestFitness = +INFINITY;
    for (int i = 0; i < maxIter; i++) {

        // start time
        clock_t begin = clock();

        std::pair<std::vector<int>, std::vector<int>> A;
        float solutionSizeTmp;
        std::tie(A, solutionSizeTmp) = solver.solve();

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

    auto average = solutionSizeSum / maxIter;

    std::cout << "\nAverage size: " << average << std::endl;
    std::cout << "Standard deviation: " << Utils::stdDev(solutionSizeArr, average) << std::endl;
    std::cout << "\nTotal time: " << totalTime << std::endl;
    std::cout << "Time per running: " << (totalTime / maxIter) << std::endl;

    KCSolution kcSolution = toKCModel(bestAssignment);
    kcSolution.setInstance(instancePath);
    validateSolution(kcSolution, n, L);

    if (printable) {
        std::cout << std::endl << kcSolution.toJson() << std::endl;
    }
}

int main(int argc, char **argv) {
    std::string instancePath = argv[1];
    int n = atoi(argv[2]);
    int k = atoi(argv[3]);
    int L = atoi(argv[4]);
    int maxIter = atoi(argv[5]);
    int numRepetitions = atoi(argv[6]);
    bool printable = strcmp(argv[7], "true") == 0;
    std::string instanceFormat = argv[8];

    execute(instancePath, n, k, L, maxIter, numRepetitions, printable, instanceFormat);

    return 0;
}
