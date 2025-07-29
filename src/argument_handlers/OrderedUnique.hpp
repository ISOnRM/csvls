// OrderedUnique.hpp
// header-only class for storing arguments in order they were typed in
#include <vector>
#include <unordered_set>

template<typename O>
concept Enum = std::is_enum_v<O>;

template <Enum O> class OrderedUnique {
  public:
    OrderedUnique(size_t reserve_size) { vec_.reserve(reserve_size); }

    bool emplace_back(const O &option) {
        auto [it, inserted] = set_.insert(option);
        if (inserted) {
            vec_.emplace_back(option);
            return inserted;
        }
        return false;
    }

    auto begin() const { return vec_.begin(); }
    auto end() const { return vec_.end(); }

  private:
    std::unordered_set<O> set_{};
    std::vector<O> vec_{};
};