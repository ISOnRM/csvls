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

std::list<struct stat> Assembler::get_stats() {
	assemble();
	return entrys_;
}

void Assembler::assemble() {
	get_stats(check_recursion());
}

bool Assembler::check_recursion() {
	return options_.find(Option::Recursive); 
}

void Assembler::get_stats() {
	for (const std::string& target : targets_) {
		attempt_create_entry(target);
		split_entry(entrys_.back());
	}
}

void Assembler::attempt_create_entry(const std::string &target) {
	struct stat sb;
	if ((::stat(target.c_str(), &sb)) != 0) {
		int err = errno;
		if (err == EACCES) {
			return;
		} else {
            throw std::system_error(err, std::generic_category();
                                    "Stat failed for " + target);
        }
	} else {
        entrys_.emplace_back(
			{
				use_canonical_ ? fs::canonical(target) : target,
				sb
			}
		);
    }
}

void Assembler::split_entry(Entry &raw_entry) {
    if (!S_ISDIR(raw_entry.stats)) {
        handle_not_dir(raw_entry);
    } else {
        handle_dir(raw_entry);
    }
}

void Assembler::handle_not_dir(Entry& raw_entry) {
	// no need
	return;
}

void Assembler::handle_dir(Entry& raw_entry) {
	if (use_recursion_) {
		traverse_dir(raw_entry.name.c_str());
	}
}

void Assembler::traverse_dir(char *path) {
    DIR *d = opendir(path);
    if (d == NULL) {
        int err = errno;
        if (err == EACCES) {
            return;
        } else {
            throw std::system_error(err, std::generic_category();
                                    "Opendir failed for " + raw_entry.name +
                                    ": " + ::strerror(err));
        }
    } else {
        Entry e;
        while (e = readdir(d)) {
            char *name = d->d_name;
            if (name == "." || name == "..")
                continue;

			char **full_path = {path, "/", name};
			if (e->d_type == DT_DIR) 
				traverse_dir(full_path)
        }
    }
}