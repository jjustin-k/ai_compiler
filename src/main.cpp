#include <iostream>

#include "utils/json_parser.hpp"
#include "utils/logger.hpp"

int main(int argc, char *argv[]) {

    JSONParser parser;
    parser.parse("../models/mnist-12.json");

    return 0;
}
