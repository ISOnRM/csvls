// CsvWriter.cpp

#include <array>
#include <list>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstring>
#include <ctime>
#include <cstdio>
#include <sys/stat.h>
#include <format>
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include <fmt/core.h>
#include <fmt/color.h>
#include "../assembler/Assembler.hpp"
#include "../cli/Option.hpp"
#include "CsvWriter.hpp"

namespace {
    constexpr fmt::color color_of(Option o) noexcept {
        using C = fmt::color;
        switch (o) {
            case Option::Name:
            case Option::Canonical:       return C::deep_sky_blue;
            case Option::ShowType:        return C::yellow;
            case Option::ShowPerms:       return C::lime_green;
            case Option::ShowNLinks:      return C::pale_violet_red;
            case Option::ShowOwner:       return C::purple;
            case Option::ShowGroup:       return C::orange;
            case Option::ShowSize:        return C::turquoise;
            case Option::ShowAccessTime:  return C::violet;
            case Option::ShowModTime:     return C::light_golden_rod_yellow;
            case Option::ShowMetaModTime: return C::teal;
            case Option::ShowInode:       return C::indigo;
            case Option::ShowBlocks:      return C::pink;
            case Option::ShowDev:         return C::coral;
            default:                      return C::white;
        }
    }
}


CsvWriter::CsvWriter(const Entries &entries,
                     ParsedOptions &options,
                     std::ostream &out, char delimiter)
    : entries_{entries}, options_{options}, out_{out}, delimiter_{delimiter} {
		// add color handling
		if (options_.find(Option::Color)) {
			use_color_ = true;
			options_.get_list().remove(Option::Color);
		} else {
			use_color_ = false;
		}
	}

std::string CsvWriter::column_name(Option option) const noexcept {
    auto paint  = [](fmt::color c, std::string_view s) {
        return fmt::format(fmt::fg(c), "{}", s);
    };
    auto plain = [](fmt::color, std::string_view s) {
        return std::string{s};
    };
    auto use = use_color_ ? paint : plain;

    switch (option) {
        case Option::Name:
        case Option::Canonical:         return use(color_of(option), "path");
        case Option::ShowType:          return use(color_of(option), "type");
        case Option::ShowPerms:         return use(color_of(option), "permissions");
        case Option::ShowNLinks:        return use(color_of(option), "links");
        case Option::ShowOwner:         return use(color_of(option), "owner");
        case Option::ShowGroup:         return use(color_of(option), "group");
        case Option::ShowSize:          return use(color_of(option), "size");
        case Option::ShowAccessTime:    return use(color_of(option), "access_time");
        case Option::ShowModTime:       return use(color_of(option), "mod_time");
        case Option::ShowMetaModTime:   return use(color_of(option), "meta_mod_time");
        case Option::ShowInode:         return use(color_of(option), "inode");
        case Option::ShowBlocks:        return use(color_of(option), "blocks");
        case Option::ShowDev:           return use(color_of(option), "dev");
        default:                        return {};
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
    auto paint  = [](fmt::color c, std::string_view s) {
        return fmt::format(fmt::fg(c), "{}", s);
    };
    auto plain = [](fmt::color, std::string_view s) {
        return std::string{s};
    };
    auto use = use_color_ ? paint : plain;

    for (const Entry& entry : entries_) {
        bool first = true;
        for (const Option option : options_) {
            if (!first) out_ << delimiter_;
            first = false;

            const auto col = color_of(option);

            switch (option) {
                case Option::Name:
                case Option::Canonical: {
                    const auto q = quote(entry.name);
                    out_ << use(col, q);
                    break;
                }

                case Option::ShowType: {
                    const char t = get_type_char(entry.stats);
                    out_ << use(col, fmt::format("{}", t));
                    break;
                }

                case Option::ShowPerms: {
                    const auto perms = get_perms_arr(entry.stats);
                    out_ << use(col, std::string_view(perms.data(), perms.size()));
                    break;
                }

                case Option::ShowNLinks:
                    out_ << use(col, fmt::format("{}", entry.stats.st_nlink));
                    break;

                case Option::ShowOwner:
                    out_ << use(col, get_owner_str(entry.stats));
                    break;

                case Option::ShowGroup:
                    out_ << use(col, get_group_str(entry.stats));
                    break;

                case Option::ShowSize:
                    out_ << use(col, get_size_str(entry.stats));
                    break;

                case Option::ShowBlocks:
                    out_ << use(col, fmt::format("{}", entry.stats.st_blocks));
                    break;

                case Option::ShowAccessTime:
                    out_ << use(col, get_time_str(entry.stats.st_atim));
                    break;

                case Option::ShowModTime:
                    out_ << use(col, get_time_str(entry.stats.st_mtim));
                    break;

                case Option::ShowMetaModTime:
                    out_ << use(col, get_time_str(entry.stats.st_ctim));
                    break;

                case Option::ShowInode:
                    out_ << use(col, fmt::format("{}", entry.stats.st_ino));
                    break;

                case Option::ShowDev:
                    out_ << use(col, fmt::format("{}", entry.stats.st_dev));
                    break;

                default:
                    break;
            }
        }
        out_ << '\n';
    }
}

std::string CsvWriter::quote(const std::string& text) const {
    bool need_quotes = false;

    if (text.empty()) {
        need_quotes = true;
    } else {
        for (char c : text) {
            if (c == delimiter_ || c == '"' || c == '\n' || c == '\r') {
                need_quotes = true;
                break;
            }
        }
        if (!need_quotes && (text.front() == ' ' || text.back() == ' ')) {
            need_quotes = true;
        }
    }

    if (!need_quotes) {
        return text;
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
	std::time_t sec = ts.tv_sec;
	std::tm tm{};

	localtime_r(&sec, &tm);
	
	return fmt::format(
		"{:02d}-{:02d}-{:04d} {:02d}:{:02d}:{:02d}",
         tm.tm_mday,
         tm.tm_mon + 1,
         tm.tm_year + 1900,
         tm.tm_hour,
         tm.tm_min,
         tm.tm_sec
	);
	
}
