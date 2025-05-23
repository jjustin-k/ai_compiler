#include<string>

class Parser{
    public:
        ~Parser() = default;
        virtual void parse(const std::string path) = 0;
};