// main.cpp
#include "include.hpp"

int main(int argc, char **argv) {
	try {
		// Parse arguments
		ArgumentParser parser(argc, argv);
		ParsedArguments parsed_arguments = parser.parse();
	
		// Check parsed arguments
		ArgumentChecker::check(parsed_arguments);

		// Assembler entries
		// TODO: js make it take ParsedArguments
		Assembler assembler(parsed_arguments.targets, parsed_arguments.options);
		Entries entries = assembler.get_entries();

		// Write (to stdout) CSV
		// Two last arguments are ostream and delimiter respectively
		// Their default valueas are cout and ',' respectively
		CsvWriter csv_writer(entries, parsed_arguments.options); 
		csv_writer.print_results();

    } catch (std::invalid_argument& err) {
        std::cerr << err.what();
        std::exit(EXIT_FAILURE);
    }
	catch (std::system_error& err) {
		std::cerr << err.what();
		std::exit(EXIT_FAILURE);
	}

    return EXIT_SUCCESS;
}