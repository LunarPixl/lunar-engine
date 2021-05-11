//
// Created by Raphael Baier on 11.05.2021.
// Copyright (c) 2021 Lunar Pixl Team

//

#include "native_mem.hpp"

namespace lunar::csharp {

    void native_free_primitive(const uint8_t* ptr)
    {
        delete ptr;
    }

    void native_free_array(const uint8_t* array)
    {
        delete[] array;
    }


    NativeMem::NativeMem(lunar::csharp::HostFXR *loader) {
        constexpr static auto ASSEMBLY_PATH = LUNAR_CSHARP_ANNOTATE_TEXT(".\\build-dotnet\\dotnetlunar.dll");

        assembly_create_info register_info{
                LUNAR_CSHARP_ANNOTATE_TEXT("Lunar"),
                LUNAR_CSHARP_ANNOTATE_TEXT("EngineResource"),
                LUNAR_CSHARP_ANNOTATE_TEXT("RegisterNativeFreeFn"),
                LUNAR_CSHARP_ANNOTATE_TEXT("RegisterNativeFree"),
                LUNAR_CSHARP_ANNOTATE_TEXT("dotnetlunar"),
        };


        auto register_assembly = loader->load_assembly(
                ASSEMBLY_PATH,
                register_info
        );

        register_assembly.cast<void(decltype(&native_free_primitive),decltype(&native_free_array))>()
                (&native_free_primitive,&native_free_array);

    }
}