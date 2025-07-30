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

	// check whether targets are present or not
	bool has_target = std::any_of(
		raw_arguments_.begin(),
		raw_arguments_.end(),
		[](const std::string_view &a){return !a.starts_with('-');}
	);

	

	if (!has_target) {
		throw std::invalid_argument("No targets specified");
	}
}

ArgumentHandler::ArgumentHandler(int &argc, char **&argv) {
	init_raw_arguments_(argc, argv);
}

parsed_pair ArgumentHandler::parse_all_(raw_arguments& raw_arguments)
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
			parse_target_(arg);
		}
    }
	
	if (parsed_options_.find(Option::Name) && parsed_options_.find(Option::Canonical)) {
		parsed_options_.get_list().remove(Option::Name);
	}
	
	if (parsed_options_.find(Option::Sort)) {
		parsed_options_.get_list().sort();
	}

	return {parsed_options_, parsed_targets_};
}

void ArgumentHandler::parse_option_(std::string_view& arg) {
	// pretty straightforward no expl needed
	size_t dashes = arg.find_first_not_of('-');
	arg.remove_prefix(dashes);
	if (dashes >= 2) {
		parsed_options_.emplace_back(deduce_full_option_(arg));
	}
	else if (dashes == 1) {
		for (const char c : arg) {
			parsed_options_.emplace_back(deduce_option_(c));
		}
	}
	else {
		throw std::invalid_argument(std::format("Option {} not found\n", arg));
	}
}

void ArgumentHandler::parse_target_(const std::string_view& arg) {
	parsed_targets_.emplace(arg);
}

parsed_pair ArgumentHandler::get_parsed() {
	return parse_all_(raw_arguments_);
}

Option ArgumentHandler::deduce_full_option_(const std::string_view& arg) {
    if (arg == "help") return Option::Help;
    else if (arg == "show-dev") return Option::ShowDev;
    else if (arg == "show-inode") return Option::ShowInode;
    else if (arg == "show-type") return Option::ShowType;
    else if (arg == "show-perms") return Option::ShowPerms;
    else if (arg == "show-nlinks") return Option::ShowNLinks;
    else if (arg == "show-owner") return Option::ShowOwner;
    else if (arg == "show-group") return Option::ShowGroup;
    else if (arg == "show-size") return Option::ShowSize;
    else if (arg == "show-blocks") return Option::ShowBlocks;
    else if (arg == "show-access-time") return Option::ShowAccessTime;
    else if (arg == "show-mod-time") return Option::ShowModTime;
    else if (arg == "show-meta-mod-time") return Option::ShowMetaModTime;
    else if (arg == "name") return Option::Name;
    else if (arg == "canonical") return Option::Canonical;
    else if (arg == "sort") return Option::Sort;
    else throw_invalid_argument_(arg);
}

Option ArgumentHandler::deduce_option_(const char c) {
    switch (c) {
    case 'h': return Option::Help;
    case 'd': return Option::ShowDev;
    case 'I': return Option::ShowInode;
    case 't': return Option::ShowType;
    case 'p': return Option::ShowPerms;
    case 'n': return Option::ShowNLinks;
    case 'O': return Option::ShowOwner;
    case 'g': return Option::ShowGroup;
    case 's': return Option::ShowSize;
    case 'b': return Option::ShowBlocks;
    case 'a': return Option::ShowAccessTime;
    case 'm': return Option::ShowModTime;
    case 'M': return Option::ShowMetaModTime;
    case 'N': return Option::Name;
    case 'c': return Option::Canonical;
    case 'S': return Option::Sort;
    default: throw_invalid_argument_(c); break;
    }
}
