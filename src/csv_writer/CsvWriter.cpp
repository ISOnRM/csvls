// CsvWriter.cpp

#include <array>
#include <list>
#include <tuple>
#include <iostream>
#include <iomanip>
#include <functional>
#include <cmath>
#include <ctime>
#include <cstdio>
#include <sys/stat.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include "../assembler/Assembler.hpp"
#include "../cli/Option.hpp"
#include "../CsvWriter.hpp"

CsvWriter::CsvWriter(const std::list<Entry> &entries,
                     const ParsedOptions &options,
                     std::ostream &out = std::cout, char delimiter = ',')
    : entries_{entries}, options_{options}, out_{out}, delimiter_{delimiter} {}

const char *CsvWriter::column_name(Option option) noexcept {
    switch (opt) {
    case Option::Name:
        return "name";
    case Option::ShowType:
        return "type";
    case Option::ShowPerms:
        return "permissions";
    case Option::ShowNLinks:
        return "links";
    case Option::ShowOwner:
        return "owner";
    case Option::ShowGroup:
        return "group";
    case Option::ShowSize:
        return "size";
    case Option::ShowAccessTime:
        return "access_time";
    case Option::ShowModTime:
        return "mod_time";
    case Option::ShowMetaModTime:
        return "meta_mod_time";
    case Option::ShowInode:
        return "inode";
	case Option::ShowBlocks;
		return "blocks";
    case Option::ShowDev:
        return "device";
    default:
        return "";
    }
}

void CsvWriter::print_results() const {
	//print the first row - options
	print_options();

	// then print the entries
	print_entries();
}

void CsvWriter::print_options() const {
        bool first = true;
        for (const Option option : options_) {
            if (!first)
                out_ << delimiter_;
            first = false;
            out_ << column_name(option);
        }
        out_ << '\n';
}
void CsvWriter::print_entries() const {
	for (const Entry entry : entries_) {
		bool first = true;
		for (const Option option : options_) {
			if (!first) out_ << delimiter_;
			first = false;

			switch (option) {
				case Option::ShowType:
					out_ << get_type_char(entry.stats); break;
				case Option::ShowPerms:
					const auto perms = get_perms_arr(entry.stats);
					out_.write(perms.data(), perms.size());
					break;
				case Option::ShowNLinks:
					out_ << entry.stats.st_nlink;
					break;
				case Option::ShowOwner:
					out_ << get_owner_str(entry.stats);
					break;
				case Option::ShowGroup:
					out_ << get_group_str(entry.stats);
					break;
				case Option::ShowSize:
					out_ << get_size_str(entry.stats);
					break;
				case Option::ShowAccessTime:
					out_ << get_time_str(entry.stats.st_atim);
					break;
				case Option::ShowModTime:
					out_ << get_time_str(entry.stats.st_mtim);
					break;
				case Option::ShowMetaModTime:
					out_ << get_time_str(entry.stats.st_ctim);
					break;
				case Option::ShowInode:
					out_ << entry.stats.st_ino;
					break;
				case Option::ShowDev:
					out_ << entry.stats.st_dev;
					break;
				default:
					break;
			}
		}
		out_ << '\n';
	}
}

std::string CsvWriter::quote(const std::string& text) {
	bool need_quotes = false;

	for (char c : text) {
		if (c == delimiter_ || c == '"' || c == '\n' || c == '\r') {
			need_quotes = true;
			break;
		}
	}

	if (!need_quotes && (text.front() == ' ' && text.back() == ' ')) {
		need_quotes = true;
	}

	std::string result;
	result.reserve(text.size() + 2);
	result.push_back('"');

	for (char c : text) {
		if (c == '"') {
			result.append("\"\"");
		} else {
			result.push_back(c);
		}
	}

	result.push_back('"');

	return result;
}

char CsvWriter::get_type_char(const struct stat &stats) const noexcept {
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
std::array<char,9> CsvWriter::get_perms_arr(const struct stat& stats) const noexcept {
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
std::string CsvWriter::get_owner_str(const struct stat &stats) const noexcept {
	struct passwd *pw = ::getpwuid(stats.st_uid);
	if (pw && pw->pw_name) return std::string(pw->pw_name);
	return std::to_string(stats.st_uid);
}
std::string CsvWriter::get_group_str(const struct stat &stats) const noexcept{
	struct group *gr = ::getgrgid(stats.st_gid);
	if (gr && gr->gr_name) return std::string(gr->gr_name);
	return std::to_string(stats.st_gid);
}
std::string CsvWriter::get_size_str(const struct stat &stats) const noexcept{
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
std::string CsvWriter::get_time_str(const struct timespec& ts) const noexcept {
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