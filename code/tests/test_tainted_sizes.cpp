#include <array>
#include <cstdint>

#include "test_include.hpp"

using rlbox::tainted;
using rlbox::tainted_volatile;

using CallbackType = int (*)(uint32_t, const char*, std::array<uint32_t, 1>);
using CallbackType2 =
  int (*)(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t);

const uint32_t RandomArrSize = 4;
struct test
{
  uint64_t fieldLong;
  const char* fieldString;
  uint32_t fieldBool;
  std::array<char, RandomArrSize> fieldFixedArr;
  int (*fieldFnPtr)(uint32_t, const char*, std::array<uint32_t, 1>);
  struct unknownClass* fieldUnknownPtr;
  void* voidPtr;
  std::array<CallbackType, RandomArrSize> fnArray;
};

// NOLINTNEXTLINE
TEST_CASE("RLBox trait types do not introduce vtables", "[tainted_trait]")
{
  REQUIRE(sizeof(rlbox::sandbox_wrapper_base) < sizeof(uintptr_t)); // NOLINT
  REQUIRE(sizeof(rlbox::sandbox_wrapper_base_of<int>) <
          sizeof(uintptr_t));                                         // NOLINT
  REQUIRE(sizeof(rlbox::tainted_base<int, int>) < sizeof(uintptr_t)); // NOLINT
  REQUIRE(sizeof(rlbox::tainted_base_impl<tainted, int, int>) <
          sizeof(uintptr_t)); // NOLINT
}

// NOLINTNEXTLINE
TEST_CASE("Tainted sizes work as expected", "[tainted_size]")
{
  REQUIRE(sizeof(tainted<long long, T_Sbx>) == sizeof(long long)); // NOLINT
  REQUIRE(sizeof(tainted<long, T_Sbx>) == sizeof(long));           // NOLINT
  REQUIRE(sizeof(tainted<int, T_Sbx>) == sizeof(int));             // NOLINT
  REQUIRE(sizeof(tainted<void*, T_Sbx>) == sizeof(void*));         // NOLINT
  // REQUIRE(sizeof(tainted<test, T_Sbx>) == sizeof(test));           // NOLINT
  // REQUIRE(sizeof(tainted<test*, T_Sbx>) == sizeof(test*));         // NOLINT

  REQUIRE(sizeof(tainted_volatile<long long, T_Sbx>) ==
          sizeof(TestSandbox::T_LongLongType)); // NOLINT
  REQUIRE(sizeof(tainted_volatile<long, T_Sbx>) ==
          sizeof(TestSandbox::T_LongType)); // NOLINT
  REQUIRE(sizeof(tainted_volatile<int, T_Sbx>) ==
          sizeof(TestSandbox::T_IntType)); // NOLINT
  REQUIRE(sizeof(tainted_volatile<void*, T_Sbx>) ==
          sizeof(TestSandbox::T_PointerType)); // NOLINT
  // REQUIRE(sizeof(tainted_volatile<test, T_Sbx>) ==
  // sizeof(T_Sbx::convert_sandbox_t<test>));   // NOLINT
  // REQUIRE(sizeof(tainted_volatile<test*, T_Sbx>) ==
  // sizeof(T_Sbx::convert_sandbox_t<test>*)); // NOLINT
}