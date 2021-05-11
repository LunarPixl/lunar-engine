
#pragma once

#include "loader.hpp"

namespace lunar::csharp {

    class Engine {
    public:
        explicit Engine(HostFXR* loader);

        void init();
        void update(float dt);

    private:
        HostFXR* m_loader;
        assembly_invoke<void()> m_initialize_fun;
        assembly_invoke<void(float)> m_update_fun;

    };
}