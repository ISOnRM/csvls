// ArgumentProcessor.hpp
// Class' interface
#ifndef ARGUMENT_PROCESSOR_H
#define ARGUMENT_PROCESSOR_H

#include <iostream>
#include <filesystem>
#include <stdexcept>
#include <format>
#include "../aliases_and_concepts/using.hpp"

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
			[&targets](auto const& t) {
				if !(check_target(t)) {
					std::cerr << std::format("Target does not exist: {}\n", t);
					return true
				}
				return false;
			}
		);

		if (targets.empty()) {
			throw std::invalid_argument("Targets that did not exist were erased, the set is empty\n");
		}
	}
}


#endif