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
    std::list<struct stat> get_stats();

  private:
    ParsedTargets &targets_;
    ParsedOptions &options_;
    std::list<Entry> entrys_;
	bool use_canonical_;
	bool use_recursion_;

    void assemble();

    void get_stats();

	void attempt_create_entry(const std::string& target);

	void split_entry(Entry& raw_entry);

	void handle_not_dir(Entry& raw_entry);

	void handle_dir(Entry& raw_entry);

	//void traverse_dir(char *...);

};

#endif