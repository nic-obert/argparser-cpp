#include "argparser.hh"

#include <iostream>

using namespace argparser;


int main(int argc, const char** argv)
{

    Parser parser = Parser();

    int iflag = 0;
    const char* sflag = "";
    bool beflag = false;
    bool biflag = false;
    const char* strPos = "";

    parser.addInteger("-i", &iflag);
    parser.addString("-str", &sflag);
    parser.addBoolExplicit("-be", &beflag);
    parser.addBoolImplicit("-bi", &biflag);
    parser.addStringPositional(&strPos);

    parser.parse(argc, argv);

    std::cout << "Integer: " << iflag << std::endl;
    std::cout << "String: " << sflag << std::endl;
    std::cout << "Explicit Bool: " << beflag << std::endl;
    std::cout << "Implicito Bool: " << biflag << std::endl;
    std::cout << "String Positional: " << strPos << std::endl;

}