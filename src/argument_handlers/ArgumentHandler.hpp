// ArgumentHandler.h
// Class that handles how command line arguments
// Are proccessed and then returned as a container of
// Options enum
#ifndef ARGUMENTHANDLER_H
#define ARGUMENTHANDLER_H

#include "Option.h"
#include <forward_list>
#include <unordered_set>
#include <string>
#include <string_view>
#include <tuple>

using raw_arguments = std::forward_list<std::string_view>;
using parsed_arguments = std::unordered_set<Option>;
using parsed_positional_arguments = std::unordered_set<std::string>;
using parsed_pair = std::pair<parsed_arguments, parsed_positional_arguments>;

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
    parsed_arguments parsed_arguments_;
    parsed_positional_arguments parsed_positional_arguments_;
    // private methods

    /*
        Initializes raw_arguments
    */
    void init_raw_arguments_(int &argc, char **&argv);

    /*
        Parses the given list
        and returns the list of Options
    */
    Option parse_argument_(std::string_view& arg);

    /*
        Parses the given list
        and returns the list of Positional arguments
    */
    std::string parse_positional_argument_(const std::string_view& arg);

    /*
        Parses the given list
        and returns a pair of lists with
        Optional and Positional arguments
    */
    parsed_pair parse_all_arguments_(raw_arguments raw_arguments);
};

#endif