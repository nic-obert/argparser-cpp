#include "argparser.hh"

#include <string.h>
#include <iostream>


using namespace argparser;


static inline void checkArgBounds(unsigned int argc, unsigned int i, const char* caller)
{
    if (argc == i)
    {
        std::cerr << "Missing argument for parameter " << caller << std::endl;
        exit(EXIT_FAILURE);
    }
}


Parser::Parser()
{
    flagsMap = std::unordered_map<std::string, std::pair<TypeName, void*>>();
}


void Parser::addBoolExplicit(std::string&& flagName, bool* store)
{
    flagsMap[flagName] = std::make_pair<TypeName, void*>(TypeName::BOOL_EXPLICIT, store);
}


void Parser::addBoolImplicit(std::string&& flagName, bool* store)
{
    flagsMap[flagName] = std::make_pair<TypeName, void*>(TypeName::BOOL_IMPLICIT, store);
    // defaults to false, true if argument is found
    *store = false;
}


void Parser::addInteger(std::string&& flagName, int* store)
{
    flagsMap[flagName] = std::make_pair<TypeName, void*>(TypeName::INT, store);
}


void Parser::addString(std::string&& flagName, const char** store)
{
    flagsMap[flagName] = std::make_pair<TypeName, void*>(TypeName::STRING, store);
}


void Parser::parse(unsigned int argc, const char** argv) const
{
    for (unsigned int i = 1; i != argc; i++)
    {
        auto it = flagsMap.find(argv[i]);
        if (it == flagsMap.end())
        {
            std::cerr << "Unrecognized argument: " << argv[i] << std::endl;
            exit(EXIT_FAILURE);
        }

        switch (it->second.first)
        {
        case TypeName::BOOL_EXPLICIT:
        {
            i ++;

            checkArgBounds(argc, i, argv[i-1]);

            if (!strcmp(argv[i], "true"))
            {
                *(bool*) (it->second.second) = true;
            }
            else if (!strcmp(argv[i], "false"))
            {
                *(bool*) (it->second.second) = false;
            }
            else
            {
                std::cerr << "Invalid boolean value: " << argv[i]
                    << "for parameter " << argv[i-1] << std::endl;
                exit(EXIT_FAILURE);
            }

            break;
        }

        case TypeName::BOOL_IMPLICIT:
        {
            *(bool*) (it->second.second) = true;
            break;
        }
        
        case TypeName::INT:
        {
            i ++;

            checkArgBounds(argc, i, argv[i-1]);

            int value = strtol(argv[i], nullptr, 10);
            if (value == 0)
            {
                std::cerr << "Could not convert to integer value: \"" << argv[i]
                    << "\" requireg by parameter " << argv[i-1] << std::endl;
                exit(EXIT_FAILURE);
            }

            *(int*) (it->second.second) = value;
            break;
        }

        case TypeName::STRING:
        {
            i ++;

            checkArgBounds(argc, i, argv[i-1]);

            *(const char**) (it->second.second) = argv[i];

            break;
        }

        } // switch (TypeName)

    }

}

