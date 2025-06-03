#include<string>
class CodeGen{

    private:
        std::string output_path;
        void writeToFile(std::string data);
    public:
        CodeGen(std::string output_path);
        //start : int i = 0, end : i < size, change : i++, statement  : int j = i
        void writeForLoop(std::string start, std::string end, std::string change, std::string statement);
};