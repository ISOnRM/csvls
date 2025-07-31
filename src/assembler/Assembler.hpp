// Assembler.hpp
// Creates a container of stat structs
#ifndef ASSEMBLER_HPP
#define ASSEMBLER_HPP

#include <string>
#include <list>
#include <sys/stat.h>
#include "../aliases_and_concepts/using.hpp"

struct Entry {
	std::string name;
	struct stat stats;
}

class Assembler {
  public:
    // Constructor
    // Assembler assumes that targets are validated
    Assembler(ParsedTargets &targets, ParsedOptions &options);
    std::list<struct stat> get_entries();

  private:
    ParsedTargets &targets_;
    ParsedOptions &options_;
    std::list<Entry> entries_;
	bool use_canonical_;
	bool use_recursion_;

    void assemble();

	void process_target(const std::string& path);

	void traverse_dir(const std::string& dir_path);

	void add_entry(
		const std::string& full_path,
		const struct stat& sb
	);

	

};

#endif