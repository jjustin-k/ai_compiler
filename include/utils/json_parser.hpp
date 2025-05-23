#include "parser.hpp"

class JSONParser : public Parser{
    public:
        void parse(const std::string path) override;
};