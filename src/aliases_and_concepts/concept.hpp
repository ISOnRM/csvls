#include <string>
#include <string_view>

template <typename T>
concept CharOrString =
    std::same_as<T, char> ||
    std::same_as<T, std::string> ||
    std::same_as<T, std::string_view>;

template<typename O>
concept Enum = std::is_enum_v<O>;
