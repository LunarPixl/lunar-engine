//
// Created by Raphael Baier on 10.05.2021.
// Copyright (c) 2021 Lunar Pixl Team

//

#include "native_function_library.hpp"

namespace lunar::csharp {

    std::unordered_map<std::string, void *> m_pointer_map;

    int fn_exists(const char *name) noexcept {
        return m_pointer_map.contains(name) ? 1 : 0;
    }

    void *get_fn(const char *name) noexcept {

        if (auto itr = m_pointer_map.find(name); itr != m_pointer_map.end()) {
            return itr->second;
        }
        return nullptr;
    }

    NativeFunctionLibrary::NativeFunctionLibrary(lunar::csharp::HostFXR *loader) {
        constexpr static auto ASSEMBLY_PATH = LUNAR_CSHARP_ANNOTATE_TEXT(".\\build-dotnet\\dotnetlunar.dll");

        assembly_create_info register_info{
            LUNAR_CSHARP_ANNOTATE_TEXT("Lunar"),
            LUNAR_CSHARP_ANNOTATE_TEXT("NativeFunctionsLibrary"),
            LUNAR_CSHARP_ANNOTATE_TEXT("RegisterGetterFn"),
            LUNAR_CSHARP_ANNOTATE_TEXT("RegisterGetter"),
            LUNAR_CSHARP_ANNOTATE_TEXT("dotnetlunar"),
        };

        auto assembly = loader->load_assembly(
            ASSEMBLY_PATH,
            register_info
        );

        assembly.cast<void(decltype(&get_fn),decltype(&fn_exists))>()(&get_fn,&fn_exists);
    }

    void NativeFunctionLibrary::register_function(void *fnptr, const std::string &fnname) {
        m_pointer_map[fnname] = fnptr;
    }



}
