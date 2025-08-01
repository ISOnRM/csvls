// CsvWriter.hpp
// Self explanatory

#ifndef CSVWRITER_HPP
#define CSVWRITER_HPP

#include <array>
#include <list>
#include <tuple>
#include <iostream>
#include <functional>
#include "../assembler/Assembler.hpp"
#include "../cli/Option.hpp"

using PrinterFunction = std::function<void(const Entry&, std::ostream&)>;

class CsvWriter {
  public:
    // Constructor
    CsvWriter(const std::list<Entry> &entries, const ParsedOptions &options,
              std::ostream &out = std::cout, char delimiter = ',');

    // Print in a passed formation
    void print_results() const;

  private:
    const std::list<Entry> entries_;
    const ParsedOptions options_;
	std::ostream out_;
    char delimiter_;

	std::string quote(const std::string& text);

	void print_options() const;
	void print_entries() const;

	static const char* column_name(Option option) noexcept;

	PrinterFunction deduce_printer_function(const Option option) const;
    char get_type_char(const struct stat& stats) const noexcept;
	std::array<char,9> get_perms_arr(const struct stat& stats) const noexcept;
	std::string get_owner_str(const struct stat& stats) const noexcept;
	std::string get_group_str(const struct stat& stats) const noexcept;
	std::string get_size_str(const struct stat& stats) const noexcept;
	std::string get_time_str(const struct timespec& ts) const noexcept;
};

#endif