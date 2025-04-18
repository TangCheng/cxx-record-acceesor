#include <gmock/gmock.h>

#include "data.h"
#include "record/getter.h"
#include "record/register_macro.h"

// 注册时保持与类型的声明在同一命名空间，避免冲突
REGISTER_STRUCT(Address, postCode, street);
REGISTER_STRUCT(Company, addr, hookFunc, name, anything);

using namespace record;

class CXX11RecordTest : public ::testing::Test {
 protected:
  char street[32] = "123 Main St";
  char name[16] = "Tech Corp";
  Company company;

 public:
  CXX11RecordTest() {
    company.addr.street = &street;
    company.addr.postCode = 12345;
    company.name = &name;
    company.hookFunc = nullptr;
  }
};

using ::testing::Eq;
using ::testing::IsFalse;
using ::testing::IsTrue;
using ::testing::NotNull;
using ::testing::StartsWith;

TEST_F(CXX11RecordTest, GetIntValueByName) {
  std::string value = getFieldAsString(&company, "addr.postCode");
  ASSERT_THAT(value, Eq(std::to_string(company.addr.postCode)));
}

TEST_F(CXX11RecordTest, GetStringValueByName) {
  std::string value = getFieldAsString(&company, "addr.street");
  ASSERT_THAT(value, Eq(street));
}

TEST_F(CXX11RecordTest, GetObjectByName) {
  auto value = getFieldAsString(&company, "addr");
  ASSERT_THAT(value, StartsWith("obj &0"))
      << "Expected a pointer to Address for object field which have no ostream operator<<, but got: " << value;
}

TEST_F(CXX11RecordTest, GetFunctionPointer) {
  auto value = getFieldAsString(&company, "hookFunc");
  ASSERT_THAT(value, StartsWith("func ->0")) << "Expected a pointer to function: " << value;
}

TEST_F(CXX11RecordTest, GetFloat) {
  company.anything = 3.14;
  auto value = getFieldAsString(&company, "anything");
  ASSERT_THAT(value, StartsWith("3.14"));
}

TEST_F(CXX11RecordTest, GetUnregField) {
  ASSERT_THROW({ getFieldAsString(&company, "hookData"); }, std::runtime_error);
}