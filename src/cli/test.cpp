//a test
#include <iostream>
#include <sstream>
#include "ArgumentParser.hpp"
#include "Option.hpp"

std::ostream& operator<<(std::ostream& o, ParsedOptions& a) {
	for (const Option& arg : a) {
		o << static_cast<int>(arg) << ' ';
	}
	o << '\n';
	return o;
}

std::ostream &operator<<(std::ostream &o, ParsedTargets &a) {
    for (const std::string &arg : a) {
        o << arg << ' ';
    }
    o << '\n';
    return o;
}

int main(int argc, char **argv) try {
	ArgumentParser parser(argc, argv);
	ParsedArguments parsed = parser.parse();




    std::cout << "parsed options: "<< parsed.options;
	std::cout << "parsed targers: "<< parsed.targets;
}
catch (std::invalid_argument& exception) {
	std::cout << "Invalid argument: " << exception.what() << '\n';
}