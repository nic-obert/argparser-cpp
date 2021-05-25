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
    positionals = std::list<std::pair<TypeName, void*>>();

    position = 0;

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


void Parser::addBoolPositional(bool* store)
{
    positionals.push_back(std::make_pair<TypeName, void*>(TypeName::BOOL_POSITIONAL, store));
}


void Parser::addInteger(std::string&& flagName, int* store)
{
    flagsMap[flagName] = std::make_pair<TypeName, void*>(TypeName::INTEGER, store);
}


void Parser::addIntegerPositional(int* store)
{
    positionals.push_back(std::make_pair<TypeName, void*>(TypeName::INTEGER_POSITIONAL, store));
}


void Parser::addString(std::string&& flagName, const char** store)
{
    flagsMap[flagName] = std::make_pair<TypeName, void*>(TypeName::STRING, store);
}


void Parser::addStringPositional(const char** store)
{
    positionals.push_back(std::make_pair<TypeName, void*>(TypeName::STRING_POSITIONAL, store));
}


void Parser::parse(unsigned int argc, const char** argv) const
{
    for (unsigned int i = 1; i != argc; i++)
    {

        void* output;
        TypeName type;

        auto it = flagsMap.find(argv[i]);
        if (it == flagsMap.end())
        {
            if (argv[i][0] != '-')
            {
                auto flag = positionals.begin();
                std::advance(flag, position);
                type = flag->first;
                output = flag->second;
            }
            else
            {
                std::cerr << "Unrecognized argument: " << argv[i] << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            type = it->second.first;
            output = it->second.second;
        }

        switch (type)
        {
        case TypeName::BOOL_EXPLICIT:
        {
            i ++;

            checkArgBounds(argc, i, argv[i-1]);

            if (!strcmp(argv[i], "true"))
            {
                *(bool*) output = true;
            }
            else if (!strcmp(argv[i], "false"))
            {
                *(bool*) output = false;
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
            *(bool*) output = true;

            break;
        }
        
        case TypeName::INTEGER:
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

            *(int*) output = value;

            break;
        }

        case TypeName::STRING:
        {
            i ++;

            checkArgBounds(argc, i, argv[i-1]);

            *(const char**) output = argv[i];

            break;
        }

        case TypeName::BOOL_POSITIONAL:
        {
            if (!strcmp(argv[i], "true"))
            {
                *(bool*) output = true;
            }
            else if (!strcmp(argv[i], "false"))
            {
                *(bool*) output = false;
            }
            else
            {
                std::cerr << "Invalid boolean value: " << argv[i]
                    << "for parameter " << argv[i-1] << std::endl;
                exit(EXIT_FAILURE);
            }

            break;
        }

        case TypeName::INTEGER_POSITIONAL:
        {
            int value = strtol(argv[i], nullptr, 10);
            if (value == 0)
            {
                std::cerr << "Could not convert to integer value: \"" << argv[i]
                    << "\" requireg by parameter " << argv[i-1] << std::endl;
                exit(EXIT_FAILURE);
            }

            *(int*) output = value;

            break;
        }

        case TypeName::STRING_POSITIONAL:
        {
            *(const char**) output = argv[i];

            break;
        }

        } // switch (TypeName)

    }

}

