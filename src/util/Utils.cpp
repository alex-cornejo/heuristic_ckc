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

using namespace std;

vector<vector<int>> Utils::loadGEucSpace(const string &file_path) {
    auto vertices = readVList(file_path);
    int n = vertices.size();
    vector<vector<int>> D(n, vector<int>(n));

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i + 1; j < n; ++j) {

            auto v1 = vertices[i];
            auto v2 = vertices[j];

            // computing euclidean distance
            vector<float> subtracted;
            subtracted.reserve(v1.size());
            transform(v1.begin(), v1.end(), v2.begin(), back_inserter(subtracted),
                      [](float a, float b) { return pow(a - b, 2); });
            double d = sqrt(accumulate(subtracted.begin(), subtracted.end(), 0.0));
            D[j][i] = D[i][j] = (int) (d + 0.5);
        }
    }
    return D;
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

vector<vector<int>> Utils::loadGMetricSpace(int n, const string &filePath) {
    vector<vector<int>> C(n, vector<int>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            C[i][j] = i != j ? numeric_limits<int>::max() : 0;
        }
    }

    string line;
    ifstream file(filePath);

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

            int v1 = stoi(line_vec[0]) - 1;
            int v2 = stoi(line_vec[1]) - 1;
            int w = stoi(line_vec[2]);

            C[v1][v2] = w;
            C[v2][v1] = w;
        }
        file.close();
    } else {
        std::cerr << "Unable to open file" << std::endl;
    }
    floydWarshall(C);
    return C;
}

tuple<vector<vector<int>>, int, int> Utils::loadORLIB(const string &filePath) {


    std::fstream file(filePath, std::ios_base::in);
    if (!file.is_open()) {
        throw invalid_argument("Unable to open instance file");
    }

    int n, m, k;
    file >> n >> m >> k;

    vector<vector<int>> C(n, vector<int>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            C[i][j] = i != j ? numeric_limits<int>::max() : 0;
        }
    }

    for (int i = 0; i < m; ++i) {
        int v, u, w;
        file >> v >> u >> w;
        v--;
        u--;
        C[u][v] = C[v][u] = w;
    }
    file.close();
    floydWarshall(C);
    return make_tuple(C, n, k);
}

void Utils::floydWarshall(vector<vector<int>> &C) {
    int n = C.size();
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            for (int l = 0; l < n; ++l) {
                int cost = (C[i][j] == numeric_limits<int>::max() || C[i][l] == numeric_limits<int>::max()) ?
                           numeric_limits<int>::max() : C[i][j] + C[i][l];
                if (cost < C[j][l]) {
                    C[j][l] = cost;
                }
            }
        }
    }
}

float Utils::stdDev(std::vector<int> &items, float average) {
    float std = 0;
    for (float item: items) {
        std += pow(item - average, 2);
    }
    int n = items.size() > 2 ? items.size() - 1 : items.size();
    return sqrt(std / n);
}

bool Utils::save(std::string &output_path, std::string &content) {
    std::ofstream output_file(output_path);
//    output_file.write(&content, content.size());
    output_file << content;
    output_file.close();
    return true;
}
