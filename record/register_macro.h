#ifndef CXX_RECORD_REFLECT_RECORD_REGISTER_MACRO_H
#define CXX_RECORD_REFLECT_RECORD_REGISTER_MACRO_H

#include "register.h"

#define BEGIN_REGISTER_STRUCT(CLASS_NAME)           \
record::RecordRegister the_##CLASS_NAME##_register(                                                                  \
    std::type_index(typeid(CLASS_NAME)), \
    record::FieldMap

#define END_REGISTER_STRUCT() \
  );

#define FIELD_ENTRY(CLASS_NAME, MEMBER_NAME)                                                                \
  {#MEMBER_NAME,                                                                                            \
   {std::type_index(typeid(decltype(std::declval<CLASS_NAME>().MEMBER_NAME))),                              \
    [](const void* instance) {                                                                              \
      return record::member_getter(static_cast<const CLASS_NAME*>(instance), &CLASS_NAME::MEMBER_NAME);     \
    },                                                                                                      \
    [](const void* instance) {                                                                              \
      return record::member_serializer(static_cast<const CLASS_NAME*>(instance), &CLASS_NAME::MEMBER_NAME); \
    }}}

#endif  // CXX_RECORD_REFLECT_RECORD_REGISTER_MACRO_H
