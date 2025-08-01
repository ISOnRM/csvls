// ArgumentChecker.hpp
// Bundle of namespaced function prototypes
#ifndef ARGUMENT_CHECKER_HPP
#define ARGUMENT_CHECKER_HPP

#include "../cli/ArgumentParser.hpp"

namespace ArgumentChecker {

	// Calls other functions like and essentially does this:
	// Checks for help
	// Validates targets
	// Validates options
	void check(ParsedArguments& parsed);

	void check_help(const ParsedOptions& options);
	void validate_options(ParsedOptions& options);
	void validate_targets(ParsedTargets& targets);

}

#endif