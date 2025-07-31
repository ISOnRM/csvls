#ifndef USING_H
#define USING_H

#include <unordered_set>
#include <forward_list>
#include "../cli/Option.hpp"
#include "../cli/OrderedUnique.hpp"


using RawArguments = std::forward_list<std::string_view>;
using ParsedOptions = OrderedUnique<Option>;
using ParsedTargets = std::unordered_set<std::string>;
//using parsed_pair = std::pair<parsed_options, parsed_targets>;

#endif