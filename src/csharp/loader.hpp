//
// Created by Raphael Baier on 02.05.2021.
// Copyright (c) 2021 Lunar Pixl Team

//

#pragma once
#include <csharp/helper.hpp>
#include <csharp/assembly.hpp>
#include <core/dylib_loader.hpp>
#include <dotnet/hostfxr.h>
#include <dotnet/coreclr_delegates.h>


namespace lunar::csharp {

    class HostFXR {
    public:
        explicit HostFXR(const csharp_string& config);

        assembly load_assembly(const csharp_string& path,const assembly_create_info& info);

    private:
        core::dylib m_module;

        //hostfxr delegates
        hostfxr_initialize_for_runtime_config_fn m_init;
        hostfxr_get_runtime_delegate_fn m_get_delegate;
        hostfxr_close_fn m_close;

        //coreclr delegate
        load_assembly_and_get_function_pointer_fn m_load_assembly;

    };
}