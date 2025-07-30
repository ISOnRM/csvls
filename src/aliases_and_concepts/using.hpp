#ifndef USING_H
#define USING_H

#include <unordered_set>
#include <forward_list>


using raw_arguments = std::forward_list<std::string_view>;
using parsed_options = OrderedUnique<Option>;
using parsed_targets = std::unordered_set<std::string>;
using parsed_pair = std::pair<parsed_options, parsed_targets>;

#endif