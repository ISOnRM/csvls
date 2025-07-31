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

	std::array<
		std::pair<Option, PrinterFunction>,
		AMOUNT_OF_PRINTABLES
	> printers_;

	void init_printers();

	std::string quote(const std::string& field);

	void print_field(const Entry& entry, Option option) const;
};

#endif