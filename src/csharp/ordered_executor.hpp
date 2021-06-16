//
// Created by Raphael Baier on 10.05.2021.
// Copyright (c) 2021 Lunar Pixl Team

//

#pragma once
#include <csharp/loader.hpp>
#include <set>

namespace lunar::csharp {
    class OrderedExecutor {
    public:
        explicit OrderedExecutor(HostFXR* loader);

        void update_index(uint8_t idx);

    private:
        assembly_invoke<uint8_t(uint8_t)> m_has_runners_sym;
        assembly_invoke<uint8_t()> m_read_reset_dirty_bit_sym;
        assembly_invoke<void(uint8_t)> m_update_sym;

        std::set<uint8_t> m_indicies_with_runners;

    };
}