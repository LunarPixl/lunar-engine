
#include <doctest/doctest.h>
#include <csharp/loader.hpp>
#include <iostream>
#include <csharp/csharp_engine.hpp>
#include <csharp/logger.hpp>
#include <csharp/ordered_executor.hpp>
#include <csharp/native_function_library.hpp>
#include <csharp/native_mem.hpp>

void intcallback(int v)
{
    std::cout << v << std::endl;
}

void stringcallback(const char* v)
{
    std::cout << v << std::endl;
}
/*
TEST_CASE("Can open dotnet")
{
    lunar::csharp::HostFXR host(LUNAR_CSHARP_ANNOTATE_TEXT("extern\\dotnet-bin\\dotnet.runtimeconfig.json"));
    lunar::csharp::assembly_create_info info
    (
            //lunar::csharp::assembly_create_info::unmanaged_callers_only{},
            LUNAR_CSHARP_ANNOTATE_TEXT("Lunar"),
            LUNAR_CSHARP_ANNOTATE_TEXT("UnitTest"),
            LUNAR_CSHARP_ANNOTATE_TEXT("RegisterFn"),
            LUNAR_CSHARP_ANNOTATE_TEXT("Register"),
            LUNAR_CSHARP_ANNOTATE_TEXT("dotnetlunar")
    );

    auto assembly = host.load_assembly(LUNAR_CSHARP_ANNOTATE_TEXT("build-dotnet\\dotnetlunar.dll"),info);
    int rc = assembly.cast<int(decltype(&intcallback),decltype(&stringcallback))>()(&intcallback,&stringcallback);
    CHECK_EQ(rc,10);
}
*/

int native_add(int a, int b)
{
    return a+b;
}
int native_sub(int a, int b)
{
    return a-b;
}
int native_mul(int a, int b)
{
    return a*b;
}
int native_div(int a, int b)
{
    return a/b;
}

uint8_t * native_buffer ()
{
    return new (std::nothrow) uint8_t[512];
}


TEST_CASE("Can Load Csharp Engine")
{
    lunar::csharp::HostFXR host(LUNAR_CSHARP_ANNOTATE_TEXT("extern\\dotnet-bin\\dotnet.runtimeconfig.json"));
    lunar::csharp::NativeMem mem(&host);
    lunar::csharp::Logger logger(&host);
    lunar::csharp::NativeFunctionLibrary library(&host);
    lunar::csharp::Engine engine(&host);

    library.register_function((void*)&native_add,"lunar::engine::test::native_add_i");
    library.register_function((void*)&native_sub,"lunar::engine::test::native_sub_i");
    library.register_function((void*)&native_mul,"lunar::engine::test::native_mul_i");
    library.register_function((void*)&native_div,"lunar::engine::test::native_div_i");
    library.register_function((void*)&native_buffer,"lunar::engine::test::native_get_buffer");

    engine.init();

    //only after the engine was initialized are all the assemblies loaded into the
    //current appdomain
    lunar::csharp::OrderedExecutor executor(&host);


    for ( int itr = 0; itr < 100; itr++){
        engine.update(10.0f);
        for(int idx = 0; idx < 256; idx++)
            executor.update_index(idx);
    }
}