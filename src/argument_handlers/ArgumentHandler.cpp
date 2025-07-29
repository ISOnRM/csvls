// ArgumentHandler.cpp
// An implementation of ArgumentHandler.h
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

parsed_pair ArgumentHandler::parse_all_arguments_(raw_arguments raw_arguments)
{
	for (std::string_view& arg : raw_arguments)
	{
		// btw this is not me commenting on the code cuz its unreadable
		// its more readable that python ong

		// If and argument starts with "-" or "--", then
		// it is not positional, thus send it to respective method
		// and emplace it to respecive list
        if (arg.starts_with("-") || arg.starts_with("--")) {
            parsed_arguments_.emplace(parse_argument_(arg));
        }
		// Else, its a positional argument
		// Which is not that positional after all
		// TODO: rename
		else {
			parsed_positional_arguments_.emplace(parse_positional_argument_(arg));
		}
    }
	return {parsed_arguments_, parsed_positional_arguments_};
}

Option ArgumentHandler::parse_argument_(std::string_view& arg) {
	// pretty straightforward no expl needed
	size_t pos = arg.find_first_not_of('-');
	arg.remove_prefix(pos);
	if (arg == "show-dev" || arg == "d") return Option::ShowDev;
	else if (arg == "show-inode" || arg == "I") return Option::ShowInode;
	else if (arg == "show-type" || arg == "t") return Option::ShowType;
	else if (arg == "show-perms" || arg == "p") return Option::ShowPerms;
	else if (arg == "show-owner" || arg == "O") return Option::ShowOwner ;
	else if (arg == "show-group" || arg == "g") return Option::ShowGroup ;
	else if (arg == "show-size" || arg == "s") return Option::ShowSize ;
	else if (arg == "show-blocks" || arg == "b") return Option::ShowBlocks ;
	else if (arg == "show-access-time" || arg == "a") return Option::ShowAccessTime;
	else if (arg == "show-mod-time" || arg == "m") return Option::ShowModTime;
	else if (arg == "show-meta-mod-time" || arg == "M") return Option::ShowMetaModTime;
	else throw std::invalid_argument(
		std::format("Argument {} not found\n", arg)
	);
}

std::string ArgumentHandler::parse_positional_argument_(const std::string_view& arg) {
	return static_cast<std::string>(arg);
}

parsed_pair ArgumentHandler::get_parsed() {
	return parse_all_arguments_(raw_arguments_);
}