
#include <doctest/doctest.h>
#include <core/dylib_loader.hpp>
#include <iostream>

#if defined(_WIN32) || defined(WIN32)
constexpr static const char * library_path = "..\\tools\\testlib\\testlib.dll";
#else
constexpr static const char * library_path = "../tools/testlib/libtestlib.so";
#endif


TEST_CASE("Can open a dylib")
{
    lunar::core::dylib lib(library_path);

    CHECK(lib.valid());
}
TEST_CASE("Can load a symbol from a dylib")
{
    lunar::core::dylib lib(library_path);

    auto sym = lib.get_symbol("simple_function");
    CHECK_NE(sym,nullptr);
}

TEST_CASE("Can load and call a simple symbol from a dylib")
{
    lunar::core::dylib lib(library_path);

    auto sym = lib.get_cast_symbol<int(int,int)>("simple_function");
    CHECK_NE(sym,nullptr);
    CHECK_EQ(sym(1,2),3);
}

TEST_CASE("Can load a more complicated symbol")
{
    lunar::core::dylib lib(library_path);

    auto sym = lib.get_cast_symbol<int(const char*)>("cstring_function");

    CHECK_NE(sym,nullptr);
    CHECK_EQ(sym("Hello World"),11);

}