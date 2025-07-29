// ArgumentHandler.hpp
// Class that handles how command line arguments
// Are proccessed and then returned as a container of
// Options enum
#ifndef ARGUMENTHANDLER_H
#define ARGUMENTHANDLER_H

#include "Option.hpp"
#include <forward_list>
#include <unordered_set>
#include <string>
#include <string_view>
#include <tuple>

using raw_arguments = std::forward_list<std::string_view>;
using parsed_options = std::unordered_set<Option>;
using parsed_targets = std::unordered_set<std::string>;
using parsed_pair = std::pair<parsed_options, parsed_targets>;

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
    Option parse_option_(std::string_view& arg);

    /*
        Parses a single target (Non option argument)
    */
    std::string parse_target_(const std::string_view& arg);

    /*
        Parses all arguments into options and targets
    */
    parsed_pair parse_all_options_and_targets_(raw_arguments raw_arguments);

};

#endif
