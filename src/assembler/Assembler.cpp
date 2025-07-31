// Assembler.cpp
// Implementation of Assembler.hpp
#include <filesystem>
#include <optional>
#include <dirent.h>
#include <sys/stat.h>
#include <cerrno>
#include <string.h>
#include <stdexcept>
#include <list>
#include "../aliases_and_concepts/using.hpp"
#include "../cli/Option.hpp"
#include "Assembler.hpp"

namespace fs = std::filesystem;

Assembler::Assembler(ParsedTargets &targets, ParsedOptions &options)
    : targets_{targets}, options_{options},
      use_canonical_{options.find(Option::Canonical)},
      use_recursion_{options.find(Option::Recursive)} {}

std::list<Entry> Assembler::get_stats() {
	assemble();
	return entries_;
}

void Assembler::assemble() {
	for (const std::string& target : targets_) {
		process_target(target);
	}
}


void Assembler::process_target(const std::string& path) {
	struct stat sb;
	if ((::stat(target.c_str(), &sb)) != 0) {
		int err = errno;
		if (err == EACCES) {
			return;
		} 
        throw std::system_error(err, std::generic_category();
                                    "Stat failed for " + path);
	}
	
	add_entry(path, sb);

	if (S_ISDIR(sb.st_mode)) {
		traverse_dir(path);
	}
}


void Assembler::traverse_dir(const std::string& dir_path) {
    DIR *d = opendir(dir_path.c_str());
	if (!d) {
		int err = errno;
		if (err == EACCES) {
			return;
		}
		throw std::system_error(err, std::generic_category(), "opendir failed for " + dir_path);
	}

	struct dirent* entry;
	while ((entry = readdir(d)) != NULL) {
        std::string name = entry->d_name;
        if (name == "." || name == "..")
            continue;
        std::string full_path = dir_path + "/" + name;
        struct stat sb;
        if ((::stat(target.c_str(), &sb)) != 0) {
            int err = errno;
            if (err == EACCES) {
                return;
            }
            throw std::system_error(err, std::generic_category();
                                    "Stat failed for " + full_path);
        }

		add_entry(full_path, sb);

		if (use_recursion_ && S_ISDIR(sb.st_mode)) {
			traverse_dir(full_path);
		}
    }

	closedir(d);
}

void Assembler::add_entry(const std::string& full_path, const struct stat& sb) {
	entries_.push_back({
			use_canonical_ ? fs::canonical(full_path).string() : full_path,
			sb
		});
}