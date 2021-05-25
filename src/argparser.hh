#pragma once

#include <unordered_map>
#include <list>


namespace argparser
{

    // Type of the parameter
    typedef enum class TypeName : unsigned char
    {
        BOOL_EXPLICIT,
        BOOL_IMPLICIT,
        INTEGER,
        STRING,
        BOOL_POSITIONAL,
        INTEGER_POSITIONAL,
        STRING_POSITIONAL
    } TypeName;


    class Parser
    {
    private:

        std::list<std::pair<TypeName, void*>> positionals;

        unsigned int position;

        std::unordered_map<std::string, std::pair<TypeName, void*>> flagsMap;

    public:

        Parser();

        // adds an explicit boolean parameter
        // its value must be set explicitely
        void addBoolExplicit(std::string&& flagName, bool* store);

        void addBoolPositional(bool* store);

        // adds an implicit boolean parameter
        // its value is false if it's not passed as an argument
        // its value is true if it's passed as an argument
        void addBoolImplicit(std::string&& flagName, bool* store);

        // adds an explicit integer parameter
        // its value must be set explicitely
        void addInteger(std::string&& flagName, int* store);

        void addIntegerPositional(int* store);

        // adds an explicit string parameter
        // its value must be set explicitely
        void addString(std::string&& flagName, const char** store);

        void addStringPositional(const char** store);

        // parse the command line arguments
        // set the added flags based on provided arguments
        void parse(unsigned int argc, const char** argv) const;

    };

};

