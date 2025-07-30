//a test
#include <iostream>
#include <sstream>
#include "ArgumentHandler.hpp"
#include "Option.hpp"

std::ostream& operator<<(std::ostream& o, parsed_options& a) {
	for (const Option& arg : a) {
		o << static_cast<int>(arg) << ' ';
	}
	o << '\n';
	return o;
}

std::ostream &operator<<(std::ostream &o, parsed_targets &a) {
    for (const std::string &arg : a) {
        o << arg << ' ';
    }
    o << '\n';
    return o;
}

int main(int argc, char **argv) try {
	ArgumentHandler handler(argc, argv);
	parsed_pair parsed = handler.get_parsed();




    std::cout << "parsed options: "<< parsed.first;
	std::cout << "parsed targers: "<< parsed.second;
}
catch (std::invalid_argument& exception) {
	std::cout << "Invalid argument: " << exception.what() << '\n';
}