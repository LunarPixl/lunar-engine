
#include <csharp/loader.hpp>
#include <iostream>

namespace lunar::csharp {
    HostFXR::HostFXR(const csharp_string &config) : m_module(std::string("..\\extern\\dotnet-bin\\hostfxr.dll"))
    {
        m_init = nullptr;
        m_get_delegate = nullptr;
        m_close = nullptr;
        m_load_assembly = nullptr;
        if(m_module.valid())
        {
            m_init = reinterpret_cast<hostfxr_initialize_for_runtime_config_fn>
            (m_module.get_symbol("hostfxr_initialize_for_runtime_config"));


            m_get_delegate = reinterpret_cast<hostfxr_get_runtime_delegate_fn>
            (m_module.get_symbol("hostfxr_get_runtime_delegate"));

            m_close = reinterpret_cast<hostfxr_close_fn>
            (m_module.get_symbol("hostfxr_close"));
        }
        if(m_init && m_get_delegate && m_close)
        {
            hostfxr_handle ctx;
            const int init_rc = m_init(config.c_str(),nullptr,&ctx);

            //check for errors when opening the config
            if(init_rc != 0 || ctx == nullptr)
            {
                m_close(ctx);
                return;
            }

            //indirection is key (this loads a delegate to load delegates)
            m_get_delegate(
                    ctx,
                    hdt_load_assembly_and_get_function_pointer,
                    reinterpret_cast<void**>(&m_load_assembly)
            );
            m_close(ctx);
        }
    }

    assembly HostFXR::load_assembly(const csharp_string &path, const assembly_create_info &info) {
        if(!m_load_assembly) return assembly(nullptr);
        component_entry_point_fn func = nullptr;

        auto dot = LUNAR_CSHARP_ANNOTATE_TEXT(".");
        auto plus = LUNAR_CSHARP_ANNOTATE_TEXT("+");
        auto comma = LUNAR_CSHARP_ANNOTATE_TEXT(", ");

        csharp_string fq_delegate_name;

        if(!info.is_unmanaged_caller)
            fq_delegate_name = info.m_ns + dot + info.m_classname + plus + info.m_delegate + comma + info.m_assembly_name;
        auto fq_class_name = info.m_ns + dot + info.m_classname + comma + info.m_assembly_name;


        m_load_assembly(
                path.c_str(),
                fq_class_name.c_str(),
                info.m_fn.c_str(),
                info.is_unmanaged_caller ? UNMANAGEDCALLERSONLY_METHOD : fq_delegate_name.c_str(),
                nullptr,
                reinterpret_cast<void**>(&func)
        );
        return assembly(func);
    }
}