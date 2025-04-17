#ifndef CXX_RECORD_REFLECT_TEST_DATA_H
#define CXX_RECORD_REFLECT_TEST_DATA_H

/******************** 结构体定义 ********************/
struct Address {
  char (*street)[32]; // 目前示例是指向字符数组的指针，实际情况还有指向整型数组的指针，指向浮点数组的指针
  int postCode;
};

struct Company {
  Address addr;
  char (*name)[16]; // 目前示例是指向字符数组的指针，实际情况还有指向整型数组的指针，指向浮点数组的指针
  void (*hookFunc)(void*);
  void* hookData;
  float anything;
};


#endif//CXX_RECORD_REFLECT_TEST_DATA_H
