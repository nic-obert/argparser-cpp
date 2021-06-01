#pragma once

#include <unordered_map>
#include <vector>
#include <iostream>


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


    typedef struct Parameter
    {
        TypeName type;
        void* store;
        bool required;

        Parameter();
        Parameter(TypeName type, void* store, bool required);

    } Parameter;


    class Parser
    {
    private:

        std::vector<Parameter> positionals;

        std::unordered_map<std::string, Parameter> flagsMap;

    public:

        // pre-allocates argNumber spaces for the parameters
        Parser(size_t argNumber = 1);

        // adds an explicit boolean parameter
        // its value must be set explicitely
        void addBoolExplicit(std::string&& flagName, bool* store, bool required = false);

        void addBoolPositional(bool* store, bool required = false);

        // adds an implicit boolean parameter
        // its value is false if it's not passed as an argument
        // its value is true if it's passed as an argument
        void addBoolImplicit(std::string&& flagName, bool* store, bool required = false);

        // adds an explicit integer parameter
        // its value must be set explicitely
        void addInteger(std::string&& flagName, int* store, bool required = false);

        void addIntegerPositional(int* store, bool required = false);

        // adds an explicit string parameter
        // its value must be set explicitely
        void addString(std::string&& flagName, const char** store, bool required = false);

        void addStringPositional(const char** store, bool required = false);

        // parse the command line arguments
        // set the added flags based on provided arguments
        void parse(unsigned int argc, const char** argv);

    };
 
};


std::ostream& operator<<(std::ostream& stream, argparser::TypeName typeName);

