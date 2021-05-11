#include <csharp/assembly.hpp>
#include <utility>

namespace lunar::csharp{
    assembly_create_info::assembly_create_info(csharp_string ns, csharp_string classname,
                                               csharp_string delegate, csharp_string fn,
                                               csharp_string assembly_name) noexcept:
                                               is_unmanaged_caller(false),
                                               m_ns(std::move(ns)),
                                               m_classname(std::move(classname)),
                                               m_delegate(std::move(delegate)),
                                               m_fn(std::move(fn)),
                                               m_assembly_name(std::move(assembly_name))
        { }

    assembly_create_info::assembly_create_info(assembly_create_info::unmanaged_callers_only, csharp_string ns,
                                               csharp_string classname, csharp_string fn,
                                               csharp_string assembly_name) noexcept:
                                               is_unmanaged_caller(true),
                                               m_ns(std::move(ns)),
                                               m_classname(std::move(classname)),
                                               m_delegate(),
                                               m_fn(std::move(fn)),
                                               m_assembly_name(std::move(assembly_name))
        { }

    assembly::assembly(component_entry_point_fn fn) noexcept : m_fn(fn){ }

    assembly::assembly(std::nullptr_t) noexcept : m_fn(nullptr) { }
}