// ArgumentParser.hpp
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

struct ParsedArguments {
	ParsedOptions options;
	ParsedTargets targets;
};

class ArgumentParser {
  public:
    explicit ArgumentParser(int argc, char **argv); // Constructor

	// Parses
    ParsedArguments parse();

  private:
    RawArguments raw_arguments_;
    ParsedOptions parsed_options_;
    ParsedTargets parsed_targets_;


    // Puts all argv[i] (i>0) in raw arguments list
    void collect_raw_arguments(int argc, char **argv);

	// Parse raw arguments
    void parse_all();

    // Parse option
    void parse_option(std::string_view& arg);

    // Parse target
    void parse_target(std::string_view& arg);

	// Long option handling
	Option deduce_full_option(const std::string_view& arg);

	// Short option handling
	Option deduce_short_option(const char c);
};

#endif
