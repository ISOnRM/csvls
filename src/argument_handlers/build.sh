clang++ -Werror -Wall -c ArgumentHandler.cpp test.cpp -std=c++23
clang++ -Werror -Wall -o test test.o ArgumentHandler.o
