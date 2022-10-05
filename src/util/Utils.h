//
// Created by alex on 26/05/20.
//

#ifndef CKC_CPP_UTILS_H
#define CKC_CPP_UTILS_H

#include <vector>

class Utils {

public:
    static std::vector<std::vector<int>> loadGEucSpace(const std::string &file_path);

    static std::vector<std::vector<float>> readVList(const std::string &file_path);

    static float stdDev(std::vector<int> &items, float average);

    static void floydWarshall(std::vector<std::vector<int>> &C);

    static std::vector<std::vector<int>> loadGMetricSpace(int n, const std::string &filePath);

    static bool save(std::string &output_path, std::string &content);

    static std::tuple<std::vector<std::vector<int>>, int, int> loadORLIB(const std::string &filePath);
};


#endif //CKC_CPP_UTILS_H
