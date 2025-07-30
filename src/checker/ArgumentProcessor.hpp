// ArgumentProcessor.hpp
// A bundle of functions to check options and targets
#ifndef ARGUMENT_PROCESSOR_H
#define ARGUMENT_PROCESSOR_H

#include <iostream>
#include <filesystem>
#include <stdexcept>
#include <format>
#include <algorithm>
#include "help.hpp"
#include "../cli/Option.hpp"
#include "../aliases_and_concepts/using.hpp"
#include "../cli/ArgumentHandler.hpp"

namespace fs = std::filesystem;

namespace ArgumentProcessor {
	constexpr int check_target(const fs::path& target) {
		return fs::exists(target);
	}

	void check_targets(parsed_targets& targets) {
		auto it = targets.begin();
		auto end = targets.end();
		
		for( ; it != end ; ) {
			if (!check_target(*it)) {
				std::cerr << std::format("Target does not exist: {}\n", *it);
				it = targets.erase(it);
			} else {
				++it;
			}
		}

		if (targets.empty()) {
			throw std::invalid_argument("Targets that did not exist were erased, the set is empty\n");
		}
	}

	void check_targets_v2(parsed_targets& targets) {
	
		std::erase_if(
			targets,
			[&](auto const& t) {
				if (!check_target(t)) {
					std::cerr << std::format("Target does not exist: {}\n", t);
					return true;
				}
				return false;
			}
		);

		if (targets.empty()) {
			throw std::invalid_argument("Targets that did not exist were erased, the set is empty\n");
		}
	}

    void check_options(parsed_options &options) {

		//check if -h or --help
		if (options.find(Option::Help)) {
			print_help();
			throw std::invalid_argument("--h or --help is present");
		}

		// check wether name is present or not. If not then place it there
		const bool name_present = options.find(Option::Name);
		if (!name_present) {
			options.emplace_back(Option::Name);
		}
        // remove name if canonical is present
        if (name_present &&
            options.find(Option::Canonical)) {
            options.get_list().remove(Option::Name);
        }

        // sort & remove respective option
        if (options.find(Option::Sort)) {
            options.get_list().sort();
            options.get_list().remove(Option::Sort);
        }
    }
    } // namespace ArgumentProcessor

#endif