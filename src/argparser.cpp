#include "argparser.hh"

#include <string.h>


using namespace argparser;


static inline void checkArgBounds(unsigned int argc, unsigned int i, const char* caller)
{
    if (argc == i)
    {
        std::cerr << "Missing argument for parameter " << caller << std::endl;
        exit(EXIT_FAILURE);
    }
}


Parameter::Parameter()
{

}


Parameter::Parameter(TypeName type, void* store, bool required)
: type(type), store(store), required(required)
{

}


Parser::Parser(size_t argNumber)
{
    positionals = std::vector<Parameter>();
    positionals.reserve(argNumber);

    flagsMap = std::unordered_map<std::string, Parameter>();
}


void Parser::addBoolExplicit(std::string&& flagName, bool* store, bool required)
{
    flagsMap[flagName] = Parameter(
                            TypeName::BOOL_EXPLICIT,
                            store,
                            required
                        );
}


void Parser::addBoolImplicit(std::string&& flagName, bool* store, bool required)
{
    flagsMap[flagName] = Parameter(
                            TypeName::BOOL_IMPLICIT,
                            store,
                            required
                        );
    
    // defaults to false, true if argument is found
    *store = false;
}


void Parser::addBoolPositional(bool* store, bool required)
{
    positionals.emplace_back(Parameter(
        TypeName::BOOL_POSITIONAL,
        store,
        required
    ));
}


void Parser::addInteger(std::string&& flagName, int* store, bool required)
{
    flagsMap[flagName] = Parameter(
                            TypeName::INTEGER,
                            store,
                            required
                        );
}


void Parser::addIntegerPositional(int* store, bool required)
{
    positionals.emplace_back(Parameter(
        TypeName::INTEGER_POSITIONAL,
        store,
        required
    ));
}


void Parser::addString(std::string&& flagName, const char** store, bool required)
{
    flagsMap[flagName] = Parameter(
                            TypeName::STRING,
                            store,
                            required
                        );
}


void Parser::addStringPositional(const char** store, bool required)
{
    positionals.emplace_back(Parameter(
        TypeName::STRING_POSITIONAL,
        store,
        required
    ));
}


void Parser::parse(unsigned int argc, const char** argv)
{
    // current positional argument
    auto positional = positionals.begin();

    for (unsigned int i = 1; i != argc; i++)
    {

        TypeName type;
        void* output;

        auto it = flagsMap.find(argv[i]);
        if (it == flagsMap.end())
        {
            if (argv[i][0] != '-')
            {
                type = positional->type;
                output = positional->store;
                // set the required bool to false even if it was not required in
                // the first place
                positional->required = false;
                // increment current positional argument
                positional ++;
            }
            else
            {
                std::cerr << "Unrecognized argument: " << argv[i] << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            type = it->second.type;
            output = it->second.store;

            it->second.required = false;
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

    } // for arg in args


    bool error = false;

    for (auto pos : positionals)
    {
        if (pos.required)
        {
            std::cerr << "Missing required positional argument of type " << pos.type << std::endl;
            error = true;
        }
    }

    for (auto flag : flagsMap)
    {
        if (flag.second.required)
        {
            std::cerr << "Missing required argument \"" << flag.first << "\" of type " << flag.second.type << std::endl;
            error = true;
        }
    }

    if (error)
    {
        std::cerr << "Argument parsing failed" << std::endl;
        exit(EXIT_FAILURE);
    }

}


// lookup table for TypeName names
static const char* const typeNameNames[] =
{
    "BOOL EXPLICIT",
    "BOOL IMPLICIT",
    "INTEGER",
    "STRING",
    "BOOL POSITIONAL",
    "INTEGER POSITIONAL",
    "STRING POSITIONAL"
};


std::ostream& operator<<(std::ostream& stream, TypeName typeName)
{
    return stream << typeNameNames[(unsigned char) typeName];
}

