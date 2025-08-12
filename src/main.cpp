#include <iostream>

#include "../output/compiled_model.h"
#include "utils/json_parser.hpp"
#include <chrono>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <vector>

std::vector<std::vector<float>> readCSVFloats(const std::string &filename) {
    std::vector<std::vector<float>> csv;
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    std::string line;
    while (std::getline(file, line)) {
        std::vector<float> values;
        std::stringstream ss(line);
        std::string number;
        while (std::getline(ss, number, ',')) {
            try {
                values.push_back(std::stof(number));
            } catch (const std::invalid_argument &) {
                // skip invalid entries
            }
        }
        if (!values.empty())
            csv.push_back(values);
    }
    return csv;
}

void compile_model() {
    JSONParser parser;
    parser.parse("../models/mnist-12.json");
}

void evaluate_model() {
    std::string output_path = "../output/opt_results.txt";

    try {
        std::vector<std::vector<float>> data = readCSVFloats("../output/input.txt");

        double total_time = 0.0f;
        std::ofstream file(output_path);
        if (file.is_open()) {
            file << "This will overwrite the file content.\n";
            file.close();
        } else {
            // Handle error
        }
        for (std::vector<float> input : data) {
            int expected = input.back();
            input.pop_back();

            auto start = std::chrono::steady_clock::now();

            int result = predict(input.data());

            auto end = std::chrono::steady_clock::now();

            auto duration_us = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
            total_time += duration_us;

            std::ofstream file(output_path, std::ios::app);
            if (file.is_open()) {
                file << expected << "," << result << "," << std::to_string(duration_us) << "\n";
                file.close();
            } else {
                // handle error
            }
        }
        std::cout << total_time << std::endl;

    } catch (const std::exception &e) {
        std::cerr << e.what() << "\n";
    }
}
int main(int argc, char *argv[]) {
    evaluate_model();
    return 0;
}
