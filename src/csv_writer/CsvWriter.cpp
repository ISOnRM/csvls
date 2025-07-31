// CsvWriter.cpp

#include <array>
#include <list>
#include <tuple>
#include <iostream>
#include <iomanip>
#include <functional>
#include "../assembler/Assembler.hpp"
#include "../cli/Option.hpp"
#include "../CsvWriter.hpp"

CsvWriter::CsvWriter(const std::list<Entry> &entries,
                     const ParsedOptions &options,
                     std::ostream &out = std::cout, char delimiter = ',')
    : entries_{entries}, options_{options}, out_{out}, delimiter_{delimiter} {
    init_printers();
}

void CsvWriter::init_printers() {
	printers_ = {{
		
	}};
}

void CsvWriter::print_results() const {

}