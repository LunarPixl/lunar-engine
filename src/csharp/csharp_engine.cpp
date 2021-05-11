
#include <csharp/csharp_engine.hpp>

namespace lunar::csharp {

    Engine::Engine(HostFXR* loader) : m_loader(loader), m_initialize_fun(nullptr), m_update_fun(nullptr) {

        constexpr static auto ASSEMBLY_PATH = LUNAR_CSHARP_ANNOTATE_TEXT(".\\build-dotnet\\dotnetlunar.dll");


        assembly_create_info prototype {
            assembly_create_info::unmanaged_callers_only{},
            LUNAR_CSHARP_ANNOTATE_TEXT("Lunar"),
            LUNAR_CSHARP_ANNOTATE_TEXT("Loader"),
            LUNAR_CSHARP_ANNOTATE_TEXT(""),
            LUNAR_CSHARP_ANNOTATE_TEXT("dotnetlunar")
        };

        auto init_info = prototype.with_function(LUNAR_CSHARP_ANNOTATE_TEXT("Initialize"));
        auto update_info = prototype.with_function(LUNAR_CSHARP_ANNOTATE_TEXT("Update"));

        m_initialize_fun =
            loader->load_assembly(
                ASSEMBLY_PATH,
                init_info
            ).cast<void()>();

        m_update_fun =
            loader->load_assembly(
                ASSEMBLY_PATH,
                update_info
            ).cast<void(float)>();

    }

    void Engine::init() {
        m_initialize_fun();
    }

    void Engine::update(float dt) {
        m_update_fun(dt);
    }
}