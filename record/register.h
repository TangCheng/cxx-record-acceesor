#ifndef CXX_RECORD_REFLECT_RECORD_REGISTER_H
#define CXX_RECORD_REFLECT_RECORD_REGISTER_H

#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <typeindex>

#include "as_string.h"

namespace record {
using getter_type = std::function<const void*(const void* instance)>;
using serializer_type = std::function<std::string(const void* instance)>;

template <typename T, typename FiledType>
std::string member_serializer(const T* instance, FiledType T::*member) {
  auto field = instance->*member;
  return asString(field);
}
template <typename T>
std::string member_serializer(const T* instance, std::string member) {
  return instance->*member;
}

template <typename T, typename FiledType>
const void* member_getter(const T* instance, FiledType T::*member) {
  return &(instance->*member);
}

struct FieldInfo {
  std::type_index type;
  getter_type getter;
  serializer_type serializer;
};

using FieldMap = std::map<std::string, FieldInfo>;

class RecordRegister {
  static std::unique_ptr<std::map<std::type_index, FieldMap>> _instance;

  static std::map<std::type_index, FieldMap>& instance() {
    if (!_instance) {
      _instance.reset(new std::map<std::type_index, FieldMap>);
    }
    return *_instance;
  }

 public:
  RecordRegister(std::type_index type, FieldMap serializer_map) {
    auto& all = instance();
    auto& m = all[type];
    std::copy(serializer_map.begin(), serializer_map.end(), std::inserter(m, m.end()));
  }

  template <typename T>
  static const FieldMap* getFieldMap(const std::type_index* type = nullptr) {
    auto key = type ? *type : std::type_index(typeid(T));
    auto& all = instance();
    return all.find(key) != all.end() ? &all[key] : nullptr;
  }
};
}  // namespace record

#endif  // CXX_RECORD_REFLECT_RECORD_REGISTER_H