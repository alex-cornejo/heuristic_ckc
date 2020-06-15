#include <iostream>
#include <cmath>
#include <chrono>
#include "util/Utils.h"
#include "CkcSolver.h"
#include "model/KCSolution.h"

KCSolution toKCModel(std::map<int, std::vector<int>> &A) {
    KCSolution kcSolution;
    for (auto &assignment : A) {
        Center center;
        center.setCenter(assignment.first);
        center.setNodes(assignment.second);
        kcSolution.addCenter(center);
    }
    return kcSolution;
}

void validateSolution(KCSolution &kcSolution, int n) {
    bool valid = true;
    std::vector<int> repetitions(n);
    for (auto &c : kcSolution.getCenters()) {
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

void execute(std::string &instancePath, int n, int k, int L, float out, bool multicenter,
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
    std::map<int, std::vector<int>> bestAssignment;
    float bestFitness = +INFINITY;
    for (int i = 0; i < maxIter; i++) {

        // start time
        clock_t begin = clock();

        auto A = solver.solve();

        // finish time
        clock_t end = clock();
        double time_spent = (double) (end - begin) / CLOCKS_PER_SEC;

        auto solutionSizeTmp = solver.getRadio(A);
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
    validateSolution(kcSolution, n);

    if (printable) {
        std::cout << kcSolution.toJson() << std::endl;
    }
}

int main(int argc, char **argv) {

    std::string instancePath = argv[1];
    int n = atoi(argv[2]);
    int k = atoi(argv[3]);
    int L = atoi(argv[4]);
    float out = atof(argv[5]);
    bool multicenter = strcmp(argv[6], "true") == 0;
    int maxIter = atoi(argv[7]);
    int numRepetitions = atoi(argv[8]);
    bool printable = strcmp(argv[9], "true") == 0;
    std::string instanceFormat = argv[10];

    execute(instancePath, n, k, L, out, multicenter, maxIter, numRepetitions, printable, instanceFormat);

    return 0;
}
