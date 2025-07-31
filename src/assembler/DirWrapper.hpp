// DirWrapper.hpp
#ifndef DIRWRAPPER_HPP
#define DIRWRAPPER_HPP

#include <memory>
#include <dirent.h>

struct DirDeleter {
	void operator()(DIR *d) const {
		if (d) closedir(d);
	}
};

using DirPtr = std::unique_ptr<DIR, DirDeleter>;

#endif