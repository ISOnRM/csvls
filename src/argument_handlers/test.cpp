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
    std::cout << "Available options:\n"
          << "0 ShowType\n"
          << "1 ShowPerms\n"
          << "2 ShowNLinks\n"
          << "3 ShowOwner\n"
          << "4 ShowGroup\n"
          << "5 ShowSize\n"
          << "6 ShowAccessTime\n"
          << "7 ShowModTime\n"
          << "8 ShowMetaModTime\n"
          << "9 Name\n"
          << "10 Canonical\n"
          << "11 ShowInode\n"
          << "12 ShowBlocks\n"
          << "13 ShowDev\n\n\n\n";




    std::cout << "parsed options: "<< parsed.first;
	std::cout << "parsed targers: "<< parsed.second;
}
catch (std::invalid_argument& exception) {
	std::cout << "Invalid argument: " << exception.what() << '\n';
}