// ArgumentHandler.cpp
// An implementation of ArgumentHandler.hpp
#include <string>
#include <string_view>
#include <forward_list>
#include <tuple>
#include <stdexcept>
#include <format>
#include "ArgumentHandler.hpp"

void ArgumentHandler::init_raw_arguments_(int &argc, char **&argv) {
    for (size_t i = 1; i < argc; ++i) { // skip argv[0]
        raw_arguments_.emplace_front(argv[i]);
    }
}

ArgumentHandler::ArgumentHandler(int &argc, char **&argv) {
	init_raw_arguments_(argc, argv);
}

parsed_pair ArgumentHandler::parse_all_(raw_arguments raw_arguments)
{
	for (std::string_view& arg : raw_arguments)
	{
		// btw this is not me commenting on the code cuz its unreadable
		// its more readable that python ong

		// If an argument starts with "-" or "--", then
		// it is an option, thus send it to respective method
		// and emplace it to respective list
        if (arg.starts_with("-")) {
			parse_option_(arg);
        }
		// Else, it’s a target (positional argument)
		else {
			parsed_targets_.emplace(parse_target_(arg));
		}
    }
	return {parsed_options_, parsed_targets_};
}

void ArgumentHandler::parse_option_(std::string_view& arg) {
	// pretty straightforward no expl needed
	size_t pos = arg.find_first_not_of('-');
	arg.remove_prefix(pos);
	if (arg.contains('-')) {
		parsed_options_.emplace(deduce_full_option_(arg));
	}
	else {
		for (const char& c : arg) {
			parsed_options_.emplace(deduce_option_(c));
		}
	}
}

std::string ArgumentHandler::parse_target_(const std::string_view& arg) {
	return static_cast<std::string>(arg);
}

parsed_pair ArgumentHandler::get_parsed() {
	return parse_all_(raw_arguments_);
}

Option ArgumentHandler::deduce_full_option_(const std::string_view& arg) {
	if (arg == "show-dev") return Option::ShowDev;
	else if (arg == "show-inode") return Option::ShowInode;
	else if (arg == "show-type") return Option::ShowType;
	else if (arg == "show-perms") return Option::ShowPerms;
	else if (arg == "show-owner") return Option::ShowOwner;
	else if (arg == "show-group") return Option::ShowGroup;
	else if (arg == "show-size") return Option::ShowSize;
	else if (arg == "show-blocks") return Option::ShowBlocks;
	else if (arg == "show-access-time") return Option::ShowAccessTime;
	else if (arg == "show-mod-time") return Option::ShowModTime;
	else if (arg == "show-meta-mod-time") return Option::ShowMetaModTime;
	else throw std::invalid_argument(
		std::format("Option {} not found\n", arg)
	);
}

Option ArgumentHandler::deduce_option_(const char c) {
    switch (c) {
    case 'd': return Option::ShowDev;
    case 'I': return Option::ShowInode;
    case 't': return Option::ShowType;
    case 'p': return Option::ShowPerms;
    case 'O': return Option::ShowOwner;
    case 'g': return Option::ShowGroup;
    case 's': return Option::ShowSize;
    case 'b': return Option::ShowBlocks;
    case 'a': return Option::ShowAccessTime;
    case 'm': return Option::ShowModTime;
    case 'M': return Option::ShowMetaModTime;
    default: throw std::invalid_argument(std::format("Option {} not found\n", c));
    }
}
