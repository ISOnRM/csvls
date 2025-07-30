// OrderedUnique.hpp
// header-only class for storing arguments in order they were typed in
#ifndef ORDERED_UNIQUE_H
#define ORDERED_UNIQUE_H

#include <list>
#include <unordered_set>
#include <ranges>
#include "../aliases_and_concepts/concept.hpp"

template <Enum O> class OrderedUnique {
  public:
    OrderedUnique() = default;

	bool find(O&& option) {
		auto it = std::ranges::find(lst_, option);
		if (it != lst_.end()) return true;
		else return false;
	}

    bool emplace_back(O&& option) {
        auto [it, inserted] = set_.insert(option);
        if (inserted) {
            lst_.emplace_back(option);
            return inserted;
        }
        return false;
    }

	std::list<O>& get_list() {return lst_;}
    auto begin() const { return lst_.begin(); }
    auto end() const { return lst_.end(); }

  private:
    std::unordered_set<O> set_{};
    std::list<O> lst_{};
};

#endif