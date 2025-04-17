#ifndef CXX_RECORD_REFLECT_TEST_RECORD_H
#define CXX_RECORD_REFLECT_TEST_RECORD_H
#include <stdexcept>
#include <utility>
#include <vector>

#include "register.h"

namespace record {
namespace detail {
std::vector<std::string> split(const char* fieldPath, char delimiter) {
  std::vector<std::string> result;
  std::string str(fieldPath);
  size_t pos = 0;
  while ((pos = str.find(delimiter)) != std::string::npos) {
    result.push_back(str.substr(0, pos));
    str.erase(0, pos + 1);
  }
  result.push_back(str);
  return result;
}

template <typename T>
std::string getFieldAsString(const T* instance, const std::vector<std::string>& fields,
                             const std::type_index* type = nullptr) {
  auto fieldMap = RecordRegister::getFieldMap<T>(type);
  if (fieldMap == nullptr) {
    throw std::runtime_error(std::string("Type: ") + typeid(T).name() + " not registered");
  }
  const auto& name = fields.front();
  auto it = fieldMap->find(name);
  if (it == fieldMap->end()) {
    throw std::runtime_error("Field not found");
  }
  const auto& info = it->second;

  if (fields.size() > 1) {
    auto* child = info.getter(instance);
    return getFieldAsString(child, std::vector<std::string>(fields.begin() + 1, fields.end()), &info.type);
  }

  return info.serializer(instance);
}
}  // namespace detail

template <typename T>
std::string getFieldAsString(const T* instance, const char* path) {
  std::vector<std::string> fields = detail::split(path, '.');
  if (fields.empty()) throw std::runtime_error("Field path is invalid");
  return detail::getFieldAsString(instance, fields);
}
}  // namespace record

#endif  // CXX_RECORD_REFLECT_TEST_RECORD_H
