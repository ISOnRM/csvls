// LambdaHandler.hpp
// Creates a vector of lambdas to later use it inside of CsvWriter

#ifndef LAMBDAHANDLER_HPP
#define LAMBDAHANDLER_HPP

#include <vector>
#include <array>
#include <string>
#include "../cli/Option.hpp"
#include "../aliases_and_concepts/using.hpp"
#include "../assembler/Assembler.hpp"

using PrinterFunction = std::string (*)(const Entry&);
using LambdaVector = std::vector<PrinterFunction>;

class LambdaHandler {
  public:
    explicit LambdaHandler(const ParsedOptions &options);

	LambdaVector get_lambdas() const;

  private:
	const ParsedOptions options_;
	LambdaVector lambda_vector_;

	PrinterFunction deduce_printer_function(const Option option) const;
    char get_type_char(const struct stat& stats) const noexcept;
	std::array<char,9> get_perms_arr(const struct stat& stats) const noexcept;
	std::string get_owner_str(const struct stat& stats) const noexcept;
	std::string get_group_str(const struct stat& stats) const noexcept;
	std::string get_size_str(const struct stat& stats) const noexcept;
	std::string get_time_str(const struct timespec& ts) const noexcept;

};	


#endif 