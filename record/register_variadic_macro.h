#ifndef CXX_RECORD_REFLECT_RECORD_REGISTER_VARIADIC_MACRO_H
#define CXX_RECORD_REFLECT_RECORD_REGISTER_VARIADIC_MACRO_H

// Utility to force deeper expansion of arguments if needed
#define EXPAND(x) x

#define FOR_EACH_1(ACTION, CLASS_NAME, A) ACTION(CLASS_NAME, A)
#define FOR_EACH_2(ACTION, CLASS_NAME, A, B) ACTION(CLASS_NAME, A) EXPAND(FOR_EACH_1(ACTION, CLASS_NAME, B))
#define FOR_EACH_3(ACTION, CLASS_NAME, A, B, C) ACTION(CLASS_NAME, A) EXPAND(FOR_EACH_2(ACTION, CLASS_NAME, B, C))
#define FOR_EACH_4(ACTION, CLASS_NAME, A, B, C, D) ACTION(CLASS_NAME, A) EXPAND(FOR_EACH_3(ACTION, CLASS_NAME, B, C, D))
#define FOR_EACH_5(ACTION, CLASS_NAME, A, B, C, D, E) \
  ACTION(CLASS_NAME, A) EXPAND(FOR_EACH_4(ACTION, CLASS_NAME, B, C, D, E))
#define FOR_EACH_6(ACTION, CLASS_NAME, A, B, C, D, E, F) \
  ACTION(CLASS_NAME, A) EXPAND(FOR_EACH_5(ACTION, CLASS_NAME, B, C, D, E, F))

// Helper to count arguments and select the appropriate FOR_EACH_N macro
#define GET_MACRO_FOR_EACH(_1, _2, _3, _4, _5, _6, NAME, ...) NAME

// The main iteration macro: applies ACTION to each field in __VA_ARGS__
#define FOR_EACH(ACTION, CLASS_NAME, ...)                                                                         \
  EXPAND(GET_MACRO_FOR_EACH(__VA_ARGS__, FOR_EACH_6, FOR_EACH_5, FOR_EACH_4, FOR_EACH_3, FOR_EACH_2, FOR_EACH_1)( \
      ACTION, CLASS_NAME, __VA_ARGS__))

/* This is technically not standard C++, but works on many compilers (GCC, Clang) */
#define REGISTER_STRUCT(CLASS_NAME, ...)                                                                               \
  BEGIN_REGISTER_STRUCT(CLASS_NAME){                                                                                   \
      EXPAND(                                                                                                          \
          FOR_EACH(FIELD_ENTRY, CLASS_NAME, __VA_ARGS__)) /* IMPORTANT: The expansion above leaves a trailing comma */ \
  } END_REGISTER_STRUCT();

#endif  // CXX_RECORD_REFLECT_RECORD_REGISTER_VARIADIC_MACRO_H
