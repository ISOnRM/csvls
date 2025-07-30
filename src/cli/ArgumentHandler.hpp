// ArgumentHandler.hpp
// Class that handles how command line arguments
// Are proccessed and then returned as a container of
// Options enum
#ifndef ARGUMENTHANDLER_H
#define ARGUMENTHANDLER_H

#include <forward_list>
#include <unordered_set>
#include <string>
#include <string_view>
#include <tuple>
#include "Option.hpp"
#include "OrderedUnique.hpp"
#include "../aliases_and_concepts/using.hpp"



class ArgumentHandler {
  public:
    explicit ArgumentHandler(int &argc, char **&argv); // Constructor
    ~ArgumentHandler() = default;                      // Destructor

    // public methods

    /*
        Returns the parsed arguments
    */
    parsed_pair get_parsed();

  private:
    // private_members
    raw_arguments raw_arguments_;
    parsed_options parsed_options_;
    parsed_targets parsed_targets_;
    // private methods

    /*
        Initializes raw_arguments
    */
    void init_raw_arguments_(int &argc, char **&argv);

    /*
        Parses a single optional argument
    */
    void parse_option_(std::string_view& arg);

    /*
        Parses a single target (Non option argument)
    */
    void parse_target_(const std::string_view& arg);

    /*
        Parses all arguments into options and targets
    */
    parsed_pair parse_all_(raw_arguments& raw_arguments);

	/*
		Deduces a full option (e.g. --show-owner) when given an arg
	*/
	Option deduce_full_option_(const std::string_view& arg);

	/*
		Deduces an option (e.g. -O) when given char
	*/
	Option deduce_option_(const char c);

	/*
		Throw an exception to reduce copy&paste code
	*/
	[[noreturn]]
	void throw_invalid_argument_(const CharOrString auto& arg) {
		throw std::invalid_argument(std::format("Option {} not found\n", arg)); 
	}
};

#endif
