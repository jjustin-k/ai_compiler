#include <iostream>

#include "utils/json_parser.hpp"

int main(int argc, char *argv[]) {
    // debug_level = (int) argv[1];
    JSONParser parser;
    parser.parse("/Users/justinkwinecki/Documents/Programming/Term_25-26/comp/"
                 "ai_compiler/templates/complex_fusion.json");
    std::cout << "New" << std::endl;
    return 0;
}
