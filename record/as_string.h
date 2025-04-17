#ifndef CXX_RECORD_REFLECT_TEST_AS_STRING_H
#define CXX_RECORD_REFLECT_TEST_AS_STRING_H

#include <ostream>
#include <sstream>
#include <type_traits>

namespace record {
namespace trait {
// C++11 equivalent of std::void_t (from C++17)
// Takes any number of template arguments, always resolves to void.
template <typename...>
using void_t = void;

// Primary template: assumes not supported by default
template <typename T, typename = void>
struct supports_ostream_impl : std::false_type {};

// Specialization: Enabled if the expression inside void_t is valid
template <typename T>
struct supports_ostream_impl<T,
                             // This is the SFINAE check:
                             void_t<decltype(std::declval<std::ostream&>() << std::declval<T const&>())
                                    // 1. std::declval<T const&>() creates a "reference" to type T const&
                                    //    in an unevaluated context. Use const& as streaming usually reads.
                                    // 2. std::declval<std::ostream&>() creates a reference to an ostream.
                                    // 3. decltype(...) checks the type of the expression: ostream& << T
                                    // const&
                                    // 4. If step 3 is valid C++, decltype succeeds.
                                    // 5. void_t<...> then becomes void.
                                    // 6. This matches the second parameter 'void' of the primary template,
                                    //    so this specialization is chosen.
                                    // 7. If step 3 is *invalid* C++ (no matching operator<<),
                                    //    substitution failure occurs for this specialization, and the
                                    //    primary template (which inherits from std::false_type) is chosen.
                                    >> : std::true_type {};  // If specialization is chosen, inherit from true_type

// The final user-facing trait
template <typename T>
struct supports_ostream : trait::supports_ostream_impl<T> {};

// Optional: Helper variable template (C++14 style, but derivable in C++11)
// template<typename T>
// constexpr bool supports_ostream_v = supports_ostream<T>::value;

// --- End Trait Definition ---
}  // namespace trait

template <typename T>
std::string asString_impl(T const& value, std::true_type /* tag: indicates support */) {
  std::stringstream ss;
  ss << value;  // Safe: we know this is valid
  return ss.str();
}

template <std::size_t N>
std::string asString_impl(char (*value)[N], std::true_type /* tag: indicates support */) {
  return {reinterpret_cast<const char*>(value)};
}

template <typename Ret, typename... Args>
std::string asString_impl(Ret (*fp)(Args...), std::true_type) {
  std::stringstream ss;
  ss << "func ->" << fp;
  return ss.str();
}

template <typename UnknownObjectType>
std::string asString_impl(UnknownObjectType const& value, std::false_type /* tag: indicates no support */) {
  std::stringstream ss;
  ss << "obj &" << &value;  // returns an address instead
  return ss.str();
}

// Your original function template, now acting as a dispatcher
template <typename T>
std::string asString(T const& value) {  // Use const&
  return asString_impl(value, trait::supports_ostream<T>{});
}
}  // namespace record
#endif  // CXX_RECORD_REFLECT_TEST_AS_STRING_H
