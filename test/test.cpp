#include <GMock/GMock.h>
#include <record/accessor.h>

#include <iostream>
#include <stdexcept>  // For exceptions like bad_any_cast

#include "data.h"

// Use Boost.Describe macro *after* the definition to declare members
//    Place this in the same namespace scope as the struct.
//    BOOST_DESCRIBE_STRUCT(StructName, (BaseClasses), (Members))
BOOST_DESCRIBE_STRUCT(Address, (), (street, postCode))
BOOST_DESCRIBE_STRUCT(Company, (), (addr, name, hookFunc, hookData, anything))

class RecordTest : public ::testing::Test {
 protected:
  char street[32] = "123 Main St";
  char name[16] = "Tech Corp";
  Address address;
  Company company;
  static void HookFoo(void *) { std::cout << "Hook Foo" << std::endl; }
  static void HookBar(void *) { std::cout << "Hook Bar" << std::endl; }

 public:
  RecordTest() {
    address.street = &street;
    address.postCode = 12345;
    company.addr = address;
    company.name = &name;
    company.hookFunc = &HookFoo;
  }
};

using ::testing::Eq;
using ::testing::IsFalse;
using ::testing::IsTrue;
using ::testing::NotNull;

TEST_F(RecordTest, GetIntValueByName) {
  std::string fieldNameToGet = "postCode";
  std::any retrievedValue =
      record::getMemberValueByName(address, fieldNameToGet);
  ASSERT_THAT(retrievedValue.has_value(), IsTrue());
  ASSERT_NO_THROW({
    // You need to know or determine the expected type to cast from std::any
    int postCode = std::any_cast<int>(retrievedValue);
    EXPECT_THAT(postCode, Eq(address.postCode));
  });
}

TEST_F(RecordTest, GetArrayPointerByName) {
  auto fieldNameToGet = "street";
  auto retrievedValue = record::getMemberValueByName(address, fieldNameToGet);
  if (retrievedValue.has_value()) {
    ASSERT_NO_THROW({
      auto *s = std::any_cast<char(*)[32]>(retrievedValue);
      EXPECT_THAT(s, Eq(&street));
    });
  }
}

TEST_F(RecordTest, GetFunctionPointerByName) {
  auto fieldNameToGet = "hookFunc";
  auto retrievedValue = record::getMemberValueByName(address, fieldNameToGet);
  if (retrievedValue.has_value()) {
    ASSERT_NO_THROW({
      auto *func = std::any_cast<void (*)(void *)>(retrievedValue);
      ASSERT_THAT(func, NotNull());
    });
  }
}

TEST_F(RecordTest, GetNonExistent) {
  auto fieldNameToGet = "non_existent";
  auto retrievedValue = record::getMemberValueByName(
      address, fieldNameToGet);  // Will print warning from function
  EXPECT_THAT(retrievedValue.has_value(), IsFalse());
}

TEST_F(RecordTest, SetArrayPointerByName) {
  char newStreet[32] = "456 Elm St";
  std::cout << "Setting values by name:" << std::endl;
  std::string fieldNameToSet = "street";
  std::cout << "Setting '" << fieldNameToSet << "' to " << newStreet
            << std::endl;
  auto ok = record::setMemberValueByName(
      address, fieldNameToSet, &newStreet);  // Pass char array pointer
  EXPECT_THAT(ok, IsTrue()) << "Set failed." << std::endl;
  EXPECT_THAT(address.street, Eq(&newStreet));
}

TEST_F(RecordTest, SetIntValueByName) {
  auto fieldNameToSet = "postCode";
  int newValueInt = 518000;
  auto ok = record::setMemberValueByName(address, fieldNameToSet, newValueInt);
  EXPECT_THAT(ok, IsTrue()) << "Set failed." << std::endl;
  EXPECT_THAT(address.postCode, Eq(newValueInt));
}

TEST_F(RecordTest, SetFunctionPointerByName) {
  auto ok = record::setMemberValueByName(company, "hookFunc", &HookBar);
  EXPECT_THAT(ok, IsTrue()) << "Set failed." << std::endl;
  EXPECT_THAT(company.hookFunc, Eq(&HookBar));
}

TEST_F(RecordTest, SetTypeMismatch) {
  auto fieldNameToSet = "street";
  auto ok =
      record::setMemberValueByName(address, fieldNameToSet, 12345);  // Pass int
  EXPECT_THAT(ok, IsFalse()) << "Set incorrectly succeeded?!" << std::endl;
}

TEST_F(RecordTest, SetNonExistent) {
  auto fieldNameToSet = "another_field";
  auto ok =
      record::setMemberValueByName(address, fieldNameToSet, "doesn't matter");
  EXPECT_THAT(ok, IsFalse());
}