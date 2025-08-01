// LambdaHandler.cpp

#include <string>
#include <sstream>
#include <cmath>
#include <iomanip>
#include <vector>
#include <array>
#include <ctime>
#include <cstdio>
#include <functional>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include "../cli/Option.hpp"
#include "../aliases_and_concepts/using.hpp"
#include "../assembler/Assembler.hpp"
#include "LambdaHandler.hpp"

LambdaHandler::LambdaHandler(const ParsedOptions &options) : options_{options} {
	lambda_vector_.reserve(options_.get_list().size());
	for (const auto option : options_)
		lambda_vector_.push_back(deduce_printer_function(option));
	
}

LambdaVector LambdaHandler::get_lambdas() const {
	return lambda_vector_;
}

PrinterFunction LambdaHandler::deduce_printer_function(const Option option) const {
    switch (option) {
    case Option::ShowType:
        return [](const Entry &e) {
            return std::string(1, get_type_char(e.stats));
        };
    case Option::ShowPerms:
        return [](const Entry &e) {
            auto arr = get_perms_arr(e.stats);
            return std::string(arr.begin(), arr.end());
        };
    case Option::ShowNLinks:
        return [](const Entry &e) { return std::to_string(e.stats.st_nlink); };
    case Option::ShowOwner:
        return [](const Entry &e) { return get_owner_str(e.stats); };
    case Option::ShowGroup:
        return [](const Entry &e) { return get_group_str(e.stats); };
    case Option::ShowSize:
        return [](const Entry &e) { return get_size_str(e.stats); };
    case Option::ShowAccessTime:
		return [](const Entry &e) { return get_time_str(e.stats.st_atim); };
    case Option::ShowModTime:
		return [](const Entry &e) { return get_time_str(e.stats.st_mtim); };
    case Option::ShowMetaModTime:
		return [](const Entry &e) { return get_time_str(e.stats.st_ctim); };
    case Option::Name:
        return [](const Entry &e) { return e.name; };
    case Option::ShowInode:
        return [](const Entry &e) { return std::to_string(e.stats.st_ino); };
    case Option::ShowDev:
        return [](const Entry &e) { return std::to_string(e.stats.st_dev); };
    default:
        return [](const Entry &) { return std::string{}; };
    }
}

constexpr char LambdaHandler::get_type_char(const struct stat &stats) const noexcept {
	switch (stats.st_mode & S_IFMT) {
		case S_IFDIR: return 'D';
		case S_IFREG: return 'F';
		case S_IFLNK: return 'L';
		case S_IFCHR: return 'C';
		case S_IFBLK: return 'B';
		case S_IFIFO: return 'P';
		case S_IFSOCK: return 'S';
		default: return '?';
	}
}
std::array<char,9> LambdaHandler::get_perms_arr(const struct stat& stats) const noexcept {
	// rwxrwxrwx
	std::array<char,9> buf; 

	// u___ g___ o___
	static const mode_t mask[9] = { 
		S_IRUSR, S_IWUSR, S_IXUSR,
		S_IRGRP, S_IWGRP, S_IXGRP,
        S_IROTH, S_IWOTH, S_IXOTH
	};

	for (size_t i = 0; i < buf.size(); ++i) {
		buf[i] = (stats.st_mode & mask[i]) ? "rwx"[i%3] : '-';
	}

	return buf;
}

std::string LambdaHandler::get_owner_str(const struct stat &stats) const noexcept {
	struct passwd *pw = ::getpwuid(stats.st_uid);
	if (pw && pw->pw_name) return std::string(pw->pw_name);
	return std::to_string(stats.st_uid);
}
std::string LambdaHandler::get_group_str(const struct stat &stats) const noexcept{
	struct group *gr = ::getgrgid(stats.st_gid);
	if (gr && gr->gr_name) return std::string(gr->gr_name);
	return std::to_string(stats.st_gid);
}
std::string LambdaHandler::get_size_str(const struct stat &stats) const noexcept{
	static constexpr const char* units[] = {"B", "K", "M", "G", "T", "P", "E"}; //hoo tf even stores the last 3
	double size = static_cast<double>(stats.st_size);
	size_t idx = 0;

	while (size >= 1024.0 && idx < std::size(units) - 1) {
		size /= 1024.0;
		++idx;
	}

	std::ostringstream oss;
	if (idx == 0) 
		oss << static_cast<long long>(std::llround(size)) << units[idx];
	else
		oss << std::fixed << std::setprecision(1) << size << units[idx];

	return oss.str();
}

std::string LambdaHandler::get_time_str(const struct timespec& ts) const noexcept {
	std::time_t t = ts.tv_sec;
	std::tm tm;
	localtime_r(&t, &tm);
	auto ptm = std::localtime(&t);
	if (!ptm) return {};
	tm = *ptm;

	char buf[20];
	std::snprintf(
		buf,
		sizeof(buf),
		"%02d-%02d-%04d %02d-%02d-%02d",
                  tm.tm_mday,
                  tm.tm_mon + 1,
                  tm.tm_year + 1900,
                  tm.tm_hour,
                  tm.tm_min,
                  tm.tm_sec
	);

	return std::string(buf);
	
}
