// help.hpp
// print usage to stderr
#ifndef HELP_HPP
#define HELP_HPP
#include <iostream>


inline void print_help() {
	std::cerr << "Usage:\n";
    std::cerr << "  csvls [options] <targets>...\n\n";
    std::cerr << "Description:\n";
    std::cerr << "  Targets are given without a leading \"-\" and must be existing directories or files to scan.\n\n";
    std::cerr << "Options:\n";
    std::cerr << "  -h, --help           Show this help message and exit."
	"\nIf -h is among other options, they will be ignored\n";
    std::cerr << "  -d, --show-dev       Display device ID.\n";
    std::cerr << "  -I, --show-inode     Display inode number.\n";
    std::cerr << "  -t, --show-type      Display file type.\n";
    std::cerr << "  -p, --show-perms     Display human readable file permissions.\n";
    std::cerr << "  -n, --show-nlinks    Display number of hard links.\n";
    std::cerr << "  -O, --show-owner     Display owner's username.\n";
    std::cerr << "  -g, --show-group     Display group's name.\n";
    std::cerr << "  -s, --show-size      Display size in human readable units.\n";
    std::cerr << "  -b, --show-blocks    Display number of blocks.\n";
    std::cerr << "  -a, --show-access-time Display last access time.\n";
    std::cerr << "  -m, --show-mod-time  Display last modification time.\n";
    std::cerr << "  -M, --show-meta-mod-time Display metadata change time.\n";
    std::cerr << "  -N, --name           Display entry name.\n";
    std::cerr << "  -c, --canonical      Display canonical path.\n";
    std::cerr << "  -S, --sort           Sort output by selected options.\n";
    std::cerr << "  -r, --recursive      Enable recursive directory traversal.\n\n";
	std::cerr << "  Look up command examples on github ISOnRM/csvls\n";
}
#endif
