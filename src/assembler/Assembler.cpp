// Assembler.cpp
// Implementation of Assembler.hpp
#include <filesystem>
#include <optional>
#include <dirent.h>
#include <sys/stat.h>
#include <cerrno>
#include <cstring>
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

std::list<Entry> Assembler::get_entries() {
	entries_.clear();
	assemble();
	return entries_;
}

void Assembler::assemble() {
	for (const std::string& target : targets_) {
		process_target(target);
	}
}


void Assembler::process_target(const std::string& path) {
	auto temp_sb = get_stat_checked(path);
	if (!temp_sb) return;
	const auto& sb = *temp_sb;
	
	add_entry(path, sb);

	if (S_ISDIR(sb.st_mode)) {
		traverse_dir(path);
	}
}

std::optional<struct stat> Assembler::get_stat_checked(const std::string& path) {
	struct stat sb;
	if ((::stat(path.c_str(), &sb)) != 0) {
		int err = errno;
		if (err == EACCES) {
			return std::nullopt;
		} 
        throw std::system_error(err, std::generic_category(),
                                    "Stat failed for " + path);
	}
	return sb;
}


void Assembler::traverse_dir(const std::string& dir_path) {
    DirPtr d(opendir(dir_path.c_str()));
	if (!d) {
		if (errno == EACCES) {
			return;
		}
		throw std::system_error(errno, std::generic_category(), "opendir failed for " + dir_path);
	}

	struct dirent* entry;
	while ((entry = readdir(d.get())) != nullptr) {
        std::string name = entry->d_name;
        if (name == "." || name == "..")
            continue;

		
        std::string full_path = dir_path + "/" + name;

        auto temp_sb = get_stat_checked(full_path);
		if (!temp_sb) continue;
		const struct stat& sb = *temp_sb;

		add_entry(full_path, sb);

		if (use_recursion_ && S_ISDIR(sb.st_mode)) {
			traverse_dir(full_path);
		}
    }
}

void Assembler::add_entry(const std::string& full_path, const struct stat& sb) {
	entries_.push_back({
			use_canonical_ ? fs::canonical(full_path).string() : full_path,
			sb
		});
}

// template<bool Canonical>
// std::string Assembler::make_name(const std::string& path) {

// }

// template<bool Recursion>
// void Assembler::traverse_dir_ct(const std::string& dir_path) {

// }