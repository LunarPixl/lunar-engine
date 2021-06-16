//
// Created by Raphael Baier on 03.05.2021.
// Copyright (c) 2021 Lunar Pixl Team

//

#pragma once
#include <csharp/helper.hpp>
#include <dotnet/coreclr_delegates.h>

namespace lunar::csharp {

    template <class T>
    struct assembly_invoke {};

    template <class Ret,class... Args>
    struct assembly_invoke<Ret(Args...)> {
        explicit assembly_invoke(component_entry_point_fn fn) noexcept : func(fn) { }

        Ret operator()(Args... args)
        {
            if(!func) throw std::exception("Function was not loaded!");
            return reinterpret_cast<Ret (CORECLR_DELEGATE_CALLTYPE*)(Args...)>(func)(std::forward<Args>(args)...);
        }

        component_entry_point_fn func;
    };

    class assembly_create_info {
    public:

        struct unmanaged_callers_only {};

        assembly_create_info(csharp_string  ns,csharp_string  classname,
                             csharp_string  delegate, csharp_string  fn,
                             csharp_string  assembly_name) noexcept;

        assembly_create_info(unmanaged_callers_only,
                             csharp_string  ns, csharp_string  classname,
                             csharp_string  fn, csharp_string  assembly_name) noexcept;

        [[nodiscard]] assembly_create_info with_namespace(const csharp_string& ns) const noexcept
        {
            if(is_unmanaged_caller)
                return assembly_create_info{unmanaged_callers_only{},ns,m_classname,m_fn,m_assembly_name};
            return assembly_create_info{ns,m_classname,m_delegate,m_fn,m_assembly_name};
        }

        [[nodiscard]] assembly_create_info with_classname(const csharp_string& classname) const noexcept
        {
            if(is_unmanaged_caller)
                return assembly_create_info{unmanaged_callers_only{},m_ns,classname,m_fn,m_assembly_name};
            return assembly_create_info{m_ns,classname,m_delegate,m_fn,m_assembly_name};
        }

        [[nodiscard]] assembly_create_info with_delegate(const csharp_string& delegate) const noexcept
        {
            return assembly_create_info{m_ns,m_classname,delegate,m_fn,m_assembly_name};
        }

        [[nodiscard]] assembly_create_info with_function(const csharp_string& fn) const noexcept
        {
            if(is_unmanaged_caller)
                return assembly_create_info{unmanaged_callers_only{},m_ns,m_classname,fn,m_assembly_name};
            return assembly_create_info{m_ns,m_classname,m_delegate,fn,m_assembly_name};
        }

        [[nodiscard]] assembly_create_info with_assembly_name(const csharp_string& assembly_name) const noexcept
        {
            if(is_unmanaged_caller)
                return assembly_create_info{unmanaged_callers_only{},m_ns,m_classname,m_fn,assembly_name};
            return assembly_create_info{m_ns,m_classname,m_delegate,m_fn,assembly_name};
        }

    private:

        bool is_unmanaged_caller = false;
        csharp_string m_ns;
        csharp_string m_classname;
        csharp_string m_delegate;
        csharp_string m_fn;
        csharp_string m_assembly_name;
        friend class HostFXR;
    };

    class assembly {
    public:
        explicit assembly(component_entry_point_fn fn) noexcept;
        explicit assembly(std::nullptr_t) noexcept;

        template <class FnType>
        [[nodiscard]] assembly_invoke<FnType> cast() const noexcept
        {
            return assembly_invoke<FnType>(m_fn);
        }

    private:
        component_entry_point_fn m_fn;
    };

}