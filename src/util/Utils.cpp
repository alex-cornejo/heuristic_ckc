//
// Created by alex on 26/05/20.
//

#include <cmath>
#include <vector>
#include <string>
#include <iostream>
#include <numeric>
#include <boost/algorithm/string.hpp>
#include <fstream>
#include "Utils.h"

std::vector<std::vector<float>> Utils::loadGEucSpace(const std::string &file_path) {
    auto vertices = readVList(file_path);
    int n = vertices.size();
    std::vector<std::vector<float>> graph(n, std::vector<float>(n));

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i + 1; j < n; ++j) {

            auto v1 = vertices[i];
            auto v2 = vertices[j];

            // computing euclidean distance
            std::vector<float> subtracted(v1.size());
            std::transform(v1.begin(), v1.end(), v2.begin(), std::back_inserter(subtracted),
                           [](float a, float b) { return pow(a - b, 2); });
            float d = sqrt(std::accumulate(subtracted.begin(), subtracted.end(), 0.0f));

            graph[i][j] = d;
            graph[j][i] = d;
        }
    }
    return graph;
}

std::vector<std::vector<float>> Utils::readVList(const std::string &file_path) {

    std::vector<std::vector<float>> vertices;
    std::string line;
    std::ifstream file(file_path);

    if (file.is_open()) {

        getline(file, line);
        boost::trim_right(line);
        boost::trim_left(line);

        std::vector<std::string> line_vec;
        boost::split(line_vec, line, boost::is_any_of(" "));

        if (line_vec.size() != 1 && line_vec.size() != 3) {
            std::cerr << "Error in line " << line << std::endl;
            throw;
        }

        std::vector<float> xy(2);
        if (line_vec.size() == 1) {
            int n = stoi(line);
            vertices.reserve(n);
        } else {
            std::transform(line_vec.begin() + 1, line_vec.end(), xy.begin(),
                           [](std::string const &val) { return stof(val); });
            vertices.push_back(xy);
        }

        while (getline(file, line)) {
            boost::trim_right(line);
            boost::trim_left(line);
            boost::split(line_vec, line, boost::is_any_of(" "));

            if (line_vec.size() != 3) {
                std::cerr << "Error in line " << line << std::endl;
                throw;
            }

            std::transform(line_vec.begin() + 1, line_vec.end(), xy.begin(),
                           [](std::string const &val) { return stof(val); });
            vertices.push_back(xy);
        }
        file.close();
    } else {
        std::cerr << "Unable to open file" << std::endl;
    }
    return vertices;
}

std::vector<std::vector<float>> Utils::loadGMetricSpace(int n, const std::string &file_path) {
    std::vector<std::vector<float>> G(n, std::vector<float>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            G[i][j] = i != j ? +INFINITY : 0;
        }
    }

    std::string line;
    std::ifstream file(file_path);

    if (file.is_open()) {

        std::vector<std::string> line_vec;
        while (getline(file, line)) {
            boost::trim_right(line);
            boost::trim_left(line);
            boost::split(line_vec, line, boost::is_any_of(" "));

            if (line_vec.size() != 3) {
                std::cerr << "Error in line " << line << std::endl;
                throw;
            }

            int v1 = stoi(line_vec[0])-1;
            int v2 = stoi(line_vec[1])-1;
            float w = stof(line_vec[2]);

            G[v1][v2] = w;
            G[v2][v1] = w;
        }
        file.close();
    } else {
        std::cerr << "Unable to open file" << std::endl;
    }
    floydWarshall(G);
    return G;
}

void Utils::floydWarshall(std::vector<std::vector<float>> &G) {
    int n = G.size();
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            for (int l = 0; l < n; ++l) {
                float cost = (G[i][j] == +INFINITY || G[i][l] == +INFINITY) ?
                             +INFINITY : G[i][j] + G[i][l];
                if (cost < G[j][l]) {
                    G[j][l] = cost;
                }
            }
        }
    }
}

float Utils::stdDev(std::vector<float> &items, float average) {
    float std = 0;
    for (float item : items) {
        std += pow(item - average, 2);
    }
    int n = items.size() > 2 ? items.size() - 1 : items.size();
    return sqrt(std / n);
}