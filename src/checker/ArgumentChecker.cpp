// ArgumentChecker.cpp
// Implementation of ArgumentChecker.hpp bundle

#include <filesystem>
#include <stdexcept>
#include "ArgumentChecker.hpp"
#include "help.hpp"

namespace fs = std::filesystem;

namespace ArgumentChecker {

	void check(ParsedArguments& parsed) {
		check_help(parsed.options);
		validate_options(parsed.options);
		validate_targets(parsed.targets);
	}

	void check_help(const ParsedOptions& options) {
		if (options.find(Option::Help)) {
			print_help();
			std::exit(0);
		}
	}

	void validate_options(ParsedOptions& options) {
		const bool name_present = options.find(Option::Name);
		const bool canonical_present = options.find(Option::Canonical);
		const bool sort_present = options.find(Option::Sort);

		if (!name_present) options.emplace_back(Option::Name);

		if (canonical_present)
			options.get_list().remove(Option::Name);

		if (sort_present) {
			options.get_list().sort();
			options.get_list().remove(Option::Sort);

		}
	}

	void validate_targets(ParsedTargets& targets) {
		if (targets.empty()) {
			targets.insert(".");
			return;
		}

		auto it = targets.begin();
		auto end = targets.end();
		for ( ; it != end; ) {
			if (!fs::exists(*it)) {
				std::cerr << std::format("Target not found: {}\n", *it);
				it = targets.erase(it);
			} else {
				++it;
			}
		}

		if (targets.empty()) 
			throw std::invalid_argument("No valid targets specified after validation");
	}
}