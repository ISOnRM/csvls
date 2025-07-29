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

int main(int argc, char **argv) {
	ArgumentHandler handler(argc, argv);
	parsed_pair parsed = handler.get_parsed();
    std::cout << "Available options:\n"

              << "0 ShowDev\n"
              << "1 ShowInode\n"
              << "2 ShowType\n"
              << "3 ShowPerms\n"
              << "4 ShowOwner\n"
              << "5 ShowGroup\n"
              << "6 ShowSize\n"
              << "7 ShowBlocks\n"
              << "8 ShowAccessTime\n"
              << "9 ShowModTime\n"
              << "10 ShowMetaModTime\n\n\n\n";



    std::cout << "parsed options: "<< parsed.first;
	std::cout << "parsed regular: "<< parsed.second;
}