#include <iostream>

#include "utils/json_parser.hpp"
#include "utils/logger.hpp"

int main(int argc, char *argv[]) {

    
    JSONParser parser;
    parser.parse("/Users/justinkwinecki/Documents/Programming/Term_25-26/comp/"
                 "ai_compiler/templates/complex_fusion.json");
    
    return 0;
}
