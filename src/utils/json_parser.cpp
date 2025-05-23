#include "../include/utils/json_parser.hpp"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream> 

using json = nlohmann::json;
    

void JSONParser::parse(const std::string path)
{
    std::ifstream file(path);
    json data;
    file >> data;
   
}