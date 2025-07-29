//a test
#include <iostream>
#include "ArgumentHandler.h"
#include "Option.h"

std::ostream& operator<<(std::ostream& o, parsed_arguments& a) {
	for (const Option& arg : a) {
		o << static_cast<int>(arg) << ' ';
	}
	o << '\n';
	return o;
}

std::ostream &operator<<(std::ostream &o, parsed_positional_arguments &a) {
    for (const std::string &arg : a) {
        o << arg << ' ';
    }
    o << '\n';
    return o;
}

int main(int argc, char **argv) {
	ArgumentHandler handler(argc, argv);
	parsed_pair parsed = handler.get_parsed();
	std::cout << "parsed options: "<< parsed.first;
	std::cout << "parsed regular: "<< parsed.second;
}