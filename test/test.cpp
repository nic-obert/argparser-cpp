#include "argparser.hh"

#include <iostream>

using namespace argparser;


int main(int argc, const char** argv)
{

    Parser parser = Parser(5, "This is the description");

    int iflag = 0;
    const char* sflag = "";
    bool beflag = false;
    bool biflag = false;
    const char* strPos = "";

    parser.addInteger("-i", &iflag, false, "integer argument");
    parser.addString("-str", &sflag, false, "stirng argument");
    parser.addBoolExplicit("-be", &beflag, false, "be or not to be");
    parser.addBoolImplicit("-bi", &biflag, false, "implicit");
    parser.addStringPositional(&strPos, true, "positional argument");

    parser.parse(argc, argv);

    std::cout << "Integer: " << iflag << std::endl;
    std::cout << "String: " << sflag << std::endl;
    std::cout << "Explicit Bool: " << beflag << std::endl;
    std::cout << "Implicito Bool: " << biflag << std::endl;
    std::cout << "String Positional: " << strPos << std::endl;

}