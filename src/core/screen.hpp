//
// Created by Raphael Baier on 25.02.2021.
//

#pragma once

#include <core/state_base.hpp>
#include <chrono>

namespace lunar::core {

struct Screen : public state_base  {

    virtual void setup() noexcept {};
    virtual void early_setup() noexcept {};
    virtual void late_setup() noexcept {};

    virtual void update(float dt) noexcept {};
    virtual void early_update(float dt) noexcept {};
    virtual void late_update(float dt) noexcept {};

    virtual void fixed_update(float dt) noexcept {};

    virtual void shutdown() noexcept {};
    virtual void early_shutdown() noexcept {};
    virtual void late_shutdown() noexcept {};

    void start() override;

    void update() override;

    void stop() override;

};
}
