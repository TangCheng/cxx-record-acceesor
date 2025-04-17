#ifndef CXX_RECORD_REFLECT_LIBRARY_H
#define CXX_RECORD_REFLECT_LIBRARY_H
#include <boost/describe.hpp>
#include <boost/mp11.hpp>

#include <any>
#include <string>

#ifdef DEBUG
#include <iostream>// For std::cerr
#endif

namespace record {
template<typename T>
std::any getMemberValueByName(const T &instance, const std::string &name) {
  std::any result;// Use std::any to store the result (type erasure)
  bool found = false;

  // Use bd::for_each to iterate over the compile-time member descriptors
  // describe_members<T>() returns a compile-time list of descriptors
  boost::mp11::mp_for_each<boost::describe::describe_members<T, boost::describe::mod_public>>([&](auto const &descriptor) {
    // The lambda is instantiated for each member at compile time,
    // but the check (descriptor.name == name) happens at runtime.
    if (!found && descriptor.name == name) {
      // descriptor.pointer holds the pointer-to-member (e.g., int T::*)
      // Access the value using the instance and the pointer-to-member
      result = instance.*(descriptor.pointer);
      found = true;
    }
  });

#ifdef DEBUG
  if (!found) {
    std::cerr << "Warning: Field '" << name << "' not found for getting." << std::endl;
  }
#endif
  // Returns an empty std::any if the field was not found
  return result;
}

template<typename T>
bool setMemberValueByName(T &instance, const std::string &name, const std::any &newValue) {
  bool success = false;

  boost::mp11::mp_for_each<boost::describe::describe_members<T, boost::describe::mod_public>>([&](auto const &descriptor) {
    if (!success && descriptor.name == name) {
      // Get the type of the member pointed to by descriptor.pointer
      // We need the actual member type, not the pointer-to-member type.
      // One way is to dereference the pointer *on the instance* within decltype.
      using MemberType = std::remove_cv_t<std::remove_reference_t<decltype(instance.*(descriptor.pointer))>>;

      // Check if the type stored in std::any matches the member's type
      if (newValue.type() == typeid(MemberType)) {
        try {
          // Cast the value from std::any and assign it via the pointer-to-member
          instance.*(descriptor.pointer) = std::any_cast<MemberType>(newValue);
          success = true;
        } catch (const std::bad_any_cast &e) {
          // This should ideally not happen if typeid check passed, but defensive coding...
#ifdef DEBUG
          std::cerr << "Error: bad_any_cast during set for field '" << name << "': " << e.what() << std::endl;
#endif
          success = false;// Ensure failure
        }
      } else {
        // Type mismatch - cannot assign
#ifdef DEBUG
        std::cerr << "Error: Type mismatch for field '" << name
                  << "'. Expected type compatible with '" << typeid(MemberType).name()
                  << "', but got value of type '" << newValue.type().name() << "'." << std::endl;
#endif
        success = false;// Ensure failure
      }
    }
  });
  return success;
}
}// namespace record
void hello();

#endif//CXX_RECORD_REFLECT_LIBRARY_H
