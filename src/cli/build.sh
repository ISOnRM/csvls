#temporary build script
clang++ -Werror -Wall -c ArgumentParser.cpp test.cpp -std=c++23
clang++ -Werror -Wall -o test test.o ArgumentParser.o
