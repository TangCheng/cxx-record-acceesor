#include <gmock/gmock.h>

#include "data.h"
#include "record/getter.h"
#include "record/register_macro.h"

// 注册时保持与类型的声明在同一命名空间，避免冲突
BEGIN_REGISTER_STRUCT(Address){
    FIELD_ENTRY(Address, postCode),
    FIELD_ENTRY(Address, street),
} END_REGISTER_STRUCT();

BEGIN_REGISTER_STRUCT(Company){
    FIELD_ENTRY(Company, addr),
    FIELD_ENTRY(Company, hookFunc),
    FIELD_ENTRY(Company, name),
    FIELD_ENTRY(Company, anything),
    FIELD_ENTRY(Company, indexOfDepartment),
} END_REGISTER_STRUCT();

using namespace record;

class CXX11RecordTest : public ::testing::Test {
 protected:
  char street[32] = "123 Main St";
  char name[16] = "Tech Corp";
  int indexOfDepartment[6] = {1, 2, 3, 4, 5, 6};
  Company company;

 public:
  CXX11RecordTest() {
    company.addr.street = &street;
    company.addr.postCode = 12345;
    company.name = &name;
    company.hookFunc = nullptr;
    company.indexOfDepartment = &indexOfDepartment;
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

TEST_F(CXX11RecordTest, GetIntArrayByName) {
  auto value = getFieldAsString(&company, "indexOfDepartment");
  ASSERT_THAT(value, Eq("1,2,3,4,5,6"));
}

TEST_F(CXX11RecordTest, GetUnregField) {
  ASSERT_THROW({ getFieldAsString(&company, "hookData"); }, std::runtime_error);
}