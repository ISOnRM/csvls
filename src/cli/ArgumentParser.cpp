// ArgumentParser.cpp
// An implementation of ArgumentParser.hpp
#include <array>
#include <string_view>
#include <stdexcept>
#include <format>
#include "ArgumentParser.hpp"

ArgumentParser::ArgumentParser(int argc, char **argv) {
	collect_raw_arguments(argc, argv);
}

void ArgumentParser::collect_raw_arguments(int argc, char **argv) {
	for (int i = 1; i < argc; ++i) raw_arguments_.emplace_front(argv[i]);
}

ParsedArguments ArgumentParser::parse() {
	parse_all();
	return {parsed_options_, parsed_targets_};
}

void ArgumentParser::parse_all() {
	for (std::string_view& arg : raw_arguments_) {
		if (arg.starts_with('-')) 	parse_option(arg);
		else 						parse_target(arg);
	}
}

void ArgumentParser::parse_option(std::string_view& arg) {
	size_t dashes = arg.find_first_not_of('-');
	arg.remove_prefix(dashes);
	if 		(dashes == 2) 	parsed_options_.emplace_back(deduce_full_option(arg));
	else if (dashes == 1) {
		for (char c : arg) 	parsed_options_.emplace_back(deduce_short_option(c));
	}
	else throw std::invalid_argument(std::format("Invalid argument format: {}\n", arg));
}

void ArgumentParser::parse_target(std::string_view& arg) {
	parsed_targets_.emplace(arg);
}

Option ArgumentParser::deduce_full_option(const std::string_view& arg) {
	// here i use mapping to reduce amount of if and else 
	// whoever discovered this is the goat
    static constexpr std::array<
		std::pair<std::string_view, Option>,
		AMOUNT_OF_OPTIONS //defined in Option.hpp
	> mapping = {{
		{"help", Option::Help},
        {"show-type", Option::ShowType},
        {"show-perms", Option::ShowPerms},
        {"show-nlinks", Option::ShowNLinks},
        {"show-owner", Option::ShowOwner},
        {"show-group", Option::ShowGroup},
        {"show-size", Option::ShowSize},
        {"show-access-time", Option::ShowAccessTime},
        {"show-mod-time", Option::ShowModTime},
        {"show-meta-mod-time", Option::ShowMetaModTime},
        {"name", Option::Name},
        {"canonical", Option::Canonical},
        {"show-inode", Option::ShowInode},
        {"show-blocks", Option::ShowBlocks},
        {"show-dev", Option::ShowDev},
        {"sort", Option::Sort},
        {"recursive", Option::Recursive}
	}};

    for (const auto& [arg_name, option] : mapping ) {
		if (arg == arg_name) return option;
	}
	throw std::invalid_argument(std::format("Unknown option \"--{}\"\n", arg));
}

Option ArgumentParser::deduce_short_option(const char c) {
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
    case 'r': return Option::Recursive;
	default:
		throw std::invalid_argument(std::format("Unknown option \"-{}\"\n", c));
    }
}
